#include "day4.h"

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using Validator = function<bool(string_view, string_view)>;
using ValidatorMap = unordered_map<string, function<bool(const string&)>>;

constexpr array<string_view, 7> expected_fields = {"byr", "iyr", "eyr", "hgt",
                                                   "hcl", "ecl", "pid"};

bool IsExpected(string_view field) {
  return find(expected_fields.begin(), expected_fields.end(), field) !=
         expected_fields.end();
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

void ProcessPassports(istream& in, ostream& out, const Validator& validator) {
  string line;
  unordered_set<string> fields;
  int total_valid = 0;
  while (getline(in, line)) {
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
  // If there was no empty line at the end, we need to account for the last
  // entry
  if (fields.size() == expected_fields.size()) ++total_valid;
  out << total_valid << endl;
}

void day4(istream& in, ostream& out) {
  out << "Part 1: ";
  ProcessPassports(in, out,
                   [](string_view field, string_view value) { return true; });
}

bool IsYearInRange(const string& value, int low, int high) {
  static const regex year_regex("\\d{4}");
  if (!regex_match(value, year_regex)) return false;
  int year = stoi(value);
  return low <= year && year <= high;
}

const ValidatorMap kValidatorMap{
    {"byr",
     [](const string& value) { return IsYearInRange(value, 1920, 2002); }},
    {"iyr",
     [](const string& value) { return IsYearInRange(value, 2010, 2020); }},
    {"eyr",
     [](const string& value) { return IsYearInRange(value, 2020, 2030); }},
    {"hgt",
     [](const string& value) {
       static const regex height_regex("\\d+(cm|in)");
       if (!regex_match(value, height_regex)) return false;
       int height = stoi(value.substr(0, value.size() - 2));
       string_view unit = string_view(value).substr(value.size() - 2);
       if (unit == "cm")
         return 150 <= height && height <= 193;
       else
         return 59 <= height && height <= 76;
     }},
    {"hcl",
     [](const string& value) {
       static const regex color_regex("#[0-9a-f]{6}");
       return regex_match(value, color_regex);
     }},
    {"ecl",
     [](const string& value) {
       static const unordered_set<string> colors = {"amb", "blu", "brn", "gry",
                                                    "grn", "hzl", "oth"};
       return colors.find(value) != colors.end();
     }},
    {"pid", [](const string& value) {
       static const regex nine_digit_number_regex("\\d{9}");
       return regex_match(value, nine_digit_number_regex);
     }}};

void day4_part2(std::istream& in, std::ostream& out) {
  out << "Part 2: ";
  ProcessPassports(in, out, [](string_view field, string_view value) {
    return kValidatorMap.at(string(field))(string(value));
  });
}
