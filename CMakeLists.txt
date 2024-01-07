cmake_minimum_required(VERSION 3.5)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

enable_language(C)

if(UNIX)
    set(CFLAGS "-Wall -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")
endif()


enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)