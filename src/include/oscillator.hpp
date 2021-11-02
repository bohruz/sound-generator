#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "./wavefile.hpp"

enum class OSC { SINE, SQUARE, TRIANGLE, SAW_ANALOG, SAW_DIGITAL, NOISE };

class Oscillator {
 public:
  Oscillator(float freq, float amp, OSC type);

  void generate_sound(int const duration);

 private:
  // General purpose oscillator
  float getWave(float angle, OSC nType);
  float const sample_rate_{44100.0f};
  int const bit_depth_{16};
  float frequency_{0.0f}, amplitude_{0.0f}, w_{0.0f};
  OSC type_;

  void write_to_file(std::ostream& wave_file, int value, int size);
};
