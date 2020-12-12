#include "../common/test.h"

#include <sstream>
#include <string>

#include "day3.h"

using namespace std;

void TestDay3() {
  istringstream in(string(
      R"(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 7
Part 2: 336
)";

  day3(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay3();
  return 0;
}