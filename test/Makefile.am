check_PROGRAMS = \
list_test \
hashtable_test \
vector_test \
slist_test \
deque_test \
hashset_test \
stack_test \
queue_test \
treetable_test

TESTS = \
list_test \
hashtable_test \
vector_test \
slist_test \
deque_test \
hashset_test \
stack_test \
queue_test \
treetable_test

list_test_LDFLAGS      = -static
hashtable_test_LDFLAGS = -static
vector_test_LDFLAGS    = -static
slist_test_LDFLAGS     = -static
deque_test_LDFLAGS     = -static
hashset_test_LDFLAGS   = -static
stack_test_LDFLAGS     = -static
queue_test_LDFLAGS     = -static
treetable_test_LDFLAGS = -static

treetable_test_CFLAGS = -DDEBUG -g -O0 -Werror
list_test_CFLAGS      = -DDEBUG -g -O0 -Werror
hashtable_test_CFLAGS = -DDEBUG -g -O0 -Werror
vector_test_CFLAGS    = -DDEBUG -g -O0 -Werror
slist_test_CFLAGS     = -DDEBUG -g -O0 -Werror
deque_test_CFLAGS     = -DDEBUG -g -O0 -Werror
hashset_test_CFLAGS   = -DDEBUG -g -O0 -Werror
stack_test_CFLAGS     = -DDEBUG -g -O0 -Werror
queue_test_CFLAGS     = -DDEBUG -g -O0 -Werror

list_test_SOURCES = \
list_test.c \
../src/list.c \
../src/list.h \
test.c \
test.h

hashtable_test_SOURCES =\
hashtable_test.c \
../src/hashtable.c \
../src/hashtable.h \
../src/vector.c \
../src/vector.h \
test.c \
test.h

vector_test_SOURCES =\
vector_test.c\
../src/vector.c\
../src/vector.h\
test.c\
test.h

slist_test_SOURCES =\
slist_test.c\
test.h\
test.c\
../src/slist.h\
../src/slist.c

deque_test_SOURCES =\
deque_test.c\
test.h test.c\
../src/deque.c\
../src/deque.h

hashset_test_SOURCES =\
hashset_test.c\
test.h\
test.c\
../src/hashset.c\
../src/hashset.h\
../src/hashtable.h\
../src/hashtable.c\
../src/vector.c\
../src/vector.h

stack_test_SOURCES =\
stack_test.c\
test.h\
test.c\
../src/stack.c\
../src/stack.h\
../src/vector.c\
../src/vector.h

queue_test_SOURCES =\
queue_test.c\
test.h\
test.c\
../src/queue.c\
../src/queue.h\
../src/deque.c\
../src/deque.h

treetable_test_SOURCES =\
treetable_test.c\
test.c\
test.h\
../src/treetable.c\
../src/treetable.h\
../src/deque.h\
../src/deque.c
