#include "Synthesizer.h"
#include <algorithm>

Synthesizer::Synthesizer(int maxVoices) : voices_(maxVoices) {
  for (auto& v : voices_) v.setSampleRate(sampleRate_);
}

void Synthesizer::setSampleRate(float rate) {
  sampleRate_ = rate;
  for (auto& v : voices_) v.setSampleRate(rate);
}

void Synthesizer::setADSR(float attack, float decay, float sustain, float release) {
  attack_ = attack;
  decay_ = decay;
  sustain_ = sustain;
  release_ = release;
}

void Synthesizer::setWaveform(Waveform wf) {
  waveform_ = wf;
}

int Synthesizer::findFreeVoice() {
  for (size_t i = 0; i < voices_.size(); ++i) {
    if (voices_[i].isIdle()) return static_cast<int>(i);
  }
  return -1;
}

int Synthesizer::findVoiceForNote(int midiNote) {
  auto it = noteToVoice_.find(midiNote);
  if (it != noteToVoice_.end()) {
    int idx = it->second;
    if (voices_[idx].isActive()) {
      return idx;
    }
    else {
      noteToVoice_.erase(it);
    }
  }
  return -1;
}

void Synthesizer::noteOn(int midiNote, float velocity) {
  int existingVoice = findVoiceForNote(midiNote);
  if (existingVoice != -1) {
    voices_[existingVoice].noteOff();
    noteToVoice_.erase(midiNote);
}

  int freeIdx = findFreeVoice();
  if (freeIdx != -1) {
    voices_[freeIdx].setWaveform(waveform_);
    voices_[freeIdx].noteOn(midiNote, velocity, attack_, decay_, sustain_, release_);
    noteToVoice_[midiNote] = freeIdx;
  }
}

void Synthesizer::noteOff(int midiNote) {
  int idx = findVoiceForNote(midiNote);
  if (idx != -1) {
    voices_[idx].noteOff();
    noteToVoice_.erase(midiNote);
  }
}

int Synthesizer::getActiveVoicesCount() const {
  int count = 0;
  for (const auto& v : voices_) {
    if (v.isActive()) ++count;
  }
  return count;
}

void Synthesizer::render(float* output, int frames) {
  for (int i = 0; i < frames * 2; ++i) output[i] = 0.0f;
  for (auto& v : voices_) {
    if (v.isActive()) {
      for (int i = 0; i < frames; ++i) {
        float s = v.nextSample();
        output[i * 2] += s;
        output[i * 2 + 1] += s;
      }
    }
  }
}