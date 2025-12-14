#pragma once

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

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

  void PrintNote();

  uint32_t GetStart() const;
  uint32_t GetDuration() const;
  std::string& GetInstrumentName();
};

namespace fs = std::filesystem;

class Song {
 public:
  Song();

  void AddInstrument(const std::string& name, std::unique_ptr<Instrument> inst);
  void AddPattern(const std::string& name, std::unique_ptr<Pattern> pattern);
  static bool cmp(const Note& lhs, const Note& rhs);
  void ReadNotes();
  void ExpandMain();
  void ExpandPattern(const std::string&, uint32_t);
  uint32_t UnitToSample(uint32_t unit, uint32_t resolution);
  void PrintNoteCalls();
  void SortNoteCalls();

  void SetBpm(uint16_t);
  void SetDuration();

  const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
  GetInstruments() const;
  int GetInstrumentsSize();
  void GetInstrumentsInfo();
  void GetPatternsSize();
  void GetPatterns();
  void GetDuration();
  void GetNotes();
  void GetBpm();
  uint32_t FindInstrumentBufferLength(const std::string& name);
  uint32_t FindSongDuration();
  void ExtractSamplersSamples();

  void RenderSong();

 private:
  uint16_t bpm_;
  uint32_t song_duration_samples_;
  std::vector<Note> note_calls_;
  std::unordered_map<std::string, std::unique_ptr<Pattern>> patterns_;
  std::unordered_map<std::string, std::unique_ptr<Instrument>> instruments_;
};
