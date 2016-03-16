[![Build Status](https://travis-ci.org/srdja/Collections-C.svg?branch=master)](https://travis-ci.org/srdja/Collections-C)

Collections-C
=============

A library of generic data structures including a list, array, hashtable, deque etc..

## Examples
Check the [documentation](https://srdja.github.io/Collections-C/) for details. And the [wiki](https://github.com/srdja/Collections-C/wiki) for more detailed examples (currently in progress).

HashTable:
```c
HashTable *table = hashtable_new();

hashtable_add(table, "some_key", "some_value");
hashtable_add(table, "foo", "bar");

char *value   = hashtable_get(table, "foo");    // returns "bar"
char *removed = hashtable_remove(table, "foo"); // removes the key and returns the value

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
## Installation

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
- note: When building for the 64 bit architecture, "ARCH_64" can be explicitly set like so: `make CFLAGS=-DARCH_64`. This will increase the maximum container capacity to ~2^64. Otherwise the maximum capacity defaults to ~2^32 elements regardless of the target architecture.

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
We can now build and statically link the library to our program:
```
gcc hello.c -I/path/to/library/include/ -static -L/path/to/library/lib/ -lcollectc -o hello
```

## Contributors

Contributions are very much welcome.

Main work is done on the [development](https://github.com/srdja/Collections-C/tree/development) branch
