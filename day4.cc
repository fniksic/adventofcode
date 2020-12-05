#include <iostream>
#include <string_view>

#include "day4_common.h"

using ::std::cout;
using ::std::string_view;

int main() {
  cout << "Part 1: ";
  ProcessPassports([](string_view field, string_view value) { return true; });
  return 0;
}
