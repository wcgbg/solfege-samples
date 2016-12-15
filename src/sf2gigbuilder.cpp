#include "sf2gigbuilder.hpp"

#include <set>
#include <fstream>

#include <sf2cute.hpp>
#include <libgig/gig.h>

#include "note.hpp"
//#include "debug.hpp"

using namespace std;
using namespace sf2cute;

Sf2GigBuilder::Sf2GigBuilder(const string &sample_dir) :
    notes_(), name_(), release_vol_env_(), transpose_(),
    bank_program_to_name_solfeges_(), solfeges_union_() {
  auto sample_files = ListFiles(sample_dir, Note::kFilenameRegex);
  sort(sample_files.begin(), sample_files.end());
  for (auto &sample_file : sample_files) {
    notes_.push_back(Note(sample_dir + '/' + sample_file));
  }
}

void Sf2GigBuilder::AddInstrument(const string &name, uint16_t bank_number,
    uint16_t program_number, const unordered_set<string> &solfeges) {
  if (!bank_program_to_name_solfeges_.insert( { { bank_number, program_number },
      { name, solfeges } }).second) {
    throw runtime_error("bank number and program number already exists");
  }
  solfeges_union_.insert(solfeges.begin(), solfeges.end());
}

void Sf2GigBuilder::SetName(const std::string& name) {
  name_ = name;
}

void Sf2GigBuilder::SetReleaseVolEnv(double release_vol_env) {
  release_vol_env_ = release_vol_env;
}

void Sf2GigBuilder::SetTranspose(int transpose) {
  if (transpose % 12) {
    throw runtime_error("transpose must be a multiple of 12");
  }
  transpose_ = transpose;
}

void Sf2GigBuilder::BuildSf2(const string &output_filename) const {
  SoundFont sf2;

  // Meta data
  sf2.set_sound_engine("EMU8000");
  if (!name_.empty()) {
    sf2.set_bank_name(name_);
  }
  sf2.set_copyright(
      "GPL v3\n"
          "The \"source code\" means this sound font including all the samples in it. "
          "The modified source versions include the sound fonts (in any format) and synthesizers that use the samples here, "
          "but doesn't include the music created by them.");
  sf2.set_engineers("Chengu Wang");
  sf2.set_comment("https://github.com/wcgbg/solfege-samples\n"
      "Acknowledgment: https://github.com/gocha/sf2cute");

  vector<pair<Note, shared_ptr<SFSample>>> notes_and_samples;
  for (auto &note : notes_) {
    if (solfeges_union_.find(note.solfege()) == solfeges_union_.end()) {
      continue;
    }
    auto sample = sf2.NewSample(note.name(), note.LoadSamples(), 0, 0,
        Note::kSampleRate, note.pitch() + transpose_, 0);
    notes_and_samples.push_back( { note, sample });
  }

  for (auto &bank_program_name_solfeges : bank_program_to_name_solfeges_) {
    auto bank_num = bank_program_name_solfeges.first.first;
    auto program_num = bank_program_name_solfeges.first.second;
    auto &name = bank_program_name_solfeges.second.first;
    auto &solfeges = bank_program_name_solfeges.second.second;

    auto instrument = sf2.NewInstrument(name);
    if (release_vol_env_ != 0) {
      SFInstrumentZone global_zone;
      global_zone.SetGenerator(
          { SFGenerator::kReleaseVolEnv, int16_t(1200 * log2(release_vol_env_)) });
      instrument->set_global_zone(global_zone);
    }
    set<int> pitch_set;
    for (auto &note_and_sample : notes_and_samples) {
      auto &note = note_and_sample.first;
      auto sample = note_and_sample.second;
      if (solfeges.find(note.solfege()) == solfeges.end()) {
        continue;
      }
      int pitch = note.pitch() + transpose_;
      if (!pitch_set.insert(pitch).second) {
        throw runtime_error("key already exists");
      }
      SFInstrumentZone instrument_zone(sample);
      instrument_zone.SetGenerator(
          { SFGenerator::kKeyRange, RangesType(pitch, pitch) });
      instrument->AddZone(move(instrument_zone));
    }
    auto preset = sf2.NewPreset(name, program_num, bank_num);
    preset->AddZone(SFPresetZone(instrument));
  }

  ofstream ofs(output_filename, ios::binary);
  sf2.Write(ofs);

#ifdef SF2CUTE_EXAMPLES_DEBUG_HPP_
  PrintSoundFont(sf2);
#endif
}

void Sf2GigBuilder::BuildGig(const std::string& output_filename) const {
  gig::File file;

  file.pInfo->Name = name_;

  vector<pair<Note, gig::Sample *>> notes_and_samples;
  for (auto &note : notes_) {
    if (solfeges_union_.find(note.solfege()) == solfeges_union_.end()) {
      continue;
    }
    gig::Sample *sample = file.AddSample();
    sample->pInfo->Name = note.name();
    sample->Channels = 1; // mono
    sample->BitDepth = 16; // 16 bits
    sample->FrameSize = 16/*bitdepth*// 8/*1 byte are 8 bits*/* 1/*mono*/;
    sample->SamplesPerSecond = Note::kSampleRate;
    sample->MIDIUnityNote = note.pitch() + transpose_;
    sample->Resize(note.NumOfSamples());
    notes_and_samples.push_back( { note, sample });
  }
  if (notes_and_samples.empty()) {
    throw runtime_error("empty");
  }

  for (auto &bank_program_name_solfeges : bank_program_to_name_solfeges_) {
    auto bank_num = bank_program_name_solfeges.first.first;
    auto program_num = bank_program_name_solfeges.first.second;
    auto &name = bank_program_name_solfeges.second.first;
    auto &solfeges = bank_program_name_solfeges.second.second;

    gig::Instrument* instrument = file.AddInstrument();
    instrument->pInfo->Name = name;
    instrument->MIDIBank = bank_num;
    instrument->MIDIProgram = program_num;

    set<int> pitch_set;
    for (auto &note_and_sample : notes_and_samples) {
      auto &note = note_and_sample.first;
      auto sample = note_and_sample.second;

      if (solfeges.find(note.solfege()) == solfeges.end()) {
        continue;
      }
      int pitch = note.pitch() + transpose_;
      if (!pitch_set.insert(pitch).second) {
        throw runtime_error("key already exists");
      }
      gig::Region* pRegion = instrument->AddRegion();
      pRegion->SetKeyRange(pitch, pitch);
      pRegion->SetSample(sample);
      pRegion->pDimensionRegions[0]->pSample = sample;
      pRegion->pDimensionRegions[0]->UnityNote = note.pitch() + transpose_;
      pRegion->pDimensionRegions[0]->EG1Release = release_vol_env_;
    }
  }

  // save file as of now
  file.Save(output_filename);
  for (auto &note_and_sample : notes_and_samples) {
    auto &note = note_and_sample.first;
    auto sample = note_and_sample.second;
    auto wav_samples = note.LoadSamples();
    sample->Write(wav_samples.data(), wav_samples.size());
  }
}

void Sf2GigBuilder::BuildBoth(
    const std::string& output_filename_without_extension) const {
  BuildSf2(output_filename_without_extension + ".sf2");
  BuildGig(output_filename_without_extension + ".gig");
}
