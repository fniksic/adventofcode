#include "../common/test.h"

#include <sstream>
#include <string>

#include "day11.h"

using namespace std;

void TestDay11_Part1() {
  istringstream in(string(
      R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 37
)";

  day11(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay11_Part2() {
  istringstream in(string(
      R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL
)"));
  ostringstream out;
  string expected =
      R"(Part 2: 26
)";

  day11_part2(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay11_Part1();
  TestDay11_Part2();
  return 0;
}