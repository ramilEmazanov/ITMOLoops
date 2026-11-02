#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <memory>

enum class EventKind { kPattern, kNote };

class Event {
 public:
  virtual ~Event() = default;
  virtual EventKind GetEventKind() = 0;

  void SetStart(uint32_t start);
  uint32_t GetStart() const;

 protected:
  uint32_t start_;
};

class NoteEvent : public Event {
 public:
  void SetInstrumentName(const std::string& name);
  void SetPitch(float pitch);
  void SetDuration(uint32_t duration);
  void SetVelocity(uint32_t velocity);

  const std::string& GetInstrumentName();
  float GetPitch();
  uint32_t GetDuration();
  uint32_t GetVelocity();

  EventKind GetEventKind();

 private:
  std::string instrument_name_;
  float pitch_;
  uint32_t duration_;
  uint32_t velocity_;
};

class PatternEvent : public Event {
 public:
  void SetPatternName(std::string name);

  EventKind GetEventKind();

  std::string& GetPatternName();

 private:
  std::string pattern_name_;
};

class Pattern {
 public:
  void AddEvent(std::unique_ptr<Event> event);

  void SetResolution(uint32_t);

  uint32_t GetResolution();
  int GetEventsSize();
  Event* GetEvent(int n);

 private:
  std::vector<std::unique_ptr<Event>> events_;
  uint32_t resolution_;
};