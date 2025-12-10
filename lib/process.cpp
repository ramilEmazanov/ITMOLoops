#include "process.h"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "instrument.h"
#include "song.h"
#include "parser.h"

void Run(std::string txt_path, std::string wav_path) {
  std::ifstream in(txt_path, std::ios::binary);
  Song song;
  Parser parser;
  parser.Parse(song, in);
}