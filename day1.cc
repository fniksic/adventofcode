#include <iostream>
#include <unordered_set>

using namespace std;

int main() {
  int n;
  unordered_set<int> entries;

  while (cin >> n) {
    if (entries.find(2020 - n) != entries.end()) {
      cout << (2020 - n) * n << endl;
      break;
    }
    entries.insert(n);
  }

  return 0;
}
