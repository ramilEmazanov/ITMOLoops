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
  virtual void PlaySound(uint32_t start, float pitch, uint32_t duration,
                         uint32_t velocity) = 0;

  virtual ~Instrument() = 0;

  void AddEffect(std::unique_ptr<Effect> eff);

  int GetEffectsSize();

  virtual void GetFields() = 0;

  void SetAttack(float attack);

  void SetRelease(float release);

  Effect* GetEffect(int n);

 protected:
  float attack_ = 0;
  float release_ = 0;
  std::vector<std::unique_ptr<Effect>> effects_;
  std::vector<float> samples_;
};

class SamplerInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint32_t velocity) override;

  SamplerInstrument();

  ~SamplerInstrument();

  void SetPath(fs::path sample_path);

  void SetRoot(float root_note);

  void SetLoop(uint32_t loop_begin, uint32_t loop_end);

  void GetFields() override;

  GainEffect* GetGain() { return dynamic_cast<GainEffect*>(effects_[0].get()); }

 private:
  fs::path sample_path_;
  std::vector<float> samples_;
  float root_note_;
  uint32_t loop_begin_ = 0;
  uint32_t loop_end_ = 0;
};

class SquareInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint32_t velocity) override;

  SquareInstrument();

  void GetFields() override;

  ~SquareInstrument();

  void SetDuty(uint32_t duty);

 private:
  uint32_t duty_ = 0;
};

class SineInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint32_t velocity) override;

  SineInstrument();
  void GetFields() override;

  ~SineInstrument();

 private:
  uint32_t attack_duration_ = 0;
  uint32_t release_duration_ = 0;
};

class TriangleInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint32_t velocity) override;

  TriangleInstrument();

  void GetFields() override;

  ~TriangleInstrument();
};