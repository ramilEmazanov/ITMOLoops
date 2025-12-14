#pragma once

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

namespace fs = std::filesystem;

struct Note {
 private:
  uint32_t start_sample_;
  std::string instrument_;
  float pitch_;
  uint32_t duration_samples_;
  size_t velocity_;

 public:
  Note() {}
  Note(uint32_t start, std::string instrument, float pitch, uint32_t duration,
       size_t velocity)
      : start_sample_(start),
        instrument_(instrument),
        pitch_(pitch),
        duration_samples_(duration),
        velocity_(velocity) {}

  uint32_t GetStart() const;
  float GetPitch() const;
  uint32_t GetVelocity() const;
  uint32_t GetDuration() const;
  const std::string& GetInstrumentName() const;
};

class Song {
 public:
  Song();

  void AddInstrument(const std::string& name, std::unique_ptr<Instrument> inst);
  void AddPattern(const std::string& name, std::unique_ptr<Pattern> pattern);
  static bool cmp(const Note& lhs, const Note& rhs);
  void ExpandMain();
  void ExpandPattern(const std::string&, uint32_t);
  uint64_t UnitToSample(uint64_t unit, uint32_t resolution);

  void SortNoteCalls();

  void SetBpm(uint16_t);
  void SetDuration();

  const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
  GetInstruments() const;
  int GetInstrumentsSize();
  void ExtractSamplersSamples();
  void RenderSong(std::string music_name);

 private:
  uint16_t bpm_;
  uint32_t song_duration_samples_;
  std::vector<Note> note_calls_;
  std::unordered_map<std::string, std::unique_ptr<Pattern>> patterns_;
  std::unordered_map<std::string, std::unique_ptr<Instrument>> instruments_;
};
