#include "day5.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>

using namespace std;

void day5(istream& in, ostream& out) {
  string line;
  int max_seat_id = 0;
  bitset<1024> seats;
  while (getline(in, line)) {
    if (line.empty()) break;
    int seat_id = 0;
    for (string::size_type i = 0; i < line.size(); ++i) {
      if (line[i] == 'B' || line[i] == 'R') {
        seat_id |= 1 << (line.size() - i - 1);
      }
    }
    max_seat_id = max(max_seat_id, seat_id);
    seats.set(seat_id);
  }
  out << "Part 1: " << max_seat_id << endl;
  out << "Part 2: ";
  for (int i = 1; i < max_seat_id; ++i) {
    if (seats.test(i - 1) && !seats.test(i) && seats.test(i + 1)) {
      out << i << endl;
      break;
    }
  }
}
