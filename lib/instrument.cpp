#include "instrument.h"

void SamplerInstrument::PlaySound(uint32_t start, float pitch, uint32_t duration,
                                 uint8_t velocity) {
  uint32_t start_unit = start;
  uint32_t duration_unit = duration;
  int bpm;
  int resolution;

  float new_note = pitch;
  float root_note = root_note_;

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

    if (higher_ind >= static_cast<int>(samples.size())) {
      break;
    }

    float frc = src_idx - lower_ind;

    note_signals[i] =
        samples[lower_ind] * (1 - frc) + samples[higher_ind] * frc;
  }

  // return note_signals;
  return;
}