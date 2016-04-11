lib_LTLIBRARIES = libcollectc.la

libcollectc_la_CFLAGS= -O2 -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wall -pedantic -std=c99 -Wcast-qual -Wshadow

libcollectc_la_LDFLAGS= -module -avoid-version -shared

libcollectc_la_SOURCES = array.c     stack.c      slist.c \
                         queue.c     hashtable.c  hashset.c \
                         deque.c     list.c       common.h \
                         array.h     list.h       slist.h \
                         hashtable.h hashset.h    deque.h \
                         stack.h     queue.h      treetable.c \
                         treetable.h treeset.c    treeset.h \
                         common.c

library_includedir = $(includedir)/collectc

library_include_HEADERS = hashtable.h  hashset.h \
                          array.h          list.h       slist.h \
                          common.h         deque.h      queue.h \
                          stack.h          treetable.h  treeset.h
