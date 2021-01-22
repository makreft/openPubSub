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
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
    add_custom_target( docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)