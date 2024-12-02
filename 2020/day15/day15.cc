#include "day15.h"

#include <iostream>
#include <vector>

using namespace std;

void day15(istream& in, ostream& out) {
  int n;
  int step = 1;
  int next = 0;
  vector<int> last_seen(30000000, 0);
  while (in >> n) {
    if (last_seen[n] != 0) next = step - last_seen[n];
    last_seen[n] = step++;
  }
  while (step <= 30000000) {
    n = next;
    int old_step = last_seen[n] == 0 ? step : last_seen[n];
    last_seen[n] = step;
    next = step - old_step;
    if (step == 2020) out << "Part 1: " << n << endl;
    ++step;
  }
  out << "Part 2: " << n << endl;
}
