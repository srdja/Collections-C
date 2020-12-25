#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cc_treetable.h"
#include "cc_deque.h"
#include "CppUTest/TestHarness_c.h"

static CC_TreeTable *table;

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

TEST_GROUP_C_SETUP(CC_TreeTableTestsWithDefaults)
{
    cc_treetable_new(cmp, &table);
};

TEST_GROUP_C_TEARDOWN(CC_TreeTableTestsWithDefaults)
{
    cc_treetable_destroy(table);
}

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableAdd)
{
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

    CHECK_EQUAL_C_INT(1, cc_treetable_contains_key(table, &a));
    CHECK_EQUAL_C_INT(1, cc_treetable_contains_key(table, &e));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableRemove)
{
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
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &f));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveAll)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");
    cc_treetable_add(table, &d, "d");

    cc_treetable_remove_all(table);

    CHECK_EQUAL_C_INT(0, cc_treetable_size(table));
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &a));
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &b));
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &c));
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &d));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableGet)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");

    char *ra;
    char *rb;
    char *rc;
    cc_treetable_get(table, &a, (void*) &ra);
    cc_treetable_get(table, &b, (void*) &rb);
    cc_treetable_get(table, &c, (void*) &rc);

    CHECK_EQUAL_C_STRING("a", ra);
    CHECK_EQUAL_C_STRING("b", rb);
    CHECK_EQUAL_C_STRING("c", rc);
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableSize)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_treetable_add(table, &a, "a");
    cc_treetable_add(table, &b, "b");
    cc_treetable_add(table, &c, "c");

    CHECK_EQUAL_C_INT(3, cc_treetable_size(table));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableGetFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int *first;
    cc_treetable_get_first_key(table, (void*) &first);

    CHECK_EQUAL_C_INT(a, *first);
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableGetLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int *last;
    cc_treetable_get_last_key(table, (void*) &last);

    CHECK_EQUAL_C_INT(d, *last);
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    cc_treetable_remove_first(table, NULL);

    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &a));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    cc_treetable_remove_last(table, NULL);

    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &d));
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableGetGreaterThan)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int *g;
    cc_treetable_get_greater_than(table, &b, (void*) &g);

    CHECK_EQUAL_C_INT(c, *g);
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableGetLessThan)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &c, "a");
    cc_treetable_add(table, &d, "b");
    cc_treetable_add(table, &b, "c");
    cc_treetable_add(table, &a, "d");

    int *g;
    cc_treetable_get_lesser_than(table, &b, (void*) &g);

    CHECK_EQUAL_C_INT(a, *g);
};


TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableIterNext)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_treetable_add(table, &a, "1");
    cc_treetable_add(table, &b, "2");
    cc_treetable_add(table, &c, "3");
    cc_treetable_add(table, &d, "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;

    CC_TreeTableIter iter;
    cc_treetable_iter_init(&iter, table);

    CC_TreeTableEntry entry;
    while (cc_treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const *key = entry.key;

        if (*key == a)
            one++;

        if (*key == b)
            two++;

        if (*key == c)
            three++;

        if (*key == d)
            four++;
    }

    CHECK_EQUAL_C_INT(1, one);
    CHECK_EQUAL_C_INT(1, two);
    CHECK_EQUAL_C_INT(1, three);
    CHECK_EQUAL_C_INT(1, four);
};

TEST_C(CC_TreeTableTestsWithDefaults, CC_TreeTableIterRemove)
{
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
        int const *key = entry.key;

        if (*key == b) {
            CHECK_EQUAL_C_INT(CC_OK,
                              cc_treetable_iter_remove(&iter, NULL));

            CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND,
                              cc_treetable_iter_remove(&iter, NULL));
        }
    }

    CHECK_EQUAL_C_INT(2, cc_treetable_size(table));
    CHECK_EQUAL_C_INT(0, cc_treetable_contains_key(table, &b));
};
