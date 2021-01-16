set(util_source_files
    util.cpp
    util.h)

add_library(util ${util_source_files})

set( CMAKE_PREFIX_PATH $ENV{HOME}/install/open62541 )
add_library( open62541 SHARED IMPORTED REQUIRED )
set_target_properties( open62541 PROPERTIES IMPORTED_LOCATION ${CMAKE_PREFIX_PATH}/lib/libopen62541.so )
target_include_directories( open62541 INTERFACE ${CMAKE_PREFIX_PATH}/include )
target_link_libraries(util open62541)
