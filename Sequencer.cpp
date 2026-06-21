#include "Sequencer.h"
#include <algorithm>

Sequencer::Sequencer(Synthesizer& synth) : synth_(synth) {
  regeneratePattern();
}

void Sequencer::regeneratePattern() {
  patternSystem_.setGeneratorType(genType_.load());
  patternSystem_.generate(steps_.load());
  notes_ = patternSystem_.getNotes(baseNote_.load());
  currentNoteIndex_ = 0;
  patternDirty_.store(false);
}

void Sequencer::tick(int samplesPerBuffer, int sampleRate) {
  if (patternDirty_.exchange(false)) {
    regeneratePattern();
  }
  if (notes_.empty()) return;

  float bpm = bpm_.load();
  samplesPerBeat_ = static_cast<int>(60.0f / bpm * sampleRate / 4.0f);
  samplesAccumulated_ += samplesPerBuffer;

  while (samplesAccumulated_ >= samplesPerBeat_) {
    samplesAccumulated_ -= samplesPerBeat_;
    int note = notes_[currentNoteIndex_ % notes_.size()];
    float vel = 0.5f + 0.5f * (currentNoteIndex_ % 10) / 10.0f;
    synth_.noteOn(note, vel);
    currentNote_.store(note);
    lastNotes_.push_back(note);
    if (lastNotes_.size() > 16) lastNotes_.pop_front();
    currentNoteIndex_++;
  }
}

std::vector<int> Sequencer::getLastNotes(int count) const {
  std::vector<int> result;
  int start = std::max(0, (int)lastNotes_.size() - count);
  for (size_t i = start; i < lastNotes_.size(); ++i) {
    result.push_back(lastNotes_[i]);
  }
  return result;
}