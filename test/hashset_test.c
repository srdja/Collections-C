#include <string.h>
#include "cc_hashset.h"
#include "CppUTest/TestHarness_c.h"

static CC_HashSetConf conf;
static CC_HashSet *set;
int stat;

TEST_GROUP_C_SETUP(CC_HashSetTests)
{
    stat = cc_hashset_new(&set);
};

TEST_GROUP_C_TEARDOWN(CC_HashSetTests)
{
    cc_hashset_destroy(set);
};

TEST_C(CC_HashSetTests, CC_HashSetAdd)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    CHECK_EQUAL_C_INT(3, cc_hashset_size(set));
    CHECK_EQUAL_C_INT(1, cc_hashset_contains(set, a));
    CHECK_EQUAL_C_INT(1, cc_hashset_contains(set, d));
};


TEST_C(CC_HashSetTests, CC_HashSetRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    cc_hashset_remove(set, "bar", NULL);

    CHECK_EQUAL_C_INT(2, cc_hashset_size(set));
    CHECK_EQUAL_C_INT(0, cc_hashset_contains(set, "bar"));
};


TEST_C(CC_HashSetTests, CC_HashSetRemoveAll)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);
    cc_hashset_add(set, d);

    cc_hashset_remove_all(set);

    CHECK_EQUAL_C_INT(0, cc_hashset_size(set));
    CHECK_C(!cc_hashset_contains(set, "bar"));
    CHECK_C(!cc_hashset_contains(set, "c"));
};

TEST_C(CC_HashSetTests, CC_HashSetIterNext)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    CC_HashSetIter iter;
    cc_hashset_iter_init(&iter, set);

    char *e;
    while (cc_hashset_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (!strcmp(e, "foo"))
            x++;

        if (!strcmp(e, "bar"))
            y++;

        if (!strcmp(e, "baz"))
            z++;
    }

    CHECK_EQUAL_C_INT(1, x);
    CHECK_EQUAL_C_INT(1, y);
    CHECK_EQUAL_C_INT(1, z);
};

TEST_C(CC_HashSetTests, CC_HashSetIterRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    cc_hashset_add(set, a);
    cc_hashset_add(set, b);
    cc_hashset_add(set, c);

    CC_HashSetIter iter;
    cc_hashset_iter_init(&iter, set);

    char *e;
    while (cc_hashset_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (!strcmp(e, "bar"))
            cc_hashset_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(2, cc_hashset_size(set));
    CHECK_C(!cc_hashset_contains(set, "bar"));
};

TEST_GROUP_C_SETUP(CC_HashSetTestsConf)
{
    cc_hashset_conf_init(&conf);
    conf.initial_capacity = 7;
    cc_hashset_new_conf(&conf, &set);
};

TEST_GROUP_C_TEARDOWN(CC_HashSetTestsConf)
{
    cc_hashset_destroy(set);
};

TEST_C(CC_HashSetTestsConf, CC_HashSetNew)
{
    CHECK_EQUAL_C_INT(0, cc_hashset_size(set));
    CHECK_EQUAL_C_INT(8, cc_hashset_capacity(set));
};
