#ifndef NOTE_H
#define NOTE_H

#include <vector>
#include <string>
#include <regex>
#include <cstdint>

class Note {
  static const std::regex filename_regex;
  std::string filename_;
  int pitch_;
  std::string pitch_name_;
  std::string solfege_;
  void Validate() const;
public:
  Note();
  Note(const std::string &filename);
  std::vector<int16_t> LoadSamples() const;
  size_t NumOfSamples() const;
  int pitch() const;
  std::string name() const;
  std::string solfege() const;
};

std::pair<bool, std::vector<std::string>> ls(const std::string &dir,
    const std::regex &filename_regex);

#endif // NOTE_H
