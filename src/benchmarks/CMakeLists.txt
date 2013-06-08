cmake_minimum_required(VERSION 2.8)

project(benchmarks)

set(SOURCES
    bench.cpp
)

add_executable(bench ${SOURCES})
target_link_libraries(bench simulator)
