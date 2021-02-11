cmake_minimum_required( VERSION 3.9.2 )

project( ops C CXX )

option( BUILD_TESTS "Turn on to build tests" ON )
option( BUILD_WIRINGPI "Turn on to build Raspberry Pi example" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_C_STANDARD 99 )

add_subdirectory(openPubSub)
find_library(ops_server REQUIRED)
find_library(ua_pubsub REQUIRED)
find_library(ops_subscription REQUIRED)



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

add_executable( subscriber_playground
    examples/subscriber_playground.cpp)

add_executable( subscriber_standalone
    examples/subscriber.cpp)

add_executable( read_server_speed
    examples/read_server_speed.cpp)

add_executable( read_client_speed
    examples/read_client_speed.cpp)

add_executable( tutorial_pubsub_publish
    examples/tutorial_pubsub_publish.c)

#add_executable( ops_server_speed
#    examples/ops_server_speed.cpp)

add_executable( esp32_information_model
    examples/microcontroller_information_model.cpp)

add_executable( rasp_information_model
    examples/sensor_information_model.cpp)


set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED COMPONENTS Events FullNamespace)
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )
set( INCLUDE_INSTALL_DIR $ENV{HOME}/install/open62541/include )
set( LIB_INSTALL_DIR  $ENV{HOME}/install/open6254/lib )
target_link_libraries( server_playground ops_server )
target_link_libraries( subscriber_playground ops_server ops_subscription )
target_link_libraries( subscriber_standalone ops_server )
target_link_libraries( read_server_speed ops_server )
target_link_libraries( read_client_speed ops_server )
target_link_libraries( tutorial_pubsub_publish ops_server )
#target_link_libraries( ops_server_speed ops_server )

target_link_libraries( esp32_information_model
    ops_server open62541)
target_link_libraries( rasp_information_model
    ops_server open62541)

if ( BUILD_WIRINGPI )
# Locate libraries and headers
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS})
add_executable( raspi_thermocouple_publisher
    examples/raspi_thermocouple_publisher.cpp)
target_link_libraries( raspi_thermocouple_publisher
    ops_server open62541 ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

#===DOCS=============
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


