#include "day4_common.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using ::std::array;
using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::find;
using ::std::getline;
using ::std::string;
using ::std::string_view;
using ::std::unordered_set;
using ::std::vector;

constexpr array<string_view, 7> expected_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

bool IsExpected(string_view field) {
  return find(expected_fields.begin(), expected_fields.end(), field) != expected_fields.end();
}

vector<string_view> SplitLine(string_view line) {
  string_view::size_type first_pos = 0;
  vector<string_view> result;
  while (first_pos < line.size()) {
    string_view::size_type next_pos = line.find(' ', first_pos);
    if (next_pos == string_view::npos) next_pos = line.size();
    result.push_back(line.substr(first_pos, next_pos - first_pos));
    first_pos = next_pos + 1;
  }
  return result;
}

void ProcessPassports(const Validator &validator) {
  string line;
  unordered_set<string> fields;
  int total_valid = 0;
  while (getline(cin, line)) {
    if (line.empty()) {
      if (fields.size() == expected_fields.size()) ++total_valid;
      fields.clear();
      continue;
    }
    for (string_view kv : SplitLine(line)) {
      string_view field = kv.substr(0, 3);
      string_view value = kv.substr(4);
      if (IsExpected(field) && validator(field, value)) {
        fields.emplace(field);
      }
    }
  }
  // If there was no empty line at the end, we need to account for the last entry
  if (fields.size() == expected_fields.size()) ++total_valid;
  cout << total_valid << endl;
}
