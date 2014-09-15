check_PROGRAMS = list_test hashtable_test vector_test

TESTS = list_test hashtable_test vector_test

list_test_LDFLAGS = -avoid-version -static
hashtable_test_LDFLAGS = -avoid-version -static
vector_test_LDFLAGS = -static

list_test_SOURCES = list_test.c ../src/list.c ../src/list.h test.c test.h

hashtable_test_SOURCES = hashtable_test.c ../src/hashtable.c \
						../src/hashtable.h ../src/vector.c ../src/vector.h test.c test.h

vector_test_SOURCES = vector_test.c ../src/vector.c ../src/vector.h test.c test.h
