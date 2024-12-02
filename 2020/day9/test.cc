#include "../common/test.h"

#include <sstream>
#include <string>

#include "day9.h"

using namespace std;

void TestDay9() {
  istringstream in(string(
      R"(35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 127
Part 2: 62
)";

  day9(in, out, /*window_size=*/5);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay9();
  return 0;
}