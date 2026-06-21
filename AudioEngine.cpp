#include "AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() : stream_(nullptr), running_(false) {
  PaError err = Pa_Initialize();
  if (err != paNoError) std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;
}

AudioEngine::~AudioEngine() {
  stop();
  Pa_Terminate();
}

void AudioEngine::setCallback(std::function<void(float* output, int frames)> cb) {
  callback_ = cb;
}

bool AudioEngine::start() {
  if (!callback_) { std::cerr << "No callback!\n"; return false; }
  PaStreamParameters outParams;
  outParams.device = Pa_GetDefaultOutputDevice();
  if (outParams.device == paNoDevice) { std::cerr << "No output device\n"; return false; }
  const PaDeviceInfo* devInfo = Pa_GetDeviceInfo(outParams.device);
  outParams.channelCount = 2;
  outParams.sampleFormat = paFloat32;
  outParams.suggestedLatency = devInfo->defaultLowOutputLatency;
  outParams.hostApiSpecificStreamInfo = nullptr;

  PaError err = Pa_OpenStream(&stream_, nullptr, &outParams, sampleRate_, framesPerBuffer_,
    paClipOff, &AudioEngine::paCallback, this);
  if (err != paNoError) { std::cerr << "Open stream error: " << Pa_GetErrorText(err) << std::endl; return false; }

  err = Pa_StartStream(stream_);
  if (err != paNoError) { std::cerr << "Start stream error: " << Pa_GetErrorText(err) << std::endl; return false; }

  running_ = true;
  std::cout << "Audio started.\n";
  return true;
}

void AudioEngine::stop() {
  if (stream_) {
    Pa_StopStream(stream_);
    Pa_CloseStream(stream_);
    stream_ = nullptr;
    running_ = false;
    std::cout << "Audio stopped.\n";
  }
}

int AudioEngine::paCallback(const void* input, void* output,
  unsigned long frameCount,
  const PaStreamCallbackTimeInfo* timeInfo,
  PaStreamCallbackFlags statusFlags,
  void* userData) {
  AudioEngine* engine = static_cast<AudioEngine*>(userData);
  float* out = static_cast<float*>(output);
  if (engine->callback_) {
    engine->callback_(out, frameCount);
  }
  return paContinue;
}