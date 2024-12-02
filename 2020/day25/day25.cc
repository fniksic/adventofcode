#include "day25.h"

#include <cstdint>
#include <iostream>
#include <utility>

using namespace std;

pair<int, int> ReadInput(istream& in) {
  int card_key;
  int door_key;
  in >> card_key >> door_key;
  return {card_key, door_key};
}

int64_t Transform(int64_t subject_number, int loop_size, int64_t value = 1) {
  while (loop_size > 0) {
    value *= subject_number;
    value %= 20201227;
    --loop_size;
  }
  return value;
}

int LoopSize(int64_t key) {
  int loop_size{0};
  int64_t value{1};
  while (value != key) {
    value = Transform(7, 1, value);
    ++loop_size;
  }
  return loop_size;
}

void day25(istream& in, ostream& out) {
  auto [card_key, door_key]{ReadInput(in)};
  int card_loop_size{LoopSize(card_key)};
  out << "Part 1: " << Transform(door_key, card_loop_size) << endl;
}
