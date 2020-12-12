#include "../common/test.h"

#include <iostream>
#include <sstream>
#include <string>

#include "day12.h"

using namespace std;

void TestDay12() {
  istringstream in(string(
      R"(F10
N3
F7
R90
F11)"));
  ostringstream out;
  string expected =
      R"(Part 1: 25
Part 2: 286
)";

  day12(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay12();
  return 0;
}