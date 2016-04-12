#include <time.h>

#include "../src/hashtable.h"
#include "test.h"
#include <string.h>

static HashTableConf conf;

void test_hashtable_new();
void test_hashtable_add();
void test_hashtable_remove();
void test_hashtable_get();
void test_hashtable_size();
void test_hashtable_remove_all();
void test_hashtable_capacity();
void test_hashtable_contains_key();
void test_hashtable_memory_chunks_as_keys();
void test_hashtable_iter_next();
void test_hashtable_iter_remove();

/* a dummy hash function used to force collisions */
static size_t collision_hash(const void *k, int l, uint32_t s)
{
    return 1;
}


int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    hashtable_conf_init(&conf);

    test_hashtable_new();
    test_hashtable_size();
    test_hashtable_remove_all();
    test_hashtable_add();
    test_hashtable_remove();
    test_hashtable_get();
    test_hashtable_capacity();
    test_hashtable_contains_key();
    test_hashtable_memory_chunks_as_keys();
    test_hashtable_iter_next();
    test_hashtable_iter_remove();

    return cc_get_status();
}


void test_hashtable_new()
{
    hashtable_conf_init(&conf);
    conf.initial_capacity = 7;

    HashTable *table;
    hashtable_new_conf(&conf, &table);

    cc_assert(hashtable_size(table) == 0,
              cc_msg("hashtable_new: Initial size not 0"));

    size_t capacity = hashtable_capacity(table);

    /* power of 2 rounding */
    cc_assert(capacity == 8,
              cc_msg("hashtable_new: Expected capactity was 8, but got %d",
                     capacity));

    hashtable_destroy(table);
}

void test_hashtable_add()
{
    HashTable *table;
    hashtable_new(&table);

    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, "5", c);

    size_t size = hashtable_size(table);

    cc_assert(size == 3,
              cc_msg("hashtable_add: Expected size was 3, but got %d", size));

    char *r;
    hashtable_get(table, "key", (void*) &r);

    cc_assert(r == a,
              cc_msg("hashtable_add: Wrong value retrieved"));

    hashtable_destroy(table);

    /*
     * test collision handling
     */
    hashtable_conf_init(&conf);
    conf.hash = collision_hash;

    hashtable_new_conf(&conf, &table);

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", c);

    size = hashtable_size(table);

    cc_assert(size == 2,
              cc_msg("hashtable_add: Expected size was 2, but got %d", size));

    hashtable_get(table, "randomstring", (void*) &r);

    cc_assert(r == c,
              cc_msg("hashtable_add: Expected 'm31' to be retrieved, but got %s", c));

    hashtable_destroy(table);
}


void test_hashtable_remove()
{
    HashTable *table;
    hashtable_new(&table);

    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", b);
    hashtable_add(table, "5", c);

    char *rm;
    hashtable_remove(table, "randomstring", (void*) &rm);
    size_t size = hashtable_size(table);

    cc_assert(size == 2,
              cc_msg("hashtable_remove: Expected size was 2, but got %d",
                     size));

    char *g;
    int stat = hashtable_get(table, "randomstring", (void*) &g);

    cc_assert(stat == CC_ERR_KEY_NOT_FOUND,
              cc_msg("hashtable_remove: 'randomstring' key"
                     " still mapped after removal"));

    hashtable_destroy(table);

    /*
     * test collision handling
     */
    hashtable_conf_init(&conf);
    conf.hash = collision_hash;

    hashtable_new_conf(&conf, &table);

    hashtable_add(table, "key", a);
    hashtable_add(table, "randomstring", c);
    hashtable_add(table, "5", c);

    hashtable_remove(table, "randomstring", (void*) &rm);
    size = hashtable_size(table);

    cc_assert(size == 2,
              cc_msg("hashtable_remove: Expected size was 2, but got %d",
                     size));

    stat = hashtable_get(table, "randomstring", (void*) &g);

    cc_assert(stat == CC_ERR_KEY_NOT_FOUND,
              cc_msg("hashtable_remove: Expected 'm31' to"
                     " be retrieved, but got %s", c));

    hashtable_destroy(table);
}


void test_hashtable_remove_all()
{
    HashTable *table;
    hashtable_new(&table);

    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomkey", "randomvalue");

    hashtable_remove_all(table);
    size_t size = hashtable_size(table);

    cc_assert(size == 0,
              cc_msg("hashtable_remove_all: Table size %d,"
                     " when it should be 0",
                     size));

    char *g;
    int stat = hashtable_get(table, "key", (void*) &g);

    cc_assert(stat == CC_ERR_KEY_NOT_FOUND,
              cc_msg("hashtable_refmove_all: Value still"
                     " accecible after removal"));

    hashtable_destroy(table);
}


void test_hashtable_get()
{
    HashTable *table;
    hashtable_new(&table);

    char *val = "567";

    hashtable_add(table, "key", "value");
    hashtable_add(table, "123", val);

    char *ret;
    hashtable_get(table, "123", (void*) &ret);

    cc_assert(ret == val,
              cc_msg("hashtable_get: Incorrect value returned."
                     " Expected %s, but got %s", val, ret));

    hashtable_destroy(table);
}


void test_hashtable_size()
{
    HashTable *table;
    hashtable_new(&table);

    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomstring", "cookies");
    hashtable_add(table, "5", "asdf");

    size_t size = hashtable_size(table);

    cc_assert(size == 3,
              cc_msg("hashtable_size: Expected size was 3, but got %d",
                     size));

    hashtable_destroy(table);
}


void test_hashtable_capacity()
{
    hashtable_conf_init(&conf);

    conf.load_factor      = 0.5f;
    conf.initial_capacity = 2;

    HashTable *t;
    hashtable_new_conf(&conf, &t);

    hashtable_add(t, "a", NULL);

    cc_assert(hashtable_capacity(t) == 2,
              cc_msg("hashtable_capacity: Expected capacity was 4, but got %d",
                     hashtable_capacity(t)));

    hashtable_add(t, "b", NULL);

    cc_assert(hashtable_capacity(t) == 4,
              cc_msg("hashtable_capacity: Expected capacity was 8, but got %d",
                     hashtable_capacity(t)));

    hashtable_add(t, "c", NULL);
    hashtable_add(t, "d", NULL);

    cc_assert(hashtable_capacity(t) == 8,
              cc_msg("hashtable_capacity: Expected capacity was 16, but got %d",
                     hashtable_capacity(t)));

    hashtable_destroy(t);
}


void test_hashtable_contains_key()
{
    HashTable *table;
    hashtable_new(&table);

    hashtable_add(table, "key", "value");
    hashtable_add(table, "randomstring", "cookies");
    hashtable_add(table, "5", "m31");

    cc_assert(hashtable_contains_key(table, "key"),
              cc_msg("hashtable_contains_key: "
                     "Key expected to be present, but isn't"));

    hashtable_remove(table, "key", NULL);

    cc_assert(!hashtable_contains_key(table, "key"),
              cc_msg("hashtable_contains_key:"
                     " Key not expected to exist"));

    hashtable_destroy(table);
}

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

void test_hashtable_memory_chunks_as_keys()
{
    int array1[] = {1,2,3,4,5,6,7};
    int array2[] = {34,1,4,1111,456,234,0};
    int array3[] = {0,9,8,7,6,5,4};

    hashtable_conf_init(&conf);

    conf.hash        = GENERAL_HASH;
    conf.key_length  = sizeof(int) * 7;
    conf.key_compare = cmp_k;

    HashTable *t;
    hashtable_new_conf(&conf, &t);

    hashtable_add(t, array1, "one");
    hashtable_add(t, array2, "two");
    hashtable_add(t, array3, "three");

    char *a;
    char *b;

    hashtable_get(t, array1, (void*) &a);
    hashtable_get(t, array3, (void*) &b);

    cc_assert(strcmp(a, "one") == 0,
              cc_msg("hashtable_memory_chunks_as_keys: "
                     "Expected value 'one', but got %s",
                     a));

    cc_assert(strcmp(b, "three") == 0,
              cc_msg("hashtable_key_test:"
                     " Expected value 'three', but got %s",
                     b));

    hashtable_destroy(t);
}

void test_hashtable_iter_next()
{
    HashTable *t;
    hashtable_new(&t);

    hashtable_add(t, "one", "1");
    hashtable_add(t, "two", "2");
    hashtable_add(t, "three", "3");
    hashtable_add(t, "four", "4");
    hashtable_add(t, "five", "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;
    int five  = 0;

    HashTableIter iter;
    hashtable_iter_init(&iter, t);

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

    bool asrt = (one   == 1) && (two  == 1)  &&
                (three == 1) && (four == 1) &&
                (five  == 1);

    cc_assert(asrt,
              cc_msg("hashtable_iter_next: Unexpected number"
                     " of entries returned"));

    hashtable_destroy(t);
}


void test_hashtable_iter_remove()
{
    HashTable *t;
    hashtable_new(&t);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashtable_add(t, a, "a");
    hashtable_add(t, b, "a");
    hashtable_add(t, c, "a");

    HashTableIter iter;
    hashtable_iter_init(&iter, t);

    TableEntry *entry;
    while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const *key = entry->key;

        if (!strcmp(key, "bar"))
            hashtable_iter_remove(&iter, NULL);
    }

    cc_assert(hashtable_size(t) == 2,
              cc_msg("hashtable_iter_remove: Expected size 2 but got %d ",
                     hashtable_size(t)));

    cc_assert(!hashtable_contains_key(t, "bar"),
              cc_msg("hastable_iter_remove: Element (%s) still pressent "
                     "after removal", "bar"));

    hashtable_destroy(t);
}
