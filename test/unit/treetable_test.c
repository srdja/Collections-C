#include "munit.h"
#include "cc_treetable.h"
#include <stdlib.h>


int cmp(const void* k1, const void* k2)
{
    int a = *((int*)k1);
    int b = *((int*)k2);

    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}


static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");
    cc_treetable_add(table, &d, "d");
    cc_treetable_add(table, &e, "e");
    cc_treetable_add(table, &f, "f");
    cc_treetable_add(table, &g, "g");
    cc_treetable_add(table, &h, "h");

    munit_assert_size(1, ==, cc_treetable_contains_key(table, &a));
    munit_assert_size(1, == , cc_treetable_contains_key(table, &e));

    cc_treetable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");
    cc_treetable_add(table, &d, "d");
    cc_treetable_add(table, &e, "e");
    cc_treetable_add(table, &f, "f");
    cc_treetable_add(table, &g, "g");
    cc_treetable_add(table, &h, "h");

    cc_treetable_remove(table, &f, NULL);
    munit_assert_size(0, == , cc_treetable_contains_key(table, &f));

    cc_treetable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");
    cc_treetable_add(table, &d, "d");

    cc_treetable_remove_all(table);

    munit_assert_size(0, == , cc_treetable_size(table));
    munit_assert_size(0, == , cc_treetable_contains_key(table, &a));
    munit_assert_size(0, == , cc_treetable_contains_key(table, &b));
    munit_assert_size(0, == , cc_treetable_contains_key(table, &c));
    munit_assert_size(0, == , cc_treetable_contains_key(table, &d));

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_get(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");

    char* ra;
    char* rb;
    char* rc;
    cc_treetable_get(table, &a, (void*)&ra);
    cc_treetable_get(table, &b, (void*)&rb);
    cc_treetable_get(table, &c, (void*)&rc);

    munit_assert_string_equal("a", ra);
    munit_assert_string_equal("b", rb);
    munit_assert_string_equal("c", rc);

    cc_treetable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_size(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");

    munit_assert_size(3, == , cc_treetable_size(table));

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_get_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int* first;
    cc_treetable_get_first_key(table, (void*)&first);

    munit_assert_int(a, == , *first);

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_get_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int* last;
    cc_treetable_get_last_key(table, (void*)&last);

    munit_assert_int(d, == , *last);

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_remove_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    cc_treetable_remove_first(table, NULL);

    munit_assert_size(0, == , cc_treetable_contains_key(table, &a));

    cc_treetable_destroy(table);

    return MUNIT_OK;
}

static MunitResult test_remove_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    cc_treetable_remove_last(table, NULL);

    munit_assert_size(0, == , cc_treetable_contains_key(table, &d));

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_get_greater_than(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int* g;
    cc_treetable_get_greater_than(table, &b, (void*)&g);

    munit_assert_int(c, == , *g);

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_get_lesser_than(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int* g;
    cc_treetable_get_lesser_than(table, &b, (void*)&g);

    munit_assert_int(a, == , *g);

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &a, "1");
    cc_treetable_add(table, &b, "2");
    cc_treetable_add(table, &c, "3");
    cc_treetable_add(table, &d, "5");

    int one = 0;
    int two = 0;
    int three = 0;
    int four = 0;

    CC_TreeTableIter iter;
    cc_treetable_iter_init(&iter, table);

    CC_TreeTableEntry entry;
    while (cc_treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const* key = entry.key;

        if (*key == a)
            one++;

        if (*key == b)
            two++;

        if (*key == c)
            three++;

        if (*key == d)
            four++;
    }

    munit_assert_int(1, == , one);
    munit_assert_int(1, == , two);
    munit_assert_int(1, == , three);
    munit_assert_int(1, == , four);

    cc_treetable_destroy(table);
    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeTable* table;
    cc_treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "a");
    cc_treetable_add(table, &c, "a");

    CC_TreeTableIter iter;
    cc_treetable_iter_init(&iter, table);

    CC_TreeTableEntry entry;
    while (cc_treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const* key = entry.key;

        if (*key == b) {
            munit_assert_int(CC_OK, == ,
                cc_treetable_iter_remove(&iter, NULL));

            munit_assert_int(CC_ERR_KEY_NOT_FOUND, == ,
                cc_treetable_iter_remove(&iter, NULL));
        }
    }

    munit_assert_size(2, == , cc_treetable_size(table));
    munit_assert_size(0, == , cc_treetable_contains_key(table, &b));

    cc_treetable_destroy(table);
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { "/treetable/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_get", test_get, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_size", test_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_get_first", test_get_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_get_last", test_get_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_remove_first", test_remove_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_remove_last", test_remove_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_get_greater_than", test_get_greater_than, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_get_lesser_than", test_get_lesser_than, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_iter_next", test_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { "/treetable/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}