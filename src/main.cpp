#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <fstream>
#include <iostream>

// General purpose oscillator
enum class OSC { SINE, SQUARE, TRIANGLE, SAW_ANALOG, SAW_DIGITAL, NOISE };
float w(float dHertz);
float oscilator(float dHertz, float dTime, OSC nType, float dLFOHertz,
                float dLFOAmplitude);

class WaveFile {
 public:
  WaveFile(std::string name) : name_{name} { set_header(); }
  WaveFile(std::string name, float const sample_rate, int const bit_depth)
      : name_{name}, sample_rate_{sample_rate}, bit_depth_{bit_depth} {
    set_header();
  }
  ~WaveFile() {
    wave_file.seekp(pre_audio_position_ - std::ofstream::off_type{4});
    write_to_file(pos_audio_postion_ - pre_audio_position_, 4);
    wave_file.seekp(4, std::ios::beg);
    write_to_file(pos_audio_postion_ - std::ofstream::off_type{4}, 4);
  }

  WaveFile(WaveFile&&) = default;

  void set_audio_data(int value, int size = 2) {
    write_to_file(value, size);
    pos_audio_postion_ += std::ofstream::off_type{1};
  }

 private:
  void write_to_file(int value, int size) {
    wave_file.write(reinterpret_cast<const char*>(&value), size);
  }
  void set_header() {
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

  void set_size() {}

  std::string name_{};
  std::ofstream wave_file{name_, std::ios::binary};
  std::ofstream::pos_type pre_audio_position_{0};
  std::ofstream::pos_type pos_audio_postion_{0};
  float const sample_rate_{44100.0f};
  int const bit_depth_{16};
};

class SineOscillator {
 public:
  SineOscillator(float freq, float amp) : frequency_{freq}, amplitude_{amp} {
    w_ = (2.0f * M_PI * frequency_) / sample_rate_;
  }

  void generate_sound(int const duration) {
    auto audio_file = WaveFile{"SineWave.wav"};

    auto const max_amplitude = pow(2, bit_depth_ - 1) - 1;
    for (auto i{0}; i < sample_rate_ * duration; ++i) {
      // auto sample = amplitude_ * sin(w_ * i);
      auto sample = amplitude_ * sin(w_ * i) > 0 ? 1.0 : -1.0;
      // auto sample =
      //    amplitude_ * oscilator(frequency_, i, OSC::SINE, 0.0f, 0.0f);
      auto int_sample = static_cast<int>(sample * max_amplitude);

      audio_file.set_audio_data(int_sample);
    }
  }

 private:
  float const sample_rate_{44100.0f};
  int const bit_depth_{16};
  float frequency_{0.0f}, amplitude_{0.0f}, w_{0.0f};

  void write_to_file(std::ostream& wave_file, int value, int size) {
    wave_file.write(reinterpret_cast<const char*>(&value), size);
  }
};

int main(int argc, char** argv) {
  int constexpr duration = 5;

  auto sineOscillator = SineOscillator{440.0f, 0.5f};
  sineOscillator.generate_sound(duration);

  return EXIT_SUCCESS;
}

// General purpose oscillator
float w(float dHertz) { return 2.0 * M_PI * dHertz; };
float oscilator(float dHertz, float dTime, OSC nType, float dLFOHertz,
                float dLFOAmplitude) {
  float dFreq = w(dHertz) * dTime + dLFOAmplitude * sin(w(dLFOHertz) * dTime);

  switch (nType) {
    case OSC::SINE:  // Sine wave bewteen -1 and +1
      return sin(dFreq);

    case OSC::SQUARE:  // Square wave between -1 and +1
      return sin(dFreq) > 0 ? 1.0 : -1.0;

    case OSC::TRIANGLE:  // Triangle wave between -1 and +1
      return asin(sin(dFreq)) * (2.0 / M_PI);

    case OSC::SAW_ANALOG:  // Saw wave (analogue / warm / slow)
    {
      float dOutput = 0.0;

      for (float n = 1.0; n < 100.0; n++) dOutput += (sin(n * dFreq)) / n;

      return dOutput * (2.0 / M_PI);
    }

    case OSC::SAW_DIGITAL:  // Saw Wave (optimised / harsh / fast)
      return (2.0 / M_PI) *
             (dHertz * M_PI * fmod(dTime, 1.0 / dHertz) - (M_PI / 2.0));

    case OSC::NOISE:  // Pseudorandom noise
      return 2.0 * ((float)rand() / (float)RAND_MAX) - 1.0;

    default:
      return 0.0;
  }
}