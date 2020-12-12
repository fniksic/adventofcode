#include <iostream>
#include <string>

#include "day2.h"
#include "day2_common.h"

using namespace std;

void day2_part2(std::istream& in, std::ostream& out) {
  string line;
  int total_valid = 0;
  while (getline(in, line)) {
    if (line.empty()) break;

    auto [bounds, letter_colon, password] = SplitLine(line);
    auto [low, high] = SplitBounds(bounds);
    char letter = letter_colon[0];

    if ((password[low - 1] == letter) ^ (password[high - 1] == letter)) {
      ++total_valid;
    }
  }
  out << total_valid << endl;
}