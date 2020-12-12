#ifndef ADVENTOFCODE_COMMON_TEST_H_
#define ADVENTOFCODE_COMMON_TEST_H_

#include <iostream>

#define ASSERT_EQ(expected, obtained)               \
  {                                                 \
    if (expected == obtained)                       \
      std::cout << __func__ << ": OK" << std::endl; \
    else                                            \
      std::cout << __func__ << ": Failed!\n"        \
                << "---\nExpected:\n"               \
                << expected << "\nObtained:\n"      \
                << obtained << "---" << std::endl;  \
  }                                                 \
  static_assert(true, "")  // Swallow semicolon

#endif  // ADVENTOFCODE_COMMON_TEST_H_
