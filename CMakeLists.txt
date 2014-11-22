cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(actout)


find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerConfig)
include(GoogleTest)
include(ProjectHelper)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

enable_testing(true)

include_directories("src/lib" ${Boost_INCLUDE_DIR})

add_projects(src/exe)
add_projects(src/lib)
add_projects(test/lib)
add_subdirectory(doc)
