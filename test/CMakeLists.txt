cmake_minimum_required(VERSION 3.9.2)

set(Sources
    publisherTests.cpp
    )

find_library(ops_server REQUIRED)
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )

add_executable(ops_tests ${Sources})
target_link_libraries(ops_tests PUBLIC
    gtest_main
    ops_server
    open62541
)

add_test(
    NAME ops_tests
    COMMAND ops_tests
)