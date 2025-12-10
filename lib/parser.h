#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "song.h"

class Parser {
 public:
  void Parse(Song& song, std::istream& in);

  std::unique_ptr<Instrument> ParseSampler(const std::string& name,
                                           std::istream& in);

  void ParseSine(std::istream& in);

  void ParseSquare(std::istream& in);

  void ParseTriangle(std::istream& in);

  std::unique_ptr<Instrument> ParseInstrument(std::istream& in,
                                              const std::string& header);

  void ParsePattern();

  void ParseBpm();
};