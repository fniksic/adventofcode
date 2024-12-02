#include "../common/test.h"

#include <sstream>
#include <string>

#include "day2.h"

using namespace std;

void TestDay2_Part1() {
  istringstream in(string(
      R"(1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc
)"));
  ostringstream out;
  string expected =
      R"(2
)";

  day2(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay2_Part2() {
  istringstream in(string(
      R"(1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc
)"));
  ostringstream out;
  string expected =
      R"(1
)";

  day2_part2(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay2_Part1();
  TestDay2_Part2();
  return 0;
}