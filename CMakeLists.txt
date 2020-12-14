cmake_minimum_required(VERSION 3.17)
project(adventofcode)

set(CMAKE_CXX_STANDARD 17)

### day1
add_library(day1_lib day1/day1.h day1/day1.cc day1/day1_part2.cc)
add_executable(day1 day1/main.cc)
target_link_libraries(day1 LINK_PRIVATE day1_lib)
add_executable(day1_part2 day1/main_part2.cc)
target_link_libraries(day1_part2 LINK_PRIVATE day1_lib)
add_executable(day1_test day1/test.cc)
target_link_libraries(day1_test LINK_PRIVATE day1_lib)

### day2
add_library(day2_common day2/day2_common.h day2/day2_common.cc)
add_library(day2_lib day2/day2.h day2/day2.cc day2/day2_part2.cc)
target_link_libraries(day2_lib LINK_PRIVATE day2_common)
add_executable(day2 day2/main.cc)
target_link_libraries(day2 LINK_PRIVATE day2_lib)
add_executable(day2_part2 day2/main_part2.cc)
target_link_libraries(day2_part2 LINK_PRIVATE day2_lib)
add_executable(day2_test day2/test.cc)
target_link_libraries(day2_test LINK_PRIVATE day2_lib)

### day3
add_library(day3_lib day3/day3.h day3/day3.cc)
add_executable(day3 day3/main.cc)
target_link_libraries(day3 LINK_PRIVATE day3_lib)
add_executable(day3_test day3/test.cc)
target_link_libraries(day3_test LINK_PRIVATE day3_lib)

### day4
add_library(day4_common day4/day4_common.h day4/day4_common.cc)
add_library(day4_lib day4/day4.h day4/day4.cc day4/day4_part2.cc)
target_link_libraries(day4_lib LINK_PRIVATE day4_common)
add_executable(day4 day4/main.cc)
target_link_libraries(day4 LINK_PRIVATE day4_lib)
add_executable(day4_part2 day4/main_part2.cc)
target_link_libraries(day4_part2 LINK_PRIVATE day4_lib)
add_executable(day4_test day4/test.cc)
target_link_libraries(day4_test LINK_PRIVATE day4_lib)

### day5
add_library(day5_lib day5/day5.h day5/day5.cc)
add_executable(day5 day5/main.cc)
target_link_libraries(day5 LINK_PRIVATE day5_lib)
add_executable(day5_test day5/test.cc)
target_link_libraries(day5_test LINK_PRIVATE day5_lib)

### day6
add_library(day6_lib day6/day6.h day6/day6.cc)
add_executable(day6 day6/main.cc)
target_link_libraries(day6 LINK_PRIVATE day6_lib)
add_executable(day6_test day6/test.cc)
target_link_libraries(day6_test LINK_PRIVATE day6_lib)

### day7
add_library(day7_lib day7/day7.h day7/day7.cc)
add_executable(day7 day7/main.cc)
target_link_libraries(day7 LINK_PRIVATE day7_lib)
add_executable(day7_test day7/test.cc)
target_link_libraries(day7_test LINK_PRIVATE day7_lib)

### day8
add_library(day8_lib day8/day8.h day8/day8.cc)
add_executable(day8 day8/main.cc)
target_link_libraries(day8 LINK_PRIVATE day8_lib)
add_executable(day8_test day8/test.cc)
target_link_libraries(day8_test LINK_PRIVATE day8_lib)

### day9
add_library(day9_lib day9/day9.h day9/day9.cc)
add_executable(day9 day9/main.cc)
target_link_libraries(day9 LINK_PRIVATE day9_lib)
add_executable(day9_test day9/test.cc)
target_link_libraries(day9_test LINK_PRIVATE day9_lib)

### day10
add_library(day10_lib day10/day10.h day10/day10.cc)
add_executable(day10 day10/main.cc)
target_link_libraries(day10 LINK_PRIVATE day10_lib)
add_executable(day10_test day10/test.cc)
target_link_libraries(day10_test LINK_PRIVATE day10_lib)

### day11
add_library(day11_common day11/day11_common.h day11/day11_common.cc)
add_library(day11_lib day11/day11.h day11/day11.cc day11/day11_part2.cc)
target_link_libraries(day11_lib LINK_PRIVATE day11_common)
add_executable(day11 day11/main.cc)
target_link_libraries(day11 LINK_PRIVATE day11_lib)
add_executable(day11_part2 day11/main_part2.cc)
target_link_libraries(day11_part2 LINK_PRIVATE day11_lib)
add_executable(day11_test day11/test.cc)
target_link_libraries(day11_test LINK_PRIVATE day11_lib)

### day12
add_library(day12_lib day12/day12.h day12/day12.cc)
add_executable(day12 day12/main.cc)
target_link_libraries(day12 LINK_PRIVATE day12_lib)
add_executable(day12_test day12/test.cc)
target_link_libraries(day12_test LINK_PRIVATE day12_lib)

### day13
add_library(day13_lib day13/day13.h day13/day13.cc)
add_executable(day13 day13/main.cc)
target_link_libraries(day13 LINK_PRIVATE day13_lib)
add_executable(day13_test day13/test.cc)
target_link_libraries(day13_test LINK_PRIVATE day13_lib)

### day14
add_library(day14_lib day14/day14.h day14/day14.cc)
add_executable(day14 day14/main.cc)
target_link_libraries(day14 LINK_PRIVATE day14_lib)
add_executable(day14_part2 day14/main_part2.cc)
target_link_libraries(day14_part2 LINK_PRIVATE day14_lib)
add_executable(day14_test day14/test.cc)
target_link_libraries(day14_test LINK_PRIVATE day14_lib)