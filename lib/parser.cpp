#include "parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

#include "song.h"

namespace detail {

bool StartsWith(const std::string& compared, const std::string& prefix) {
  if (compared.size() < prefix.size()) {
    return false;
  }
  return !compared.compare(0, prefix.size(), prefix);
}

InstrumentType DefineInstrumentType(const std::string& str) {
  if (str == "sampler") {
    return InstrumentType::kSampler;
  } else if (str == "square") {
    return InstrumentType::kSquare;
  } else if (str == "sine") {
    return InstrumentType::kSine;
  }
  return InstrumentType::kTriangle;
}
}  // namespace detail

using namespace detail;

void Parser::ParsePattern() {}

void Parser::ParseBpm() {}

void Parser::ParseSingleArguments(SamplerInstrument* sampler,
                                  std::string line) {
  std::string sample_path, root, loop_begin_str, loop_end_str, attack_str,
      release_str;
  if (StartsWith(line, "sample")) {
    sample_path = line.substr(line.find("=") + 1);
    sampler->SetPath(sample_path);
  } else if (StartsWith(line, "root")) {
    root = line.substr(line.find("=") + 1);
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
    sampler->SetAttack(attack);
  } else if (StartsWith(line, "release")) {
    release_str = line.substr(line.find('=') + 1);
    float release = std::stof(release_str);
    sampler->SetRelease(release);
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

std::unique_ptr<Instrument> Parser::ParseSampler(const std::string& name,
                                                 std::istream& in) {
  std::unique_ptr<SamplerInstrument> inst =
      std::make_unique<SamplerInstrument>();
  SamplerInstrument* sampler = inst.get();

  std::string line;
  while (std::getline(in, line)) {
    if (line == "end") break;
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);

    if (StartsWith(line, "effect")) {
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
    } else {
      ParseSingleArguments(sampler, line);
    }
  }
  return inst;
}

std::unique_ptr<Instrument> Parser::ParseSquare(const std::string& name,
                                                std::istream& in) {
  std::unique_ptr<SquareInstrument> inst =
      std::make_unique<SquareInstrument>();
  SquareInstrument* square = inst.get();

  std::string line;
  while (std::getline(in, line)) {
    if (line == "end") break;
    size_t start = line.find_first_not_of(" ");
    if (start == std::string::npos) continue;
    line.erase(0, start);

    if (StartsWith(line, "effect")) {
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
    } else {
      ParseSingleArguments(sampler, line);
    }
  }
  return inst;
}

std::unique_ptr<Instrument> Parser::ParseTriangle(const std::string& name,
                                                  std::istream& in) {}

std::unique_ptr<Instrument> Parser::ParseSine(const std::string& name,
                                              std::istream& in) {}

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

void Parser::Parse(Song& song, std::istream& in) {
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
      song.AddInstrument(std::move(inst));
    } else if (data_type == "pattern") {
      Parser::ParsePattern();
    }
  }
}

/*
1. song хранит вектор указателей - почему?
  Потому что есть object slicing. Вектор инструментов не сможет хранить
  классы-наследники
*/
