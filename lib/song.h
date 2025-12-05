#include <cstdint>

class Song {
 private:
  uint8_t bpm_;
  uint8_t beat_ = bpm_ / 60;

 public:
  Song(uint8_t bpm) : bpm_(bpm) {}

  void MakeSong();

};