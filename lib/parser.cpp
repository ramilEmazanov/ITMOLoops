#include "parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "song.h"

constexpr uint32_t kSampleRate = 44100;

namespace fs = std::filesystem;

namespace detail {

bool StartsWith(const std::string& compared, const std::string& prefix) {
  if (compared.size() < prefix.size()) {
    return false;
  }
  return !compared.compare(0, prefix.size(), prefix);
}
}  // namespace detail

using namespace detail;

void Parser::Parse(Song& song, std::string& txt_path) {
  std::ifstream in(txt_path, std::ios::binary);
  ReadNotesFromFile();
  std::string line;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line.empty() || line[0] == '#') {
      continue;
    }
    std::stringstream ss(line);
    std::string data_type;
    ss >> data_type;

    if (data_type == "bpm") {
      std::string bpm_str;
      ss >> bpm_str;
      uint16_t bpm = static_cast<uint16_t>(std::stoi(bpm_str));
      song.SetBpm(bpm);
    } else if (data_type == "instrument") {
      std::unique_ptr<Instrument> inst = Parser::ParseInstrument(in, line);
      std::string name;
      ss >> name;
      song.AddInstrument(name, std::move(inst));
    } else if (data_type == "pattern") {
      std::string name;
      ss >> name;
      std::unique_ptr<Pattern> pattern = Parser::ParsePattern(in, line);
      song.AddPattern(name, std::move(pattern));
    }
  }
}

std::unique_ptr<Instrument> Parser::ParseInstrument(std::istream& in,
                                                    const std::string& header) {
  std::stringstream ss(header);
  std::string keyword, name, type;
  ss >> keyword >> name >> type;

  if (type == "sampler") {
    return ParseSampler(name, in);
  } else if (type == "square") {
    return ParseSquare(name, in);
  } else if (type == "sine") {
    return ParseSine(name, in);
  } else if (type == "triangle") {
    return ParseTriangle(name, in);
  }
  return nullptr;
}

std::unique_ptr<Instrument> Parser::ParseSampler(const std::string& name,
                                                 std::istream& in) {
  std::unique_ptr<Instrument> inst = std::make_unique<SamplerInstrument>();
  SamplerInstrument* sampler = dynamic_cast<SamplerInstrument*>(inst.get());
  Instrument* insts = inst.get();

  std::string line;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line == "end") break;
    if (StartsWith(line, "effect")) {
      ParseEffect(inst, line);
    } else {
      ParseSamplerArguments(sampler, line);
    }
  }
  fs::path temp = sampler->GetPath();
  std::string filename = temp.filename().string();
  fs::path sample_path = fs::path("../samples") / filename;
  std::ifstream sample_file(sample_path, std::ios::binary);

  ExtractSamples(sample_file, sampler->GetBuffer());
  return inst;
}

std::unique_ptr<Instrument> Parser::ParseSquare(const std::string& name,
                                                std::istream& in) {
  std::unique_ptr<Instrument> inst = std::make_unique<SquareInstrument>();
  SquareInstrument* square = dynamic_cast<SquareInstrument*>(inst.get());

  std::string line;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line == "end") break;
    if (StartsWith(line, "effect")) {
      ParseEffect(inst, line);
    } else {
      ParseSquareArguments(square, line);
    }
  }
  return inst;
}

std::unique_ptr<Instrument> Parser::ParseTriangle(const std::string& name,
                                                  std::istream& in) {
  std::unique_ptr<Instrument> inst = std::make_unique<TriangleInstrument>();
  TriangleInstrument* triangle = dynamic_cast<TriangleInstrument*>(inst.get());

  std::string line;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line == "end") break;
    if (StartsWith(line, "effect")) {
      ParseEffect(inst, line);
    } else {
      ParseTriangleArguments(triangle, line);
    }
  }
  return inst;
}

std::unique_ptr<Instrument> Parser::ParseSine(const std::string& name,
                                              std::istream& in) {
  std::unique_ptr<Instrument> inst = std::make_unique<SineInstrument>();
  SineInstrument* sine = dynamic_cast<SineInstrument*>(inst.get());

  std::string line;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line == "end") break;

    if (StartsWith(line, "effect")) {
      ParseEffect(inst, line);
    } else {
      ParseSineArguments(sine, line);
    }
  }
  return inst;
}

void Parser::ParseEffect(std::unique_ptr<Instrument>& inst, std::string& line) {
  std::stringstream effects(line);
  std::string effect_type;
  effects >> effect_type >> effect_type;
  if (effect_type == "gain") {
    std::unique_ptr<GainEffect> eff = std::make_unique<GainEffect>();
    GainEffect* gain_pointer = eff.get();
    std::string gain_str;
    effects >> gain_str;
    ParseGainEffect(gain_pointer, gain_str);
    inst->AddEffect(std::move(eff));
  } else if (effect_type == "echo") {
    std::unique_ptr<EchoEffect> eff = std::make_unique<EchoEffect>();
    EchoEffect* echo_pointer = eff.get();
    std::string delay_str, decay_str;
    effects >> delay_str >> decay_str;
    ParseEchoEffect(echo_pointer, delay_str, decay_str);
    inst->AddEffect(std::move(eff));
  } else if (effect_type == "tremolo") {
    std::unique_ptr<TremoloEffect> eff = std::make_unique<TremoloEffect>();
    TremoloEffect* tremolo_pointer = eff.get();
    std::string freq_str, depth_str;
    effects >> freq_str >> depth_str;
    ParseTremoloEffect(tremolo_pointer, freq_str, depth_str);
    inst->AddEffect(std::move(eff));
  }
}

void Parser::ParseGainEffect(GainEffect* gain_pointer, std::string gain_str) {
  gain_str = gain_str.substr(gain_str.find('=') + 1);
  float gain = std::stof(gain_str);
  gain_pointer->SetGain(gain);
}

void Parser::ParseEchoEffect(EchoEffect* echo_pointer, std::string delay_str,
                             std::string decay_str) {
  delay_str = delay_str.substr(delay_str.find('=') + 1);
  decay_str = decay_str.substr(decay_str.find('=') + 1);
  float delay = std::stof(delay_str);
  float decay = std::stof(decay_str);
  echo_pointer->SetDelay(delay);
  echo_pointer->SetDecay(decay);
}

void Parser::ParseTremoloEffect(TremoloEffect* tremolo_pointer,
                                std::string freq_str, std::string depth_str) {
  freq_str = freq_str.substr(freq_str.find('=') + 1);
  depth_str = depth_str.substr(depth_str.find('=') + 1);
  uint32_t freq = std::stoul(freq_str);
  float depth = std::stof(depth_str);
  tremolo_pointer->SetFreq(freq);
  tremolo_pointer->SetDepth(depth);
}

void Parser::ParseSamplerArguments(SamplerInstrument* sampler,
                                   std::string line) {
  std::string sample_path, root_str, loop_begin_str, loop_end_str, attack_str,
      release_str;
  if (StartsWith(line, "sample")) {
    sample_path = line.substr(line.find("=") + 1);
    sampler->SetPath(sample_path);
  } else if (StartsWith(line, "root")) {
    root_str = line.substr(line.find("=") + 1);
    float root = notes_[root_str];
    sampler->SetRoot(root);
  } else if (StartsWith(line, "loop")) {
    size_t equal_pos = line.find('=');
    size_t dot_pos = line.find(',');
    loop_begin_str = line.substr(equal_pos + 1, dot_pos - (equal_pos + 1));
    loop_end_str = line.substr(dot_pos + 1);
    uint32_t loop_begin = std::stoul(loop_begin_str);
    uint32_t loop_end = std::stoul(loop_end_str);
    sampler->SetLoop(loop_begin, loop_end);
  } else if (StartsWith(line, "attack")) {
    attack_str = line.substr(line.find('=') + 1);
    float attack = std::stof(attack_str);
    uint32_t attack_samples = static_cast<uint32_t>(attack * kSampleRate);
    sampler->SetAttack(attack_samples);
  } else if (StartsWith(line, "release")) {
    release_str = line.substr(line.find('=') + 1);
    float release = std::stof(release_str);
    uint32_t release_samples = static_cast<uint32_t>(release * kSampleRate);
    sampler->SetRelease(release_samples);
  }
}

void Parser::ParseSquareArguments(SquareInstrument* square, std::string line) {
  std::string duty_str, attack_str, release_str;
  if (StartsWith(line, "attack")) {
    attack_str = line.substr(line.find('=') + 1);
    float attack = std::stof(attack_str);
    uint32_t attack_samples = static_cast<uint32_t>(attack * kSampleRate);
    square->SetAttack(attack_samples);
  } else if (StartsWith(line, "release")) {
    release_str = line.substr(line.find('=') + 1);
    float release = std::stof(release_str);
    uint32_t release_samples = static_cast<uint32_t>(release * kSampleRate);
    square->SetRelease(release_samples);
  } else if (StartsWith(line, "duty")) {
    duty_str = line.substr(line.find('=') + 1);
    uint16_t duty = std::stoi(duty_str);
    square->SetDuty(duty);
  }
}

void Parser::ParseTriangleArguments(TriangleInstrument* triangle,
                                    std::string line) {
  std::string attack_str, release_str;
  if (StartsWith(line, "attack")) {
    attack_str = line.substr(line.find('=') + 1);
    float attack = std::stof(attack_str);
    uint32_t attack_samples = static_cast<uint32_t>(attack * kSampleRate);
    triangle->SetAttack(attack_samples);
  } else if (StartsWith(line, "release")) {
    release_str = line.substr(line.find('=') + 1);
    float release = std::stof(release_str);
    uint32_t release_samples = static_cast<uint32_t>(release * kSampleRate);
    triangle->SetRelease(release_samples);
  }
}

void Parser::ParseSineArguments(SineInstrument* sine, std::string line) {
  std::string attack_str, release_str;
  if (StartsWith(line, "attack")) {
    attack_str = line.substr(line.find('=') + 1);
    float attack = std::stof(attack_str);
    uint32_t attack_samples = static_cast<uint32_t>(attack * kSampleRate);
    sine->SetAttack(attack_samples);
  } else if (StartsWith(line, "release")) {
    release_str = line.substr(line.find('=') + 1);
    float release = std::stof(release_str);
    uint32_t release_samples = static_cast<uint32_t>(release * kSampleRate);
    sine->SetRelease(release_samples);
  }
}

std::unique_ptr<Pattern> Parser::ParsePattern(std::istream& in,
                                              std::string line) {
  std::unique_ptr<Pattern> new_pattern = std::make_unique<Pattern>();

  std::stringstream ss(line);
  std::string name;
  std::string resolution;
  ss >> name >> name >> resolution >> resolution;
  while (std::getline(in, line)) {
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);
    if (line == "end") break;
    std::stringstream event(line);
    std::string event_start, event_name;
    event >> event_start >> event_name;
    uint32_t start_int = std::stoi(event_start);
    uint32_t resolution_int = std::stoi(resolution);
    if (event_name[0] == '@') {
      std::unique_ptr<PatternEvent> pattern_event =
          std::make_unique<PatternEvent>();
      pattern_event->SetStart(start_int);
      event_name = event_name.substr(1);
      pattern_event->SetPatternName(event_name);
      new_pattern->AddEvent(std::move(pattern_event));
      new_pattern->SetResolution(resolution_int);
    } else {
      std::unique_ptr<NoteEvent> note_event = std::make_unique<NoteEvent>();
      std::string instrument_name, pitch_str, duration_str, velocity_str;
      instrument_name = event_name;
      event >> pitch_str >> duration_str >> velocity_str;
      uint32_t duration = std::stoi(duration_str);
      uint32_t velocity = std::stoi(velocity_str);
      float pitch = notes_[pitch_str];
      note_event->SetStart(start_int);
      note_event->SetInstrumentName(instrument_name);
      note_event->SetPitch(pitch);
      note_event->SetDuration(duration);
      note_event->SetVelocity(velocity);
      new_pattern->AddEvent(std::move(note_event));
      new_pattern->SetResolution(resolution_int);
    }
  }
  return new_pattern;
}

void Parser::ReadNotesFromFile() {
  std::ifstream in(notes_path_);
  std::string line;
  while (std::getline(in, line)) {
    std::stringstream ss(line);
    std::string note, value_str;
    ss >> note >> value_str;
    float value = std::stof(value_str);
    notes_.emplace(note, value);
  }
}

#include <cstdint>
#include <cstring>
#include <istream>
#include <vector>

uint16_t Parser::Read16tNumber(std::istream& in) {
  uint16_t ans = 0;
  for (int i = 0; i < 2; ++i) {
    char reading = 0;
    in.get(reading);
    uint16_t temp = static_cast<uint8_t>(reading);
    temp <<= (8 * i);
    ans += temp;
  }
  return ans;
}

uint32_t Parser::Read32tNumber(std::istream& in) {
  uint32_t ans = 0;
  for (int i = 0; i < 4; ++i) {
    char reading = 0;
    in.get(reading);
    uint32_t temp = static_cast<uint8_t>(reading);
    temp <<= (8 * i);
    ans += temp;
  }
  return ans;
}

int16_t Parser::ReadSample(std::istream& in) {
  uint16_t temp = Read16tNumber(in);
  return static_cast<int16_t>(temp);
}

float Parser::ExtractFloatSample(std::istream& in) {
  float sample = ReadSample(in) / 32768.0f;
  return sample;
}

void Parser::ExtractSamples(std::istream& in, std::vector<float>& samples) {
  samples.clear();

  in.seekg(40, std::ios::beg);
  uint32_t data_size = Read32tNumber(in);
  uint32_t sample_count = data_size / 2;

  for (uint32_t i = 0; i < sample_count; ++i) {
    float s = ExtractFloatSample(in);
    if (!in) break;
    samples.push_back(s);
  }
}