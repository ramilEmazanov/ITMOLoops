#include <string>

#include "instrument.h"
#include "process.h"
#include "song.h"

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  fs::path txt_path = argv[1];
  std::string wav_path = argv[2];
  Run(txt_path, wav_path);
  return 0;
}