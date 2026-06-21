#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include "SummOscEnvel.h"
#include <vector>
#include <unordered_map>

class Synthesizer {
public:
  Synthesizer(int maxVoices = 16);
  void setSampleRate(float rate);
  void noteOn(int midiNote, float velocity = 0.7f);
  void noteOff(int midiNote);
  void render(float* output, int frames);

  void setADSR(float attack, float decay, float sustain, float release);
  void setWaveform(Waveform wf);
  int getActiveVoicesCount() const;

private:
  std::vector<SummOscEnvel> voices_;
  std::unordered_map<int, int> noteToVoice_;
  float sampleRate_ = 48000.0f;
  float attack_ = 0.005f, decay_ = 0.1f, sustain_ = 0.6f, release_ = 0.4f;
  Waveform waveform_ = Waveform::Triangle;

  int findFreeVoice();
  int findVoiceForNote(int midiNote);
};

#endif // !SYNTHESIZER_H