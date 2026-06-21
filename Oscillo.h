#ifndef OSCILLO_H
#define OSCILLO_H

#include <cmath>

enum class Waveform { Sine, Saw, Square, Triangle };

class Oscillo {
public:
  Oscillo();
  void setFrequency(float freq, float sampleRate);
  void setWaveform(Waveform wf) { waveform_ = wf; }
  void setAmplitude(float amp) { amplitude_ = amp; }
  void setModulator(Oscillo* mod, float modDepth) { modulator_ = mod; modDepth_ = modDepth; }
  float nextSample();
  void reset() { phase_ = 0.0f; }

private:
  float phase_ = 0.0f;
  float increment_ = 0.0f;
  float amplitude_ = 0.8f;
  Waveform waveform_ = Waveform::Sine;
  Oscillo* modulator_ = nullptr;
  float modDepth_ = 0.0f;
};

#endif // !OSCILLO_H