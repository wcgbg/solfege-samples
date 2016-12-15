#include "sf2gigbuilder.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  string sample_dir;
  int transpose;
  string output_filename = "solfege-chengu";
  if (argc == 2) {
    sample_dir = argv[1];
    transpose = 0;
  } else if (argc == 4) {
    if (string(argv[1]) != "--transpose") {
      return 1;
    }
    transpose = atoi(argv[2]);
    output_filename += '.';
    output_filename += argv[2];
    sample_dir = argv[3];
  } else {
    return 1;
  }

  Sf2GigBuilder builder(sample_dir);
  builder.SetTranspose(transpose);
  builder.SetName("Solfege by Chengu Wang");
  builder.SetReleaseVolEnv(0.8);
  builder.AddInstrument("Sotorrio", 0, 0, { "do", "ga", "re", "nu", "mi", "fa",
      "jur", "so", "ki", "la", "pe", "ti" });
  builder.BuildBoth(output_filename);
}
