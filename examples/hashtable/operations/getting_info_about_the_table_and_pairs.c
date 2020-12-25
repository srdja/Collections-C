#include <cc_hashtable.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    // Make a new string key table
    CC_HashTable *table;
    cc_hashtable_new(&table);

    cc_hashtable_add(table, (void*) "1", (void*) "one");
    cc_hashtable_add(table, (void*) "2", (void*) "two");
    cc_hashtable_add(table, (void*) "3", (void*) "three");
    cc_hashtable_add(table, (void*) "4", (void*) "four");
    cc_hashtable_add(table, (void*) "5", (void*) "five");

    // ************************************************************
    // Getting the size of the table.
    // ************************************************************

    // Size represents the number of key value pairs currently
    // stored inside the table.
    size_t size = cc_hashtable_size(table);
    printf("Table size: %lu\n", size);

    // ************************************************************
    // Getting the capacity of the table
    // ************************************************************

    // Capacity represents the size of the internal array. (check
    // the initialization example for more info).
    size_t capacity = cc_hashtable_capacity(table);
    printf("Table capacity: %lu\n", capacity);

    // ************************************************************
    // Check if a key is present
    // ************************************************************
    bool is_present = cc_hashtable_contains_key(table, (void*) "3");

    if (is_present) {
        printf("Key exists = %s\n", is_present ? "true" : "false");
    }


    cc_hashtable_destroy(table);
}
