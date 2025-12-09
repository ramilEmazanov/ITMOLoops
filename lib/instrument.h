#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "effect.h"

namespace fs = std::filesystem;

// Абстрактный класс
class Instrument {
 public:
  virtual void PlaySound(uint32_t start, float pitch, uint32_t duration,
                         uint8_t velocity) = 0;

  virtual ~Instrument() = 0;

 private:
  std::vector<Effect> effects_;
};

class SamplerInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

 private:
  std::vector<float> samples;
  float root_note_;
  uint32_t loop_begin_ = 0;
  uint32_t loop_end_ = 0;
};

class SquareInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

 private:
  uint8_t duty_ = 0;
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};

class SineInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

 private:
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};

class TriangleInstrument : public Instrument {
 public:
  void PlaySound(uint32_t start, float pitch, uint32_t duration,
                 uint8_t velocity) override;

 private:
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
};
