#pragma once
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "effect.h"

namespace fs = std::filesystem;

enum class InstrumentType { kSampler, kSquare, kSine, kTriangle };

class Instrument {
 public:
  virtual void PlaySound(std::vector<float>& out, uint32_t start,
                         float pitch_hz, uint32_t duration,
                         uint32_t velocity) = 0;
  virtual ~Instrument() = default;
  float NormalizeVelocity(uint32_t velocity);
  float AttackRelease(uint32_t current_sample, uint32_t duration_samples);
  void AddEffect(std::unique_ptr<Effect> eff);

  void SetAttack(uint32_t attack);
  void SetRelease(uint32_t release);

  uint32_t GetRelease();
  int GetEffectsSize() const;
  Effect* GetEffect(int n) const;

  const std::vector<std::unique_ptr<Effect>>& GetEffects();

 protected:
  uint32_t attack_ = 0;
  uint32_t release_ = 0;
  std::vector<std::unique_ptr<Effect>> effects_;
};

class SamplerInstrument : public Instrument {
 public:
  void PlaySound(std::vector<float>& out, uint32_t start, float pitch_hz,
                 uint32_t duration, uint32_t velocity) override;

  float LinearInterpolate(float a, float b, float t);

  void SetPath(fs::path sample_path);
  void SetRoot(float root_note);
  void SetLoop(uint32_t loop_begin, uint32_t loop_end);

  fs::path GetPath() const;
  std::vector<float>& GetBuffer();
  GainEffect* GetGain() const {
    return dynamic_cast<GainEffect*>(effects_[0].get());
  }

 private:
  std::vector<float> samples_;
  fs::path sample_path_;
  float root_note_;
  uint32_t loop_begin_ = 0;
  uint32_t loop_end_ = 0;
};

class SquareInstrument : public Instrument {
 public:
  void PlaySound(std::vector<float>& out, uint32_t start, float pitch_hz,
                 uint32_t duration, uint32_t velocity) override;

  void SetDuty(uint32_t duty);

 private:
  uint32_t duty_ = 0;
};

class SineInstrument : public Instrument {
 public:
  void PlaySound(std::vector<float>& out, uint32_t start, float pitch_hz,
                 uint32_t duration, uint32_t velocity) override;

};

class TriangleInstrument : public Instrument {
 public:
  void PlaySound(std::vector<float>& out, uint32_t start, float pitch_hz,
                 uint32_t duration, uint32_t velocity) override;
  float TriangleValue(float phase);
};