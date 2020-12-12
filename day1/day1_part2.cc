#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "day1.h"

using namespace std;

void day1_part2(std::istream& in, std::ostream& out) {
  int64_t n;
  vector<int64_t> entries;
  unordered_map<int64_t, int64_t> sum_to_prod;

  while (in >> n) {
    const auto sum_and_prod = sum_to_prod.find(2020 - n);
    if (sum_and_prod != sum_to_prod.end()) {
      const auto [sum, prod] = *sum_and_prod;
      out << prod * n << endl;
      break;
    }

    for (const int64_t m : entries) {
      sum_to_prod[n + m] = n * m;
    }
    entries.push_back(n);
  }
}
