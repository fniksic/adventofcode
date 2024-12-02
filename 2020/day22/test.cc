#include "../common/test.h"

#include <sstream>
#include <string>

#include "day22.h"

using namespace std;

void TestDay22() {
  istringstream in(string(
      R"(Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 306
Part 2: 291
)";

  day22(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay22();
  return 0;
}