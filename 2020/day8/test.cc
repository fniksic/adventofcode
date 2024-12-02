#include "../common/test.h"

#include <sstream>
#include <string>

#include "day8.h"

using namespace std;

void TestDay8() {
  istringstream in(string(
      R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 5
Part 2: 8
)";

  day8(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay8();
  return 0;
}