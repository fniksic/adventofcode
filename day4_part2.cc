#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "day4_common.h"

using namespace std;

using ValidatorMap = unordered_map<string, function<bool(const string&)>>;

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

int main() {
  std::cout << "Part 2: ";
  ProcessPassports([](string_view field, string_view value) {
    return kValidatorMap.at(string(field))(string(value));
  });
  return 0;
}
