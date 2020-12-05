#ifndef ADVENTOFCODE__DAY4_COMMON_H_
#define ADVENTOFCODE__DAY4_COMMON_H_

#include <functional>
#include <string_view>

using Validator = std::function<bool(std::string_view, std::string_view)>;

void ProcessPassports(const Validator &validator);

#endif //ADVENTOFCODE__DAY4_COMMON_H_
