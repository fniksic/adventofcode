#include "../common/test.h"

#include <sstream>
#include <string>

#include "day16.h"

using namespace std;

void TestDay16() {
  istringstream in(string(
      R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 71
Part 2: 1
)";

  day16(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay16_2() {
  istringstream in(string(
      R"(class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 0
Part 2: 1
)";

  day16(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay16();
  TestDay16_2();
  return 0;
}