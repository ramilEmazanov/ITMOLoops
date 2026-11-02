#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

void WriteWavMono16(const std::string& path, const std::vector<float>& samples);
void Write16t(std::ostream& out, uint16_t num);
void Write32t(std::ostream& out, uint32_t num);
float Normalize(float raw_num);