cmake_minimum_required(VERSION 3.9.2)
set( BIN_NAME ops_tests )
set(Sources
    ServerTests.cpp
    )

find_library(ops_server REQUIRED)
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )

add_executable(${BIN_NAME} ${Sources})

add_test(
    NAME ops_tests
    COMMAND ops_tests
)
#include( ../cmake/CodeCoverage.cmake )
# SETUP_TARGET_FOR_COVERAGE(
#     coverage_${BIN_NAME}  # Name for custom target.
#     ${BIN_NAME}         # Name of the test driver executable that runs the tests.
#     # NOTE! This should always have a ZERO as exit code
#     # otherwise the coverage generation will not complete.
#     coverage_${BIN_NAME}_dir            # Name of output directory.
# )
target_link_libraries(${BIN_NAME} PUBLIC
    gtest_main
    ops_server
    open62541
    )
