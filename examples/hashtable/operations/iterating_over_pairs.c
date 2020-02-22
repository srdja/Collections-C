#include <hashtable.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    HashTable *table;
    hashtable_new(&table);

    hashtable_add(table, (void*) "1", (void*) "one");
    hashtable_add(table, (void*) "2", (void*) "two");
    hashtable_add(table, (void*) "3", (void*) "three");
    hashtable_add(table, (void*) "4", (void*) "four");
    hashtable_add(table, (void*) "5", (void*) "five");

    /* They easiest way to iterate over key-value pairs is by using the
       foreach macro. This method requires the least amount of boilerplate.
       The downside is that pairs can't be removed during iteration. */

    /* Define key-value output variables. */
    char *key, *value;
    HASHTABLE_FOREACH(table, key, value, { /* This block is the body of the loop. */
            printf("K: %s, V: %s\n", key, value);
        });


    /*************************************************************
      Alternatively, one can iterate manually
    ************************************************************/

    /* Define and initialize the iterator. */
    HashTableIter iterator;
    hashtable_iter_init(&iterator, table);

    /* Define pair entry output. */
    TableEntry *next_entry;
    while (hashtable_iter_next(&iterator, &next_entry) != CC_ITER_END) {
        printf("K: %s, V: %s\n", (char*) next_entry->key, (char*) next_entry->value);
    }


    /************************************************************
      Safely removing pairs during iteration
    ************************************************************/

    /* To safely remove a pair without invalidating the iterator, one should use
       the hashtable_iter_remove function. */

    /* Reinitialize the iterator. */
    hashtable_iter_init(&iterator, table);

    while (hashtable_iter_next(&iterator, &next_entry) != CC_ITER_END) {
        printf("Reading pair: [%s, %s]", (char*) next_entry->key, (char*) next_entry->value);
        if (strcmp((char*) next_entry->key, "4") == 0) {
            void *removed;

            /* Remove the current pair and store the removed value in *removed* */
            hashtable_iter_remove(&iterator, &removed);
            printf(" | removed (val=%s). Moving on...", (char*) removed);
        }
        printf("\n");
    }

    return 0;
}
