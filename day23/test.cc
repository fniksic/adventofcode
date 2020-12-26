#include "../common/test.h"

#include <sstream>
#include <string>

#include "day23.h"

using namespace std;

void TestDay23() {
  istringstream in(string(
      R"(389125467
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 67384529
)";

  day23(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay23();
  return 0;
}