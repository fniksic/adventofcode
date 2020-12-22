#include "../common/test.h"

#include <sstream>
#include <string>

#include "day21.h"

using namespace std;

void TestDay21() {
  istringstream in(string(
      R"(
)"));
  ostringstream out;
  string expected =
      R"(
)";

  day21(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay21();
  return 0;
}