#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

class Song {
 public:
  Song();

  void AddInstrument(std::unique_ptr<Instrument> inst);

  void SetBpm(uint16_t);

  void GetBpm();

  Instrument* GetInstrument(int n);

  int GetInstrumentsSize();

 private:
  uint16_t bpm_;
  // uint16_t beat_ = bpm_ / 60;

  std::vector<std::unique_ptr<Instrument>> instruments_;
  // std::vector<std::unique_ptr<Pattern>> patterns_;

  std::unordered_map<std::string, Pattern> patterns_;
  // std::unordered_map<std::string, Instrument> instruments_;

};