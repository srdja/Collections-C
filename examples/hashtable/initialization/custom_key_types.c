/* Example of a table using the data behind the pointer as keys.

   (check the pointer key example for direct pointer keys) */

#include <cc_hashtable.h>
#include <stdio.h>


/* Structure that we will use as a key. */
struct Point {
    int x;
    int y;
};


/* We need to create a custom comparator function for our
   Point structure. If keys are equal the function should
   return 0. */
int point_compare(const void *key1, const void *key2)
{
    struct Point p1 = *((struct Point*) key1);
    struct Point p2 = *((struct Point*) key2);
    return !(p1.x == p2.x && p1.y == p2.y);
}


int main(int argc, char **argv)
{
    /* Define the config structure (for more details check the
       configuration example) */
    CC_HashTableConf config;

    /* While it's not necessary, it's always a good idea to initialize the
       config structure to default values and then override whichever
       value we need. */
    cc_hashtable_conf_init(&config);

    /* First we need to set the key length to match the length of
       our Point structure. */
    config.key_length = sizeof(struct Point);

    /* Next, we set the hash function. The library provides a general
       hash function we can use. */
    config.hash = GENERAL_HASH;

    /* Finally we need to set the key comparator function. */
    config.key_compare = point_compare;

    /* We can define a new table */
    CC_HashTable *table;

    /* Create a new hashtable that Point structures as keys and assign to
       *table*. The return value indicates the success or failure of the
       operation. */
    enum cc_stat status = cc_hashtable_new_conf(&config, &table);

    /* It's always a good idea to check whether the allocation of a new
       structure was successful or not. */
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

    struct Point point;
    point.x = 8;
    point.y = 10;

    /* Add a new key value pair. */
    cc_hashtable_add(table, (void*) &point, "foo");

    /* Destroy the table structure */
    cc_hashtable_destroy(table);
    return 0;
}
