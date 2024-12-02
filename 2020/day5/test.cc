#include "../common/test.h"

#include <sstream>
#include <string>

#include "day5.h"

using namespace std;

void TestDay5() {
  istringstream in(string(
      R"(BFFFBBFRRR
BFFFBBBLLL
BFFFBBBLLR
BFFFBBBLRR
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 571
Part 2: 570
)";

  day5(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay5();
  return 0;
}