#include "../common/test.h"

#include <sstream>
#include <string>

#include "day14.h"

using namespace std;

void TestDay14_Part1() {
  istringstream in(string(
      R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 165
)";

  day14(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay14_Part2() {
  istringstream in(string(
      R"(mask = 000000000000000000000000000000X1001X
mem[42] = 100
mask = 00000000000000000000000000000000X0XX
mem[26] = 1
)"));
  ostringstream out;
  string expected =
      R"(Part 2: 208
)";

  day14_part2(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay14_Part1();
  TestDay14_Part2();
  return 0;
}