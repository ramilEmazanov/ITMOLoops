#include <algorithm>
#include <cstdint>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "wav_writer.h"
#include "song.h"

float Note::GetPitch() const { return pitch_; }

uint32_t Note::GetVelocity() const { return velocity_; }

void Song::RenderSong(std::string music_name) {
  ExpandMain();
  SortNoteCalls();
  uint32_t max_release = 0;
  uint64_t song_len = 0;
  for (const auto& note : note_calls_) {
    Instrument* temp = instruments_[note.GetInstrumentName()].get();
    uint32_t release = temp->GetRelease();
    max_release = std::max(release, max_release);
    song_len =
        std::max<uint64_t>(song_len, note.GetStart() + note.GetDuration());
  }
  song_len += max_release;

  std::vector<float> song(song_len, 0.0f);
  for (const auto& [name, inst_ptr] : instruments_) {
    Instrument* inst = inst_ptr.get();
    std::vector<float> inst_buf(song_len, 0.0f);
    for (const auto& note : note_calls_) {
      if (note.GetInstrumentName() != name) {
        continue;
      }
      inst->PlaySound(inst_buf, note.GetStart(), note.GetPitch(),
                      note.GetDuration(), note.GetVelocity());
    }

    for (int ei = 0; ei < inst->GetEffectsSize(); ++ei) {
      Effect* eff = inst->GetEffect(ei);
      eff->ChangeSound(inst_buf);
    }

    for (size_t i = 0; i < song.size(); ++i) {
      song[i] += inst_buf[i];
    }
  }

  WriteWavMono16(music_name, song);
  return;
}

uint32_t Note::GetDuration() const { return duration_samples_; }

const std::string& Note::GetInstrumentName() const { return instrument_; }

void Song::SetBpm(uint16_t bpm) {
  bpm_ = bpm; 
  return;
}

uint32_t Note::GetStart() const { return start_sample_; }

int Song::GetInstrumentsSize() { return instruments_.size(); }

const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
Song::GetInstruments() const {
  return instruments_;
}

void Song::AddInstrument(const std::string& name,
                         std::unique_ptr<Instrument> inst) {
  instruments_.emplace(name, std::move(inst));
}

void Song::AddPattern(const std::string& name,
                      std::unique_ptr<Pattern> pattern) {
  patterns_.emplace(name, std::move(pattern));
}

uint64_t Song::UnitToSample(uint64_t unit, uint32_t resolution) {
  uint64_t num = unit * 60 * 44100;
  uint64_t den = bpm_ * resolution;
  return static_cast<uint64_t>(num / den);
}

void Song::ExpandPattern(const std::string& pattern_name,
                         uint32_t pattern_start_samples) {
  Pattern* pattern = patterns_[pattern_name].get();
  uint32_t resolution = pattern->GetResolution();

  for (int i = 0; i < pattern->GetEventsSize(); ++i) {
    Event* event = pattern->GetEvent(i);
    if (event->GetEventKind() == EventKind::kNote) {
      NoteEvent* note_event = static_cast<NoteEvent*>(event);
      uint32_t note_start_samples = UnitToSample(event->GetStart(), resolution);
      uint32_t abs_note_start = pattern_start_samples + note_start_samples;
      uint32_t duration_samples =
          UnitToSample(note_event->GetDuration(), resolution);
      Note note(abs_note_start, note_event->GetInstrumentName(),
                note_event->GetPitch(), duration_samples,
                note_event->GetVelocity());
      note_calls_.push_back(note);
    } else {
      PatternEvent* pattern = static_cast<PatternEvent*>(event);
      uint32_t delta_start_samples =
          UnitToSample(event->GetStart(), resolution);
      std::string pattern_called = pattern->GetPatternName();
      ExpandPattern(pattern_called,
                    pattern_start_samples + delta_start_samples);
    }
  }
  return;
}
void Song::ExpandMain() { ExpandPattern("main", 0); }

void Song::SetDuration() { song_duration_samples_ = note_calls_[0].GetStart(); }

void Song::SortNoteCalls() {
  std::sort(note_calls_.begin(), note_calls_.end(), Cmp);
  SetDuration();
}

bool Song::Cmp(const Note& lhs, const Note& rhs) {
  return lhs.GetStart() > rhs.GetStart();
}
