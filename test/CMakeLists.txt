cmake_minimum_required(VERSION 3.9.2)

set(This ops_tests)

set(Sources
    publisherTests.cpp
    )

find_library(ops_server REQUIRED)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    ops_server
)

add_test(
    NAME ${This}
    COMMAND ${This}
)