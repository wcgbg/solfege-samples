#include "sf2gigbuilder.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  string sample_dir = argv[1];

  Sf2GigBuilder builder(sample_dir);
  builder.SetName("Solfege by Katy");
  builder.SetReleaseVolEnv(0.2);
  builder.AddInstrument("Solfege", 0, 0, { "do", "re", "mi", "fa", "so", "la",
      "ti" });
  builder.BuildBoth("solfege-katy");
}
