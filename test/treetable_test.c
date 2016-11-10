#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "treetable.h"
#include "deque.h"
#include "CppUTest/TestHarness_c.h"

static TreeTable *table;

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

TEST_GROUP_C_SETUP(TreeTableTestsWithDefaults)
{
    treetable_new(cmp, &table);
};

TEST_GROUP_C_TEARDOWN(TreeTableTestsWithDefaults)
{
    treetable_destroy(table);
}

TEST_C(TreeTableTestsWithDefaults, TreeTableAdd)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");
    treetable_add(table, &e, "e");
    treetable_add(table, &f, "f");
    treetable_add(table, &g, "g");
    treetable_add(table, &h, "h");

    CHECK_EQUAL_C_INT(1, treetable_contains_key(table, &a));
    CHECK_EQUAL_C_INT(1, treetable_contains_key(table, &e));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableRemove)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");
    treetable_add(table, &e, "e");
    treetable_add(table, &f, "f");
    treetable_add(table, &g, "g");
    treetable_add(table, &h, "h");

    treetable_remove(table, &f, NULL);
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &f));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableRemoveAll)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");

    treetable_remove_all(table);

    CHECK_EQUAL_C_INT(0, treetable_size(table));
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &a));
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &b));
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &c));
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &d));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableGet)
{
    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");

    char *ra;
    char *rb;
    char *rc;
    treetable_get(table, &a, (void*) &ra);
    treetable_get(table, &b, (void*) &rb);
    treetable_get(table, &c, (void*) &rc);

    CHECK_EQUAL_C_STRING("a", ra);
    CHECK_EQUAL_C_STRING("b", rb);
    CHECK_EQUAL_C_STRING("c", rc);
};

TEST_C(TreeTableTestsWithDefaults, TreeTableSize)
{
    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");

    CHECK_EQUAL_C_INT(3, treetable_size(table));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableGetFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *first;
    treetable_get_first_key(table, (void*) &first);

    CHECK_EQUAL_C_INT(a, *first);
};

TEST_C(TreeTableTestsWithDefaults, TreeTableGetLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *last;
    treetable_get_last_key(table, (void*) &last);

    CHECK_EQUAL_C_INT(d, *last);
};

TEST_C(TreeTableTestsWithDefaults, TreeTableRemoveFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    treetable_remove_first(table, NULL);

    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &a));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableRemoveLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    treetable_remove_last(table, NULL);

    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &d));
};

TEST_C(TreeTableTestsWithDefaults, TreeTableGetGreaterThan)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *g;
    treetable_get_greater_than(table, &b, (void*) &g);

    CHECK_EQUAL_C_INT(c, *g);
};

TEST_C(TreeTableTestsWithDefaults, TreeTableGetLessThan)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *g;
    treetable_get_lesser_than(table, &b, (void*) &g);

    CHECK_EQUAL_C_INT(a, *g);
};


TEST_C(TreeTableTestsWithDefaults, TreeTableIterNext)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &a, "1");
    treetable_add(table, &b, "2");
    treetable_add(table, &c, "3");
    treetable_add(table, &d, "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;

    TreeTableIter iter;
    treetable_iter_init(&iter, table);

    TreeTableEntry entry;
    while (treetable_iter_next(&iter, &entry) != CC_ITER_END) {
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

TEST_C(TreeTableTestsWithDefaults, TreeTableIterRemove)
{
    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "a");
    treetable_add(table, &c, "a");

    TreeTableIter iter;
    treetable_iter_init(&iter, table);

    TreeTableEntry entry;
    while (treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const *key = entry.key;

        if (*key == b) {
            CHECK_EQUAL_C_INT(CC_OK,
                              treetable_iter_remove(&iter, NULL));

            CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND,
                              treetable_iter_remove(&iter, NULL));
        }
    }

    CHECK_EQUAL_C_INT(2, treetable_size(table));
    CHECK_EQUAL_C_INT(0, treetable_contains_key(table, &b));
};
