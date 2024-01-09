Collections-C
=============

> A library of generic data structures including a list, array, hashtable, deque etc..

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](http://www.gnu.org/licenses/lgpl-3.0)

- [Examples](#examples)
- [Building and Installing](#building-and-installation)
- [Using the library](#using-collections-c-in-your-programs)
- [Contributing](#contributing)

# Examples
Check the [documentation page](https://srdja.github.io/Collections-C/) for mode detailed examples. (This is still in progress).
The source of the documentation can be found [here](https://github.com/srdja/cc-doc-slate).

### HashTable
```c

// Create a new table
CC_HashTable *table;
if (cc_hashtable_new(&table) != CC_OK) {
    // something went wrong
    ...
}
// Add key-value pair
if (cc_hashtable_add(table, "some_key", "some_value") != CC_OK) {
    // something went wrong
    ...
}
// Retrieve a value associated with a key
char *value;
if (cc_hashtable_get(table, "some_key", (void*) &value) == CC_OK)
    printf("%s", value);

// Remove a key
cc_hashtable_remove(table, "foo", NULL);
cc_hashtable_destroy(table);
```
### Array (dynamic array)
```c
// Create a new array
CC_Array *ar;
if (cc_array_new(&ar) != CC_OK) {
    // something went wrong
    ...
}
// Add an element
enum cc_stat status = cc_array_add(ar, "foo");
if (status == CC_OK) {
    ...
} else if (status == CC_ERR_ALLOC) {
    ...
} else {
    ...
}
// Retrieve a value
char *foo;
cc_array_get_at(ar, 0, (void*) &foo);

// Remove a value
char *removed;
cc_array_remove_at(ar, 0, (void*) &removed);

cc_array_destroy(ar);
```
# Building and Installation

## Dependencies 
### Linux

- C compiler (gcc or clang)
- cmake (>= 3.5)
- pkg-config

These packages can usually be installed through your distributions package manager.

### Windows

* [Visual Studio](https://visualstudio.microsoft.com) (recommended) ***or*** [MinGW](http://mingw.org)
* [cmake](https://cmake.org/download/)


## Building the project
### Linux

To build the project, we first need to create a separate build directory (if it doesn't already exist):

```
mkdir build
```

From this directory we can run the `cmake` command and configure the build:

* `cmake ..` or `cmake -DSHARED=True` to make Collections-C build as a shared library
* `cmake -DSHARED=False` to build a static library

Once `cmake` is done generating makefiles, we can build the library by running `make` inside our build directory.

An example of cloning and building a static library:
```
git clone https://github.com/Collections-C.git
cd Collections-C
mkdir build
cd build
cmake -DSHARED=False
make
```

## Running tests

To run tests (from the `build` directory):

```
make test
```

To run individual tests, simply run the appropriate executable. For example:
```
build/test/array_test
```
### Installing

To install the library run:
```
sudo make install
```
By default the libraries and headers will be installed in `/usr/local/lib/` and `/usr/local/include` directories.

You have to make the system's runtime aware of the location of the new library to be able to run dynamically linked applications. This might be as simple as running the following command if your `/etc/ld.so.conf` contains the install directory.

**Note: macOS doesn't support ldconfig.**
```
sudo ldconfig
```

## Using Collections-C in your programs

### A simple program
If we already built and installed the library, we can write a simple hello world program and save it to a file named `hello.c`:

```c
#include <stdio.h>
#include <collectc/cc_array.h>

int main(int argc, char **argv) {
    // Create a new array
    CC_Array *ar;
    cc_array_new(&ar);

    // Add a string to the array
    cc_array_add(ar, "Hello World!\n");

    // Retreive the string and print it
    char *str;
    cc_array_get_at(ar, 0, (void*) &str);
    printf("%s", str);

    return 0;
}
```

Now we need to compile and link our program. Since `make` builds both the static and the dynamic library we can choose which one we wish to link into our program.

### Linking statically

If we wish to statically link the library to our program we can pass the `-static` flag to the compiler

**Note:** On macOS, the **`-static`** flag is not very friendly (it requires that all the libraries are statically linked). So we can replace **`-static -lcollectc`** with the full path to the static library. Which is **`/usr/local/lib/libcollectc.a`** by default.

```
gcc hello.c -static -lcollectc -o hello
```


or similarly when compiling with clang:
```
clang hello.c -static -lcollectc -o hello
```

This will link the library by copying it into the executable. We can use this option if we **don't** wish to have Collections-C as a **runtime dependency**, however this comes at the expense of generating a larger executable.

### Linking dynamically

We can also choose to link with the library dynamically at runtime. This is the default behaviour if omit the `-static` compiler flag:

```
gcc hello.c -lcollectc -o hello
```

or with clang:

```
clang hello.c -lcollectc -o hello
```

Linking dynamically produces a smaller executable, but requires `libcollectc.so` to be present on every system on which the program is going to be executed.

### Linking problems

Sometimes the compiler may have trouble finding the library or the headers. This is usually because it's looking for them in the wrong directory, which may happen if the library or the headers or both are installed in a non-standard directory or not installed at all.

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
