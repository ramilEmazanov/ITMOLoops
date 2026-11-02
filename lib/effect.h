#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

enum class EffectType { kGain, kEcho, kTremolo };

class Effect {
 public:
  virtual void ChangeSound(std::vector<float>& buffer) = 0;
  virtual ~Effect() = default;
};

class GainEffect : public Effect {
 public:
  void ChangeSound(std::vector<float>& buffer) override;

  void SetGain(float gain);
  float GetGain() const;

 private:
  float gain_ = 1.0f;
};

class EchoEffect : public Effect {
 public:
  void ChangeSound(std::vector<float>& buffer) override;

  void SetDelay(float delay);
  void SetDecay(float decay);

 private:
  float delay_ = 0.0f;
  float decay_ = 0.0f;
};

class TremoloEffect : public Effect {
 public:
  void ChangeSound(std::vector<float>& buffer) override;

  void SetFreq(uint32_t freq);
  void SetDepth(float depth);

 private:
  uint32_t freq_ = 5;
  float depth_ = 0.0f;
};
