#include "../common/test.h"

#include <sstream>
#include <string>

#include "day25.h"

using namespace std;

void TestDay25() {
  istringstream in(string(
      R"(5764801
17807724
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 14897079
)";

  day25(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay25();
  return 0;
}