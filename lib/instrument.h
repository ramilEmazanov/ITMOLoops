#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// struct Effect {
//   EffectType effect_type_;
//   // kGain
//   float gain_;
//   // kEcho
//   float delay_;
//   float decay_;
//   // kTremolo
//   size_t freq_;
//   float depth_;
// };

class Instrument {
 private:
  // Sample fields
  fs::path sample_path_;
  float root_note_;
  uint32_t loop_begin_ = 0;
  uint32_t loop_end_ = 0;
  // Sample, Square, Sine, Triangle fields
  uint8_t duty_ = 0;
  uint16_t attack_duration_ = 0;
  uint16_t release_duration_ = 0;
  // std::vector<Effect> effects_;

 public:
  Instrument(fs::path sample_path, float root_note,
             uint32_t loop_begin, uint32_t loop_end, uint8_t duty,
             uint16_t attack_duration, uint16_t release_duration);

  // here we calculate value
  size_t PlayNote(size_t start, size_t pitch, size_t duration, size_t velocity);
};
