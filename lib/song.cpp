#include <cstdint>
#include <vector>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

Song::Song() = default;

void Song::SetBpm(uint16_t bpm) { bpm_ = bpm; }

void Song::GetBpm() { std::cout << bpm_ << '\n'; }

void Song::AddInstrument(const std::string& name,
                         std::unique_ptr<Instrument> inst) {
  instruments_.emplace(name, std::move(inst));
}

void Song::AddPattern(const std::string& name,
                      std::unique_ptr<Pattern> pattern) {
  patterns_.emplace(name, std::move(pattern));
}

const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
Song::GetInstruments() const {
  return instruments_;
}

int Song::GetInstrumentsSize() { return instruments_.size(); }

void Song::GetInstrumentsInfo() {
  int instruments_size = instruments_.size();
  std::cout << "Bpm : " << bpm_ << '\n';

  for (const auto& [name, inst_ptr] : instruments_) {
    std::cout << "Instrument: " << name << '\n';

    Instrument* inst = inst_ptr.get();
    inst->GetFields();

    int effects_size = inst->GetEffectsSize();
    std::cout << "Effects:\n";

    for (int j = 0; j < effects_size; ++j) {
      Effect* eff = inst->GetEffect(j);
      eff->GetFields();
    }

    std::cout << '\n';
  }
}

void Song::GetPatternsSize() { std::cout << patterns_.size() << '\n'; }

void Song::GetPatterns() {
  for (const auto& [name, patterns_ptr] : patterns_) {
    std::cout << "Pattern: " << name << '\n';
    for(int i = 0; i < patterns_ptr->GetEventsSize(); ++i) {
      Event* now = patterns_ptr->GetEvent(i);
      now->GetFields();
    }
    std::cout << '\n';
  }
}
