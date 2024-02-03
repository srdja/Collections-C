#include "munit.h"
#include "cc_hashtable.h"
#include <stdlib.h>

struct table {
    CC_HashTableConf c;
    CC_HashTable* t;
};

int stat;

/* a dummy hash function used to force collisions */
static size_t collision_hash(const void* k, int l, uint32_t s)
{
    (void)k;
    (void)l;
    (void)s;

    return 1;
}

static size_t zero_hash(const void* k, int l, uint32_t s)
{
    (void)k;
    (void)l;
    (void)s;
    return 0;
}


static void* default_conf_table(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct table* t = malloc(sizeof(struct table));
    munit_assert_not_null(t);
    cc_hashtable_conf_init(&t->c);
    t->c.initial_capacity = 7;
    stat = cc_hashtable_new_conf(&t->c, &t->t);
    return (void*)t;
}

static void default_conf_table_teardown(void* fixture) 
{
    struct table* t = (struct table*)fixture;
    cc_hashtable_destroy(t->t);
}

static void* default_table(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    CC_HashTable* table;
    cc_hashtable_new(&table);
    return (void*)table;
}

static void default_table_teardown(void* fixture)
{
    CC_HashTable* table = (CC_HashTable*)fixture;
    cc_hashtable_destroy(table);
}

static void* default_collision_table(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct table* t = malloc(sizeof(struct table));
    munit_assert_not_null(t);
    cc_hashtable_conf_init(&t->c);
    t->c.hash = collision_hash;
    cc_hashtable_new_conf(&t->c, &t->t);
    return (void*)t;
}

static void default_collision_table_teardown(void* fixture)
{
    struct table* t = (struct table*)fixture;
    cc_hashtable_destroy(t->t);
}

static void* default_zero_hash_table(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct table* t = malloc(sizeof(struct table));
    munit_assert_not_null(t);
    cc_hashtable_conf_init(&t->c);
    t->c.hash = zero_hash;
    cc_hashtable_new_conf(&t->c, &t->t);
    return (void*)t;
}

static void default_zero_hash_table_teardown(void* fixture)
{
    struct table* t = (struct table*)fixture;
    cc_hashtable_destroy(t->t);
}

/***********************************************
                TESTS
***********************************************/

static MunitResult test_new(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct table* t = (struct table*)fixture;
    munit_assert_int(CC_OK, == , stat);
    munit_assert_size(0, == , cc_hashtable_size(t->t));

    /* power of 2 rounding */
    munit_assert_size(8, ==, cc_hashtable_capacity(t->t));

    return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable *table = (CC_HashTable*)fixture;

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, "5", c);

    munit_assert_size(3, == , cc_hashtable_size(table));

    char* r;
    cc_hashtable_get(table, "key", (void*)&r);
    munit_assert_ptr_equal(r, a);

    return MUNIT_OK;
}

static MunitResult test_collision_get(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct table* t = (struct table*)fixture;

    char* a = "value";
    char* c = "m31";

    cc_hashtable_add(t->t, "key", a);
    cc_hashtable_add(t->t, "randomstring", c);

    munit_assert_size(2, == , cc_hashtable_size(t->t));

    char* r;
    cc_hashtable_get(t->t, "randomstring", (void*)&r);
    munit_assert_ptr_equal(r, c);

    return MUNIT_OK;
}

static MunitResult test_collision_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct table* t = (struct table*)fixture;

    char* a = "value";
    char* c = "m31";

    cc_hashtable_add(t->t, "key", a);
    cc_hashtable_add(t->t, "randomstring", c);
    cc_hashtable_add(t->t, "5", c);

    char* rm;
    cc_hashtable_remove(t->t, "randomstring", (void*)&rm);
    munit_assert_size(2, == , cc_hashtable_size(t->t));
    void* g;

    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , 
    cc_hashtable_get(t->t, "randomstring", (void*)&g));

    return MUNIT_OK;
}

static MunitResult test_null_key_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct table* t = (struct table*)fixture;

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_hashtable_add(t->t, "key", a);
    cc_hashtable_add(t->t, NULL, c);
    cc_hashtable_add(t->t, "randomstring", b);
    cc_hashtable_add(t->t, "5", c);

    void* val = NULL;
    munit_assert_int(CC_OK, ==, cc_hashtable_get(t->t, NULL, &val));
    munit_assert_not_null(val);
    munit_assert_string_equal(val, c);

    return MUNIT_OK;
}

static MunitResult test_null_key_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct table* t = (struct table*)fixture;

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_hashtable_add(t->t, "key", a);
    cc_hashtable_add(t->t, "randomstring", b);
    cc_hashtable_add(t->t, NULL, c);
    cc_hashtable_add(t->t, "5", c);

    void* out = NULL;
    munit_assert_int(CC_OK, == , cc_hashtable_remove(t->t, "randomstring", &out));
    munit_assert_ptr_not_null(out);
    munit_assert_string_equal(out, b);

    out = NULL;
    munit_assert_int(CC_OK, == , cc_hashtable_remove(t->t, NULL, &out));
    munit_assert_ptr_not_null(out);
    munit_assert_string_equal(out, c);

    out = NULL;
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , cc_hashtable_get(t->t, "randomstring", &out));
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , cc_hashtable_get(t->t, NULL, &out));
    munit_assert_ptr_null(out);

    return MUNIT_OK;
}

static MunitResult test_null_key_get(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct table* t = (struct table*)fixture;

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_hashtable_add(t->t, "key", a);
    cc_hashtable_add(t->t, "randomstring", b);
    cc_hashtable_add(t->t, NULL, c);
    cc_hashtable_add(t->t, "5", c);

    void* out = NULL;
    munit_assert_int(CC_OK, ==, cc_hashtable_get(t->t, "randomstring", &out));
    munit_assert_ptr_not_null(out);
    munit_assert_string_equal(out, b);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable *table = (CC_HashTable*)fixture;

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_hashtable_add(table, "key", a);
    cc_hashtable_add(table, "randomstring", b);
    cc_hashtable_add(table, "5", c);

    char* rm = NULL;
    cc_hashtable_remove(table, "randomstring", (void*)&rm);
    munit_assert_ptr_not_null(rm);
    munit_assert_size(2, == , cc_hashtable_size(table));
    munit_assert_string_equal(rm, b);

    char* g = NULL;
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , cc_hashtable_get(table, "randomstring", (void*)&g));
    munit_assert_ptr_null(g);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable* table = (CC_HashTable*)fixture;

    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomkey", "randomvalue");

    munit_assert_size(2, == , cc_hashtable_size(table));

    cc_hashtable_remove_all(table);
    munit_assert_size(0, ==, cc_hashtable_size(table));

    char* g;
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, ==, cc_hashtable_get(table, "key", (void*)&g));

    return MUNIT_OK;
}

static MunitResult test_remove_get(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable* table = (CC_HashTable*)fixture;

    char* val = "567";

    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "123", val);

    char* ret = NULL;
    munit_assert_int(CC_OK, == , cc_hashtable_get(table, "123", (void*)&ret));
    munit_assert_ptr_not_null(ret);
    munit_assert_string_equal(ret, val);

    return MUNIT_OK;
}

static MunitResult test_size(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable* table = (CC_HashTable*)fixture;

    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomstring", "cookies");
    cc_hashtable_add(table, "5", "asdf");

    munit_assert_size(3, == , cc_hashtable_size(table));

    return MUNIT_OK;
}

static MunitResult test_capacity(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashTable* table;
    CC_HashTableConf conf;

    cc_hashtable_conf_init(&conf);

    conf.load_factor = 0.5f;
    conf.initial_capacity = 2;
    cc_hashtable_new_conf(&conf, &table);

    cc_hashtable_add(table, "a", NULL);
    munit_assert_size(2, == , cc_hashtable_capacity(table));

    cc_hashtable_add(table, "b", NULL);
    munit_assert_size(4, == , cc_hashtable_capacity(table));

    cc_hashtable_add(table, "c", NULL);
    cc_hashtable_add(table, "d", NULL);
    munit_assert_size(8, == , cc_hashtable_capacity(table));

    cc_hashtable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_contains_key(const MunitParameter params[], void* fixture)
{
    (void)params;
    CC_HashTable* table = (CC_HashTable*)fixture;

    cc_hashtable_add(table, "key", "value");
    cc_hashtable_add(table, "randomstring", "cookies");
    cc_hashtable_add(table, "5", "m31");

    munit_assert_int(1, ==, cc_hashtable_contains_key(table, "key"));

    cc_hashtable_remove(table, "key", NULL);
    munit_assert_int(0, == , cc_hashtable_contains_key(table, "key"));

    return MUNIT_OK;
}

static int cmp_k(const void* k1, const void* k2)
{
    char* key1 = (char*)k1;
    char* key2 = (char*)k2;
    int i;
    for (i = 0; i < (sizeof(int) * 7); i++) {
        if (*key1 != *key2)
            return 1;
        key1++;
        key2++;
    }
    return 0;
}

static MunitResult test_memory_chunk_key(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashTable* table;
    CC_HashTableConf conf;

    cc_hashtable_conf_init(&conf);

    conf.hash = GENERAL_HASH;
    conf.key_length = sizeof(int) * 7;
    conf.key_compare = cmp_k;

    cc_hashtable_new_conf(&conf, &table);

    int array1[] = { 1,2,3,4,5,6,7 };
    int array2[] = { 34,1,4,1111,456,234,0 };
    int array3[] = { 0,9,8,7,6,5,4 };

    cc_hashtable_add(table, array1, "one");
    cc_hashtable_add(table, array2, "two");
    cc_hashtable_add(table, array3, "three");

    char* a;
    char* b;

    cc_hashtable_get(table, array1, (void*)&a);
    cc_hashtable_get(table, array3, (void*)&b);

    munit_assert_string_equal("one", a);
    munit_assert_string_equal("three", b);

    cc_hashtable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;

    CC_HashTable* table = (CC_HashTable*)fixture;

    cc_hashtable_add(table, "one", "1");
    cc_hashtable_add(table, "two", "2");
    cc_hashtable_add(table, "three", "3");
    cc_hashtable_add(table, "four", "4");
    cc_hashtable_add(table, "five", "5");

    int one = 0;
    int two = 0;
    int three = 0;
    int four = 0;
    int five = 0;

    CC_HashTableIter iter;
    cc_hashtable_iter_init(&iter, table);

    TableEntry* entry;
    while (cc_hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const* key = entry->key;

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

    munit_assert_int(1, ==, one);
    munit_assert_int(1, == , two);
    munit_assert_int(1, == , three);
    munit_assert_int(1, == , four);
    munit_assert_int(1, == , five);

    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;

    CC_HashTable* table = (CC_HashTable*)fixture;

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";

    cc_hashtable_add(table, a, "a");
    cc_hashtable_add(table, b, "a");
    cc_hashtable_add(table, c, "a");

    CC_HashTableIter iter;
    cc_hashtable_iter_init(&iter, table);

    TableEntry* entry;
    while (cc_hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
        char const* key = entry->key;

        if (!strcmp(key, "bar"))
            cc_hashtable_iter_remove(&iter, NULL);
    }

    munit_assert_size(2, == , cc_hashtable_size(table));
    munit_assert_false(cc_hashtable_contains_key(table, "bar"));

    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    {(char*)"/hashtable/test_new", test_new, default_conf_table, default_conf_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_add", test_add, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_collision_get", test_collision_get, default_collision_table, default_collision_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_collision_remove", test_collision_remove, default_collision_table, default_collision_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_null_key_add", test_null_key_add, default_zero_hash_table, default_zero_hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_null_key_remove", test_null_key_remove, default_zero_hash_table, default_zero_hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_null_key_get", test_null_key_get, default_zero_hash_table, default_zero_hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_remove", test_remove, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_remove_all", test_remove_all, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_remove_get", test_remove_get, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_size", test_size, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_capacity", test_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_contains_key", test_contains_key, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_iter_next", test_iter_next, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_iter_remove", test_iter_remove, default_table, default_table_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/hashtable/test_memory_chunk_key", test_memory_chunk_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}