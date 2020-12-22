#include "../common/test.h"

#include <sstream>
#include <string>

#include "day22.h"

using namespace std;

void TestDay22() {
  istringstream in(string(
      R"(
)"));
  ostringstream out;
  string expected =
      R"(
)";

  day22(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay22();
  return 0;
}