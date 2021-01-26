set(ua_pubsub_source_files
    open62541_queue.h
    types_generated_encoding_binary.h
    ua_pubsub.h
    ua_pubsub_networkmessage.c
    ua_pubsub_networkmessage.h
    ua_types_encoding_binary.c
    ua_types_encoding_binary.h
    ua_util_internal.h
    ua_server_internal.h)



add_library(ua_pubsub ${ua_pubsub_source_files})
set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )
target_link_libraries( ua_pubsub open62541 )
