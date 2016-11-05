#include <memory>
#include <fstream>
#include <iostream>

#include <sf2cute.hpp>

//#include "debug.hpp"
#include "note.h"

using namespace std;
using namespace sf2cute;

void MakeSf2(const vector<string> &filenames) {
  SoundFont sf2;

  // Meta data
  sf2.set_sound_engine("EMU8000");
  sf2.set_bank_name("Solfege");
  sf2.set_copyright(
      "GPL v3\n"
          "The \"source code\" means this sound font including all the samples in it. "
          "The modified source versions include the sound fonts (in any format) and synthesizers that use the samples here, "
          "but doesn't include the music created by them.");
  sf2.set_engineers("Chengu Wang");
  sf2.set_comment("https://github.com/wcgbg/solfege-samples\n"
      "Acknowledgment: https://github.com/gocha/sf2cute, Alter/Ego");

  // Add samples
  unordered_map<string, pair<Note, shared_ptr<SFSample>>> notes[128];
  for (auto &filename : filenames) {
    Note note = Note(filename);
    auto sample = sf2.NewSample(note.name(), note.LoadSamples(), 0, 0, 44100,
        note.pitch(), 0);
    notes[note.pitch()][note.solfege()] = {note,sample};
  }

  // Add an instrument
  const string solfege_in_octave[12] = { "do", "ga", "re", "nu", "mi", "fa",
      "jur", "so", "ki", "la", "pe", "ti" };
  auto instrument = sf2.NewInstrument("Sotorrio");
  for (int pitch = 0; pitch < 128; pitch++) {
    auto &solfege = solfege_in_octave[pitch % 12];
    auto it = notes[pitch].find(solfege);
    if (it == notes[pitch].end()) {
      continue;
    }
    auto &note = it->second.first;
    auto sample = it->second.second;
    SFInstrumentZone instrument_zone(sample);
    instrument_zone.SetGenerator(
        { SFGenerator::kKeyRange, RangesType(note.pitch(), note.pitch()) });
    instrument->AddZone(move(instrument_zone));
  }

  // Add a preset.
  std::shared_ptr<SFPreset> preset = sf2.NewPreset("Sotorrio", 0, 0);
  preset->AddZone(SFPresetZone(instrument));

  // Print the tree for debugging.
#ifdef SF2CUTE_EXAMPLES_DEBUG_HPP_
  PrintSoundFont(sf2);
#endif

  // Write SoundFont file.
  try {
    ofstream ofs("solfege.sf2", ios::binary);
    sf2.Write(ofs);
  } catch (const fstream::failure & e) {
    // File output error.
    cerr << e.what() << endl;
  } catch (const exception & e) {
    // Other errors.
    // For example: Too many samples.
    cerr << e.what() << endl;
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
  MakeSf2(sample_files);
}
