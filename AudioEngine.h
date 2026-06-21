#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H


#include <portaudio.h>
#include <functional>
#include <atomic>

class AudioEngine {
public:
  AudioEngine();
  ~AudioEngine();
  void setCallback(std::function<void(float* output, int frames)> cb);
  bool start();
  void stop();
  bool isRunning() const { return running_; }
  int getSampleRate() const { return sampleRate_; }

private:
  static int paCallback(const void* input, void* output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData);
  PaStream* stream_;
  std::function<void(float* output, int frames)> callback_;
  std::atomic<bool> running_;
  int sampleRate_ = 48000;
  int framesPerBuffer_ = 256;
};

#endif // !AUDIOENGINE_H