#ifndef ADVENTOFCODE_COMMON_TEST_H_
#define ADVENTOFCODE_COMMON_TEST_H_

#include <iostream>
#include <set>
#include <utility>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> vec) {
  const char* separator = "";
  out << '[';
  for (const auto& element : vec) {
    out << separator << element;
    separator = ", ";
  }
  out << ']';
  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::set<T> s) {
  const char* separator = "";
  out << '{';
  for (const auto& element : s) {
    out << separator << element;
    separator = ", ";
  }
  out << '}';
  return out;
}

template <typename S, typename T>
inline std::ostream& operator<<(std::ostream& out, std::pair<S, T> vec) {
  return out << '(' << vec.first << ", " << vec.second << ')';
}

#define ASSERT_EQ(expected, obtained)                              \
  {                                                                \
    const auto eval_obtained = obtained;                           \
    if (expected == eval_obtained)                                 \
      std::cout << __func__ << ": OK" << std::endl;                \
    else                                                           \
      std::cout << __func__ << ": Failed!\n"                       \
                << "---\nExpected: " << expected                   \
                << "\nObtained: " << eval_obtained << "\n---\n\n"; \
  }                                                                \
  static_assert(true, "")  // Swallow semicolon

#endif  // ADVENTOFCODE_COMMON_TEST_H_
