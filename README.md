Collections-C
=============

A library of common container types including a list, vector, hashtable, deque etc..

## Contents
- [Getting started](#gettingstarted)
- [Examples](#examples)

### Getting started
- [Setting up the environment](#evn)
- [Building the project](#project)

#### Setting up the environment

##### GNU/Linux
In order to successfully build the project, the following packages must installed first:
- gcc
- autoconf
- automake
- libtool
- m4

These packages can usually be installed through your distributions package manager with the fillowing commands:

- debian / ubuntu `apt-get install gcc autoconf automake libtool m4`
- fedora `yum install gcc autoconf automake libtool m4`
- arch linux `pacman -S gcc autoconf automake libtool m4`

##### Windows
Building on windows requires MinGW which provides all the tools needed to build the project and it can be downloaded from the [MinGW website](http://mingw.org).

#### Building the project
Using the therminal emulator navigate to the project directory
```
cd /path/to/project/Collections-C
```
once there, run the autogen script
```
./autogen.sh
```
If autogen complains that it couldn't find a directory named "m4", run: `mkdir m4` and then `./autogen.sh`.  
  
After autogen run:
```
./configure
```
At this point we should be able to build the project by running:
```
make
```
The test can be run by:
```
make check
```

### Examples
- [List](#list)
- [HashTable](#hashtable)

#### List
* * *
##### Creating a new list:
```c
List *mylist = list_new();
```
##### Adding elements to the list:
```c
list_add(mylist, &my_list_element);
```
##### Removing elements from the list:
```c
list_remove(mylist, &my_list_element);
```
or from a specific place
```c
list_remove_at(mylist, elmenet_index);
```
##### Iterating over the list with foreach:

For example, if we had a list of strings and we simply wanted to print every item, we could do it like this:

First would we define our callback function that will be called on each list element
```c
void print(void *e) {
    printf("%s\n", (char*) e);
}
```
and then we could call the `list_foreach()` with our callback to print the entire list.
```c
list_foreach(mylist, print);
```

##### Getting a specific element from the list:

```c
MyType *my_element = (MyType*) list_get(mylist, index); 
```

##### Destroying the list structure:
```c
list_destroy(mylist);
```

#### HashTable
* * *
##### Creating a new HashTable

```c
HashTable *table = hashtable_new();
```
By default the hashtable is configured to work with string keys, however, it can be configured to work
with other types of keys by 

##### Adding new mappings
```c
hashtable_put(table, "key", "value");
```
##### Removing maps
The `hashtable_remove()` also returns the value that was associated with the removed key
```c
char *removed_value = (char*) hashtable_remove(table, "key");
```
##### Map lookup
```c
char *value = (char*) hashtable_get(table, "key");
````
##### Iterating over HashTable values with foreach
First we need a callback function. In this case a function that will print each value:
```c
void print(void *val) {
    printf("%s\n", (char*) val);
}
```
Now we can call `hashtable_foreach_value()` with our callback function:
```c
hashtable_foreach_value(table, print);
```
##### Iterating over HashTable keys with foreach
The callback function signature that we need to pass to `hashtable_foreach_key()` is slightly different in that it has
a `void const*` instead of a `void*` parameter. This is because the HashTable key must not be modified, as
modification of the key would invalidate the table structure.
```c
void print(void const *key) {
    printf("%s\n", (char*) key);
}
```
```c
hashtable_foreach_key(table, callback);
```
##### Iterating over maps using a `HashTableIter` object
Creating a new `HashTableIter` object:
```c
HashTableIter *iter = hashtable_iter_new(table);
```
We iterate over the entries by with `hashtable_iter_has_next()` and `hashtable_iter_next(iter)` functions. The
`hashtable_iter_has_next()` function checks whether there are more entries in the sequence, while the
`hashtable_iter_next()` advances the iterator. In order to access the key or the value of the current entry,
we need to call `hashtable_iter_get_key` and `hashtable_iter_get_value`.
```c
while (hashtable_iter_has_next(iter)) {
    hashtable_iter_next(iter);
    
    cosnt char *key = (const char*) hashtable_iter_get_key(iter);
    char *val = (char*) hashtable_iter_get_value(iter);
    
    ...
}
```
After we are done, we can destroy the iterator:
```c
hashtable_iter_destroy(iter);
```
