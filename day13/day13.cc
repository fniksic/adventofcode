#include "day13.h"

#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

OffsetsAndBusIds ParseLine(const string& line) {
  OffsetsAndBusIds result;
  string::size_type token_start = 0;
  int64_t counter = 0;
  while (token_start < line.size()) {
    string::size_type comma = line.find(',', token_start);
    if (comma == string::npos) comma = line.size();
    string token = line.substr(token_start, comma - token_start);
    if (token != "x") result.emplace_back(-counter, stoi(token));
    token_start = comma + 1;
    ++counter;
  }
  return result;
}

int64_t GetBestBusTime(const OffsetsAndBusIds& offsets_and_bus_ids,
                       int64_t timestamp) {
  int64_t best_bus_id = 0;
  int64_t min_wait_time = numeric_limits<int64_t>::max();
  for (auto [offset, bus_id] : offsets_and_bus_ids) {
    int64_t wait_time = (bus_id - timestamp % bus_id) % bus_id;
    if (wait_time < min_wait_time) {
      best_bus_id = bus_id;
      min_wait_time = wait_time;
    }
  }
  return best_bus_id * min_wait_time;
}

BezoutCoefficients ExtendedEuclid(int64_t m, int64_t n) {
  int64_t prev_x = 1, x = 0;
  int64_t prev_y = 0, y = 1;
  int64_t prev_rem = m, rem = n;

  // Invariant: m * prev_x + n * prev_y == prev_rem
  // Invariant: m * x + n * y == rem
  while (rem != 0) {
    int64_t quot = prev_rem / rem;
    int64_t next_rem = prev_rem % rem;
    int64_t next_x = prev_x - quot * x;
    int64_t next_y = prev_y - quot * y;

    // Note: next_rem == prev_rem - quot * rem
    // From that and the invariants:
    //   m * next_x + n * next_y == next_rem
    prev_rem = rem;
    prev_x = x;
    prev_y = y;
    rem = next_rem;
    x = next_x;
    y = next_y;
  }
  return {.x = prev_x, .y = prev_y, .gcd = prev_rem};
}

int64_t Mul(int64_t a, int64_t b, int64_t m) {
  int64_t result = 0;
  a %= m;
  if (b < 0) b = b % m + m;
  while (b != 0) {
    if (b & 1) result = (result + a) % m;
    a = (a << 1) % m;
    b >>= 1;
  }
  return result;
}

int64_t GetEarliestTimestmap(const OffsetsAndBusIds& offsets_and_bus_ids) {
  auto [a, m] = offsets_and_bus_ids.front();
  for (int i = 1; i < offsets_and_bus_ids.size(); ++i) {
    auto [b, n] = offsets_and_bus_ids[i];
    // Solve for t: t == a (mod m) && t == b (mod n)
    auto [x, y, gcd] = ExtendedEuclid(m, n);
    int64_t lcm = m / gcd * n;
    // Need to multiply using modular arithmetic to stay within int64_t.
    int64_t a_y_n = Mul(Mul(a, y, lcm), n / gcd, lcm);
    int64_t b_x_m = Mul(Mul(b, x, lcm), m / gcd, lcm);
    a = (a_y_n + b_x_m) % lcm;
    if (a < 0) a += lcm;
    m = lcm;
  }
  return a;
}

void day13(istream& in, ostream& out) {
  int64_t timestamp;
  string line;
  in >> timestamp >> line;
  OffsetsAndBusIds offsets_and_bus_ids = ParseLine(line);
  out << "Part 1: " << GetBestBusTime(offsets_and_bus_ids, timestamp) << endl;
  out << "Part 2: " << GetEarliestTimestmap(offsets_and_bus_ids) << endl;
}
