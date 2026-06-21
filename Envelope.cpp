#include "Envelope.h"

Envelope::Envelope() {}

void Envelope::noteOn(float attack, float decay, float sustain, float release) {
  attackTime_ = attack; decayTime_ = decay; sustainLevel_ = sustain; releaseTime_ = release;
  attackRate_ = 1.0f / (attackTime_ * sampleRate_);
  decayRate_ = (1.0f - sustainLevel_) / (decayTime_ * sampleRate_);
  releaseRate_ = sustainLevel_ / (releaseTime_ * sampleRate_);
  state_ = State::Attack;
  level_ = 0.0f;
}

void Envelope::noteOff() {
  if (state_ != State::Idle) state_ = State::Release;
}

float Envelope::nextSample() {
  switch (state_) {
  case State::Attack:
    level_ += attackRate_;
    if (level_ >= 1.0f) { level_ = 1.0f; state_ = State::Decay; }
    break;
  case State::Decay:
    level_ -= decayRate_;
    if (level_ <= sustainLevel_) { level_ = sustainLevel_; state_ = State::Sustain; }
    break;
  case State::Sustain:
    break;
  case State::Release:
    level_ -= releaseRate_;
    if (level_ <= 0.0f) { level_ = 0.0f; state_ = State::Idle; }
    break;
  case State::Idle:
    level_ = 0.0f;
    break;
  }
  return level_;
}