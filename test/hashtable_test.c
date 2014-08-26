/*
 * hastable_test.c
 *
 *  Created on: Aug 4, 2014
 *      Author: srdja
 */

#include "../src/hashtable.h"
#include "hashtable_test.h"

void test_hashtable_init()
{
    test_hashtable_new();
}

bool cmp_double(void *a, void *b)
{
    return *(double*) a == *(double*) b;
}

void test_hashtable_new()
{
    HashTableProperties *properties = hashtable_properties_new();

    properties->hash = GENERAL_HASH;
    properties->key_compare = cmp_double;
    properties->key_length = sizeof(double);

    HashTable *table = hashtable_new(properties);

    double a = 1.5;
    double b = 1.5345;
    double c = 3.2;
    double d = 61.2222;
    double f = 0.01;
    double e = 0.912384;
    double g = 7.7;

   // hashtable_print(table);

    hashtable_put(table, &a, "String keys");
    hashtable_put(table, &b, "lemons");
    hashtable_put(table, &c, "moon");
    hashtable_put(table, &d, "value");
    hashtable_put(table, &e, "that");
    hashtable_put(table, &f, "overthere");
    hashtable_put(table, &g, "not");

    printf("\n\n capacity = %d\n", hashtable_capacity(table));
    hashtable_print(table);

    hashtable_remove(table, &b);
    hashtable_remove(table, &f);
    hashtable_remove(table, &d);

    printf("\n\n capacity = %d\n", hashtable_capacity(table));
    hashtable_print(table);

}
