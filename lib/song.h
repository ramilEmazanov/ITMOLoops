#pragma once

#include <cstdint>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

class Song {
 public:
  Song();

  void AddInstrument(std::unique_ptr<Instrument> inst);
 private:
  uint8_t bpm_;
  uint8_t beat_ = bpm_ / 60;
  std::vector<std::unique_ptr<Instrument>> instruments_;

};