#include "process.h"
#include <string>

int main(int argc, const char* argv[]) {
  std::string txt_path = argv[1];
  std::string wav_path = argv[2];
  Run(txt_path, wav_path);
  return 0;
}
