#ifndef SUMMOSCENVEL_H
#define SUMMOSCENVEL_H

#include "Oscillo.h"
#include "Envelope.h"

class SummOscEnvel {
public:
  SummOscEnvel();
  void setSampleRate(float rate);
  void setWaveform(Waveform wf) { waveform_ = wf; }
  void noteOn(int midiNote, float velocity, float attack, float decay, float sustain, float release);
  void noteOff();
  float nextSample();
  bool isActive() const { return envelope_.isActive(); }
  bool isIdle() const { return envelope_.isIdle(); }

private:
  Oscillo oscillo_;
  Envelope envelope_;
  float sampleRate_ = 48000.0f;
  float baseFreq_ = 0.0f;
  Waveform waveform_ = Waveform::Triangle;
};

#endif // !SUMMOSCENVEL_H