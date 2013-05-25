cmake_minimum_required(VERSION 2.8)

set(TEST_LIBS simulator)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(entities)
add_subdirectory(interactions)
