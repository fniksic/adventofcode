#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
  std::vector<std::int64_t> entries;
  std::unordered_map<std::int64_t, std::int64_t> sum_to_prod;

  while (!std::cin.eof()) {
    std::int64_t n;
    std::cin >> n;

    const auto sum_and_prod = sum_to_prod.find(2020 - n);
    if (sum_and_prod != sum_to_prod.end()) {
      const auto [sum, prod] = *sum_and_prod;
      std::cout << prod * n << std::endl;
      break;
    }

    for (const std::int64_t m : entries) {
      sum_to_prod[n + m] = n * m;
    }
    entries.push_back(n);
  }

  return 0;
}
