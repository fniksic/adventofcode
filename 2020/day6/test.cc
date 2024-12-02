#include "../common/test.h"

#include <sstream>
#include <string>

#include "day6.h"

using namespace std;

void TestDay6_SingleGroup() {
  istringstream in(string(
      R"(abcx
abcy
abcz
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 6
Part 2: 3
)";

  day6(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay6_MultipleGroups() {
  istringstream in(string(
      R"(abc

a
b
c

ab
ac

a
a
a
a

b
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 11
Part 2: 6
)";

  day6(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay6_SingleGroup();
  TestDay6_MultipleGroups();
  return 0;
}