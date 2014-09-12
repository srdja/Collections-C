check_PROGRAMS = list_test hashtable_test

TESTS = list_test hashtable_test

list_test_LDFLAGS = -avoid-version -static
hashtable_test_LDFLAGS = -avoid-version -static


list_test_SOURCES = list_test.c ../src/list.c ../src/list.h test.c test.h

hashtable_test_SOURCES = hashtable_test.c ../src/hashtable.c \
						../src/hashtable.h ../src/list.c ../src/list.h test.c test.h

