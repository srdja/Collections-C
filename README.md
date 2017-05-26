Collections-C
=============

> A library of generic data structures including a list, array, hashtable, deque etc..

[![Build Status](https://travis-ci.org/srdja/Collections-C.svg?branch=master)](https://travis-ci.org/srdja/Collections-C) [![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](http://www.gnu.org/licenses/lgpl-3.0)

- [Examples](#examples)
- [Building and Installing](#building-and-installation)
- [Using the library](#using-collections-c-in-your-programs)
- [Contributing](#contributing)

## Examples
Check the [documentation page](https://srdja.github.io/Collections-C/) for mode detailed examples. (This is still in progress).
The source of the documentation can be found [here](https://github.com/srdja/cc-doc-slate).

#### HashTable
```c

// Crate a new table
HashTable *table;
if (hashtable_new(&table) != CC_OK) {
    // something went wrong
    ...
}
// Add key-value pair
if (hashtable_add(table, "some_key", "some_value") != CC_OK) {
    // something went wrong
    ...
}
// Retrieve a value associated with a key
char *value;
if (hashtable_get(table, "some_key", (void*) &value) == CC_OK)
    printf("%s", value);

// Remove a key
hashtable_remove(table, "foo", NULL);
hashtable_destroy(table);
```
#### Array (dynamic array)
```c
// Create a new array
Array *ar;
if (array_new(&ar) != CC_OK) {
    // something went wrong
    ...
}
// Add an element
enum cc_stat status = array_add(ar, "foo");
if (status == CC_OK) {
    ...
} else if (status == CC_ERR_ALLOC) {
    ...
} else {
    ...
}
// Retrieve a value
char *foo;
array_get_at(ar, 0, (void*) &foo);

// Remove a value
char *removed;
array_remove_at(ar, 0, (void*) &removed);

array_destroy(ar);
```
## Building and Installation

### Dependencies

- C compiler (gcc, clang, etc...)
- cmake (>= 3.5)
- [testing only] cpputest (>=3.8)
- pkg-config

These packages can usually be installed through your distributions package manager.

Building on windows requires [MinGW](http://mingw.org) which provides all the tools needed to build the project.

### Building the project

To build the project, we first need to create a separate build directory:

```
mkdir build
```

Now that we've created our build directory (assuming it's created in the projct root), we can `cd` into it and run `cmake` and pass the parent directory path to it, which is where the `CMakeLists.txt` file is located:

```
cd build
cmake ..
```

Once `cmake` is done generating makefiles, we can build the library by running `make` inside our build directory:

```
make
```
This will build both the static and the dynamic library.

### Runing tests

```
make test
```

Example:
```
make test
...
Running tests...
Test project /Users/radu/Dropbox/projects/Collections-C/build
      Start  1: ArrayTest
 1/10 Test  #1: ArrayTest ........................   Passed    0.00 sec
      Start  2: DequeTest
 2/10 Test  #2: DequeTest ........................   Passed    0.00 sec
      Start  3: ListTest
 3/10 Test  #3: ListTest .........................   Passed    0.01 sec
      Start  4: HashSetTest
 4/10 Test  #4: HashSetTest ......................   Passed    0.00 sec
      Start  5: HashTableTest
 5/10 Test  #5: HashTableTest ....................   Passed    0.00 sec
      Start  6: QueueTest
 6/10 Test  #6: QueueTest ........................   Passed    0.00 sec
      Start  7: SlistTest
 7/10 Test  #7: SlistTest ........................   Passed    0.00 sec
      Start  8: StackTest
 8/10 Test  #8: StackTest ........................   Passed    0.00 sec
      Start  9: TreeSetTest
 9/10 Test  #9: TreeSetTest ......................   Passed    0.00 sec
      Start 10: TreeTableTest
10/10 Test #10: TreeTableTest ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 10

Total Test time (real) =   0.04 sec
```

Running individual tests
```
make build
...
> build/test/array_test -c -v
TEST(ArrayTestsFilter, ArrayFilter2) - 0 ms
TEST(ArrayTestsFilter, ArrayFilter1) - 0 ms
TEST(ArrayTestsFilter, ArrayFilterMut2) - 0 ms
TEST(ArrayTestsFilter, ArrayFilterMut1) - 0 ms
TEST(ArrayTestsArrayConf, ArrayCapacity) - 0 ms
TEST(ArrayTestsArrayConf, ArrayAddAt) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayReduce) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayZipIterReplace) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayZipIterAdd) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayZipIterRemove) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayZipIterNext) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayIterReplace) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayIterRemove) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayContains) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayReverse) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayDeepCopy) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayShallowCopy) - 0 ms
TEST(ArrayTestsWithDefaults, ArraySubarray) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayIndexOf) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayGetAt) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayRemoveAll) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayRemoveAt) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayRemove) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayReplaceAt) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayAddAt2) - 0 ms
TEST(ArrayTestsWithDefaults, ArrayAdd) - 0 ms

OK (26 tests, 26 ran, 115 checks, 0 ignored, 0 filtered out, 1 ms)

```
### Installing

To install the library run:
```
sudo make install
```
By default the libraries and headers will be installed in `/usr/local/lib/` and `/usr/local/include` directories.

## Using Collections-C in your programs

### A simple program
If we already built and installed the library, we can write a simple hello world program and save it to a file named `hello.c`:

```c
#include <stdio.h>
#include <collectc/array.h>

int main(int argc, char **argv) {
    // Create a new array
    Array *ar;
    array_new(&ar);

    // Add a string to the array
    array_add(ar, "Hello World!\n");

    // Retreive the string and print it
    char *str;
    array_get_at(ar, 0, (void*) &str);
    printf("%s", str);

    return 0;
}
```

Now we need to compile and link our program. Since `make` builds both the static and the dynamic library we can choose which one we wish to link into our program.

### Linking statically

If we wish to statically link the library to our program we can pass the `-static` flag to the compiler

```
gcc hello.c -static -lcollectc -o hello
```


or similarly when compiling with clang:
```
clang hello.c -static -lcollectc -o hello
```

This will link the library by copying it into the executable. We can use this option if we **don't** wish to have Collections-C as a **runtime dependency**, however this comes at the expense of generating a larger executable.

### Linking dynamically

We can also choose to link with the library dynamically at runtime. This is the default behaviour if ommit the `-static` compiler flag:

```
gcc hello.c -lcollectc -o hello
```

or with clang:

```
clang hello.c -lcollectc -o hello
```

Linking dynamically produces a smaller executable, but requires `libcollectc.so` to be present on every system on which the program is going to be executed.

### Linking problems

Sometimes the compiler may have trouble finding the library or the headers. This is usually because the it's looking for them in the wrong directory, which may happen if the library or the headers or both are installed in a non-standard directory or not installed at all.

If this is the case, we can explicitly tell the compiler where to look for them by passing the `-I[path to headers]` and `-L[path to libraries]` options:

```
gcc hello.c `pkg-config --cflags --libs collectionc` -o hello
```

### Running the program

If everything went well with the compilation we can run the executable:

```
./hello
```

and it should print `Hello, World!` to the console.

## Contributing

Contributions are welcome.

If you have a feature request, or have found a bug, feel free to open a [new issue](https://github.com/srdja/Collections-C/issues/new). If you wish to contribute code, see [CONTRIBUTING.md](https://github.com/srdja/Collections-C/blob/master/CONTRIBUTING.md) for more details.
