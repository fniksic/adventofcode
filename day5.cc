#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>

using ::std::bitset;
using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::getline;
using ::std::max;
using ::std::sort;
using ::std::string;

int main() {
  string line;
  int max_seat_id = 0;
  bitset<1024> seats;
  while (getline(cin, line)) {
    if (line.empty()) continue;
    int seat_id = 0;
    for (string::size_type i = 0; i < line.size(); ++i) {
      if (line[i] == 'B' || line[i] == 'R') {
        seat_id |= 1 << (line.size() - i - 1);
      }
    }
    max_seat_id = max(max_seat_id, seat_id);
    seats.set(seat_id);
  }
  cout << "Part 1: " << max_seat_id << endl;
  cout << "Part 2: ";
  for (int i = 1; i < max_seat_id; ++i) {
    if (seats.test(i - 1) && !seats.test(i) && seats.test(i + 1)) {
      cout << i << endl;
      break;
    }
  }
  return 0;
}