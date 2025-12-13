#include "instrument.h"

#include <iostream>

Effect* Instrument::GetEffect(int n) { return effects_[n].get(); }

Instrument::~Instrument() = default;

SamplerInstrument::SamplerInstrument() = default;

SamplerInstrument::~SamplerInstrument() = default;

SquareInstrument::SquareInstrument() = default;

SquareInstrument::~SquareInstrument() = default;

SineInstrument::SineInstrument() = default;

SineInstrument::~SineInstrument() = default;

TriangleInstrument::TriangleInstrument() = default;

TriangleInstrument::~TriangleInstrument() = default;

int Instrument::GetEffectsSize() { return effects_.size(); }

void Instrument::AddEffect(std::unique_ptr<Effect> eff) {
  effects_.push_back(std::move(eff));
}

void SamplerInstrument::GetFields() {
  std::cout << "Sample Path : " << sample_path_ << '\n';
  std::cout << "Root note : " << root_note_ << '\n';
  std::cout << "Loop begin : " << loop_begin_ << '\n';
  std::cout << "Loop end : " << loop_end_ << '\n';
  std::cout << "Attack : " << attack_ << '\n';
  std::cout << "Release : " << release_ << '\n';
}

void SquareInstrument::GetFields() {
  std::cout << "Duty : " << duty_ << '\n';
  std::cout << "Attack : " << attack_ << '\n';
  std::cout << "Release : " << release_ << '\n';
}

void SineInstrument::GetFields() {
  std::cout << "Attack : " << attack_ << '\n';
  std::cout << "Release : " << release_ << '\n';
}

void TriangleInstrument::GetFields() {
  std::cout << "Attack : " << attack_ << '\n';
  std::cout << "Release : " << release_ << '\n';
}

void Instrument::SetAttack(float attack) { attack_ = attack; }

void Instrument::SetRelease(float release) { release_ = release; }

void SquareInstrument::SetDuty(uint8_t duty) { duty_ = duty; }

void SamplerInstrument::SetPath(fs::path sample_path) {
  sample_path_ = sample_path;
}

void SamplerInstrument::SetRoot(float root_note) {
  root_note_ = root_note;
}

void SamplerInstrument::SetLoop(uint32_t loop_begin, uint32_t loop_end) {
  loop_begin_ = loop_begin;
  loop_end_ = loop_end;
}

void SamplerInstrument::PlaySound(uint32_t start, float pitch,
                                  uint32_t duration, uint8_t velocity) {
  uint32_t start_unit = start;
  uint32_t duration_unit = duration;
  int bpm;
  int resolution;

  float new_note = pitch;
  float root_note = 1; /* root_note_ */

  // перевод одного юнита в секунду
  float unit_to_sec = 60.0f / (bpm * resolution);

  // находим место старта ноты в секундах
  float start_sec = start_unit * (unit_to_sec);
  float duration_sec = duration_unit * (unit_to_sec);

  // длительность проигрывания в сэмплах - сколько сэмплов нам нужно вытащить
  // для того чтобы сыграть ноту
  int note_length_sample = 44100 * duration_sec;
  // хранятся нужные нам значения сэмплов
  // то что мы сюда положим - решение задачи
  std::vector<float> note_signals(note_length_sample, 0.0f);

  // коэффицент ресемплирования
  float pitch_factor = new_note / root_note;

  for (int i = 0; i < note_length_sample; ++i) {
    float src_idx = i * pitch_factor;

    int lower_ind = static_cast<int>(src_idx);
    int higher_ind = lower_ind + 1;

    if (higher_ind >= static_cast<int>(samples_.size())) {
      break;
    }

    float frc = src_idx - lower_ind;

    note_signals[i] =
        samples_[lower_ind] * (1 - frc) + samples_[higher_ind] * frc;
  }

  // return note_signals;
  return;
}

void SquareInstrument::PlaySound(uint32_t start, float pitch, uint32_t duration,
                                 uint8_t velocity) {}

void SineInstrument::PlaySound(uint32_t start, float pitch, uint32_t duration,
                               uint8_t velocity) {}

void TriangleInstrument::PlaySound(uint32_t start, float pitch,
                                   uint32_t duration, uint8_t velocity) {}
