#include "wavefile.hpp"

WaveFile::WaveFile(std::string name) : name_{name} { set_header(); }
WaveFile::WaveFile(std::string name, float const sample_rate,
                   int const bit_depth)
    : name_{name}, sample_rate_{sample_rate}, bit_depth_{bit_depth} {
  set_header();
}
WaveFile::~WaveFile() {
  wave_file.seekp(pre_audio_position_ - std::ofstream::off_type{4});
  write_to_file(pos_audio_postion_ - pre_audio_position_, 4);
  wave_file.seekp(4, std::ios::beg);
  write_to_file(pos_audio_postion_ - std::ofstream::off_type{4}, 4);
}

void WaveFile::set_audio_data(int value, int size) {
  write_to_file(value, size);
  pos_audio_postion_ += std::ofstream::off_type{1};
}

void WaveFile::write_to_file(int value, int size) {
  wave_file.write(reinterpret_cast<const char*>(&value), size);
}
void WaveFile::set_header() {
  // Wav file definition

  // Header chunk
  wave_file << "RIFF";
  wave_file << "----";
  wave_file << "WAVE";

  // Format chunk
  wave_file << "fmt ";
  write_to_file(16, 4);                             // Size
  write_to_file(1, 2);                              // Compression code
  write_to_file(1, 2);                              // Number of channels
  write_to_file(sample_rate_, 4);                   // Sample rate
  write_to_file(sample_rate_ * bit_depth_ / 8, 4);  // Byte rate
  write_to_file(bit_depth_ / 8, 2);                 // Block align
  write_to_file(bit_depth_, 2);                     // Bit depth

  // Data chunk
  wave_file << "data";
  wave_file << "----";

  pre_audio_position_ = wave_file.tellp();
  pos_audio_postion_ = wave_file.tellp();
}
