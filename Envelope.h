#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope {
public:
  enum class State { Idle, Attack, Decay, Sustain, Release };

  Envelope();
  void noteOn(float attack, float decay, float sustain, float release);
  void noteOff();
  float nextSample();
  bool isIdle() const { return state_ == State::Idle; }
  bool isActive() const { return state_ != State::Idle; }
  void setSampleRate(float sr) { sampleRate_ = sr; }

private:
  State state_ = State::Idle;
  float level_ = 0.0f;
  float attackTime_ = 0.01f, decayTime_ = 0.1f, sustainLevel_ = 0.7f, releaseTime_ = 0.5f;
  float attackRate_ = 0.0f, decayRate_ = 0.0f, releaseRate_ = 0.0f;
  float sampleRate_ = 48000.0f;
};

#endif // !ENVELOPE_H