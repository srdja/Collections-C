#include <cc_hashtable.h>


int main(int argc, char **argv)
{
    /* Make a new string key table */
    CC_HashTable *table;
    cc_hashtable_new(&table);

    /* Assign value "one" to key "1" inside the table "table" */
    enum cc_stat status = cc_hashtable_add(table, (void*) "1", (void*) "one");

    /* Checking the status of the operation is alwasy a good idea since
       memory allocation can fail (lack of space, etc.). */
    if (status == CC_ERR_ALLOC) {
        /* Internal allocation error. This is the only type of error
           cc_hashtable_add can return. If everything went well CC_OK is
           returned instead. */
    }

    cc_hashtable_destroy(table);
}
