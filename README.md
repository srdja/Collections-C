Collections-C
=============

A library of generic container types including a list, vector, hashtable, deque etc..

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

At this point we should be able to build the project by running:
```
make
```
The tests can be run by:
```
make check
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
Vector (dynamic array):
```c
Vector *vec = vector_new();

vector_add(vec, "foo");
vector_add(vec, "bar");
vector_add(vec, "baz");

char *foo = vector_get(vec, 0);
char *baz = vector_get(vec, 2);

char *removed = vector_remove_at(vec, 1);

vector_destroy(vec);
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
