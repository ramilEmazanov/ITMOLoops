#include "pattern.h"

#include <string>
#include <memory>

void Event::SetStart(uint32_t start) { start_ = start; }

uint32_t Event::GetStart() const { return start_; }

void PatternEvent::SetPatternName(std::string name) { pattern_name_ = name; }

void Pattern::AddEvent(std::unique_ptr<Event> event) {
  events_.push_back(std::move(event));
}

void NoteEvent::SetInstrumentName(const std::string& name) { instrument_name_ = name; }
void NoteEvent::SetPitch(float pitch) { pitch_ = pitch; }
void NoteEvent::SetDuration(uint32_t duration) { duration_ = duration; }
void NoteEvent::SetVelocity(uint32_t velocity) { velocity_ = velocity; }

const std::string& NoteEvent::GetInstrumentName() { return instrument_name_; }
float NoteEvent::GetPitch() { return pitch_; }
uint32_t NoteEvent::GetDuration() { return duration_; }
uint32_t NoteEvent::GetVelocity() { return velocity_; }

int Pattern::GetEventsSize() { return events_.size(); }

Event* Pattern::GetEvent(int n) { return events_[n].get(); }

void Pattern::SetResolution(uint32_t resolution) { resolution_ = resolution; }

uint32_t Pattern::GetResolution() { return resolution_; }

EventKind NoteEvent::GetEventKind() { return EventKind::kNote; }

EventKind PatternEvent::GetEventKind() { return EventKind::kPattern; }

std::string& PatternEvent::GetPatternName() { return pattern_name_; }
