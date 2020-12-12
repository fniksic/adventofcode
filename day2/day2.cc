#include "day2.h"

#include <iostream>
#include <string>

#include "day2_common.h"

using namespace std;

void day2(istream& in, ostream& out) {
  string line;
  int total_valid = 0;
  while (getline(in, line)) {
    if (line.empty()) break;

    auto [bounds, letter_colon, password] = SplitLine(line);
    auto [low, high] = SplitBounds(bounds);
    char letter = letter_colon[0];
    int count = Count(password, letter);

    if (low <= count && count <= high) {
      ++total_valid;
    }
  }
  out << total_valid << endl;
}
