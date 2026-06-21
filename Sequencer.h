#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "PatternSystem.h"
#include "Synthesizer.h"
#include <vector>
#include <atomic>
#include <deque>

class Sequencer {
public:
  Sequencer(Synthesizer& synth);
  void setBPM(float bpm) { bpm_.store(bpm); }
  void setBaseNote(int note) { baseNote_.store(note); patternDirty_.store(true); }
  void setPatternSteps(int steps) { steps_.store(steps); patternDirty_.store(true); }
  void setGeneratorType(GeneratorType type) { genType_.store(type); patternDirty_.store(true); }
  void regenerate() { patternDirty_.store(true); }
  void setWaveform(Waveform wf) { synth_.setWaveform(wf); }
  void setADSR(float attack, float decay, float sustain, float release) {
    synth_.setADSR(attack, decay, sustain, release);
  }
  int getCurrentNote() const { return currentNote_.load(); }
  std::vector<int> getLastNotes(int count = 16) const;
  void tick(int samplesPerBuffer, int sampleRate);

private:
  void regeneratePattern();
  Synthesizer& synth_;
  PatternSystem patternSystem_;
  std::vector<int> notes_;
  int currentNoteIndex_ = 0;
  std::deque<int> lastNotes_;
  std::atomic<float> bpm_{ 120.0f };
  std::atomic<int> baseNote_{ 60 };
  std::atomic<int> steps_{ 32 };
  std::atomic<GeneratorType> genType_{ GeneratorType::CellularAutomaton };
  std::atomic<bool> patternDirty_{ true };
  std::atomic<int> currentNote_{ 0 };
  int samplesPerBeat_ = 0;
  int samplesAccumulated_ = 0;
};

#endif // !SEQUENCER_H