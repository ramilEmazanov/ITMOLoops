#pragma once

#include <cstdint>

class Effect {

};

class GainEffect : public Effect {
 private:
  float gain_;
};

class EchoEffect : public Effect {
 private:
  float delay_;
  float decay_;
};

class TremoloEffect : public Effect {
 private:
  uint32_t freq_;
  float depth_;
};
