#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "instrument.h"

enum class InstrumentType { kSampler, kSquare, kSine, kTriangle };

void Parse(std::istream& in);

std::unique_ptr<Instrument> CreateInstrument(InstrumentType type);

void Run(std::string song_description_path);