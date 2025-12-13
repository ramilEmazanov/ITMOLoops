#include "pattern.h"

#include <iostream>
#include <string>

void Event::SetStart(uint32_t start) { start_ = start; }

void PatternEvent::SetPatternName(std::string name) { pattern_name_ = name; }

void Pattern::AddEvent(std::unique_ptr<Event> event) {
  events_.push_back(std::move(event));
}

void NoteEvent::SetInstrumentName(std::string name) { instrument_name_ = name; }
void NoteEvent::SetPitch(std::string pitch) { pitch_ = pitch; }
void NoteEvent::SetDuration(uint32_t duration) { duration_ = duration; }
void NoteEvent::SetVelocity(uint32_t velocity) { velocity_ = velocity; }

void NoteEvent::GetFields() {
  std::cout << "Note Event : ";
  std::cout << start_ << " " << instrument_name_ << " " << pitch_ << " " << duration_
            << velocity_ << '\n';
}

void PatternEvent::GetFields() {
  std::cout << "Pattern Event : ";
  std::cout << start_ << " " << pattern_name_ << '\n';
}

int Pattern::GetEventsSize() {
  return events_.size();
}

Event* Pattern::GetEvent(int n) { return events_[n].get(); }
