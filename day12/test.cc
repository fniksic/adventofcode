#include <iostream>
#include <sstream>
#include <string>

#include "day12.h"

using namespace std;

void TestDay12() {
  istringstream in(string(
      R"(F10
N3
F7
R90
F11)"));
  ostringstream out;
  string expected =
      R"(Part 1: 25
Part 2: 276
)";

  day12(in, out);

  if (out.str() == expected)
    cout << "TestDay12: OK" << endl;
  else
    cout << "TestDay12: Failed!\n\n"
         << "Expected:\n---\n"
         << expected << "\nProduced:\n---\n"
         << out.str() << endl
         << endl;
}

int main() {
  TestDay12();
  return 0;
}