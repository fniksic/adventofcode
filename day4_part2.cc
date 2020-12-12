#include <iostream>
#include <functional>
#include <string>
#include <string_view>
#include <regex>
#include <unordered_set>
#include <unordered_map>

#include "day4_common.h"

using ::std::cout;
using ::std::function;
using ::std::stoi;
using ::std::string;
using ::std::string_view;
using ::std::regex;
using ::std::regex_match;
using ::std::unordered_set;
using ::std::unordered_map;

using ValidatorMap = unordered_map<string, function<bool(const string&)>>;

bool IsYearInRange(const string& value, int low, int high) {
  static const regex year_regex("\\d{4}");
  if (!regex_match(value, year_regex)) return false;
  int year = stoi(value);
  return low <= year && year <= high;
}

const ValidatorMap kValidatorMap{
    // byr (Birth Year) - four digits; at least 1920 and at most 2002.
    {"byr", [](const string& value) { return IsYearInRange(value, 1920, 2002); }},
    // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    {"iyr", [](const string& value) { return IsYearInRange(value, 2010, 2020); }},
    // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    {"eyr", [](const string& value) { return IsYearInRange(value, 2020, 2030); }},
    // hgt (Height) - a number followed by either cm or in:
    //    If cm, the number must be at least 150 and at most 193.
    //    If in, the number must be at least 59 and at most 76.
    {"hgt", [](const string& value) {
      static const regex height_regex("\\d+(cm|in)");
      if (!regex_match(value, height_regex)) return false;
      int height = stoi(value.substr(0, value.size() - 2));
      string_view unit = string_view(value).substr(value.size() - 2);
      if (unit == "cm") return 150 <= height && height <= 193;
      else return 59 <= height && height <= 76;
    }},
    // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    {"hcl", [](const string& value) {
      static const regex color_regex("#[0-9a-f]{6}");
      return regex_match(value, color_regex);
    }},
    // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    {"ecl", [](const string& value) {
      static const unordered_set<string> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
      return colors.find(value) != colors.end();
    }},
    // pid (Passport ID) - a nine-digit number, including leading zeroes.
    {"pid", [](const string& value) {
      static const regex nine_digit_number_regex("\\d{9}");
      return regex_match(value, nine_digit_number_regex);
    }}};

int main() {
  std::cout << "Part 2: ";
  ProcessPassports([](string_view field, string_view value) {
    return kValidatorMap.at(string(field))(string(value));
  });
  return 0;
}
