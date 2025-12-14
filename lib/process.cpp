#include "process.h"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "instrument.h"
#include "parser.h"
#include "song.h"

void Run(std::string txt_path, std::string wav_path) {
  Song song;
  Parser parser;
  parser.Parse(song, txt_path);
  song.RenderSong(wav_path);
}