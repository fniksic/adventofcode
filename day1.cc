#include <iostream>
#include <unordered_set>

int main() {
  std::unordered_set<int> entries;
  while (!std::cin.eof()) {
    int n;
    std::cin >> n;
    if (entries.find(2020 - n) != entries.end()) {
      std::cout << (2020 - n) * n << std::endl;
      break;
    }
    entries.insert(n);
  }
  return 0;
}
