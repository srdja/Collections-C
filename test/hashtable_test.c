#include <time.h>
#include <string.h>

#include "cc_hashtable.h"
#include "CppUTest/TestHarness_c.h"

static CC_HashTableConf conf;
static CC_HashTable *table;
int stat;

/* a dummy hash function used to force collisions */
static size_t collision_hash(const void *k, int l, uint32_t s)
{
    return 1;
}

static size_t zero_hash(const void *k, int l, uint32_t s)
{
    return 0;
}

TEST_GROUP_C_SETUP(CC_HashTableTestsConf)
{
    cc_hashtable_conf_init(&conf);
    conf.initial_capacity = 7;
    stat = cc_hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTestsConf)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTestsConf, CC_HashTableNew)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    CHECK_EQUAL_C_INT(0, cc_hashtable_size(table));
    /* power of 2 rounding */
    CHECK_EQUAL_C_INT(8, cc_hashtable_capacity(table));
};

TEST_GROUP_C_SETUP(CC_HashTableTests)
{
    cc_hashtable_new(&table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTests)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTests, CC_HashTableAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, "5", c);

    CHECK_EQUAL_C_INT(3, cc_hashtable_size(table));

    char *r;
    cc_hashtable_get(table, "key", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, a);
};

TEST_GROUP_C_SETUP(CC_HashTableTestsCollision)
{
    cc_hashtable_conf_init(&conf);
    conf.hash = collision_hash;
    cc_hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTestsCollision)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTestsCollision, CC_HashTableCollisionGet)
{
    char *a = "value";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", c);

    CHECK_EQUAL_C_INT(2, cc_hashtable_size(table));

    char *r;
    cc_hashtable_get(table, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, c);
};

TEST_C(CC_HashTableTestsCollision, CC_HashTableCollisionRemove)
{
    char *a = "value";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", c);
    cc_hashtable_add(table, "5", c);

    char *rm;
    cc_hashtable_remove(table, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, cc_hashtable_size(table));
    void *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, cc_hashtable_get(table, "randomstring", (void*) &g));
};

TEST_GROUP_C_SETUP(CC_HashTableTestsZeroHash)
{
    cc_hashtable_conf_init(&conf);
    conf.hash = zero_hash;
    cc_hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTestsZeroHash)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTestsZeroHash, CC_HashTableAddStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, NULL, c);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, "5", c);
};


TEST_C(CC_HashTableTestsZeroHash, CC_HashTableRemoveStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, NULL, c);
    cc_hashtable_add(table, "5", c);

    cc_hashtable_remove(table, "randomstring", NULL);
};


TEST_C(CC_HashTableTestsZeroHash, CC_HashTableGetStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, NULL, c);
    cc_hashtable_add(table, "5", c);

    void *out;
    cc_hashtable_get(table, "randomstring", &out);
};


TEST_C(CC_HashTableTests, CC_HashTableRemove)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, "5", c);

    char *rm;
    cc_hashtable_remove(table, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, cc_hashtable_size(table));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, cc_hashtable_get(table, "randomstring", (void*) &g));
};


TEST_C(CC_HashTableTests, CC_HashTableRemoveAll)
{
    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomkey", "randomvalue");

    CHECK_EQUAL_C_INT(2, cc_hashtable_size(table));

    cc_hashtable_remove_all(table);
    CHECK_EQUAL_C_INT(0, cc_hashtable_size(table));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, cc_hashtable_get(table, "key", (void*) &g));
};


TEST_C(CC_HashTableTests, CC_HashTableGet)
{
    char *val = "567";

    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "123", val);

    char *ret;
    cc_hashtable_get(table, "123", (void*) &ret);

    CHECK_EQUAL_C_STRING(val, ret);
};


TEST_C(CC_HashTableTests, CC_HashTableSize)
{
    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomstring", "cookies");
    cc_hashtable_add(table, "5", "asdf");

    CHECK_EQUAL_C_INT(3, cc_hashtable_size(table));
};


TEST_GROUP_C_SETUP(CC_HashTableTestsCapacity)
{
    cc_hashtable_conf_init(&conf);

    conf.load_factor      = 0.5f;
    conf.initial_capacity = 2;
    cc_hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTestsCapacity)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTestsCapacity, CC_HashTableCapacity)
{
    cc_hashtable_add(table, "a", NULL);
    CHECK_EQUAL_C_INT(2, cc_hashtable_capacity(table));

    cc_hashtable_add(table, "b", NULL);
    CHECK_EQUAL_C_INT(4, cc_hashtable_capacity(table));

    cc_hashtable_add(table, "c", NULL);
    cc_hashtable_add(table, "d", NULL);
    CHECK_EQUAL_C_INT(8, cc_hashtable_capacity(table));
};


TEST_C(CC_HashTableTests, CC_HashTableContainsKey)
{
    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomstring", "cookies");
    cc_hashtable_add(table, "5", "m31");

    CHECK_EQUAL_C_INT(1, cc_hashtable_contains_key(table, "key"));

    cc_hashtable_remove(table, "key", NULL);
    CHECK_EQUAL_C_INT(0, cc_hashtable_contains_key(table, "key"));
};

int cmp_k(const void *k1, const void *k2)
{
    char *key1 = (char*) k1;
    char *key2 = (char*) k2;
    int i;
    for (i = 0; i < (sizeof(int) * 7); i++) {
        if (*key1 != *key2)
            return 1;
        key1++;
        key2++;
    }
    return 0;
}

TEST_GROUP_C_SETUP(CC_HashTableTestsCompare)
{
    cc_hashtable_conf_init(&conf);

    conf.hash        = GENERAL_HASH;
    conf.key_length  = sizeof(int) * 7;
    conf.key_compare = cmp_k;

    cc_hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(CC_HashTableTestsCompare)
{
    cc_hashtable_destroy(table);
};

TEST_C(CC_HashTableTestsCompare, CC_HashTableTestsMemoryChunksAsKeys)
{
    int array1[] = {1,2,3,4,5,6,7};
    int array2[] = {34,1,4,1111,456,234,0};
    int array3[] = {0,9,8,7,6,5,4};

    cc_hashtable_add(table, array1, "one");
    cc_hashtable_add(table, array2, "two");
    cc_hashtable_add(table, array3, "three");

    char *a;
    char *b;

    cc_hashtable_get(table, array1, (void*) &a);
    cc_hashtable_get(table, array3, (void*) &b);

    CHECK_EQUAL_C_STRING("one", a);
    CHECK_EQUAL_C_STRING("three", b);
};

TEST_C(CC_HashTableTests, CC_HashTableIterNext)
{
    cc_hashtable_add(table, "one", "1");
    cc_hashtable_add(table, "two", "2");
    cc_hashtable_add(table, "three", "3");
    cc_hashtable_add(table, "four", "4");
    cc_hashtable_add(table, "five", "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;
    int five  = 0;

    CC_HashTableIter iter;
    cc_hashtable_iter_init(&iter, table);

    TableEntry *entry;
    while (cc_hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const *key = entry->key;

        if (!strcmp(key, "one"))
            one++;

        if (!strcmp(key, "two"))
            two++;

        if (!strcmp(key, "three"))
            three++;

        if (!strcmp(key, "four"))
            four++;

        if (!strcmp(key, "five"))
            five++;
    }

    CHECK_EQUAL_C_INT(1, one);
    CHECK_EQUAL_C_INT(1, two);
    CHECK_EQUAL_C_INT(1, three);
    CHECK_EQUAL_C_INT(1, four);
    CHECK_EQUAL_C_INT(1, five);
};

TEST_C(CC_HashTableTests, CC_HashTableIterRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    cc_hashtable_add(table, a, "a");
    cc_hashtable_add(table, b, "a");
    cc_hashtable_add(table, c, "a");

    CC_HashTableIter iter;
    cc_hashtable_iter_init(&iter, table);

    TableEntry *entry;
    while (cc_hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const *key = entry->key;

        if (!strcmp(key, "bar"))
            cc_hashtable_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(2, cc_hashtable_size(table));
    CHECK_C(!cc_hashtable_contains_key(table, "bar"));
};
