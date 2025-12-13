#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Event {
 public:
  virtual ~Event() = default;

  void SetStart(uint32_t start);

  virtual void GetFields() = 0;

protected:
  uint32_t start_;
};

class NoteEvent : public Event {
 public:
  void SetInstrumentName(std::string name);
  void SetPitch(std::string pitch);
  void SetDuration(uint32_t duration);
  void SetVelocity(uint32_t velocity);
  
  void GetFields();
 private:
  std::string instrument_name_;
  std::string pitch_;
  uint32_t duration_;
  uint32_t velocity_;
};

class PatternEvent : public Event {
 public:
  void SetPatternName(std::string name);

  void GetFields();

 private:
  std::string pattern_name_;
};

class Pattern {
 public:
  void AddEvent(std::unique_ptr<Event> event);

  int GetEventsSize();

  Event* GetEvent(int n);

 private:
  std::vector<std::unique_ptr<Event>> events_;
};

// struct Note {
//  private:
//   size_t start_;
//   std::string instrument_;
//   std::string pitch_;
//   size_t duration_;
//   size_t velocity_;

//  public:
//   Note(size_t start, std::string instrument, std::string pitch, size_t
//   duration,
//        size_t velocity);
//   // : start_(start),
//   //   instrument_(instrument),
//   //   pitch_(pitch),
//   //   duration_(duration),
//   //   velocity_(velocity) {}
// };

// class Pattern {
//  private:
//   size_t resolution;
//   std::vector<Note> notes_;
//   std::vector<Pattern> patterns_;
// };
