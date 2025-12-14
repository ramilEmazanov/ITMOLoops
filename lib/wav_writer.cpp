#include "wav_writer.h"

#include <algorithm>
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

/*
[Master RIFF chunk]
   FileTypeBlocID  (4 bytes) : Identifier « RIFF »  (0x52, 0x49, 0x46, 0x46)
   FileSize        (4 bytes) : Overall file size minus 8 bytes
   FileFormatID    (4 bytes) : Format = « WAVE »  (0x57, 0x41, 0x56, 0x45)

[Chunk describing the data format]
   FormatBlocID    (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
   BlocSize        (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
   AudioFormat     (2 bytes) : Audio format (1: PCM integer, 3: IEEE 754 float)
   NbrChannels     (2 bytes) : Number of channels
   Frequency       (4 bytes) : Sample rate (in hertz)
   BytePerSec      (4 bytes) : Number of bytes to read per second (Frequency * BytePerBloc).
   BytePerBloc     (2 bytes) : Number of bytes per block (NbrChannels * BitsPerSample / 8).
   BitsPerSample   (2 bytes) : Number of bits per sample

[Chunk containing the sampled data]
   DataBlocID      (4 bytes) : Identifier « data »  (0x64, 0x61, 0x74, 0x61)
   DataSize        (4 bytes) : SampledData size
   SampledData
*/
