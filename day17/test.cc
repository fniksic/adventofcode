#include "../common/test.h"

#include <sstream>
#include <string>

#include "day17.h"

using namespace std;

void TestDay17() {
  istringstream in(string(
      R"(.#.
..#
###
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 112
Part 2: 848
)";

  day17(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay17();
  return 0;
}