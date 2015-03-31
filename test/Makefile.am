check_PROGRAMS = list_test hashtable_test vector_test slist_test deque_test

TESTS = list_test hashtable_test vector_test slist_test deque_test

list_test_LDFLAGS = -static
hashtable_test_LDFLAGS = -static
vector_test_LDFLAGS = -static
slist_test_LDFLAGS = -static
deque_test_LDFLAGS = -static

list_test_SOURCES = list_test.c ../src/list.c ../src/list.h test.c test.h

hashtable_test_SOURCES = hashtable_test.c ../src/hashtable.c \
						../src/hashtable.h ../src/vector.c ../src/vector.h test.c test.h

vector_test_SOURCES = vector_test.c ../src/vector.c ../src/vector.h test.c test.h

slist_test_SOURCES = slist_test.c test.h test.c ../src/slist.h ../src/slist.c

deque_test_SOURCES = deque_test.c test.h test.c ../src/deque.c ../src/deque.h
