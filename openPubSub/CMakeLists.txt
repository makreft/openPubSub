cmake_minimum_required( VERSION 3.9.2 )

add_subdirectory(util)
add_subdirectory(ua_pubsub)
find_library(ua_pubsub REQUIRED)
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )

set(ops_server_src_files openPubSub.cpp
    #UATypes.cpp
    #UATypes.h
    #VariableNode.h
    )
add_library(ops_server ${ops_server_src_files})

set(ops_subscription_src_files subscription.cpp)
add_library(ops_subscription ${ops_subscription_src_files})

target_link_libraries(ops_server open62541 util ua_pubsub)
target_link_libraries(ops_subscription open62541 ua_pubsub)
