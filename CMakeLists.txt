cmake_minimum_required( VERSION 3.9.2 )

project( ops C CXX )

option( BUILD_TESTS "Turn on build for tests" ON )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_C_STANDARD 99 )

add_subdirectory(openPubSub)

if (MSVC)
    add_compile_options( /W4 )
else()
    add_compile_options( -Wall -Wextra -Wpedantic )
endif()

if ( BUILD_TESTS )
    add_subdirectory( external/googletest )
    add_subdirectory( test )
endif()

add_executable( server_playground
    examples/server_playground.cpp)

target_link_libraries(server_playground ops_server)
