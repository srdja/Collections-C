cmake_minimum_required(VERSION 3.5)
project(cc_pool_bench)

include_directories(${PROJECT_SOURCE_DIR}/include ${collectc_INCLUDE_DIRS})

add_executable(dpool_bench dpool_bench.c)
target_link_libraries(dpool_bench collectc)