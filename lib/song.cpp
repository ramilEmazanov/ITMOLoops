#include <algorithm>
#include <cstdint>
#include <vector>

#include "effect.h"
#include "instrument.h"
#include "pattern.h"
#include "process.h"

// Calculates the latest ending of a note -> it is buffer length
uint32_t Song::FindInstrumentBufferLength(const std::string& name) {
  uint32_t instrument_buffer_len = 0;
  for (int i = 0; i < note_calls_.size(); ++i) {
    Note& note = note_calls_[i];
    const std::string& instrument_name = note.GetInstrumentName();
    if (name == instrument_name) {
      instrument_buffer_len =
          std::max(instrument_buffer_len, note.GetStart() + note.GetDuration());
    }
  }
  return instrument_buffer_len;
}

uint32_t Song::FindSongDuration() {
  uint32_t ans = 0;
  for (const auto& [name, inst_ptr] : instruments_) {
    uint32_t temp = FindInstrumentBufferLength(name);
    ans = std::max(ans, temp);
  }
  return ans;
}

void Song::RenderSong() {
  // std::vector<float> Song
  for (const auto& [name, inst_ptr] : instruments_) {
    Instrument* inst = inst_ptr.get();
    uint32_t buffer_length = FindInstrumentBufferLength(name);
    std::vector<float> instrument_sounds(buffer_length);

    for (int i = 0; i < note_calls_.size(); ++i) {
      const std::string& instrument_name = note_calls_[i].GetInstrumentName();
      if (name == instrument_name) {
        // std::cout << name << '\n';
        // inst.RenderSound();
      }
    }
  }
}

uint32_t Note::GetDuration() const { return duration_samples_; }

std::string& Note::GetInstrumentName() { return instrument_; }

Song::Song() = default;

void Song::SetBpm(uint16_t bpm) { bpm_ = bpm; }

void Song::GetBpm() { std::cout << bpm_ << '\n'; }

uint32_t Note::GetStart() const { return start_sample_; }

int Song::GetInstrumentsSize() { return instruments_.size(); }

void Song::GetInstrumentsInfo() {
  int instruments_size = instruments_.size();
  std::cout << "Bpm : " << bpm_ << '\n';

  for (const auto& [name, inst_ptr] : instruments_) {
    std::cout << "Instrument: " << name << '\n';

    Instrument* inst = inst_ptr.get();
    inst->GetFields();

    int effects_size = inst->GetEffectsSize();
    std::cout << "Effects:\n";

    for (int j = 0; j < effects_size; ++j) {
      Effect* eff = inst->GetEffect(j);
      eff->GetFields();
    }

    std::cout << '\n';
  }
}

void Song::GetPatternsSize() { std::cout << patterns_.size() << '\n'; }

void Song::GetPatterns() {
  for (const auto& [name, patterns_ptr] : patterns_) {
    std::cout << "Pattern: " << name << " Resolution "
              << patterns_ptr->GetResolution() << '\n';
    for (int i = 0; i < patterns_ptr->GetEventsSize(); ++i) {
      Event* now = patterns_ptr->GetEvent(i);
      now->PrintFields();
    }
    std::cout << '\n';
  }
}

const std::unordered_map<std::string, std::unique_ptr<Instrument>>&
Song::GetInstruments() const {
  return instruments_;
}

void Song::GetDuration() { std::cout << song_duration_samples_ << '\n'; }

void Song::AddInstrument(const std::string& name,
                         std::unique_ptr<Instrument> inst) {
  instruments_.emplace(name, std::move(inst));
}

void Song::AddPattern(const std::string& name,
                      std::unique_ptr<Pattern> pattern) {
  patterns_.emplace(name, std::move(pattern));
}

uint32_t Song::UnitToSample(uint32_t unit, uint32_t resolution) {
  uint32_t sample = (unit * 60 * 44100) / (bpm_ * resolution);
  return sample;
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

void Song::PrintNoteCalls() {
  for (int i = 0; i < note_calls_.size(); ++i) {
    note_calls_[i].PrintNote();
  }
}

void Note::PrintNote() {
  std::cout << start_sample_ << " " << instrument_ << " " << pitch_ << " "
            << duration_samples_ << " " << velocity_ << '\n';
}

void Song::SortNoteCalls() {
  std::sort(note_calls_.begin(), note_calls_.end(), cmp);
  SetDuration();
}

bool Song::cmp(const Note& lhs, const Note& rhs) {
  return lhs.GetStart() > rhs.GetStart();
}
