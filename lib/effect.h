#pragma once

#include <cstdint>
#include <memory>

enum class EffectType { kGain, kEcho, kTremolo };

class Effect {
 public:
  virtual void ChangeSound() = 0;

  virtual void GetFields();

  virtual ~Effect() = default;
};

class GainEffect : public Effect {
 public:
  void ChangeSound() override;

  void SetGain(float gain);

  void GetGain();

  void GetFields() override;

 private:
  float gain_;
};

class EchoEffect : public Effect {
 public:
  void ChangeSound() override;

  void SetDelay(float delay);

  void SetDecay(float decay);

  void GetFields() override;

 private:
  float delay_;
  float decay_;
};

class TremoloEffect : public Effect {
 public:
  void ChangeSound() override;

  void SetFreq(uint32_t freq);

  void SetDepth(float depth);

  void GetFields() override;

 private:
  uint32_t freq_;
  float depth_;
};

std::unique_ptr<Effect> CreateEffect(EffectType type);