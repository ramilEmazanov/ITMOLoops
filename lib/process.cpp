#include "process.h"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "instrument.h"

void Run(std::string song_description_path) {
  std::ifstream in(song_description_path, std::ios::binary);
  Parse(in);
}

std::unique_ptr<Instrument> CreateInstrument(InstrumentType type) {
  switch (type) {
    case InstrumentType::kSampler:
      return std::make_unique<SamplerInstrument>();
    case InstrumentType::kSine:
      return std::make_unique<SineInstrument>();
    case InstrumentType::kSquare:
      return std::make_unique<SquareInstrument>();
    case InstrumentType::kTriangle:
      return std::make_unique<TriangleInstrument>();
  }
  return std::make_unique<SamplerInstrument>();
}

void Parse(std::istream& in) {
  std::vector<std::unique_ptr<Instrument>> instruments;
  bool instrument = true;
  InstrumentType type;
  if (instrument) {
    std::unique_ptr<Instrument> inst = CreateInstrument(type);
    instruments.push_back(inst);

  }
}