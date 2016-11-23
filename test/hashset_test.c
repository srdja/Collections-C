#include <string.h>
#include "hashset.h"
#include "CppUTest/TestHarness_c.h"

static HashSetConf conf;
static HashSet *set;
int stat;

TEST_GROUP_C_SETUP(HashSetTests)
{
    stat = hashset_new(&set);
};

TEST_GROUP_C_TEARDOWN(HashSetTests)
{
    hashset_destroy(set);
};

TEST_C(HashSetTests, HashSetAdd)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(set, a);
    hashset_add(set, b);
    hashset_add(set, c);
    hashset_add(set, d);

    CHECK_EQUAL_C_INT(3, hashset_size(set));
    CHECK_EQUAL_C_INT(1, hashset_contains(set, a));
    CHECK_EQUAL_C_INT(1, hashset_contains(set, d));
};


TEST_C(HashSetTests, HashSetRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(set, a);
    hashset_add(set, b);
    hashset_add(set, c);
    hashset_add(set, d);

    hashset_remove(set, "bar", NULL);

    CHECK_EQUAL_C_INT(2, hashset_size(set));
    CHECK_EQUAL_C_INT(0, hashset_contains(set, "bar"));
};


TEST_C(HashSetTests, HashSetRemoveAll)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(set, a);
    hashset_add(set, b);
    hashset_add(set, c);
    hashset_add(set, d);

    hashset_remove_all(set);

    CHECK_EQUAL_C_INT(0, hashset_size(set));
    CHECK_C(!hashset_contains(set, "bar"));
    CHECK_C(!hashset_contains(set, "c"));
};

TEST_C(HashSetTests, HashSetIterNext)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashset_add(set, a);
    hashset_add(set, b);
    hashset_add(set, c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    HashSetIter iter;
    hashset_iter_init(&iter, set);

    char *e;
    while (hashset_iter_next(&iter, (void*)&e) != CC_ITER_END) {
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

TEST_C(HashSetTests, HashSetIterRemove)
{
    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashset_add(set, a);
    hashset_add(set, b);
    hashset_add(set, c);

    HashSetIter iter;
    hashset_iter_init(&iter, set);

    char *e;
    while (hashset_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (!strcmp(e, "bar"))
            hashset_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(2, hashset_size(set));
    CHECK_C(!hashset_contains(set, "bar"));
};

TEST_GROUP_C_SETUP(HashSetTestsConf)
{
    hashset_conf_init(&conf);
    conf.initial_capacity = 7;
    hashset_new_conf(&conf, &set);
};

TEST_GROUP_C_TEARDOWN(HashSetTestsConf)
{
    hashset_destroy(set);
};

TEST_C(HashSetTestsConf, HashSetNew)
{
    CHECK_EQUAL_C_INT(0, hashset_size(set));
    CHECK_EQUAL_C_INT(8, hashset_capacity(set));
};
