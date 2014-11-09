Collections-C
=============

A collections library for C.


##Contents
### Getting started:

### Examples:
- [List](#list)
- [HashTable] (#hashtable)
- [Vector] (#vector)
- [Stack] (#stack)
- [Queue] (#queue)
- [Deque] (#deque)
- [HashSet] (#hashset)

#### List

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
or from a specific splace
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
##### Iterating over the list with an iterator object
While the foreach function is handy when we simply want to go over each list element, it does have limitations.
For instance we could not edit the list during iteration (insert or remove elements). This is where the List Iterator
comes into play. 

We first create a new iterator object that will work with our list
```c
ListIter *iter = list_iter_new(mylist);
```
Now we can iterate over the list using the `list_iter_has_next()` and `list_iter_next()` functions.
The `list_iter_has_next()` function checks whether there are any more elements in the sequence, while the
`list_iter_next()` returns the next element and advances the iterator.
```c
while (list_iter_has_next(iter)) {
    MyType *e = list_iter_next(iter);
}
```
After we are done, we can destroy the iterator.
```c
list_iter_destroy(iter);
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

#### Vector

#### Stack

#### Queue

#### HashSet

#### Deque
