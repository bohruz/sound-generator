#include "./include/oscillator.hpp"

Oscillator::Oscillator(float freq, float amp, OSC type)
    : frequency_{freq}, amplitude_{amp}, type_{type} {
  w_ = (2.0f * M_PI * frequency_) / sample_rate_;
}

void Oscillator::generate_sound(std::string name, int const duration) {
  auto audio_file = WaveFile{name};

  auto const max_amplitude = pow(2, bit_depth_ - 1) - 1;
  for (auto i{0}; i < sample_rate_ * duration; ++i) {
    auto sample = amplitude_ * get_sound_wave_value(w_ * i, type_);

    auto int_sample = static_cast<int>(sample * max_amplitude);

    audio_file.set_audio_data(int_sample);
  }
}
// General purpose oscillator
float Oscillator::get_sound_wave_value(float angle, OSC nType) {
  switch (nType) {
    case OSC::SINE:  // Sine wave bewteen -1 and +1
      return sin(angle);

    case OSC::SQUARE:  // Square wave between -1 and +1
      return sin(angle) > 0 ? 1.0 : -1.0;

    case OSC::TRIANGLE:  // Triangle wave between -1 and +1
      return asin(sin(angle)) * (2.0 / M_PI);

    case OSC::SAW_ANALOG:  // Saw wave (analogue / warm / slow)
    {
      float dOutput = 0.0;

      for (float n = 1.0; n < 100.0; n++) dOutput += (sin(n * angle)) / n;

      return dOutput * (2.0 / M_PI);
    }

      // case OSC::SAW_DIGITAL:  // Saw Wave (optimised / harsh / fast)
      //   return (2.0 / M_PI) *
      //          (dHertz * M_PI * fmod(dTime, 1.0 / dHertz) - (M_PI / 2.0));

    case OSC::NOISE:  // Pseudorandom noise
      return 2.0 * ((float)rand() / (float)RAND_MAX) - 1.0;

    default:
      return 0.0;
  }
}

void Oscillator::write_to_file(std::ostream& wave_file, int value, int size) {
  wave_file.write(reinterpret_cast<const char*>(&value), size);
}
