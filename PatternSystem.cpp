#include "PatternSystem.h"
#include <random>
#include <cmath>

PatternSystem::PatternSystem() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);
  for (int i = 0; i < 64; ++i) state_[i] = dis(gen);
}

void PatternSystem::generate(int steps) {
  pattern_.clear();
  if (steps <= 0) return;

  if (genType_ == GeneratorType::CellularAutomaton) {
    for (int step = 0; step < steps; ++step) {
      std::bitset<64> newState;
      for (int i = 1; i < 63; ++i) {
        newState[i] = state_[i - 1] ^ state_[i + 1];
      }
      newState[0] = state_[63] ^ state_[1];
      newState[63] = state_[62] ^ state_[0];
      state_ = newState;
      int sum = 0;
      for (int i = 0; i < 8; ++i) sum += state_[i];
      pattern_.push_back(sum % 9);
    }
  }
  else if (genType_ == GeneratorType::LSystem) {
    std::string str = generateLSystem(4);
    for (char c : str) {
      int offset = 0;
      switch (c) {
      case 'A': offset = 0; break;
      case 'B': offset = 4; break;
      case 'C': offset = 7; break;
      default: continue;
      }
      pattern_.push_back(offset);
    }
    while ((int)pattern_.size() < steps) {
      pattern_.push_back(pattern_[pattern_.size() % pattern_.size()]);
    }
    if ((int)pattern_.size() > steps) pattern_.resize(steps);
  }
  else if (genType_ == GeneratorType::Random) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 8);
    for (int i = 0; i < steps; ++i) {
      pattern_.push_back(dis(gen));
    }
  }
}

std::vector<int> PatternSystem::getNotes(int baseNote) const {
  std::vector<int> notes;
  for (int offset : pattern_) {
    notes.push_back(baseNote + offset);
  }
  return notes;
}

std::string PatternSystem::generateLSystem(int iterations) const {
  std::string current = lsystemAxiom_;
  for (int i = 0; i < iterations; ++i) {
    std::string next;
    for (char c : current) {
      auto it = lsystemRules_.find(c);
      if (it != lsystemRules_.end()) next += it->second;
      else next += c;
    }
    current = next;
  }
  return current;
}