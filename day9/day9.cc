#include "day9.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

using namespace std;

vector<int64_t> ReadInput(istream& in) {
  vector<int64_t> numbers;
  int64_t n;
  while (in >> n) {
    numbers.push_back(n);
  }
  return numbers;
}

int64_t FindInvalid(const vector<int64_t>& numbers, int window_size) {
  int64_t invalid = -1;
  unordered_multiset<int64_t> window;
  for (int i = 0; i < numbers.size() && invalid < 0; ++i) {
    if (i >= window_size) {
      bool not_sum = true;
      for (int64_t m : window) {
        if (numbers[i] - m != m &&
            window.find(numbers[i] - m) != window.end()) {
          not_sum = false;
          break;
        }
      }
      if (not_sum) invalid = numbers[i];
      window.erase(window.find(numbers[i - window_size]));
    }
    window.insert(numbers[i]);
  }
  return invalid;
}

int64_t FindSum(const vector<int64_t>& numbers, int64_t invalid) {
  int64_t sum = 0;
  auto low = numbers.begin();
  auto high = numbers.begin();
  while (sum != invalid && high != numbers.end()) {
    if (sum < invalid || distance(low, high) < 2)
      sum += *high++;
    else
      sum -= *low++;
  }
  return *min_element(low, high) + *max_element(low, high);
}

void day9(istream& in, ostream& out, int window_size) {
  vector<int64_t> numbers = ReadInput(in);
  int64_t invalid = FindInvalid(numbers, window_size);
  out << "Part 1: " << invalid << endl;
  out << "Part 2: " << FindSum(numbers, invalid) << endl;
}
