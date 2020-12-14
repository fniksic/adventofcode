#ifndef ADVENTOFCODE_DAY13_DAY13_H_
#define ADVENTOFCODE_DAY13_DAY13_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using OffsetsAndBusIds = std::vector<std::pair<int64_t, int64_t>>;

OffsetsAndBusIds ParseLine(const std::string& line);

int64_t GetBestBusTime(const OffsetsAndBusIds& offsets_and_bus_ids,
                       int64_t timestamp);

int64_t GetEarliestTimestmap(const OffsetsAndBusIds& offsets_and_bus_ids);

struct BezoutCoefficients {
  int64_t x;
  int64_t y;
  int64_t gcd;
};

BezoutCoefficients ExtendedEuclid(int64_t m, int64_t n);

void day13(std::istream& in, std::ostream& out);

#endif  // ADVENTOFCODE_DAY13_DAY13_H_
