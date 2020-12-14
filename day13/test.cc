#include "../common/test.h"

#include <ostream>
#include <sstream>
#include <string>

#include "day13.h"

using namespace std;

void TestDay13() {
  istringstream in(string(
      R"(939
7,13,x,x,59,x,31,19
)"));
  ostringstream out;
  string expected =
      R"(Part 1: 295
Part 2: 1068781
)";

  day13(in, out);

  ASSERT_EQ(expected, out.str());
}

void TestParseLine_CorrectlyParses() {
  string line = "7,13,x,x,59,x,31,19";
  OffsetsAndBusIds expected = {{0, 7}, {-1, 13}, {-4, 59}, {-6, 31}, {-7, 19}};
  ASSERT_EQ(expected, ParseLine(line));
}

void TestGetBestBusTime_ReturnsCorrectSolution() {
  OffsetsAndBusIds offsets_and_bus_ids = {
      {0, 7}, {-1, 13}, {-4, 59}, {-6, 31}, {-7, 19}};
  ASSERT_EQ(295, GetBestBusTime(offsets_and_bus_ids, 939));
}

ostream& operator<<(ostream& out, const BezoutCoefficients& bc) {
  return out << "{x=" << bc.x << ", y=" << bc.y << ", gcd=" << bc.gcd << "}";
}

bool operator==(const BezoutCoefficients& lhs, const BezoutCoefficients& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.gcd == rhs.gcd;
}

void TestExtendedEuclid_ReturnsCorrectBezoutCoefficients() {
  BezoutCoefficients expected = {.x = -9, .y = 47, .gcd = 2};
  ASSERT_EQ(expected, ExtendedEuclid(240, 46));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_1() {
  OffsetsAndBusIds offsets_and_bus_ids = {
      {0, 7}, {-1, 13}, {-4, 59}, {-6, 31}, {-7, 19}};
  ASSERT_EQ(1068781, GetEarliestTimestmap(offsets_and_bus_ids));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_2() {
  OffsetsAndBusIds offsets_and_bus_ids = {{0, 17}, {-2, 13}, {-3, 19}};
  ASSERT_EQ(3417, GetEarliestTimestmap(offsets_and_bus_ids));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_3() {
  OffsetsAndBusIds offsets_and_bus_ids = {{0, 67}, {-1, 7}, {-2, 59}, {-3, 61}};
  ASSERT_EQ(754018, GetEarliestTimestmap(offsets_and_bus_ids));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_4() {
  OffsetsAndBusIds offsets_and_bus_ids = {{0, 67}, {-2, 7}, {-3, 59}, {-4, 61}};
  ASSERT_EQ(779210, GetEarliestTimestmap(offsets_and_bus_ids));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_5() {
  OffsetsAndBusIds offsets_and_bus_ids = {{0, 67}, {-1, 7}, {-3, 59}, {-4, 61}};
  ASSERT_EQ(1261476, GetEarliestTimestmap(offsets_and_bus_ids));
}

void TestGetEarliestTimestmap_ReturnsCorrectTimestamp_6() {
  OffsetsAndBusIds offsets_and_bus_ids = {
      {0, 1789}, {-1, 37}, {-2, 47}, {-3, 1889}};
  ASSERT_EQ(1202161486, GetEarliestTimestmap(offsets_and_bus_ids));
}

int main() {
  TestParseLine_CorrectlyParses();
  TestGetBestBusTime_ReturnsCorrectSolution();
  TestExtendedEuclid_ReturnsCorrectBezoutCoefficients();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_1();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_2();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_3();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_4();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_5();
  TestGetEarliestTimestmap_ReturnsCorrectTimestamp_6();
  TestDay13();
  return 0;
}