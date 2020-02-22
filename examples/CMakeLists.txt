cmake_minimum_required(VERSION 3.5)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CFLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")

add_subdirectory(hashtable)
