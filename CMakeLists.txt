cmake_minimum_required(VERSION 3.5)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

enable_language(C)

if(UNIX)
    set(CFLAGS "-Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")

elseif(MSVC)
    set(CFLAGS "/W4")
	set(CMAKE_C_FLAGS "$CMAKE_C_FLAGS ${CFLAGS}")
endif()


enable_testing()

add_subdirectory(src)
add_subdirectory(test/unit)
add_subdirectory(test/benchmark)
add_subdirectory(examples)