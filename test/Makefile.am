check_PROGRAMS = \
list_test \
hashtable_test \
array_test \
slist_test \
deque_test \
hashset_test \
stack_test \
queue_test \
treetable_test \
treeset_test

TESTS = \
list_test \
hashtable_test \
array_test \
slist_test \
deque_test \
hashset_test \
stack_test \
queue_test \
treetable_test \
treeset_test

list_test_LDFLAGS      = -static
hashtable_test_LDFLAGS = -static
array_test_LDFLAGS    = -static
slist_test_LDFLAGS     = -static
deque_test_LDFLAGS     = -static
hashset_test_LDFLAGS   = -static
stack_test_LDFLAGS     = -static
queue_test_LDFLAGS     = -static
treetable_test_LDFLAGS = -static
treeset_test_LDFLAGS   = -static

treetable_test_CFLAGS = -DDEBUG -g -O0 -Werror -std=c99
list_test_CFLAGS      = -DDEBUG -g -O0 -Werror -std=c99
hashtable_test_CFLAGS = -DDEBUG -g -O0 -Werror -std=c99
array_test_CFLAGS    = -DDEBUG -g -O0 -Werror -std=c99
slist_test_CFLAGS     = -DDEBUG -g -O0 -Werror -std=c99
deque_test_CFLAGS     = -DDEBUG -g -O0 -Werror -std=c99
hashset_test_CFLAGS   = -DDEBUG -g -O0 -Werror -std=c99
stack_test_CFLAGS     = -DDEBUG -g -O0 -Werror -std=c99
queue_test_CFLAGS     = -DDEBUG -g -O0 -Werror -std=c99
treeset_test_CFLAGS   = -DDEBUG -g -O0 -Werror -std=c99

treeset_test_SOURCES =\
treeset_test.c\
../src/common.h \
../src/common.c \
../src/treeset.c\
../src/treeset.h\
../src/treetable.c\
../src/treetable.h\
test.c\
test.h

list_test_SOURCES =\
list_test.c \
../src/common.h \
../src/common.c \
../src/list.c \
../src/list.h \
test.c \
test.h

hashtable_test_SOURCES =\
hashtable_test.c \
../src/common.h \
../src/common.c \
../src/hashtable.c \
../src/hashtable.h \
../src/array.c \
../src/array.h \
test.c \
test.h

array_test_SOURCES =\
array_test.c\
../src/common.h \
../src/common.c \
../src/array.c\
../src/array.h\
test.c\
test.h

slist_test_SOURCES =\
../src/common.h \
../src/common.c \
slist_test.c\
test.h\
test.c\
../src/slist.h\
../src/slist.c

deque_test_SOURCES =\
../src/common.h \
../src/common.c \
deque_test.c\
test.h test.c\
../src/deque.c\
../src/deque.h

hashset_test_SOURCES =\
hashset_test.c\
test.h\
test.c\
../src/common.c \
../src/common.h \
../src/hashset.c\
../src/hashset.h\
../src/hashtable.h\
../src/hashtable.c\
../src/array.c\
../src/array.h

stack_test_SOURCES =\
stack_test.c\
test.h\
test.c\
../src/common.h \
../src/common.c \
../src/stack.c\
../src/stack.h\
../src/array.c\
../src/array.h

queue_test_SOURCES =\
queue_test.c\
test.h\
test.c\
../src/common.h \
../src/common.c \
../src/queue.c\
../src/queue.h\
../src/deque.c\
../src/deque.h

treetable_test_SOURCES =\
treetable_test.c\
test.c\
test.h\
../src/common.h \
../src/common.c \
../src/treetable.c\
../src/treetable.h\
../src/deque.h\
../src/deque.c
