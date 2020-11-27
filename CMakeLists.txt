cmake_minimum_required(VERSION 3.9.2)
set(This openPubSub)
project(${This} C CXX)

option(BUILD_TESTS "Turn on build for tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_PREFIX_PATH "/home/marco/install/")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# CMAKE_POSITION_INDEPENDENT_CODE is nice to have for mixing libraries
# executables and so on together. But not really needed.

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(.)
include_directories(/home/marco/install/open62541/include)
link_directories(/home/marco/install/open62541/lib)
find_library(open62541 REQUIRED)

if (BUILD_TESTS)
    add_subdirectory(googletest)
    add_subdirectory(test)
endif()


set(Headers
    include/openPubSub.h
)

set(Sources
    src/openPubSub.cpp
)

add_library(${This} STATIC ${Sources} ${Headers}
)

add_executable(playground
        ${CMAKE_CURRENT_SOURCE_DIR}/playground.cpp
        )

target_link_libraries(${This} PUBLIC open62541)
target_link_libraries(playground PUBLIC ${This})
