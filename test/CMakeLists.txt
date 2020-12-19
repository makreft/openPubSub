cmake_minimum_required(VERSION 3.9.2)

set(This openPubSubTests)

set(Sources
    publisherTests.cpp
    )

find_library(openPubSub REQUIRED)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    openPubSub
)

add_test(
    NAME ${This}
    COMMAND ${This}
)