#include "effect.h"

constexpr uint32_t kSampleRate = 44100;
constexpr float kTwoPi = 6.28f;

void GainEffect::ChangeSound(std::vector<float>& buffer) {
  for (auto& s : buffer) {
    s *= gain_;
  }
}

void EchoEffect::ChangeSound(std::vector<float>& buffer) {
  if (delay_ <= 0.0f || decay_ == 0.0f) {
    return;
  }

  uint32_t delay_samples = static_cast<uint32_t>(delay_ * kSampleRate);
  if (delay_samples == 0 || delay_samples >= buffer.size()) {
    return;
  }
  for (size_t i = delay_samples; i < buffer.size(); ++i) {
    buffer[i] += buffer[i - delay_samples] * decay_;
  }
}

void TremoloEffect::ChangeSound(std::vector<float>& buffer) {
  if (depth_ <= 0.0f || freq_ <= 0.0f) return;

  float argument = 0.0f;
  float argument_delta_samples = kTwoPi * freq_ / (static_cast<float>(kSampleRate));
  for (size_t i = 0; i < buffer.size(); ++i) {
    float modulation = (1.0f - depth_) + depth_ * std::abs(std::sin(argument));
    buffer[i] *= modulation;
    argument += argument_delta_samples;
  }
}

void GainEffect::SetGain(float gain) { gain_ = gain; }
float GainEffect::GetGain() const { return gain_; }

void EchoEffect::SetDelay(float delay) { delay_ = delay; }
void EchoEffect::SetDecay(float decay) { decay_ = decay; }

void TremoloEffect::SetFreq(uint32_t freq) { freq_ = freq; }
void TremoloEffect::SetDepth(float depth) { depth_ = depth; }