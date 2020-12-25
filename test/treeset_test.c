#include "cc_treeset.h"
#include "CppUTest/TestHarness_c.h"

static CC_TreeSet *set;

int cmp(const void *k1, const void *k2)
{
    int a = *((int*) k1);
    int b = *((int*) k2);

    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

TEST_GROUP_C_SETUP(CC_TreeSetTestsWithDefaults)
{
    cc_treeset_new(cmp, &set);
};

TEST_GROUP_C_TEARDOWN(CC_TreeSetTestsWithDefaults)
{

};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetAdd)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);
    cc_treeset_add(set, &c);

    CHECK_EQUAL_C_INT(3, cc_treeset_size(set));
    CHECK_EQUAL_C_INT(1, cc_treeset_contains(set, &a));
    CHECK_EQUAL_C_INT(1, cc_treeset_contains(set, &b));
};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetRemove)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    cc_treeset_remove(set, &a, NULL);

    CHECK_EQUAL_C_INT(2, cc_treeset_size(set));
    CHECK_EQUAL_C_INT(0, cc_treeset_contains(set, &a));
};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetRemoveAll)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    cc_treeset_remove_all(set);

    CHECK_EQUAL_C_INT(0, cc_treeset_size(set));
    CHECK_EQUAL_C_INT(0, cc_treeset_contains(set, &a));
    CHECK_EQUAL_C_INT(0, cc_treeset_contains(set, &c));
};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetSetSize)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    CHECK_EQUAL_C_INT(3, cc_treeset_size(set));
};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetIterNext)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);
    cc_treeset_add(set, &d);

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;

    CC_TreeSetIter iter;
    cc_treeset_iter_init(&iter, set);

    void *e;
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

    CHECK_EQUAL_C_INT(1, one);
    CHECK_EQUAL_C_INT(1, two);
    CHECK_EQUAL_C_INT(1, three);
    CHECK_EQUAL_C_INT(1, four);
};

TEST_C(CC_TreeSetTestsWithDefaults, CC_TreeSetIterRemove)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treeset_add(set, &a);
    cc_treeset_add(set, &b);
    cc_treeset_add(set, &c);

    CC_TreeSetIter iter;
    cc_treeset_iter_init(&iter, set);

    void *e;
    while (cc_treeset_iter_next(&iter, &e) != CC_ITER_END) {
        if (*((int*)e) == b)
            cc_treeset_iter_remove(&iter, NULL);
    }
    CHECK_EQUAL_C_INT(2, cc_treeset_size(set));
    CHECK_EQUAL_C_INT(0, cc_treeset_contains(set, &b));
};
