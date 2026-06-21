#include "AudioEngine.h"
#include "Synthesizer.h"
#include "GUI.h"
#include <imgui.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

Synthesizer* g_synth = nullptr;

struct KeyMapping {
  ImGuiKey key;
  const char* name;
  int note;
};

std::vector<KeyMapping> keyMappings = {
  {ImGuiKey_Q, "Q", 60},
  {ImGuiKey_W, "W", 62},
  {ImGuiKey_E, "E", 64},
  {ImGuiKey_R, "R", 65},
  {ImGuiKey_T, "T", 67},
  {ImGuiKey_Y, "Y", 69},
  {ImGuiKey_U, "U", 71},
  {ImGuiKey_I, "I", 72},
  {ImGuiKey_O, "O", 74},
  {ImGuiKey_P, "P", 76},
  {ImGuiKey_LeftBracket, "[", 77},
  {ImGuiKey_RightBracket, "]", 79},
  {ImGuiKey_1, "1", 61},
  {ImGuiKey_2, "2", 63},
  {ImGuiKey_3, "3", 66},
  {ImGuiKey_4, "4", 68},
  {ImGuiKey_5, "5", 70},
  {ImGuiKey_6, "6", 73},
  {ImGuiKey_7, "7", 75},
  {ImGuiKey_8, "8", 78},
};

std::map<ImGuiKey, bool> keyState;

bool isBlackNote(int midiNote) {
  int pitch = midiNote % 12;
  return (pitch == 1 || pitch == 3 || pitch == 6 || pitch == 8 || pitch == 10);
}

std::string noteName(int midiNote) {
  const char* notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
  int octave = midiNote / 12 - 1;
  int pitch = midiNote % 12;
  return std::string(notes[pitch]) + std::to_string(octave);
}

bool isNoteMapped(int note) {
  for (auto& km : keyMappings) if (km.note == note) return true;
  return false;
}

const char* getKeyNameForNote(int note) {
  for (auto& km : keyMappings) if (km.note == note) return km.name;
  return nullptr;
}

bool isNotePressed(int note) {
  for (auto& km : keyMappings) if (km.note == note && keyState[km.key]) return true;
  return false;
}

void renderGUI() {
  if (!g_synth) return;

  ImGui::Begin("Virtual Synthesizer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Text("Sound Controls");
  static int waveformType = 2;
  const char* waveformNames[] = { "Sine", "Saw", "Square", "Triangle" };
  if (ImGui::Combo("Waveform", &waveformType, waveformNames, IM_ARRAYSIZE(waveformNames))) {
    g_synth->setWaveform(static_cast<Waveform>(waveformType));
  }

  static float attack = 0.005f, decay = 0.1f, sustain = 0.6f, release = 0.4f;
  if (ImGui::SliderFloat("Attack", &attack, 0.001f, 0.5f))
    g_synth->setADSR(attack, decay, sustain, release);
  if (ImGui::SliderFloat("Decay", &decay, 0.001f, 1.0f))
    g_synth->setADSR(attack, decay, sustain, release);
  if (ImGui::SliderFloat("Sustain", &sustain, 0.0f, 1.0f))
    g_synth->setADSR(attack, decay, sustain, release);
  if (ImGui::SliderFloat("Release", &release, 0.001f, 2.0f))
    g_synth->setADSR(attack, decay, sustain, release);

  ImGui::Separator();
  ImGui::Text("Active voices: %d", g_synth->getActiveVoicesCount());
  ImGui::Separator();

  ImGui::Text("White: Q W E R T Y U I O P [ ]");
  ImGui::Text("Black: 1 2 3 4 5 6 7 8");

  ImDrawList* draw = ImGui::GetWindowDrawList();
  ImVec2 pos = ImGui::GetCursorScreenPos();
  const float keyWidth = 26.0f;
  const float keyHeight = 130.0f;
  const float blackKeyWidth = 17.0f;
  const float blackKeyHeight = 80.0f;

  const int minNote = 60;
  const int maxNote = 79;

  std::map<int, float> whiteKeyX;
  float whiteX = pos.x;
  for (int note = minNote; note <= maxNote; ++note) {
    if (isBlackNote(note)) continue;

    float x = whiteX;
    bool mapped = isNoteMapped(note);
    bool pressed = isNotePressed(note);

    ImColor color;
    if (mapped && pressed)      color = ImColor(255, 200, 50);
    else if (mapped)            color = ImColor(240, 240, 240);
    else                        color = ImColor(180, 180, 180);

    draw->AddRectFilled(ImVec2(x, pos.y), ImVec2(x + keyWidth, pos.y + keyHeight), color);
    draw->AddRect(ImVec2(x, pos.y), ImVec2(x + keyWidth, pos.y + keyHeight), ImColor(0, 0, 0));

    if (mapped) {
      const char* keyName = getKeyNameForNote(note);
      std::string label = std::string(keyName) + "\n" + noteName(note);
      draw->AddText(ImVec2(x + 4, pos.y + keyHeight - 30), ImColor(0, 0, 0), label.c_str());
    }
    else {
      draw->AddText(ImVec2(x + 4, pos.y + keyHeight - 30), ImColor(100, 100, 100), noteName(note).c_str());
    }

    whiteKeyX[note] = x;
    whiteX += keyWidth;
  }

  for (int note = minNote; note <= maxNote; ++note) {
    if (!isBlackNote(note) || !isNoteMapped(note)) continue;

    int left = note - 1;
    while (left >= minNote && isBlackNote(left)) left--;
    int right = note + 1;
    while (right <= maxNote && isBlackNote(right)) right++;

    if (whiteKeyX.find(left) != whiteKeyX.end() && whiteKeyX.find(right) != whiteKeyX.end()) {
      float xLeft = whiteKeyX[left];
      float xRight = whiteKeyX[right];

      float gapStart = xLeft + keyWidth;
      float gapEnd = xRight;
      float gapWidth = gapEnd - gapStart;
      float idealX = gapStart + (gapWidth - blackKeyWidth) / 2.0f;

      int pitch = note % 12;
      float offset = 0.0f;

      if (pitch == 3 || pitch == 10) {
        offset = 3.0f;
      }

      float x = idealX + offset;

      bool pressed = isNotePressed(note);
      ImColor color = pressed ? ImColor(100, 100, 255) : ImColor(30, 30, 30);
      draw->AddRectFilled(ImVec2(x, pos.y), ImVec2(x + blackKeyWidth, pos.y + blackKeyHeight), color);
      draw->AddRect(ImVec2(x, pos.y), ImVec2(x + blackKeyWidth, pos.y + blackKeyHeight), ImColor(0, 0, 0));

      const char* keyName = getKeyNameForNote(note);
      draw->AddText(ImVec2(x + 2, pos.y + 4), ImColor(200, 200, 200), keyName);
    }
  }

  ImGui::Dummy(ImVec2(whiteX - pos.x + 20, keyHeight + 30));
  ImGui::Separator();

  ImGui::Text("Keyboard Mapping:");
  std::vector<KeyMapping> sorted = keyMappings;
  std::sort(sorted.begin(), sorted.end(), [](const KeyMapping& a, const KeyMapping& b) { return a.note < b.note; });
  if (ImGui::BeginTable("KeyMap", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    for (size_t i = 0; i < sorted.size(); ++i) {
      ImGui::TableNextColumn();
      const auto& km = sorted[i];
      bool isPressed = keyState[km.key];
      ImGui::TextColored(isPressed ? ImVec4(0, 1, 0, 1) : ImVec4(1, 1, 1, 1),
        "%s -> %s", km.name, noteName(km.note).c_str());
    }
    ImGui::EndTable();
  }

  ImGui::End();

  for (auto& km : keyMappings) {
    bool isPressed = ImGui::IsKeyDown(km.key);
    if (isPressed && !keyState[km.key]) {
      g_synth->noteOn(km.note, 0.8f);
      keyState[km.key] = true;
    }
    else if (!isPressed && keyState[km.key]) {
      g_synth->noteOff(km.note);
      keyState[km.key] = false;
    }
  }
}

int main() {
  std::cout << "=== Virtual Synthesizer ===\n";
  std::cout << "White: Q W E R T Y U I O P [ ]\n";
  std::cout << "Black: 1 2 3 4 5 6 7 8\n";

  Synthesizer synth(16);
  synth.setSampleRate(48000.0f);
  synth.setADSR(0.005f, 0.1f, 0.6f, 0.4f);
  synth.setWaveform(Waveform::Triangle);
  g_synth = &synth;

  AudioEngine engine;
  engine.setCallback([&](float* output, int frames) {
    synth.render(output, frames);
    });

  GUI gui;
  if (!gui.init(1000, 750, "Virtual Synthesizer")) {
    std::cerr << "GUI init failed\n";
    return 1;
  }

  if (!engine.start()) {
    std::cerr << "Audio start failed\n";
    return 1;
  }

  for (auto& km : keyMappings) keyState[km.key] = false;

  gui.run(renderGUI);

  engine.stop();
  std::cout << "Program finished.\n";
  return 0;
}
