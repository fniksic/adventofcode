#ifndef ADVENTOFCODE__DAY2_COMMON_H_
#define ADVENTOFCODE__DAY2_COMMON_H_

#include <string>
#include <tuple>

std::tuple<std::string, std::string, std::string> SplitLine(
    const std::string& line);

std::tuple<int, int> SplitBounds(const std::string& bounds);

int Count(const std::string& str, char letter);

#endif  // ADVENTOFCODE__DAY2_COMMON_H_
