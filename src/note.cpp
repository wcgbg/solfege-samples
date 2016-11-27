#include "note.hpp"

#include <dirent.h>
#include <sndfile.h>
#include <iostream>

using namespace std;

const regex Note::kPathRegex("(.*/)?note([[:digit:]]{3})-([a-z]+)\\.wav");
const regex Note::kFilenameRegex("note[[:digit:]]{3}-[a-z]+\\.wav");
const int Note::kSampleRate = 44100;

Note::Note() :
    pitch_(-1) {
}

Note::Note(const string &filename) :
    filename_(filename), pitch_(-1) {
  smatch match;
  if (!regex_match(filename, match, kPathRegex)) {
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
  if (pitch_ == -1) {
    throw std::logic_error("Empty note");
  }
}

int Note::pitch() const {
  Validate();
  return pitch_;
}

string Note::name() const {
  Validate();
  return pitch_name_ + '-' + solfege_;
}

string Note::solfege() const {
  Validate();
  return solfege_;
}

vector<int16_t> Note::LoadSamples() const {
  Validate();
  SF_INFO info;
  SNDFILE *file;
  file = sf_open(filename_.c_str(), SFM_READ, &info);
  if (file == nullptr) {
    throw std::runtime_error("Cannot open file: " + filename_);
  }
  if (info.channels == 1 && (info.format & SF_FORMAT_PCM_16)
      && info.samplerate == kSampleRate) {
    static_assert(sizeof(short) == 2, "Size doesn't match");
    vector<int16_t> samples(info.frames);
    sf_read_short(file, (short *) samples.data(), samples.size());
    sf_close(file);
    return samples;
  } else {
    sf_close(file);
    cout << info.channels << endl;
    cout << info.format << endl;
    cout << info.samplerate << endl;
    throw std::runtime_error("Unexpected format: " + filename_);
  }
}

size_t Note::NumOfSamples() const {
  Validate();
  SF_INFO info;
  SNDFILE *file;
  file = sf_open(filename_.c_str(), SFM_READ, &info);
  if (file == nullptr) {
    throw std::runtime_error("Cannot open file: " + filename_);
  }
  sf_close(file);
  return info.frames;
}

vector<string> ListFiles(const string &dir, const regex &filename_regex) {
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(dir.c_str())) == nullptr) {
    throw runtime_error("cannot open directory: " + dir);
  }
  vector<string> files;
  while ((dirp = readdir(dp)) != nullptr) {
    if (regex_match(dirp->d_name, filename_regex)) {
      files.push_back(string(dirp->d_name));
    }
  }
  closedir(dp);
  return files;
}
