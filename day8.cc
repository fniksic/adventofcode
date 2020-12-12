#include <iostream>
#include <istream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using namespace std;

class Machine {
 private:
  enum Operation { kNop, kAcc, kJmp };

  struct Instruction {
    Operation op;
    int val;
  };

  vector<Instruction> program_;
  int accumulator_ = 0;
  int current_ = 0;
  unordered_set<int> executed_;

  void AddInstructionFromLine(string_view line) {
    string_view op_text = line.substr(0, 3);
    Operation op = kNop;
    if (op_text == "acc")
      op = kAcc;
    else if (op_text == "jmp")
      op = kJmp;

    int val = stoi(string(line.substr(5)));
    if (line[4] == '-') val *= -1;

    program_.emplace_back(Instruction{.op = op, .val = val});
  }

  void ExecuteInstruction() {
    Instruction instruction = program_[current_];
    executed_.insert(current_);
    switch (instruction.op) {
      case kJmp:
        current_ += instruction.val;
        break;
      case kAcc:
        accumulator_ += instruction.val;
      default:
        ++current_;
    }
  }

 public:
  int accumulator() const { return accumulator_; }

  void Reset() {
    accumulator_ = 0;
    current_ = 0;
    executed_.clear();
  }

  void ReadProgram(istream& in) {
    string line;
    while (getline(in, line)) {
      if (line.empty()) break;
      AddInstructionFromLine(line);
    }
  }

  // Returns `true` if terminated
  bool DetectLassoOrTerminate() {
    while (current_ < program_.size()) {
      if (executed_.find(current_) != executed_.end()) return false;
      ExecuteInstruction();
    }
    return true;
  }

  void EscapeLasso() {
    vector<int> candidates(executed_.begin(), executed_.end());
    for (int candidate : candidates) {
      Instruction& instruction = program_[candidate];
      if ((instruction.op == kNop &&
           executed_.find(candidate + instruction.val) == executed_.end()) ||
          (instruction.op == kJmp &&
           executed_.find(candidate + 1) == executed_.end())) {
        executed_.erase(candidate);
        instruction.op = instruction.op == kNop ? kJmp : kNop;
        current_ = candidate;
        if (DetectLassoOrTerminate()) break;
        // Revert the instruction as we'll need to rerun the program
        instruction.op = instruction.op == kNop ? kJmp : kNop;
      }
    }
  }
};

int main() {
  Machine machine;
  machine.ReadProgram(cin);
  machine.DetectLassoOrTerminate();

  cout << "Part 1: " << machine.accumulator() << endl;

  machine.EscapeLasso();
  machine.Reset();
  machine.DetectLassoOrTerminate();

  cout << "Part 2: " << machine.accumulator() << endl;

  return 0;
}