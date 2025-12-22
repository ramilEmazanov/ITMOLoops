#include "instrument.h"

#include <math.h>

constexpr uint32_t kSampleRate = 44100;
constexpr float kTwoPi = 6.28f;

float Instrument::NormalizeVelocity(uint32_t velocity) {
  return velocity / 100.0f;
}

float Instrument::AttackRelease(uint32_t current_sample,
                                uint32_t duration_samples) {
  float gain = 1.0f;

  if (attack_ > 0 && current_sample < attack_) {
    gain = static_cast<float>(current_sample) / static_cast<float>(attack_);
  }

  if (current_sample + release_ >= duration_samples) {
    uint32_t left = duration_samples - current_sample;
    gain =
        std::min(gain, static_cast<float>(left) / static_cast<float>(release_));
  }

  if (gain < 0.0f) {
    gain = 0.0f;
  }
  if (gain > 1.0f) {
    gain = 1.0f;
  }
  return gain;
}

void SineInstrument::PlaySound(std::vector<float>& out, uint32_t start,
                               float pitch, uint32_t duration,
                               uint32_t velocity) {
  float normalized_velocity = NormalizeVelocity(velocity);
  if (start >= out.size()) {
    return;
  }
  uint32_t actual_duration =
      std::min<uint32_t>(duration, static_cast<uint32_t>(out.size()) - start) +
      release_;
  for (uint32_t ith_sample = 0; ith_sample < actual_duration; ++ith_sample) {
    float t = static_cast<float>(start + ith_sample) /
              static_cast<float>(kSampleRate);
    float amplitude = std::sin(kTwoPi * pitch * t);
    float gain = AttackRelease(ith_sample, actual_duration);
    out[start + ith_sample] += amplitude * normalized_velocity * gain;
  }
}

void SquareInstrument::PlaySound(std::vector<float>& out, uint32_t start,
                                 float pitch, uint32_t duration,
                                 uint32_t velocity) {
  float normalized_velocity = NormalizeVelocity(velocity);
  if (start >= out.size()) {
    return;
  }
  uint32_t actual_duration =
      std::min<uint32_t>(duration, static_cast<uint32_t>(out.size()) - start) +
      release_;
  float duty = duty_ / 100.0f;
  float phase = 0.0f;
  float step_per_sample = pitch / static_cast<float>(kSampleRate);

  for (uint32_t ith_sample = 0; ith_sample < actual_duration; ++ith_sample) {
    phase += step_per_sample;
    if (phase >= 1.0f) {
      phase -= 1.0f;
    }
    float func_value = -1.0f;
    if (phase < duty) {
      func_value = 1.0f;
    }
    float gain = AttackRelease(ith_sample, actual_duration);
    out[start + ith_sample] += func_value * normalized_velocity * gain;
  }
}

float TriangleInstrument::TriangleValue(float phase) {
  float func_value = 0.0f;
  if (phase < 0.5f) {
    func_value = phase * 4.0f - 1.0f;
  } else {
    func_value = 3.0f - phase * 4.0f;
  }
  return func_value;
}

void TriangleInstrument::PlaySound(std::vector<float>& out, uint32_t start,
                                   float pitch, uint32_t duration,
                                   uint32_t velocity) {
  float normalized_velocity = NormalizeVelocity(velocity);
  if (start >= out.size()) {
    return;
  }
  uint32_t actual_duration =
      std::min<uint32_t>(duration, static_cast<uint32_t>(out.size()) - start) +
      release_;
  float phase = 0.0f;
  float step = pitch / static_cast<float>(kSampleRate);
  for (uint32_t ith_sample = 0; ith_sample < actual_duration; ++ith_sample) {
    phase += step;
    if (phase >= 1.0f) {
      phase -= 1.0f;
    }
    float func_value = TriangleValue(phase);
    float gain = AttackRelease(ith_sample, actual_duration);
    out[start + ith_sample] += func_value * normalized_velocity * gain;
  }
}

float SamplerInstrument::LinearInterpolate(float a, float b, float t) {
  return a + (b - a) * t;
}

void SamplerInstrument::PlaySound(std::vector<float>& out, uint32_t start,
                                  float pitch, uint32_t duration,
                                  uint32_t velocity) {
  float normalized_velocity = NormalizeVelocity(velocity);
  if (samples_.empty() || start >= out.size()) {
    return;
  }
  uint32_t actual_duration =
      std::min<uint32_t>(duration, static_cast<uint32_t>(out.size()) - start) +
      release_;

  float ratio = pitch / root_note_;
  float position = 0.0f;

  uint32_t loop_begin = loop_begin_;
  uint32_t loop_end = loop_end_;
  bool loop_ok = (loop_end > loop_begin) &&
                 (loop_end <= static_cast<uint32_t>(samples_.size()));

  for (uint32_t ith_sample = 0; ith_sample < actual_duration; ++ith_sample) {
    uint32_t lower_ind = static_cast<uint32_t>(position);
    uint32_t higher_ind = lower_ind + 1;

    if (lower_ind >= static_cast<uint32_t>(samples_.size())) {
      break;
    }
    if (higher_ind >= static_cast<uint32_t>(samples_.size())) {
      higher_ind = lower_ind;
    }

    float frac = position - static_cast<float>(lower_ind);
    float func_value =
        LinearInterpolate(samples_[lower_ind], samples_[higher_ind], frac);

    float gain = AttackRelease(ith_sample, actual_duration);

    out[start + ith_sample] += func_value * normalized_velocity * gain;

    position += ratio;

    if (loop_ok && position >= static_cast<float>(loop_end)) {
      position = loop_begin;
    }
  }
}

Effect* Instrument::GetEffect(int n) const { return effects_[n].get(); }

const std::vector<std::unique_ptr<Effect>>& Instrument::GetEffects() {
  return effects_;
}

int Instrument::GetEffectsSize() const { return effects_.size(); }

uint32_t Instrument::GetRelease() { return release_; }

void Instrument::AddEffect(std::unique_ptr<Effect> eff) {
  effects_.push_back(std::move(eff));
}

void Instrument::SetAttack(uint32_t attack) { attack_ = attack; }

void Instrument::SetRelease(uint32_t release) { release_ = release; }

void SquareInstrument::SetDuty(uint32_t duty) { duty_ = duty; }

void SamplerInstrument::SetPath(fs::path sample_path) {
  sample_path_ = sample_path;
}

fs::path SamplerInstrument::GetPath() const { return sample_path_; }

std::vector<float>& SamplerInstrument::GetBuffer() { return samples_; }

void SamplerInstrument::SetRoot(float root_note) { root_note_ = root_note; }

void SamplerInstrument::SetLoop(uint32_t loop_begin, uint32_t loop_end) {
  loop_begin_ = loop_begin;
  loop_end_ = loop_end;
}