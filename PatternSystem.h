#ifndef PATTERNSYSTEM_H
#define PATTERNSYSTEM_H

#include <vector>
#include <bitset>
#include <string>
#include <unordered_map>

enum class GeneratorType {
  CellularAutomaton,
  LSystem,
  Random
};

class PatternSystem {
public:
  PatternSystem();
  void setGeneratorType(GeneratorType type) { genType_ = type; }
  void generate(int steps);
  std::vector<int> getNotes(int baseNote = 60) const;

private:
  std::vector<int> pattern_;
  std::bitset<64> state_;
  GeneratorType genType_ = GeneratorType::CellularAutomaton;
  std::string lsystemAxiom_ = "A";
  std::unordered_map<char, std::string> lsystemRules_ = { {'A', "AB"}, {'B', "A"} };
  std::string generateLSystem(int iterations) const;
};

#endif // !PATTERNSYSTEM_H