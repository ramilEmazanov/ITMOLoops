#include "parser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

std::unique_ptr<Instrument> Parser::ParseSampler(const std::string& name,
                                                 std::istream& in) {
  std::string sample, root, loop_begin, loop_end, attack, release;
  std::string line;
  
  while (std::getline(in, line)) {
    if (line == "end") {
      break;
    }
    size_t start = line.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
      continue;
    }
    line.erase(0, start);
    if (StartsWith(line, "sample")) {
      sample = line.substr(line.find("=") + 1);
    } else if (StartsWith(line, "root")) {
      root = line.substr(line.find("=") + 1);
    } else if (StartsWith(line, "loop")) {
      size_t equal_pos = line.find('=');
      size_t dot_pos = line.find(',');
      loop_begin = line.substr(equal_pos + 1, dot_pos - (equal_pos + 1));
      loop_end = line.substr(dot_pos + 1);
    } else if (StartsWith(line, "attack")) {
      attack = line.substr(line.find('=') + 1);
    } else if (StartsWith(line, "release")) {
      release = line.substr(line.find('=') + 1);
    } else if (StartsWith(line, "effect")) {
      std::stringstream effects(line);
      std::string effect_type;
      effects >> effect_type >> effect_type;
      if (effect_type == "gain") {
        std::string gain;
        effects >> gain;
        gain = gain.substr(gain.find('=') + 1);
      } else if (effect_type == "echo") {
        std::string delay, decay;
        effects >> delay >> decay;
        delay = delay.substr(delay.find('=') + 1);
        decay = decay.substr(decay.find('=') + 1);
      } else if (effect_type == "tremolo") {
        std::string freq, depth;
        effects >> freq >> depth;
        freq = freq.substr(freq.find('=') + 1);
        depth = depth.substr(depth.find('=') + 1);
      }
    }

    std::cout << line << '\n';
  }
  std::unique_ptr<Instrument> inst = CreateInstrument(InstrumentType::kSampler);

  return inst;
}

void Parser::ParseSquare(std::istream& in) {}

void Parser::ParseTriangle(std::istream& in) {}

void Parser::ParseSine(std::istream& in) {}

std::unique_ptr<Instrument> Parser::ParseInstrument(std::istream& in,
                                                    const std::string& header) {
  std::stringstream ss(header);
  std::string keyword, name, type;
  ss >> keyword >> name >> type;

  if (type == "sampler") {
    return ParseSampler(name, in);
  } 
  // else if (type == "square") {
  //   return ParseSampler(name, in);
  //   // return ParseSquare(in);
  // } else if (type == "sine") {
  //   return ParseSampler(name, in);
  //   ParseSine(in);
  // } else if (type == "triangle") {
  //   return ParseSampler(name, in);
  //   ParseTriangle(in);
  // }
  return nullptr;
}

void Parser::Parse(Song& song, std::istream& in) {
  std::string line;
  while (std::getline(in, line)) {
    std::stringstream ss(line);
    std::string data_type;
    ss >> data_type;

    if (data_type == "bpm") {
      Parser::ParseBpm();
    } else if (data_type == "instrument") {
      std::unique_ptr<Instrument> inst = Parser::ParseInstrument(in, line);
      song.AddInstrument(std::move(inst));
    } else if (data_type == "pattern") {
      Parser::ParsePattern();
    }
  }
}

/*
Документация
*/