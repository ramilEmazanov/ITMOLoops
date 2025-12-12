#include <string>

#include "instrument.h"
#include "process.h"
#include "song.h"

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  fs::path txt_path = argv[1];
  std::string wav_path = "";
  Run(txt_path, wav_path);
  // std::ifstream in(txt_path);


  return 0;
}

//////////////////////
// INSTRUMENT PARSING TEST
// Song song;
// Parser parser;
// parser.Parse(song, in);
// int instruments_size = song.GetInstrumentsSize();
// song.GetBpm();

// for (const auto& [name, inst_ptr] : song.GetInstruments()) {
//   std::cout << "Instrument: " << name << '\n';

//   Instrument* inst = inst_ptr.get();
//   inst->GetFields();

//   int effects_size = inst->GetEffectsSize();
//   std::cout << "Effects:\n";

//   for (int j = 0; j < effects_size; ++j) {
//     Effect* eff = inst->GetEffect(j);
//     eff->GetFields();
//   }

//   std::cout << '\n';
// }
/////////////////////////
