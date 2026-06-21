#include "SummOscEnvel.h"
#include <cmath>
#include <iostream>  

SummOscEnvel::SummOscEnvel() {}

void SummOscEnvel::setSampleRate(float rate) {
  sampleRate_ = rate;
  envelope_.setSampleRate(rate);
}

void SummOscEnvel::noteOn(int midiNote, float velocity, float attack, float decay, float sustain, float release) {
  baseFreq_ = 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
  oscillo_.setFrequency(baseFreq_, sampleRate_);
  oscillo_.setAmplitude(velocity * 0.8f);
  oscillo_.setWaveform(waveform_);   

  std::cout << "Note On: freq=" << baseFreq_ << " Hz, waveform=" << (int)waveform_
    << " (" << (waveform_ == Waveform::Sine ? "Sine" :
      waveform_ == Waveform::Saw ? "Saw" :
      waveform_ == Waveform::Square ? "Square" : "Triangle") << ")\n";

  envelope_.noteOn(attack, decay, sustain, release);
}

void SummOscEnvel::noteOff() {
  envelope_.noteOff();
}

float SummOscEnvel::nextSample() {
  float env = envelope_.nextSample();
  float sample = oscillo_.nextSample();
  return sample * env;
}