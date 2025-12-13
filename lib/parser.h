#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "song.h"

class Parser {
 public:
  void Parse(Song& song, std::istream& in);

  std::unique_ptr<Instrument> ParseInstrument(std::istream& in,
                                              const std::string& header);

  std::unique_ptr<Instrument> ParseSampler(const std::string& name,
                                           std::istream& in);

  std::unique_ptr<Instrument> ParseSine(const std::string& name,
                                        std::istream& in);

  std::unique_ptr<Instrument> ParseSquare(const std::string& name,
                                          std::istream& in);

  std::unique_ptr<Instrument> ParseTriangle(const std::string& name,
                                            std::istream& in);

  void ParseEffect(std::unique_ptr<Instrument>& inst, std::string& line);

  void ParseGainEffect(GainEffect* gain_pointer, std::string gain_str);

  void ParseEchoEffect(EchoEffect* echo_pointer, std::string delay_str,
                       std::string decay_str);

  void ParseTremoloEffect(TremoloEffect* tremolo_pointer, std::string freq_str,
                          std::string depth_str);

  void ParseSamplerArguments(SamplerInstrument* sampler, std::string line);

  void ParseSquareArguments(SquareInstrument* square, std::string line);

  void ParseTriangleArguments(TriangleInstrument* triangle, std::string line);

  void ParseSineArguments(SineInstrument* sine, std::string line);

  std::unique_ptr<Pattern> ParsePattern(std::istream& in, std::string line);

  void ReadNotesFromFile();

  void PrintNotes() const;

 private:
  fs::path notes_path_ = "../notes.txt";
  std::unordered_map<std::string, float> notes_;
};