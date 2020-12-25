#include <cc_hashtable.h>


int main(int argc, char **argv)
{
    // Make a new string key table
    CC_HashTable *table;
    cc_hashtable_new(&table);
    cc_hashtable_add(table, (void*) "1", (void*) "one");
    cc_hashtable_add(table, (void*) "2", (void*) "two");
    cc_hashtable_add(table, (void*) "3", (void*) "three");

    // Remove the value at key *"1"* and store it at *removed_value*.
    void *removed_value;
    enum cc_stat status = cc_hashtable_remove(table, (void*) "1", &removed_value);

    // Before doing anything with the removed_value it's a good idea to
    // check if the value was actually removed.
    if (status == CC_ERR_KEY_NOT_FOUND) {
        // Handle error.
    }

    // WARNING: You should never use this function inside an iterator loop
    // to remove keys during iteration. Check the iterator example for
    // more details.

    // ************************************************************
    // Clearing the table
    // ************************************************************
    cc_hashtable_remove_all(table);

    return 0;
}
