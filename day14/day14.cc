#include "day14.h"

#include <bitset>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

using Word = bitset<36>;

struct Instruction {
  enum Type { kMask, kMem };

  Type type = kMask;
  Word fst;
  Word snd;
};

Instruction ParseLine(const string& line) {
  Instruction instruction;
  if (line[1] == 'e') {
    auto bracket = line.find(']');
    instruction.type = Instruction::kMem;
    instruction.fst = stoull(line.substr(4, bracket - 4));
    instruction.snd = stoull(line.substr(bracket + 4));
  } else {
    Word ones;
    Word zeros = ~Word();
    for (int i = 7; i < line.size(); ++i) {
      if (line[i] == '0')
        zeros.reset(line.size() - i - 1);
      else if (line[i] == '1')
        ones.set(line.size() - i - 1);
    }
    instruction.type = Instruction::kMask;
    instruction.fst = zeros;
    instruction.snd = ones;
  }
  return instruction;
}

class Machine {
 public:
  void ReadProgram(istream& in) {
    string line;
    while (getline(in, line)) {
      if (line.empty()) break;
      program_.push_back(ParseLine(line));
    }
  }

  void Reset() {
    memory_.clear();
    mask_zeros_.set();
    mask_ones_.reset();
  }

  void ExecutePart1() {
    Execute([this](const Instruction& instruction) {
      memory_[instruction.fst] = (instruction.snd & mask_zeros_) | mask_ones_;
    });
  }

  void ExecutePart2() {
    Execute([this](const Instruction& instruction) {
      Word address = (instruction.fst & mask_xs_) | mask_ones_;
      while (true) {
        memory_[address] = instruction.snd;
        int next_zero = 0;
        while (next_zero < pos_xs_.size() && address[pos_xs_[next_zero]])
          ++next_zero;
        if (next_zero == pos_xs_.size()) break;
        address.set(pos_xs_[next_zero]);
        while (--next_zero >= 0) address.reset(pos_xs_[next_zero]);
      }
    });
  }

  uint64_t GetSum() const {
    uint64_t sum = 0;
    for (const auto& [addr, val] : memory_) {
      sum += val.to_ullong();
    }
    return sum;
  }

 private:
  void Execute(const function<void(const Instruction&)>& apply) {
    for (Instruction& instruction : program_) {
      if (instruction.type == Instruction::kMask) {
        mask_zeros_ = instruction.fst;
        mask_ones_ = instruction.snd;
        mask_xs_ = ~mask_zeros_ | mask_ones_;
        pos_xs_.clear();
        for (int i = 0; i < mask_xs_.size(); ++i) {
          if (!mask_xs_[i]) pos_xs_.push_back(i);
        }
      } else {
        apply(instruction);
      }
    }
  }

  unordered_map<Word, Word> memory_;
  vector<Instruction> program_;
  Word mask_xs_;
  Word mask_zeros_;
  Word mask_ones_;
  vector<int> pos_xs_;
};

void day14(istream& in, ostream& out) {
  Machine machine;
  machine.ReadProgram(in);
  machine.ExecutePart1();
  out << "Part 1: " << machine.GetSum() << endl;
}

void day14_part2(istream& in, ostream& out) {
  Machine machine;
  machine.ReadProgram(in);
  machine.ExecutePart2();
  out << "Part 2: " << machine.GetSum() << endl;
}
