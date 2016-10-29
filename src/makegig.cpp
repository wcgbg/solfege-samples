#include <sys/types.h>
#include <dirent.h>
#include <libgig/gig.h>
#include <cstdint>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <map>

using namespace std;

class Note {
  static const regex filename_regex;
  string filename_;
  int pitch_;
  string pitch_name_;
  string solfege_;
  gig::Sample *gig_sample_;
  void Validate() const;
public:
  Note();
  Note(const string &filename, gig::Sample *gig_sample);
  vector<int16_t> LoadSamples() const;
  int pitch() const;
  gig::Sample *gig_sample() const;
  string name() const;
  string solfege() const;
};

const regex Note::filename_regex("(.*/)?note([[:digit:]]{3})-([a-z]+)\\.wav");

Note::Note() :
    pitch_(-1), gig_sample_(nullptr) {
}

Note::Note(const string &filename, gig::Sample *gig_sample) :
    filename_(filename), pitch_(-1), gig_sample_(gig_sample) {
  smatch match;
  if (!regex_match(filename, match, filename_regex)) {
    throw invalid_argument("Unknown filename format: " + filename);
  }
  pitch_name_ = match[2];
  solfege_ = match[3];
  pitch_ = stoi(pitch_name_);
  if (pitch_ < 0 && pitch_ >= 128) {
    throw out_of_range("filename=" + filename);
  }
}

void Note::Validate() const {
  if (pitch_ == -1 || gig_sample_ == nullptr) {
    throw std::logic_error("Empty note");
  }
}

int Note::pitch() const {
  Validate();
  return pitch_;
}

gig::Sample *Note::gig_sample() const {
  Validate();
  return gig_sample_;
}

string Note::name() const {
  Validate();
  return pitch_name_ + '-' + solfege_;
}

string Note::solfege() const {
  Validate();
  return solfege_;
}

void MakeGig(const vector<string> &filenames) {
  gig::File file;
  // we give it an internal name, not mandatory though
  file.pInfo->Name = "Solfege";

  unordered_map<string, Note> notes[128];
  for (auto &filename : filenames) {
    gig::Sample *sample = file.AddSample();
    Note note = Note(filename, sample);
    sample->pInfo->Name = note.name();
    sample->Channels = 1; // mono
    sample->BitDepth = 16; // 16 bits
    sample->FrameSize = 16/*bitdepth*// 8/*1 byte are 8 bits*/* 1/*mono*/;
    sample->SamplesPerSecond = 44100;
    sample->MIDIUnityNote = note.pitch();
    sample->Resize(3);
    notes[note.pitch()][note.solfege()] = note;
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
    gig::Sample* p_sample = it->second.gig_sample();
    pRegion->SetSample(p_sample);
    pRegion->pDimensionRegions[0]->pSample = p_sample;
    pRegion->pDimensionRegions[0]->UnityNote = pitch;
  }

  // save file as of now
  file.Save("foo.gig");
//  for (int pitch = 0; pitch < 128; pitch++) {
//    for (auto &solfege_and_note : notes[pitch]) {
//      solfege_and_note.second.gig_sample()->Write(sampleData1, 3);
//    }
//  }
}

pair<bool, vector<string>> ls(const string &dir, const regex &filename_regex) {
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(dir.c_str())) == NULL) {
    return {false, {}};
  }

  vector<string> files;
  while ((dirp = readdir(dp)) != NULL) {
    if (regex_match(dirp->d_name, filename_regex)) {
      files.push_back(string(dirp->d_name));
    }
  }
  closedir(dp);
  return {true, files};
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
//  // four stupid little sample "waves"
//  // (each having three sample points length, 16 bit depth, mono)
//  int16_t sampleData1[] = { 1, 2, 3 };
//  int16_t sampleData2[] = { 4, 5, 6 };
//  int16_t sampleData3[] = { 7, 8, 9 };
//  int16_t sampleData4[] = { 10, 11, 12 };
//  try {
//  } catch (RIFF::Exception &e) {
//    e.PrintMessage();
//    return -1;
//  }
//  return 0;
}
