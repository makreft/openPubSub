cmake_minimum_required( VERSION 3.9.2 )

add_subdirectory(util)
add_subdirectory(ua_pubsub)

set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )
set(ops_server_src_files server.cpp)
set(ops_subscriber_src_files subscriber.cpp)
add_library(ops_server ${ops_server_src_files})
add_library(ops_subscriber ${ops_subscriber_src_files})


target_link_libraries(ops_subscriber open62541)
target_link_libraries(ops_server open62541 util ua_pubsub)