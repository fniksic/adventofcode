#include "../common/test.h"

#include <array>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "day15.h"

using namespace std;

void TestDay15() {
  istringstream in(string(R"(0 3 6)"));
  ostringstream out;
  string expected =
      R"(Part 1: 436
Part 2: 175594
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_1() {
  istringstream in(string(R"(1 3 2)"));
  ostringstream out;
  string expected =
      R"(Part 1: 1
Part 2: 2578
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_2() {
  istringstream in(string(R"(2 1 3)"));
  ostringstream out;
  string expected =
      R"(Part 1: 10
Part 2: 3544142
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_3() {
  istringstream in(string(R"(1 2 3)"));
  ostringstream out;
  string expected =
      R"(Part 1: 27
Part 2: 261214
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_4() {
  istringstream in(string(R"(2 3 1)"));
  ostringstream out;
  string expected =
      R"(Part 1: 78
Part 2: 6895259
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_5() {
  istringstream in(string(R"(3 2 1)"));
  ostringstream out;
  string expected =
      R"(Part 1: 438
Part 2: 18
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestDay15_6() {
  istringstream in(string(R"(3 1 2)"));
  ostringstream out;
  string expected =
      R"(Part 1: 1836
Part 2: 362
)";

  day15(in, out);

  ASSERT_EQ(expected, out.str());
}

int main() {
  array tests = {TestDay15,   TestDay15_1, TestDay15_2, TestDay15_3,
                 TestDay15_4, TestDay15_5, TestDay15_6};
  vector<thread> threads;
  threads.reserve(tests.size());
  for (const auto& test : tests) {
    threads.emplace_back(test);
  }
  for (auto& th : threads) {
    th.join();
  }
  return 0;
}