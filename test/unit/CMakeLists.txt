cmake_minimum_required(VERSION 3.15)
project(collectc_test)

#find_package(PkgConfig)
#pkg_check_modules(CPPUTEST REQUIRED cpputest>=3.8)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUTEST_CXX_FLAGS}")

if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

set(array_test_sources munit.c array_test.c)
set(deque_test_sources munit.c "deque_test.c")
set(list_test_sources munit.c "list_test.c")
set(hashset_test_sources munit.c "hashset_test.c")
set(hashtable_test_sources munit.c "hashtable_test.c")
set(pqueue_test_sources munit.c "pqueue_test.c")
set(queue_test_sources munit.c "queue_test.c")
set(slist_test_sources munit.c "slist_test.c")
set(stack_test_sources munit.c "stack_test.c")
set(treeset_test_sources munit.c "treeset_test.c")
set(treetable_test_sources munit.c "treetable_test.c")
set(rbuf_test_sources munit.c "ring_buffer_test.c")
set(tsttable_test_sources munit.c "tst_table_test.c")

set(array_sized_test_sources munit.c array_sized_test.c)
set(dynamic_pool_test_sources munit.c "dynamic_pool_test.c")
set(static_pool_test_sources munit.c static_pool_test.c)

include_directories(${PROJECT_SOURCE_DIR}/include ${collectc_INCLUDE_DIRS})
message(${collectc_INCLUDE_DIRS})

add_executable(array_test ${array_test_sources})
add_executable(deque_test ${deque_test_sources})
add_executable(list_test ${list_test_sources})
add_executable(hashset_test ${hashset_test_sources})
add_executable(hashtable_test ${hashtable_test_sources})
add_executable(pqueue_test ${pqueue_test_sources})
add_executable(queue_test ${queue_test_sources})
add_executable(slist_test ${slist_test_sources})
add_executable(stack_test ${stack_test_sources})
add_executable(treeset_test ${treeset_test_sources})
add_executable(treetable_test ${treetable_test_sources})
add_executable(rbuf_test ${rbuf_test_sources})
add_executable(tsttable_test ${tsttable_test_sources})

add_executable(array_sized_test ${array_sized_test_sources})
add_executable(dynamic_pool_test ${dynamic_pool_test_sources})
add_executable(static_pool_test ${static_pool_test_sources})

target_link_libraries(array_test collectc)
target_link_libraries(deque_test collectc)
target_link_libraries(list_test collectc)
target_link_libraries(hashset_test collectc)
target_link_libraries(hashtable_test collectc)
target_link_libraries(pqueue_test collectc)
target_link_libraries(queue_test collectc)
target_link_libraries(slist_test collectc)
target_link_libraries(stack_test collectc)
target_link_libraries(treeset_test collectc)
target_link_libraries(treetable_test collectc)
target_link_libraries(rbuf_test collectc)
target_link_libraries(tsttable_test collectc)

target_link_libraries(array_sized_test collectc)
target_link_libraries(dynamic_pool_test collectc)
target_link_libraries(static_pool_test collectc)

add_test(ArrayTest array_test)
add_test(DequeTest deque_test)
add_test(ListTest list_test)
add_test(HashSetTest hashset_test)
add_test(HashTableTest hashtable_test)
add_test(PQueueTest pqueue_test)
add_test(QueueTest queue_test)
add_test(SlistTest slist_test)
add_test(StackTest stack_test)
add_test(TreeSetTest treeset_test)
add_test(TreeTableTest treetable_test)
add_test(RbufTest rbuf_test)
add_test(TSTTableTest tsttable_test)

add_test(ArraySizedTest array_sized_test)
add_test(DynamicPoolTest dynamic_pool_test)
add_test(StaticPoolTest static_pool_test)