#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

#include "effect.h"

namespace fs = std::filesystem;

enum class InstrumentType { kSampler, kSquare, kSine, kTriangle };

class Instrument {
 public:
  virtual void PlaySound(uint32_t start, float pitch, uint32_t duration,
                         uint8_t velocity) = 0;

  virtual ~Instrument() = 0;

  void AddEffect(std::unique_ptr<Effect> eff);

  int GetEffectsSize();

 protected:
  std::vector<std::unique_ptr<Effect>> effects_;
};

class SamplerInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

  SamplerInstrument();

  ~SamplerInstrument();

  void SetPath(fs::path sample_path);

  void SetRoot(std::string root_note);

  void SetLoop(uint32_t loop_begin, uint32_t loop_end);

  void SetAttack(float attack);

  void SetRelease(float attack);

  void GetPath();

  void GetRootNote();

  void GetLoopBegin();

  void GetLoopEnd();

  void GetAttack();

  void GetRelease();

  GainEffect* GetGain() {
    return dynamic_cast<GainEffect*>(effects_[0].get());
  }

 private:
  fs::path sample_path_;
  std::vector<float> samples_;
  float root_note_;
  uint32_t loop_begin_ = 0;
  uint32_t loop_end_ = 0;
  float attack_ = 0;
  float release_ = 0;
};

class SquareInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

  SquareInstrument();

  ~SquareInstrument();

 private:
  uint8_t duty_ = 0;
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};

class SineInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

  SineInstrument();

  ~SineInstrument();

 private:
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};

class TriangleInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

  TriangleInstrument();
  ~TriangleInstrument();

 private:
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};

// std::unique_ptr<Instrument> CreateInstrument(InstrumentType type);