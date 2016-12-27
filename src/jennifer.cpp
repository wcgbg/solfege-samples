#include "sf2gigbuilder.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  string sample_dir = argv[1];

  Sf2GigBuilder builder(sample_dir);
  builder.SetName("Solfege by Jennifer");
  builder.SetReleaseVolEnv(0.8);
  builder.AddInstrument("Sotorrio", 0, 0, { "do", "ga", "re", "nu", "mi", "fa",
      "jur", "so", "ki", "la", "pe", "ti" });
  builder.BuildBoth("solfege-jennifer");
}
