cmake_minimum_required( VERSION 3.9.2 )

project( openPubSub C CXX )

# if using the YCM-Plugin for vim
set( CMAKE_EXPORT_COMPILE_COMMANDS OFF )

option( BUILD_TESTS "Turn on build for tests" ON )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_C_STANDARD 99 )

# assuming that open62541 has bin installed to $ENV{HOME}/install/
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541/ )

# CMAKE_POSITION_INDEPENDENT_CODE is nice to have for mixing libraries
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

if (MSVC)
    add_compile_options( /W4 )
else()
    add_compile_options( -Wall -Wextra -Wpedantic )
endif()


add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )

if ( BUILD_TESTS )
    add_subdirectory( external/googletest )
    add_subdirectory( test )
endif()


set( Sources
    openPubSub/openPubSub.cpp )

add_library( openPubSub STATIC ${Sources} )

add_executable( playground
        ${CMAKE_CURRENT_SOURCE_DIR}/playground.cpp )
add_executable( client
        ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp )
add_executable( pubsub
        ${CMAKE_CURRENT_SOURCE_DIR}/pubsub.cpp)
add_executable( sub
    ${CMAKE_CURRENT_SOURCE_DIR}/sub.cpp)

target_link_libraries( openPubSub PUBLIC open62541 )
target_link_libraries( playground PUBLIC openPubSub )
target_link_libraries( client     PUBLIC openPubSub )
target_link_libraries( pubsub     PUBLIC openPubSub )
target_link_libraries( sub        PUBLIC openPubSub )
