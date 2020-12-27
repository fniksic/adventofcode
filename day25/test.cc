#include "../common/test.h"

#include <sstream>
#include <string>

#include "day25.h"

using namespace std;

void TestDay25() {
  istringstream in(string(
      R"(
)"));
  ostringstream out;
  string expected =
      R"(
)";

  day25(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay25();
  return 0;
}