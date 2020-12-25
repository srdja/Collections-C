/* Example of performance tuning a table by setting it's capacity
   and load factor. */


#include <cc_hashtable.h>


int main(int argc, char **argv)
{
    CC_HashTableConf config;
    cc_hashtable_conf_init(&config);

    /* As explained in the string_key example if we don't specifically set
     the key options they'll default to string.

     ************************************************************

     Set the initial capacity of the internal array. Note that
     the value here is a power of 2. Non 2^n values can be passed,
     but they'll be rounded to the nearest upper 2^n value for performance
     reasons (12 would round to 16, etc.).

     Note that a hashtable can hold an unlimited (limited only by
     memory) number key/value pairs. This value is better viewed
     not as an absolute capacity, but as a performant capacity.
     Anything beyond this number is going to see a sharp decrease in
     performance. Of course, by default, the table resizes before
     this point is reached. However, this default behavior can be
     changed by changing the load_factor. */
    config.initial_capacity = 32;

    /*
     This value controls the rate of expansion of the internal array.
     It is usually set between 0 and 1. Although values above 1 are valid,
     there's little value in setting them.

     The load factor represents the percentage of fullness of the internal
     array (capacity set above) at which the internal array resize is triggered.
     In this case, since we've set the initial capacity to 32 and the load
     factor to 0.5, the resize will happen as soon as we add the 16th pair.
     The rule here is capacity * load_factor, which translates to 32 * 0.5
     in our case.

     So why set this value?

     By setting this value (and the capacity) we can control the performance
     of the table. If space is abundant and we don't care about it that much,
     we can set the load factor to a smaller value to keep the table sparsely
     populated and thus bring the performance close to O(1) by avoiding
     hash collisions (collision happen fairly often because of hash truncation).
     On the other hand, if the space is a problem we can set the load factor
     to a higher value and keep the table tightly packed at the expense of some
     speed.

     In short:
     capacity up   + load_factor down = fast table but space wasteful
     capacity down + load_factor up   = slow table but space conserving
    */
    config.load_factor      = 0.5;


    CC_HashTable *table;
    cc_hashtable_new_conf(&config, &table);
    cc_hashtable_destroy(table);
    return 0;
}
