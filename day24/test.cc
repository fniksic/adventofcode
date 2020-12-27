#include "../common/test.h"

#include <sstream>
#include <string>

#include "day24.h"

using namespace std;

void TestDay24() {
  istringstream in(string(
      R"(
)"));
  ostringstream out;
  string expected =
      R"(
)";

  day24(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay24();
  return 0;
}