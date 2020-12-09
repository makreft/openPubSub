cmake_minimum_required( VERSION 3.9.2 )
set( This openPubSub )
project( ${This} C CXX )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
option( BUILD_TESTS "Turn on build for tests" ON )
set( BOOST_ROOT $ENV{HOME}/install/boost_1_75_0/ )
set( BOOST_INCLUDE_DIRS ${BOOST_ROOT}/boost )
set( BOOST_LIBRARY_DIRS ${BOOST_ROOT}/libs )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_C_STANDARD 99 )
# assuming that open62541 has bin installed to $ENV{HOME}/install/
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/ )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )
# CMAKE_POSITION_INDEPENDENT_CODE is nice to have for mixing libraries
# executables and so on together. But not really needed.

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package( Boost REQUIRED )

include_directories(.)
include_directories( $ENV{HOME}/install/open62541/include )
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( $ENV{HOME}/install/open62541/lib )
if ( BUILD_TESTS )
    add_subdirectory( external/googletest )
    add_subdirectory( test )
endif()


set( Headers
    include/openPubSub.h
 )

set( Sources
    src/openPubSub.cpp
 )

add_library( ${This} STATIC ${Sources} ${Headers}
 )

add_executable( playground
        ${CMAKE_CURRENT_SOURCE_DIR}/playground.cpp
        )
add_executable( client
        ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
        )

target_link_libraries( ${This} PUBLIC open62541 )
target_link_libraries( playground PUBLIC ${This} )
target_link_libraries( client PUBLIC ${This} )
