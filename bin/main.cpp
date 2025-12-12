#include <string>

#include "instrument.h"
#include "process.h"
#include "song.h"

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  fs::path txt_path = argv[1];
  // std::string wav_path = argv[2];
  // Run(txt_path, wav_path);
  std::ifstream in(txt_path);

  Song song;
  Parser parser;
  parser.Parse(song, in);
  int instruments_size = song.GetInstrumentsSize();
  song.GetBpm();
  for (int i = 0; i < 1; ++i) {
    SamplerInstrument* sampler =
        dynamic_cast<SamplerInstrument*>(song.GetInstrument(i));
        sampler->GetPath();
    sampler->GetRootNote();
    sampler->GetLoopBegin();
    sampler->GetLoopEnd();
    sampler->GetAttack();
    sampler->GetRelease();
    // int effects_size = sampler->GetEffectsSize();

    // for(int j = 0; j < 1; ++j) {
    //   GainEffect* gain_pointer = dynamic_cast<GainEffect*>(sampler->GetGain());
    //   gain_pointer->GetGain();
    // }
    std::cout << '\n';
  }
  return 0;
}

// std::string line;
// while(std::getline(in, line)) {
//   std::cout << line << '\n';
// }

// std::string text = R"(
//   bpm 120
//   instrument sax sampler
//       sample=./samples/hihc.wav
//       root=C5
//       loop=10,20
//       attack=0.01
//       release=0.1
//   end
// )";
