#include "day10.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReadInput(istream& in) {
  int n;
  vector<int> result;
  while (in >> n) result.push_back(n);
  return result;
}

int GetDifferenceDistribution(const vector<int>& adapters) {
  // diffs[0] = -1 accounts for the zero inserted at the beginning
  array diffs = {-1, 0, 0, 0};
  int previous = 0;
  for (int joltage : adapters) {
    ++diffs[joltage - previous];
    previous = joltage;
  }
  return diffs[1] * diffs[3];
}

int64_t GetTotalArrangements(const vector<int>& adapters) {
  vector<int64_t> total(adapters.size(), 0);
  total[0] = 1;
  for (int i = 1; i < adapters.size(); ++i) {
    for (int j = i - 1; j >= 0 && adapters[i] - adapters[j] <= 3; --j) {
      total[i] += total[j];
    }
  }
  return total.back();
}

void day10(istream& in, ostream& out) {
  vector<int> adapters = ReadInput(in);
  sort(adapters.begin(), adapters.end());
  adapters.insert(adapters.begin(), 0);
  adapters.push_back(adapters.back() + 3);
  out << "Part 1: " << GetDifferenceDistribution(adapters) << endl;
  out << "Part 2: " << GetTotalArrangements(adapters) << endl;
}
