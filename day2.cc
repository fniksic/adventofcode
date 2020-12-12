#include <iostream>
#include <string>

#include "day2_common.h"

int main() {
  int total_valid = 0;
  while (!std::cin.eof()) {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) break;

    auto [bounds, letter_colon, password] = SplitLine(line);
    auto [low, high] = SplitBounds(bounds);
    char letter = letter_colon[0];
    int count = Count(password, letter);
    if (low <= count && count <= high) {
      ++total_valid;
    }
  }
  std::cout << total_valid << std::endl;
  return 0;
}