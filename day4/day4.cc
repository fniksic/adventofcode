#include "day4.h"

#include <iostream>
#include <string_view>

#include "day4_common.h"

using namespace std;

void day4(istream& in, ostream& out) {
  out << "Part 1: ";
  ProcessPassports(in, out,
                   [](string_view field, string_view value) { return true; });
}
