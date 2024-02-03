#include "munit.h"
#include "cc_tsttable.h"
#include <stdlib.h>

static MunitResult test_new(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTableConf conf;
    CC_TSTTable* table;

    cc_tsttable_conf_init(&conf);
    munit_assert_int(CC_OK, ==, cc_tsttable_new_conf(&conf, &table));
    cc_tsttable_destroy(table);

    munit_assert_int(CC_OK, == , cc_tsttable_new(&table));
    cc_tsttable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);

    munit_assert_size(3, == , cc_tsttable_size(tst));

    char* r;
    cc_tsttable_get(tst, "key", (void*)&r);
    munit_assert_ptr_equal(r, a);

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_add_subkeys(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    int stat;

    char pairs[16][2][20] = {
    {"one", "111"},
    {"two", "222"},
    {"three", "333"},
    {"onee", "444"},
    {"onez", "555"},
    {"onec", "666"},
    {"oneea", "777"},
    {"oneec", "888"},
    {"oneez", "999"},
    {"onezz", "aaa"},
    {"oneza", "bbb"},
    {"onezk", "ccc"},
    {"onecc", "ddd"},
    {"oneca", "eee"},
    {"onecz", "fff"},
    {"oneck", "000"},
    };
    int n = 16;

    for (int i = 0; i < n; i++) {
        stat = cc_tsttable_add(tst, pairs[i][0], pairs[i][1]);
        munit_assert_int(stat, == , CC_OK);
    }

    munit_assert_size(n, == , cc_tsttable_size(tst));

    for (int i = 0; i < n; i++) {
        char* value;
        stat = cc_tsttable_get(tst, pairs[i][0], (void**)&value);
        munit_assert_int(stat, ==, CC_OK);
        munit_assert_string_equal(value, pairs[i][1]);
    }

    cc_tsttable_destroy(tst);
    return MUNIT_OK;
}

static MunitResult test_replacing_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";
    char* d = "okay";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);
    cc_tsttable_add(tst, "randomstring", d);

    munit_assert_size(3, == , cc_tsttable_size(tst));

    char* r;
    cc_tsttable_get(tst, "randomstring", (void*)&r);
    munit_assert_ptr_equal(r, d);

    cc_tsttable_destroy(tst);
    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    cc_tsttable_add(tst, "one", "111");
    munit_assert_size(1, == , cc_tsttable_size(tst));

    cc_tsttable_remove(tst, "one", NULL);
    munit_assert_size(0, == , cc_tsttable_size(tst));

    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");
    munit_assert_size(3, == , cc_tsttable_size(tst));

    cc_tsttable_remove(tst, "five", NULL);
    cc_tsttable_remove(tst, "three", NULL);
    cc_tsttable_remove(tst, "random", NULL);

    munit_assert_size(0, == , cc_tsttable_size(tst));

    char* a = "value";
    char* b = "cookies";
    char* c = "m31";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);

    char* rm;
    cc_tsttable_remove(tst, "randomstring", (void*)&rm);
    munit_assert_size(2, == , cc_tsttable_size(tst));

    char* g;
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , cc_tsttable_get(tst, "randomstring", (void*)&g));

    cc_tsttable_add(tst, "randomstring", b);

    char* r;
    cc_tsttable_get(tst, "randomstring", (void*)&r);
    munit_assert_ptr_equal(r, b);

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomkey", "randomvalue");

    munit_assert_size(2, == , cc_tsttable_size(tst));

    cc_tsttable_remove_all(tst);
    munit_assert_size(0, == , cc_tsttable_size(tst));

    char* g;

    munit_assert_int(CC_ERR_KEY_NOT_FOUND, == , cc_tsttable_get(tst, "key", (void*)&g));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_get(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    char* val = "567";

    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "aask", val);

    munit_assert_size(2, == , cc_tsttable_size(tst));

    void* ret;
    cc_tsttable_get(tst, "aask", &ret);
    munit_assert_string_equal(val, ret);

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_size(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomstring", "cookies");
    cc_tsttable_add(tst, "5", "asdf");

    munit_assert_size(3, == , cc_tsttable_size(tst));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}


static MunitResult test_contains_key(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);

    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomstring", "cookies");
    cc_tsttable_add(tst, "5", "m31");

    munit_assert_int(1, == , cc_tsttable_contains_key(tst, "key"));

    cc_tsttable_remove(tst, "key", NULL);
    munit_assert_size(0, == , cc_tsttable_contains_key(tst, "key"));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_iter_next_empty(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    CC_TSTTableIter iter;

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry* data;
    munit_assert_int(CC_ITER_END, == , cc_tsttable_iter_next(&iter, &data));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    CC_TSTTableIter iter;

    cc_tsttable_add(tst, "one", "111");

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry* data;
    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_string_equal(data->key, "one");
    munit_assert_string_equal(data->value, "111");

    cc_tsttable_add(tst, "two", "222");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");

    cc_tsttable_iter_init(&iter, tst);
    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_string_equal(data->key, "one");
    munit_assert_string_equal(data->value, "111");

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_string_equal(data->key, "random");
    munit_assert_string_equal(data->value, "439834");

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_string_equal(data->key, "three");
    munit_assert_string_equal(data->value, "333");

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_string_equal(data->key, "two");
    munit_assert_string_equal(data->value, "222");

    munit_assert_int(CC_ITER_END, ==, cc_tsttable_iter_next(&iter, &data));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}


static MunitResult test_iter_remove_empty(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    CC_TSTTableIter iter;

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry* data;
    munit_assert_int(CC_ITER_END, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, ==, cc_tsttable_iter_remove(&iter, (void**)&data));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    CC_TSTTableIter iter;

    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry* data;
    void* value;

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_size(3, == , cc_tsttable_size(tst));
    munit_assert_int(CC_OK, ==, cc_tsttable_iter_remove(&iter, &value));
    munit_assert_size(2, == , cc_tsttable_size(tst));
    munit_assert_string_equal(value, "555");

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_size(2, == , cc_tsttable_size(tst));
    munit_assert_int(CC_OK, ==, cc_tsttable_iter_remove(&iter, &value));
    munit_assert_size(1, == , cc_tsttable_size(tst));
    munit_assert_string_equal(value, "439834");

    munit_assert_int(CC_OK, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_size(1, == , cc_tsttable_size(tst));
    munit_assert_int(CC_OK, ==, cc_tsttable_iter_remove(&iter, &value));
    munit_assert_size(0, == , cc_tsttable_size(tst));
    munit_assert_string_equal(value, "333");

    munit_assert_int(CC_ITER_END, ==, cc_tsttable_iter_next(&iter, &data));
    munit_assert_size(0, == , cc_tsttable_size(tst));
    munit_assert_int(CC_ERR_KEY_NOT_FOUND, ==, cc_tsttable_iter_remove(&iter, &value));
    munit_assert_size(0, == , cc_tsttable_size(tst));

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}

static MunitResult test_loop(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TSTTable* tst;
    cc_tsttable_new(&tst);
    CC_TSTTableIter iter;

    CC_TSTTableEntry* data;

    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");
    munit_assert_size(3, == , cc_tsttable_size(tst));

    char keys[3][20] = { "five", "random", "three" };
    char values[3][20] = { "555", "439834", "333" };

    int i = 0;
    cc_tsttable_iter_init(&iter, tst);
    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END) {
        munit_assert_string_equal(data->key, keys[i++]);
    }
    munit_assert_int(i, == , 3);

    i = 0;
    cc_tsttable_iter_init(&iter, tst);
    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END) {
        munit_assert_string_equal(data->value, values[i++]);
    }
    munit_assert_int(i, == , 3);

    i = 0;
    char* key, * value;
    CC_TSTTABLE_FOREACH(tst, key, value, {
            munit_assert_string_equal(key, keys[i]);
    munit_assert_string_equal(value, values[i]);
            i++;
        });
    munit_assert_int(i, == , 3);

    cc_tsttable_destroy(tst);

    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*)"/tst_table/test_new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_add_subkeys", test_add_subkeys, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_replacing_add", test_replacing_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_get", test_get, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_size", test_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_contains_key", test_contains_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_iter_next_empty", test_iter_next_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_iter_next", test_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_iter_remove_empty", test_iter_remove_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/tst_table/test_loop", test_loop, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}