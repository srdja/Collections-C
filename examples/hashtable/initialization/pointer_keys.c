/* An example of directly using pointers as keys. */


#include <cc_hashtable.h>
#include <stdlib.h>
#include <stdio.h>

/* In order to use pointers as keys directly as opposed to
   the data behind the pointer, we need to define a function
   that check for pointer equality. */
int pointer_equality(const void *k1, const void *k2)
{
    return !(k1 == k2);
}


int main(int argc, char **argv)
{
    /* ************************************************************
      Config strcture
    ************************************************************/


    /* Define the config structure (for more details check the
       configuration example) */
    CC_HashTableConf config;

    /* While it's not necessary, it's always a good idea to initialize the
       config structure to default values and then override whichever
       value we need. */
    cc_hashtable_conf_init(&config);

    /* The length of our key should be equal to the length of a pointer. */
    config.key_length  = sizeof(void*);

    /* Next, we set the hash function. The library provides a hash function
       that hashes the pointer itself rather than the data behind it. */
    config.hash        = POINTER_HASH;

    /* Finally, we set our comparator function. */
    config.key_compare = pointer_equality;

    /* ************************************************************
       Creating a new table.
    ************************************************************/

    /* Define a new table pointer */
    CC_HashTable *table;

    /* Create a new hashtable that accepts pointers as keys and assign to
     *table*. The return value indicates the success or failure of the
     operation. */
    enum cc_stat status = cc_hashtable_new_conf(&config, &table);

    /* It's always a good idea to check whether the allocation of a new
       structure was successful or not.*/
    if (status != CC_OK) {
        /* Do some error handling */
        if (status == CC_ERR_ALLOC) {
            /* This is the only kind of error cc_hashtable_new can return.
               It means that the allocation has failed. */
        }
    }

    /* ************************************************************
       Adding keys
    ************************************************************/

    /* We can now add pointer keys to our table. */
    void *pointer = malloc(16);
    cc_hashtable_add(table, (void*) pointer, "bar");

    /* We can even hackishly use it for direct integer keys. */
    uintptr_t number  = 23123;
    cc_hashtable_add(table, (void*) number, "foo");

    /* After we're done using the table, we can destroy it with. This
       only destroys the structure, not the actual data inside of it. */
    cc_hashtable_destroy(table);
    return 0;
}
