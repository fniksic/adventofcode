#include <iostream>
#include <string>

#include "day2_common.h"

using namespace std;

int main() {
  string line;
  int total_valid = 0;
  while (getline(cin, line)) {
    if (line.empty()) break;

    auto [bounds, letter_colon, password] = SplitLine(line);
    auto [low, high] = SplitBounds(bounds);
    char letter = letter_colon[0];
    int count = Count(password, letter);

    if (low <= count && count <= high) {
      ++total_valid;
    }
  }
  cout << total_valid << endl;
  return 0;
}