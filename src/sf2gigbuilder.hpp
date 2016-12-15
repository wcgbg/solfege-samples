#ifndef SF2GIGBUILDER_HPP_
#define SF2GIGBUILDER_HPP_

#include <map>
#include <unordered_set>
#include <string>
#include <vector>

#include "note.hpp"


class Sf2GigBuilder {
  std::vector<Note> notes_;
  std::string name_;
  double release_vol_env_;
  int transpose_;
  std::map<std::pair<int, int>, std::pair<std::string, std::unordered_set<std::string>>>
      bank_program_to_name_solfeges_;
  std::unordered_set<std::string> solfeges_union_;
public:
  Sf2GigBuilder(const std::string &sample_dir);
  void SetName(const std::string &name);
  void SetReleaseVolEnv(double release_vol_env);  // in seconds
  void SetTranspose(int transpose);
  void AddInstrument(const std::string &name, uint16_t bank_number,
      uint16_t program_number, const std::unordered_set<std::string> &solfeges);
  void BuildSf2(const std::string &output_filename) const;
  void BuildGig(const std::string &output_filename) const;
  void BuildBoth(const std::string &output_filename_without_extension) const;
};

#endif /* SF2GIGBUILDER_HPP_ */
