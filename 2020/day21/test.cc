#include "../common/test.h"

#include <sstream>
#include <string>

#include "day21.h"

using namespace std;

void TestDay21() {
  istringstream in(string(
      R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish)
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 5
Part 2: mxmxvkd,sqjhc,fvjkl
)";

  day21(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  TestDay21();
  return 0;
}