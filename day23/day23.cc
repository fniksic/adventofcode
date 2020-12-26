#include "day23.h"

#include <array>
#include <deque>
#include <iostream>

using namespace std;

deque<int> ReadInput(istream& in) {
  deque<int> cups{};
  string line{};
  in >> line;
  for (char c : line) {
    cups.push_back(c - '0');
  }
  return cups;
}

void day23(istream& in, ostream& out) {
  deque<int> cups{ReadInput(in)};
  size_t current{};
  size_t total{cups.size()};
  for (int i = 0; i < 100; ++i) {
    while (current + 3 >= total) {
      cups.push_back(cups.front());
      cups.pop_front();
      --current;
    }
    while (current + 4 < total) {
      cups.push_front(cups.back());
      cups.pop_back();
      ++current;
    }
    array<int, 3> three_cups{};
    for (size_t j = 3; j > 0; --j) {
      three_cups[j - 1] = cups[current + j];
      cups.pop_back();
    }
    int next_value{cups[current] - 1};
    if (next_value < 1) next_value += total;
    while (next_value == three_cups[0] || next_value == three_cups[1] ||
           next_value == three_cups[2]) {
      --next_value;
      if (next_value < 1) next_value += total;
    }
    size_t pos{};
    while (cups[pos] != next_value) ++pos;
    for (size_t j = 0; j <= pos; ++j) {
      cups.push_back(cups.front());
      cups.pop_front();
    }
    current -= pos;
    for (size_t j = 0; j < 3; ++j) cups.push_back(three_cups[j]);
  }
  while (cups.front() != 1) {
    cups.push_back(cups.front());
    cups.pop_front();
  }
  out << "Part 1: ";
  for (size_t j = 1; j < cups.size(); ++j) out << cups[j];
  out << endl;
}
