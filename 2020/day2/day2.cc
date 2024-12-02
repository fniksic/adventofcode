#include "day2.h"

#include <iostream>
#include <string>
#include <tuple>

using namespace std;

tuple<string, string, string> SplitLine(const string& line) {
  auto first_space = line.find(' ');
  auto second_space = line.find(' ', first_space + 1);
  return {line.substr(0, first_space),
          line.substr(first_space + 1, second_space - first_space - 1),
          line.substr(second_space + 1)};
}

tuple<int, int> SplitBounds(const string& bounds) {
  auto pos = bounds.find('-');
  int low = stoi(bounds.substr(0, pos));
  int high = stoi(bounds.substr(pos + 1));
  return {low, high};
}

int Count(const string& str, char letter) {
  int count = 0;
  for (char c : str) {
    if (c == letter) {
      ++count;
    }
  }
  return count;
}

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