#include "munit.h"
#include "cc_hashset.h"
#include <stdlib.h>

static MunitResult test_new(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    CC_HashSetConf conf;

    cc_hashset_conf_init(&conf);
    conf.initial_capacity = 7;
    munit_assert_int(CC_OK, ==, cc_hashset_new_conf(&conf, &set));

    munit_assert_size(0, == , cc_hashset_size(set));
    munit_assert_size(8, == , cc_hashset_capacity(set));

    cc_hashset_destroy(set);

    return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    munit_assert_int(CC_OK, == , cc_hashset_new(&set));

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";
    char* d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    munit_assert_size(3, == , cc_hashset_size(set));
    munit_assert_true(cc_hashset_contains(set, a));
    munit_assert_true(cc_hashset_contains(set, d));

    cc_hashset_destroy(set);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    cc_hashset_new(&set);

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";
    char* d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    cc_hashset_remove(set, "bar", NULL);

    munit_assert_size(2, == , cc_hashset_size(set));
    munit_assert_false(cc_hashset_contains(set, "bar"));

    cc_hashset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    cc_hashset_new(&set);

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";
    char* d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    cc_hashset_remove_all(set);

    munit_assert_size(0, == , cc_hashset_size(set));
    munit_assert_false(cc_hashset_contains(set, "bar"));
    munit_assert_false(cc_hashset_contains(set, "c"));

    cc_hashset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    cc_hashset_new(&set);

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    CC_HashSetIter iter;
    cc_hashset_iter_init(&iter, set);

    char* e;
    while (cc_hashset_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (!strcmp(e, "foo"))
            x++;

        if (!strcmp(e, "bar"))
            y++;

        if (!strcmp(e, "baz"))
            z++;
    }

    munit_assert_size(1, == , x);
    munit_assert_size(1, == , y);
    munit_assert_size(1, == , z);

    cc_hashset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_HashSet* set;
    cc_hashset_new(&set);

    char* a = "foo";
    char* b = "bar";
    char* c = "baz";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);

    CC_HashSetIter iter;
    cc_hashset_iter_init(&iter, set);

    char* e;
    while (cc_hashset_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (!strcmp(e, "bar")) {
            cc_hashset_iter_remove(&iter, NULL);
        }
    }

    munit_assert_size(2, == , cc_hashset_size(set));
    munit_assert_false(cc_hashset_contains(set, "bar"));

    cc_hashset_destroy(set);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/hashset/test_new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/hashset/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/hashset/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/hashset/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/hashset/test_iter_next", test_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/hashset/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}