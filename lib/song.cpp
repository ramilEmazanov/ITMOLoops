#include <cstdint>
#include <vector>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

Song::Song() = default;

void Song::SetBpm(uint16_t bpm) { bpm_ = bpm; }

void Song::GetBpm() { std::cout << bpm_ << '\n'; }

void Song::AddInstrument(std::unique_ptr<Instrument> inst) {
  instruments_.push_back(std::move(inst));
}

Instrument* Song::GetInstrument(int n) { return instruments_[n].get(); }

int Song::GetInstrumentsSize() { return instruments_.size(); }
