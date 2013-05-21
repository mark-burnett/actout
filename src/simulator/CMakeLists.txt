cmake_minimum_required(VERSION 2.8)

project(simulator)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(PROTO_SOURCES
    requests.proto
    responses.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SOURCES PROTO_HEADERS ${PROTO_SOURCES})

set(SOURCES
    interactions/IInteraction.hpp
    interactions/Ping.cpp
    interactions/Ping.hpp

    ${PROTO_HEADERS}
    ${PROTO_SOURCES}
)

add_library(simulator ${SOURCES})
target_link_libraries(simulator ${PROTOBUF_LIBRARIES})
