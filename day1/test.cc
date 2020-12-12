#include "../common/test.h"

#include <sstream>
#include <string>

#include "day1.h"

using namespace std;

void TestDay1_Part1() {
  istringstream in(string(
      R"(1721
979
366
299
675
1456)"));
  ostringstream out;
  string expected =
      R"(514579
)";

  day1(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay1_Part2() {
  istringstream in(string(
      R"(1721
979
366
299
675
1456)"));
  ostringstream out;
  string expected =
      R"(241861950
)";

  day1_part2(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay1_Part1();
  TestDay1_Part2();
  return 0;
}