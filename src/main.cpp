#include "./include/oscillator.hpp"

int main(int argc, char** argv) {
  int constexpr duration = 5;

  auto saw_wave = Oscillator{440.0f, 1.0f, OSC::SAW_ANALOG};
  saw_wave.generate_sound("saw.wav", duration);

  return EXIT_SUCCESS;
}
