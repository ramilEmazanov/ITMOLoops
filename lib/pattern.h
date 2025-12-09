#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Note {
 private:
  size_t start_;
  std::string instrument_;
  std::string pitch_;
  size_t duration_;
  size_t velocity_;

 public:
  Note(size_t start, std::string instrument, std::string pitch, size_t duration,
       size_t velocity);
  // : start_(start),
  //   instrument_(instrument),
  //   pitch_(pitch),
  //   duration_(duration),
  //   velocity_(velocity) {}
};

class Pattern {
 private:
  size_t resolution;
  std::vector<Note> notes_;
  std::vector<Pattern> patterns_;
};

class MainPattern {};  //???