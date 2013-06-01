cmake_minimum_required(VERSION 2.8)

set(TEST_LIBS simulator)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_subdirectory(entities)
add_subdirectory(interactions)
