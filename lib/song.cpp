#include <cstdint>
#include <vector>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

Song::Song() = default;

void Song::AddInstrument(std::unique_ptr<Instrument> inst) {
  instruments_.push_back(std::move(inst));
}
