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

  std::unique_ptr<Instrument> ParseSine(const std::string& name,
                                        std::istream& in);

  std::unique_ptr<Instrument> ParseSquare(const std::string& name,
                                          std::istream& in);

  std::unique_ptr<Instrument> ParseTriangle(const std::string& name,
                                            std::istream& in);

  std::unique_ptr<Instrument> ParseInstrument(std::istream& in,
                                              const std::string& header);

  void ParseSingleArguments(SamplerInstrument* sampler, std::string line);

  void ParseGainEffect(GainEffect* gain_pointer, std::string gain_str);

  void ParseEchoEffect(EchoEffect* echo_pointer, std::string delay_str,
                       std::string decay_str);

  void ParseTremoloEffect(TremoloEffect* tremolo_pointer, std::string freq_str,
                          std::string depth_str);
  void ParsePattern();

  void ParseBpm();
};