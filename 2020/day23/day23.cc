#include "day23.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Circle {
 public:
  Circle(const vector<size_t>& initial, size_t total)
      : current_{initial[0]}, total_{total}, cups_(total + 1, Cup{}) {
    for (size_t i = 0; i + 1 < initial.size(); ++i) {
      cups_[initial[i]].next = initial[i + 1];
    }
    if (initial.size() < total) {
      cups_[initial.back()].next = initial.size() + 1;
      cups_.back().next = initial[0];
    } else {
      cups_[initial.back()].next = initial[0];
    }
    for (size_t i = initial.size() + 1; i < total; ++i) {
      cups_[i].next = i + 1;
    }
  }

  void Move(int steps) {
    for (int i = 0; i < steps; ++i) {
      Move();
    }
  }

  void PrintPart1(ostream& out) const {
    out << "Part 1: ";
    for (size_t cup{cups_[1].next}; cup != 1; cup = cups_[cup].next) {
      out << cup;
    }
    out << endl;
  }

  void PrintPart2(ostream& out) const {
    int64_t result = cups_[1].next;
    result *= cups_[cups_[1].next].next;
    out << "Part 2: " << result << endl;
  }

 private:
  void Move() {
    size_t first{cups_[current_].next};
    size_t second{cups_[first].next};
    size_t third{cups_[second].next};
    size_t destination{current_};
    while (destination == current_ || destination == first ||
           destination == second || destination == third) {
      --destination;
      if (destination < 1) destination += total_;
    }
    cups_[current_].next = cups_[third].next;
    cups_[third].next = cups_[destination].next;
    cups_[destination].next = first;
    current_ = cups_[current_].next;
  }

  struct Cup {
    size_t next{};
  };

  size_t current_;
  size_t total_;
  vector<Cup> cups_;
};

vector<size_t> ReadInput(istream& in) {
  vector<size_t> result;
  string line;
  in >> line;
  for (char c : line) {
    result.push_back(c - '0');
  }
  return result;
}

void day23(istream& in, ostream& out) {
  vector<size_t> initial{ReadInput(in)};

  Circle circle_1{initial, initial.size()};
  circle_1.Move(100);
  circle_1.PrintPart1(out);

  Circle circle_2{initial, 1'000'000};
  circle_2.Move(10'000'000);
  circle_2.PrintPart2(out);
}
