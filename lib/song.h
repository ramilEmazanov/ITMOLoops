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

  void AddInstrument(const std::string& name, std::unique_ptr<Instrument> inst);

  void SetBpm(uint16_t);

  void GetBpm();

  const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
  GetInstruments() const;

  int GetInstrumentsSize();

  void GetInstrumentsInfo();

 private:
  uint16_t bpm_;

  std::unordered_map<std::string, std::unique_ptr<Pattern>> patterns_;
  std::unordered_map<std::string, std::unique_ptr<Instrument>> instruments_;

};
