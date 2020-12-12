#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
  int64_t n;
  vector<int64_t> entries;
  unordered_map<int64_t, int64_t> sum_to_prod;

  while (cin >> n) {
    const auto sum_and_prod = sum_to_prod.find(2020 - n);
    if (sum_and_prod != sum_to_prod.end()) {
      const auto [sum, prod] = *sum_and_prod;
      cout << prod * n << endl;
      break;
    }

    for (const int64_t m : entries) {
      sum_to_prod[n + m] = n * m;
    }
    entries.push_back(n);
  }

  return 0;
}
