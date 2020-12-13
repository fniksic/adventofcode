#include "../common/test.h"

#include <sstream>
#include <string>

#include "day10.h"

using namespace std;

void TestDay10_Small() {
  istringstream in(string(
      R"(16
10
15
5
1
11
7
19
6
12
4
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 35
Part 2: 8
)";

  day10(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay10_Large() {
  istringstream in(string(
      R"(28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 220
Part 2: 19208
)";

  day10(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay10_Small();
  TestDay10_Large();
  return 0;
}