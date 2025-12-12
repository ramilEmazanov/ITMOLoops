#include "effect.h"

#include <iostream>
#include <memory>
#include <string>

void Effect::GetFields() {}

void EchoEffect::ChangeSound() {}
void GainEffect::ChangeSound() {}
void TremoloEffect::ChangeSound() {}

void EchoEffect::GetFields() {
  std::cout << "EchoEffect\n";
  std::cout << "Delay : " << delay_ << '\n';
  std::cout << "Decay : " << decay_ << '\n';
}
void GainEffect::GetFields() {
  std::cout << "GainEffect\n";
  std::cout << "Gain : " << gain_ << '\n';
}

void TremoloEffect::GetFields() {
  std::cout << "TremoloEffect\n";
  std::cout << "Freq : " << freq_ << '\n';
  std::cout << "Depth : " << depth_ << '\n';
}

void GainEffect::SetGain(float gain) { gain_ = gain; }

void EchoEffect::SetDelay(float delay) { delay_ = delay; }

void EchoEffect::SetDecay(float decay) { decay_ = decay; }

void TremoloEffect::SetFreq(uint32_t freq) { freq_ = freq; }

void TremoloEffect::SetDepth(float depth) { depth_ = depth; }

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