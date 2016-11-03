#include <sys/types.h>
#include <libgig/gig.h>

#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

#include "note.h"

using namespace std;

void MakeGig(const vector<string> &filenames) {
  gig::File file;
  // we give it an internal name, not mandatory though
  file.pInfo->Name = "Solfege";

  unordered_map<string, pair<Note, gig::Sample *>> notes[128];
  for (auto &filename : filenames) {
    gig::Sample *gig_sample = file.AddSample();
    Note note = Note(filename);
    gig_sample->pInfo->Name = note.name();
    gig_sample->Channels = 1; // mono
    gig_sample->BitDepth = 16; // 16 bits
    gig_sample->FrameSize = 16/*bitdepth*// 8/*1 byte are 8 bits*/* 1/*mono*/;
    gig_sample->SamplesPerSecond = 44100;
    gig_sample->MIDIUnityNote = note.pitch();
    gig_sample->Resize(note.LoadSamples().size());
    notes[note.pitch()][note.solfege()] = {note,gig_sample};
  }

  const string solfege_in_octave[12] = { "do", "ga", "re", "nu", "mi", "fa",
      "jur", "so", "ki", "la", "pe", "ti" };

  gig::Instrument* instrument = file.AddInstrument();
  // give them a name (not mandatory)
  instrument->pInfo->Name = "Sotorrio";

  for (int pitch = 0; pitch < 128; pitch++) {
    auto &solfege = solfege_in_octave[pitch % 12];
    auto it = notes[pitch].find(solfege);
    if (it == notes[pitch].end()) {
      continue;
    }
    gig::Region* pRegion = instrument->AddRegion();
    pRegion->SetKeyRange(pitch, pitch);
    gig::Sample* p_sample = it->second.second;
    pRegion->SetSample(p_sample);
    pRegion->pDimensionRegions[0]->pSample = p_sample;
    pRegion->pDimensionRegions[0]->UnityNote = pitch;
  }

  // save file as of now
  file.Save("solfege.gig");
  for (int pitch = 0; pitch < 128; pitch++) {
    for (auto &entry : notes[pitch]) {
      auto &note = entry.second.first;
      auto gig_sample = entry.second.second;
      auto samples = note.LoadSamples();
      gig_sample->Write(samples.data(), samples.size());
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  string sample_dir = argv[1];
  auto p = ls(sample_dir, regex("note[[:digit:]]{3}-[a-z]+\\.wav"));
  if (!p.first) {
    return 2;
  }
  auto &sample_files = p.second;
  for (auto &sample_file : sample_files) {
    sample_file = sample_dir + '/' + sample_file;
  }
  MakeGig(sample_files);
}
