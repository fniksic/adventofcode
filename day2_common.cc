#include "day2_common.h"

#include <string>
#include <tuple>

std::tuple<std::string, std::string, std::string> SplitLine(const std::string& line) {
  auto first_space = line.find(' ');
  auto second_space = line.find(' ', first_space + 1);
  return {line.substr(0, first_space), line.substr(first_space + 1, second_space - first_space - 1),
          line.substr(second_space + 1)};
}

std::tuple<int, int> SplitBounds(const std::string& bounds) {
  auto pos = bounds.find('-');
  int low = std::stoi(bounds.substr(0, pos));
  int high = std::stoi(bounds.substr(pos + 1));
  return {low, high};
}

int Count(const std::string& str, char letter) {
  int count = 0;
  for (char c : str) {
    if (c == letter) {
      ++count;
    }
  }
  return count;
}
