#include <hashtable.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    // Make a new string key table
    HashTable *table;
    hashtable_new(&table);

    hashtable_add(table, (void*) "1", (void*) "one");
    hashtable_add(table, (void*) "2", (void*) "two");
    hashtable_add(table, (void*) "3", (void*) "three");
    hashtable_add(table, (void*) "4", (void*) "four");
    hashtable_add(table, (void*) "5", (void*) "five");

    // ************************************************************
    // Getting the size of the table.
    // ************************************************************

    // Size represents the number of key value pairs currently
    // stored inside the table.
    size_t size = hashtable_size(table);
    printf("Table size: %lu\n", size);

    // ************************************************************
    // Getting the capacity of the table
    // ************************************************************

    // Capacity represents the size of the internal array. (check
    // the initialization example for more info).
    size_t capacity = hashtable_capacity(table);
    printf("Table capacity: %lu\n", capacity);

    // ************************************************************
    // Check if a key is present
    // ************************************************************
    bool is_present = hashtable_contains_key(table, (void*) "3");

    if (is_present) {
        printf("Key exists = %s\n", is_present ? "true" : "false");
    }


    hashtable_destroy(table);
}
