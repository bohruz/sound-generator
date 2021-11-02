#include "./oscillator/oscillator.hpp"

int main(int argc, char** argv) {
  auto saw_wave = Oscillator{440.0f, 1.0f, OSC::SAW_ANALOG};
  saw_wave.generate_sound("saw.wav", 5);

  return EXIT_SUCCESS;
}
