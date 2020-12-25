#include <cc_hashtable.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    CC_HashTable *table;
    cc_hashtable_new(&table);

    cc_hashtable_add(table, (void*) "1", (void*) "one");
    cc_hashtable_add(table, (void*) "2", (void*) "two");
    cc_hashtable_add(table, (void*) "3", (void*) "three");
    cc_hashtable_add(table, (void*) "4", (void*) "four");
    cc_hashtable_add(table, (void*) "5", (void*) "five");

    /* They easiest way to iterate over key-value pairs is by using the
       foreach macro. This method requires the least amount of boilerplate.
       The downside is that pairs can't be removed during iteration. */

    /* Define key-value output variables. */
    char *key, *value;
    CC_HASHTABLE_FOREACH(table, key, value, { /* This block is the body of the loop. */
            printf("K: %s, V: %s\n", key, value);
        });


    /*************************************************************
      Alternatively, one can iterate manually
    ************************************************************/

    /* Define and initialize the iterator. */
    CC_HashTableIter iterator;
    cc_hashtable_iter_init(&iterator, table);

    /* Define pair entry output. */
    TableEntry *next_entry;
    while (cc_hashtable_iter_next(&iterator, &next_entry) != CC_ITER_END) {
        printf("K: %s, V: %s\n", (char*) next_entry->key, (char*) next_entry->value);
    }


    /************************************************************
      Safely removing pairs during iteration
    ************************************************************/

    /* To safely remove a pair without invalidating the iterator, one should use
       the cc_hashtable_iter_remove function. */

    /* Reinitialize the iterator. */
    cc_hashtable_iter_init(&iterator, table);

    while (cc_hashtable_iter_next(&iterator, &next_entry) != CC_ITER_END) {
        printf("Reading pair: [%s, %s]", (char*) next_entry->key, (char*) next_entry->value);
        if (strcmp((char*) next_entry->key, "4") == 0) {
            void *removed;

            /* Remove the current pair and store the removed value in *removed* */
            cc_hashtable_iter_remove(&iterator, &removed);
            printf(" | removed (val=%s). Moving on...", (char*) removed);
        }
        printf("\n");
    }

    return 0;
}
