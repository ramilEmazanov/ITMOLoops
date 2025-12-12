#include "effect.h"

#include <iostream>
#include <memory>
#include <string>

void EchoEffect::ChangeSound() {}
void GainEffect::ChangeSound() {}
void TremoloEffect::ChangeSound() {}

void GainEffect::SetGain(float gain) { gain_ = gain; }

void EchoEffect::SetDelay(float delay) { delay_ = delay; }

void EchoEffect::SetDecay(float decay) { decay_ = decay; }

void TremoloEffect::SetFreq(uint32_t freq) { freq_ = freq; }

void TremoloEffect::SetDepth(float depth) { depth_ = depth; }

void GainEffect::GetGain() { std::cout << "gain : " << gain_ << '\n'; }

std::unique_ptr<Effect> CreateEffect(EffectType type) {
  switch (type) {
    case EffectType::kEcho:
      return std::make_unique<EchoEffect>();
    case EffectType::kGain:
      return std::make_unique<GainEffect>();
    case EffectType::kTremolo:
      return std::make_unique<TremoloEffect>();
  }
  return nullptr;
}