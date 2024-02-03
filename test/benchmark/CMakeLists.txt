cmake_minimum_required(VERSION 3.15)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if (UNIX)
set(CFLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")
endif()

add_subdirectory(pool)