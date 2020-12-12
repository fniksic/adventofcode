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

add_library(day2_common day2_common.h day2_common.cc)
target_include_directories(day2_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(day2 day2.cc day2_common.h)
target_link_libraries(day2 LINK_PRIVATE day2_common)
add_executable(day2_part2 day2_part2.cc day2_common.h)
target_link_libraries(day2_part2 LINK_PRIVATE day2_common)

add_executable(day3 day3.cc)

add_library(day4_common day4_common.h day4_common.cc)
target_include_directories(day4_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(day4 day4.cc day4_common.h)
target_link_libraries(day4 LINK_PRIVATE day4_common)
add_executable(day4_part2 day4_part2.cc day4_common.h)
target_link_libraries(day4_part2 LINK_PRIVATE day4_common)

add_executable(day5 day5.cc)
add_executable(day6 day6.cc)
add_executable(day7 day7.cc)
add_executable(day8 day8.cc)
add_executable(day9 day9.cc)
add_executable(day10 day10.cc)

add_library(day11_common day11_common.h day11_common.cc)
target_include_directories(day11_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(day11 day11.cc day11_common.h)
target_link_libraries(day11 LINK_PRIVATE day11_common)
add_executable(day11_part2 day11_part2.cc day11_common.h)
target_link_libraries(day11_part2 LINK_PRIVATE day11_common)

### day12
add_library(day12_lib day12/day12.h day12/day12.cc)
add_executable(day12 day12/main.cc)
target_link_libraries(day12 LINK_PRIVATE day12_lib)
add_executable(day12_test day12/test.cc)
target_link_libraries(day12_test LINK_PRIVATE day12_lib)
