cmake_minimum_required(VERSION 3.17)
project(adventofcode)

set(CMAKE_CXX_STANDARD 17)

add_executable(day1 day1.cc)
add_executable(day1_part2 day1_part2.cc)

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
