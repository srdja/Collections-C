Collections-C
=============

A library of generic container types including a list, array, hashtable, deque etc..

####Dependencies
- gcc
- autoconf
- automake
- libtool
- m4

These packages can usually be installed through your distributions package manager.

Building on windows requires [MinGW](http://mingw.org) which provides all the tools needed to build the project.

#### Building the project
In the project directory, run the following commands: `./autogen.sh` then run `./configure`. If autogen complains that it couldn't find a directory named "m4", run: `mkdir m4` and then `./autogen.sh`.

- note: Running configure with a prefix like: `./configure --prefix=/some/custom/path` will cause `make install` to install the library to that directory instead of the default one.

At this point we should be able to build the project by running:
```
make
```
By default `make` builds a shared library. To build a static library, run make with "static" flag on: `make CFLAGS=-static`.  


Installing the library:
```
make install
```

The tests can be run by:
```
make check
```
#### Compiling and linking a simple program
- hello.c
```c
#include <stdio.h>
#include <array.h>

int main(int argc, char **argv) {
    Array *ar = array_new();
    array_add(ar, "Hello World!\n");
    
    printf("%s", (char*) array_get(ar, 0));
    
    return 0;
}
```
We can now build and staticly link the library to our program:
```
gcc hello.c -I/path/to/library/include/ -static -L/path/to/library/lib/ -lcollectc -o hello
```

#### Examples

HashTable:
```c
HashTable *table = hashtable_new();

hashtable_add(table, "some_key", "some_value");
hashtable_add(table, "foo", "bar");

char *value   = hashtable_get("foo");    // returns "bar"
char *removed = hashtable_remove("foo"); // removes the key and returns the value

hashtable_destroy(table);
```
Array (dynamic array):
```c
Array *vec = array_new();

array_add(vec, "foo");
array_add(vec, "bar");
array_add(vec, "baz");

char *foo = array_get(vec, 0);
char *baz = array_get(vec, 2);

char *removed = array_remove_at(vec, 1);

array_destroy(vec);
```
TreeSet (sorted set):
```c
int cmp(void *e1, void *e2) {
    if (*((int*) e1) < *((int*) e2)) return -1;
    if (*((int*) e1) > *((int*) e2)) return 1;
    return 0;
}

TreeSet *set = treeset_new(cmp);

int a = 1;
int b = 2;
int c = 3;

treeset_add(set, &c);
treeset_add(set, &d);
treeset_add(set, &a);
treeset_add(set, &c); // does nothing since the element is already in the set

int first = *((int*) treeset_get_first(set)); // the set is sorted so this returns a

treeset_destroy(set);
```
