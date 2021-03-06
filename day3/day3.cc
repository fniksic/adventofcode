#include "day3.h"

#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

void day3(istream& in, ostream& out) {
  string line;
  string::size_type r[] = {0, 0, 0, 0, 0};
  string::size_type dr[] = {1, 1, 3, 5, 7};
  string::size_type dd[] = {1, 2, 1, 1, 1};
  int64_t counts[] = {0, 0, 0, 0, 0};
  for (int line_num = 0; getline(in, line); ++line_num) {
    for (int i = 0; i < 5; ++i) {
      if (line_num % dd[i] == 0) {
        if (line[r[i]] == '#') ++counts[i];
        r[i] = (r[i] + dr[i]) % line.size();
      }
    }
  }
  out << "Part 1: " << counts[2] << endl;
  out << "Part 2: " << counts[0] * counts[1] * counts[2] * counts[3] * counts[4]
      << endl;
}
