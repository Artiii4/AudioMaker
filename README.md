# AudioMaker — Virtual Synthesizer

## Overview

AudioMaker is a real-time software synthesizer developed in C++. It provides an interactive graphical interface that allows users to play music using the computer keyboard, with immediate control over sound parameters. The application is designed for low‑latency audio performance and is distributed as a single standalone executable for Windows.

## Features

- Two-octave keyboard range from C4 to G5, with 12 white keys and 8 black keys.
- Keyboard mapping: white keys – `Q W E R T Y U I O P [ ]`; black keys – `1 2 3 4 5 6 7 8`.
- Four waveform types: Sine, Saw, Square, Triangle.
- ADSR envelope controls (Attack, Decay, Sustain, Release) for shaping the sound.
- Polyphonic playback with up to 16 simultaneous voices.
- Visual piano display with key highlighting for pressed notes.
- Low-latency audio engine using PortAudio at 48 kHz sample rate.
- Graphical user interface built with Dear ImGui, rendered with OpenGL and GLFW.
- Statically linked executable – runs on Windows 10/11 (64‑bit) without external dependencies.

## Usage

Launch `AudioMaker.exe` by double‑clicking it. The main window shows the piano keyboard and control panels. Play notes using the computer keyboard as follows:

- White keys: `Q W E R T Y U I O P [ ]`
- Black keys: `1 2 3 4 5 6 7 8`

While playing, you can adjust the waveform and ADSR sliders in real time to modify the sound. The "Active voices" counter shows the number of notes currently sounding.

## Building from Source

If you wish to modify or rebuild the project, the following prerequisites apply:

- Windows 10/11 (64‑bit)
- Visual Studio 2022 with C++ development tools
- vcpkg for dependency management

Install dependencies via vcpkg:

```cmd
.\bootstrap-vcpkg.bat
.\vcpkg install portaudio:x64-windows-static
.\vcpkg install glfw3:x64-windows-static
.\vcpkg install imgui:x64-windows-static
