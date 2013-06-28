cmake_minimum_required(VERSION 2.8)

set(TEST_LIBS simulator)
include_directories(${GTEST_INCLUDE_DIRS})

def_test(Simulation)

add_subdirectory(event_generators)
add_subdirectory(measurements)
add_subdirectory(state)
