#pragma once

#include <fstream>

class WaveFile {
 public:
  WaveFile(std::string name);
  WaveFile(std::string name, float const sample_rate, int const bit_depth);
  ~WaveFile();

  WaveFile(WaveFile&&) = default;

  void set_audio_data(int value, int size = 2);

 private:
  void write_to_file(int value, int size);
  void set_header();

  std::string name_{};
  std::ofstream wave_file{name_, std::ios::binary};
  std::ofstream::pos_type pre_audio_position_{0};
  std::ofstream::pos_type pos_audio_postion_{0};
  float const sample_rate_{44100.0f};
  int const bit_depth_{16};
};