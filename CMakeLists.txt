cmake_minimum_required(VERSION 3.9.2)
project(openPubSub)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/home/marco/install/")

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(.)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
find_library(open62541 REQUIRED)
include_directories(/home/marco/install/open62541/include)
# include_directories(/home/marco/install/open62541)
# include_directories(open62541 PUBLIC /home/marco/install/open62541/include)
link_directories(/home/marco/install/open62541/lib)
# include_directories(/home/marco/git/openPubSub/open62541/build)
# set(/home/marco/git/openPubSub/open62541/build/open62541Config.cmake)
# target_link_libraries(open62541 ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libopen62541.a INTERFACE)
# add_library(open62541 SHARED IMPORTED)
# set_property(TARGET open62541 PROPERTY IMPORTED_LOCATION /home/marco/git/openPubSub/lib/libopen62541.a)
# set_property(TARGET open62541 PROPERTY IMPORTED_LOCATION /home/marco/git/openPubSub/lib/libopen62541.so.1)
# set_property(TARGET open62541 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopen62541.so.1)

add_library(openPubSub
        include/openPubSub.h
        src/openPubSub.cpp
)

add_executable(pubsubServer
        # ${CMAKE_CURRENT_SOURCE_DIR}/server_pubsub.h
        ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)

find_package(Threads REQUIRED)

add_executable(playground
        ${CMAKE_CURRENT_SOURCE_DIR}/playground.cpp
        )

message(${CMAKE_THREAD_LIBS_INIT})#pthread library

target_link_libraries(openPubSub PUBLIC open62541)
target_link_libraries(pubsubServer PUBLIC open62541)
# target_link_libraries(playground ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
target_link_libraries(playground PUBLIC openPubSub)
