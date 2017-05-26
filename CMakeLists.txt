cmake_minimum_required(VERSION 3.5)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

enable_language(C)
enable_language(CXX)

set(CFLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")
enable_testing()

add_subdirectory(src)
add_subdirectory(test)
