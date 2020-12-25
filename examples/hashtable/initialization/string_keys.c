/* An example of creating a hash table that works with string keys. */

#include <cc_hashtable.h>


int main(int argc, char **argv)
{
    /* Define a new table pointer */
    CC_HashTable *string_table;

    /* Create a new hashtable that accepts strings as keys and assign to
     *string_pointer*. The return value indicates success or failure
     of the operation.

     A thing to note here is that when cc_hashtable_new is used and no
     additional configuration is passed, the default behavior is to
     create a string key table.

     This can be changed by using a different constructor that accepts
     a configuration structure as a parameter. (more details below) */
    enum cc_stat status = cc_hashtable_new(&string_table);

    /* It's always a good idea to check whether the allocation of a new
       structure was successful or not. */
    if (status != CC_OK) {
        /* Do some error handling */

        if (status == CC_ERR_ALLOC) {
            /* This is the only kind of error cc_hashtable_new can return.
               It means that the allocation has failed. */
        }
    }

    /* After we're done using the table, we can destroy it with. This
       only destroys the structure, not the actual data inside of it. */
    cc_hashtable_destroy(string_table);

    /************************************************************
     Alternatively a string key table can be constructed using the
     config structure
    ************************************************************/

    /* Define the config structure (for more details check the
       configuration example) */
    CC_HashTableConf config;

    /* While it's not necessary, it's always a good idea to initialize the
       config structure to default values and then override whichever
       value we need. */
    cc_hashtable_conf_init(&config);

    /* The hash function needs to know the length of the key and since we
       are using strings, the key length will be variable. */
    config.key_length = KEY_LENGTH_VARIABLE;

    /* Next, we set the hash function. The library already provides a
       string hashing function. */
    config.hash = STRING_HASH;

    /* Finally, we need to set the key comparator function. The library
       provides a default comparator. */
    config.key_compare = CC_CMP_STRING;

    /* Now we can create a new table described by the config structure */
    status = cc_hashtable_new_conf(&config, &string_table);


    cc_hashtable_destroy(string_table);
    return 0;
}
