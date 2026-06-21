#define _USE_MATH_DEFINES
#include "Oscillo.h"
#include <cmath>

Oscillo::Oscillo() {}

void Oscillo::setFrequency(float freq, float sampleRate) {
  increment_ = freq / sampleRate;
}

float Oscillo::nextSample() {
  phase_ += increment_;
  if (phase_ >= 1.0f) phase_ -= 1.0f;

  float value = 0.0f;
  switch (waveform_) {
  case Waveform::Sine:   value = sinf(phase_ * 2.0f * M_PI); break;
  case Waveform::Saw:    value = 2.0f * phase_ - 1.0f; break;
  case Waveform::Square: value = (phase_ < 0.5f) ? 1.0f : -1.0f; break;
  case Waveform::Triangle: value = 2.0f * fabs(2.0f * phase_ - 1.0f) - 1.0f; break;
  }

  return value * amplitude_;
}