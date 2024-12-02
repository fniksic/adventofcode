#include "../common/test.h"

#include <sstream>
#include <string>

#include "day18.h"

using namespace std;

void TestDay18_1() {
  istringstream in(string(
      R"(1 + 2 * 3 + 4 * 5 + 6
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 71
Part 2: 231
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_2() {
  istringstream in(string(
      R"(1 + (2 * 3) + (4 * (5 + 6))
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 51
Part 2: 51
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_3() {
  istringstream in(string(
      R"(2 * 3 + (4 * 5)
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 26
Part 2: 46
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_4() {
  istringstream in(string(
      R"(5 + (8 * 3 + 9 + 3 * 4 * 3)
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 437
Part 2: 1445
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_5() {
  istringstream in(string(
      R"(5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 12240
Part 2: 669060
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_6() {
  istringstream in(string(
      R"(((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 13632
Part 2: 23340
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay18_7() {
  istringstream in(string(
      R"(1
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 1
Part 2: 1
)";

  day18(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay18_1();
  TestDay18_2();
  TestDay18_3();
  TestDay18_4();
  TestDay18_5();
  TestDay18_6();
  TestDay18_7();
  return 0;
}