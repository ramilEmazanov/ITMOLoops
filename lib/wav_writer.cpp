#include "wav_writer.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

void WriteWavMono16(const std::string& path,
                    const std::vector<float>& samples) {
  std::ofstream out(path, std::ios::binary | std::ios::trunc);

  uint16_t audio_format = 1;
  uint16_t channels = 1;
  uint32_t frequency = 44100;
  uint16_t byte_per_block = 2;
  uint32_t byte_per_sec = 88200;
  uint16_t bits_per_sample = 16;
  uint32_t data_size = static_cast<uint32_t>(samples.size() * byte_per_block);

  out.write("RIFF", 4);
  Write32t(out, 36 + data_size);
  out.write("WAVE", 4);

  out.write("fmt ", 4);
  Write32t(out, 16);
  Write16t(out, audio_format);
  Write16t(out, channels);
  Write32t(out, frequency);
  Write32t(out, byte_per_sec);
  Write16t(out, byte_per_block);
  Write16t(out, bits_per_sample);

  out.write("data", 4);
  Write32t(out, data_size);

  for (float x : samples) {
    float amplitude = Normalize(x);
    int16_t amplitude_int = static_cast<int16_t>(amplitude * 32767.0f);
    Write16t(out, static_cast<uint16_t>(amplitude_int));
  }
}

void Write16t(std::ostream& out, uint16_t num) {
  out.put(static_cast<char>(num & 0xFF));
  out.put(static_cast<char>((num >> 8) & 0xFF));
}

void Write32t(std::ostream& out, uint32_t num) {
  out.put(static_cast<char>(num & 0xFF));
  out.put(static_cast<char>((num >> 8) & 0xFF));
  out.put(static_cast<char>((num >> 16) & 0xFF));
  out.put(static_cast<char>((num >> 24) & 0xFF));
}

float Normalize(float raw_num) {
  if (raw_num > 1.0f) {
    return 1.0f;
  }
  if (raw_num < -1.0f) {
    return -1.0f;
  }
  return raw_num;
}
