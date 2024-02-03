#include "munit.h"
#include "cc_treeset.h"
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

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);
    cc_treeset_add(set, &c);

    munit_assert_size(3, == , cc_treeset_size(set));
    munit_assert_true(cc_treeset_contains(set, &a));
    munit_assert_true(cc_treeset_contains(set, &b));

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    cc_treeset_remove(set, &a, NULL);

    munit_assert_size(2, == , cc_treeset_size(set));
    munit_assert_false(cc_treeset_contains(set, &a));

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    cc_treeset_remove_all(set);

    munit_assert_size(0, == , cc_treeset_size(set));
    munit_assert_false(cc_treeset_contains(set, &a));
    munit_assert_false(cc_treeset_contains(set, &c));

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_size(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    munit_assert_size(3, == , cc_treeset_size(set));

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);
    cc_treeset_add(set, &d);

    int one = 0;
    int two = 0;
    int three = 0;
    int four = 0;

    CC_TreeSetIter iter;
    cc_treeset_iter_init(&iter, set);

    void* e;
    while (cc_treeset_iter_next(&iter, &e) != CC_ITER_END) {
        if (*((int*)e) == a)
            one++;

        if (*((int*)e) == b)
            two++;

        if (*((int*)e) == c)
            three++;

        if (*((int*)e) == d)
            four++;
    }

	munit_assert_int(1, == , one);
	munit_assert_int(1, == , two);
	munit_assert_int(1, == , three);
	munit_assert_int(1, == , four);

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_TreeSet* set;
    cc_treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    CC_TreeSetIter iter;
    cc_treeset_iter_init(&iter, set);

    void* e;
    while (cc_treeset_iter_next(&iter, &e) != CC_ITER_END) {
        if (*((int*)e) == b)
            cc_treeset_iter_remove(&iter, NULL);
    }
    munit_assert_size(2, == , cc_treeset_size(set));
    munit_assert_false(cc_treeset_contains(set, &b));

    cc_treeset_destroy(set);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/treeset/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/treeset/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/treeset/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/treeset/test_size", test_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/treeset/test_iter_next", test_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/treeset/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}