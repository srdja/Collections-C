#include <time.h>
#include <string.h>

#include "hashtable.h"
#include "CppUTest/TestHarness_c.h"

static HashTableConf conf;
static HashTable *table;
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

TEST_GROUP_C_SETUP(HashTableTestsConf)
{
    hashtable_conf_init(&conf);
    conf.initial_capacity = 7;
    stat = hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(HashTableTestsConf)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTestsConf, HashTableNew)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    CHECK_EQUAL_C_INT(0, hashtable_size(table));
    /* power of 2 rounding */
    CHECK_EQUAL_C_INT(8, hashtable_capacity(table));
};

TEST_GROUP_C_SETUP(HashTableTests)
{
    hashtable_new(&table);
};

TEST_GROUP_C_TEARDOWN(HashTableTests)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTests, HashTableAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, "5", c);

    CHECK_EQUAL_C_INT(3, hashtable_size(table));

    char *r;
    hashtable_get(table, "key", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, a);
};

TEST_GROUP_C_SETUP(HashTableTestsCollision)
{
    hashtable_conf_init(&conf);
    conf.hash = collision_hash;
    hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(HashTableTestsCollision)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTestsCollision, HashTableCollisionGet)
{
    char *a = "value";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", c);

    CHECK_EQUAL_C_INT(2, hashtable_size(table));

    char *r;
    hashtable_get(table, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, c);
};

TEST_C(HashTableTestsCollision, HashTableCollisionRemove)
{
    char *a = "value";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", c);
    hashtable_add(table, "5", c);

    char *rm;
    hashtable_remove(table, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, hashtable_size(table));
    void *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, hashtable_get(table, "randomstring", (void*) &g));
};

TEST_GROUP_C_SETUP(HashTableTestsZeroHash)
{
    hashtable_conf_init(&conf);
    conf.hash = zero_hash;
    hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(HashTableTestsZeroHash)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTestsZeroHash, HashTableAddStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, NULL, c);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, "5", c);
};


TEST_C(HashTableTestsZeroHash, HashTableRemoveStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, NULL, c);
    hashtable_add(table, "5", c);

    hashtable_remove(table, "randomstring", NULL);
};


TEST_C(HashTableTestsZeroHash, HashTableGetStringWithNullKey)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, NULL, c);
    hashtable_add(table, "5", c);

    void *out;
    hashtable_get(table, "randomstring", &out);
};


TEST_C(HashTableTests, HashTableRemove)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, "5", c);

    char *rm;
    hashtable_remove(table, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, hashtable_size(table));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, hashtable_get(table, "randomstring", (void*) &g));
};


TEST_C(HashTableTests, HashTableRemoveAll)
{
    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomkey", "randomvalue");

    CHECK_EQUAL_C_INT(2, hashtable_size(table));

    hashtable_remove_all(table);
    CHECK_EQUAL_C_INT(0, hashtable_size(table));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, hashtable_get(table, "key", (void*) &g));
};


TEST_C(HashTableTests, HashTableGet)
{
    char *val = "567";

    hashtable_add(table, "key", "value");
    hashtable_add(table, "123", val);

    char *ret;
    hashtable_get(table, "123", (void*) &ret);

    CHECK_EQUAL_C_STRING(val, ret);
};


TEST_C(HashTableTests, HashTableSize)
{
    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomstring", "cookies");
    hashtable_add(table, "5", "asdf");

    CHECK_EQUAL_C_INT(3, hashtable_size(table));
};


TEST_GROUP_C_SETUP(HashTableTestsCapacity)
{
    hashtable_conf_init(&conf);

    conf.load_factor      = 0.5f;
    conf.initial_capacity = 2;
    hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(HashTableTestsCapacity)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTestsCapacity, HashTableCapacity)
{
    hashtable_add(table, "a", NULL);
    CHECK_EQUAL_C_INT(2, hashtable_capacity(table));

    hashtable_add(table, "b", NULL);
    CHECK_EQUAL_C_INT(4, hashtable_capacity(table));

    hashtable_add(table, "c", NULL);
    hashtable_add(table, "d", NULL);
    CHECK_EQUAL_C_INT(8, hashtable_capacity(table));
};


TEST_C(HashTableTests, HashTableContainsKey)
{
    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomstring", "cookies");
    hashtable_add(table, "5", "m31");

    CHECK_EQUAL_C_INT(1, hashtable_contains_key(table, "key"));

    hashtable_remove(table, "key", NULL);
    CHECK_EQUAL_C_INT(0, hashtable_contains_key(table, "key"));
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

TEST_GROUP_C_SETUP(HashTableTestsCompare)
{
    hashtable_conf_init(&conf);

    conf.hash        = GENERAL_HASH;
    conf.key_length  = sizeof(int) * 7;
    conf.key_compare = cmp_k;

    hashtable_new_conf(&conf, &table);
};

TEST_GROUP_C_TEARDOWN(HashTableTestsCompare)
{
    hashtable_destroy(table);
};

TEST_C(HashTableTestsCompare, HashTableTestsMemoryChunksAsKeys)
{
    int array1[] = {1,2,3,4,5,6,7};
    int array2[] = {34,1,4,1111,456,234,0};
    int array3[] = {0,9,8,7,6,5,4};

    hashtable_add(table, array1, "one");
    hashtable_add(table, array2, "two");
    hashtable_add(table, array3, "three");

    char *a;
    char *b;

    hashtable_get(table, array1, (void*) &a);
    hashtable_get(table, array3, (void*) &b);

    CHECK_EQUAL_C_STRING("one", a);
    CHECK_EQUAL_C_STRING("three", b);
};

TEST_C(HashTableTests, HashTableIterNext)
{
    hashtable_add(table, "one", "1");
    hashtable_add(table, "two", "2");
    hashtable_add(table, "three", "3");
    hashtable_add(table, "four", "4");
    hashtable_add(table, "five", "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;
    int five  = 0;

    HashTableIter iter;
    hashtable_iter_init(&iter, table);

    TableEntry *entry;
    while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
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

TEST_C(HashTableTests, HashTableIterRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashtable_add(table, a, "a");
    hashtable_add(table, b, "a");
    hashtable_add(table, c, "a");

    HashTableIter iter;
    hashtable_iter_init(&iter, table);

    TableEntry *entry;
    while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const *key = entry->key;

        if (!strcmp(key, "bar"))
            hashtable_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(2, hashtable_size(table));
    CHECK_C(!hashtable_contains_key(table, "bar"));
};
