#include "day1.h"

#include <iostream>
#include <unordered_set>

using namespace std;

void day1(istream& in, ostream& out) {
  int n;
  unordered_set<int> entries;

  while (in >> n) {
    if (entries.find(2020 - n) != entries.end()) {
      out << (2020 - n) * n << endl;
      break;
    }
    entries.insert(n);
  }
}
