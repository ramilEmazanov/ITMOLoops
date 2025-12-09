#pragma once

#include <cstdint>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

class Song {
 public:
  Song(uint8_t bpm) : bpm_(bpm) {}

  void MakeSong();

 private:
  uint8_t bpm_;
  uint8_t beat_ = bpm_ / 60;
  std::vector<Instrument> instruments_;
  // std::vector<> instruments_;

};