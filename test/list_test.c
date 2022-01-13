/*
 * dcc_list_test.c
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cc_list.h"

#include "CppUTest/TestHarness_c.h"

static CC_List *list1;
static CC_List *list2;

int cmp(void const *e1, void const *e2)
{
    int i = *(*((int**) e1));
    int j = *(*((int**) e2));

    if (i < j)
        return -1;
    if (i == j)
        return 0;
    return 1;
}

int zero_if_ptr_eq(void const *e1, void const *e2)
{
    return !(e1 == e2);
}

void *copy(void *e1)
{
    int *cp = (int *) malloc(sizeof(int));
    *cp = *((int*) e1);
    return cp;
}

bool pred1(const void *e)
{
    return *(int*)e == 0;
}

bool pred2(const void *e)
{
    return *(int*)e > 3;
}

void sum_reduce(void *e1, void *e2, void *res)
{
    int i = *(int *)e1;
    int j = *(int *)e2;

    *(int *)res = i + j;
}

void bool_and(void *e1, void *e2, void *res)
{
    bool p = *(bool *)e1;
    bool q = *(bool *)e2;

    *(bool *)res = p && q;
}

TEST_GROUP_C_SETUP(CC_ListTestsWithDefaults)
{
    cc_list_new(&list1);
    cc_list_new(&list2);
};

TEST_GROUP_C_TEARDOWN(CC_ListTestsWithDefaults)
{
    cc_list_destroy(list1);
    cc_list_destroy(list2);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListNew)
{
    CHECK_C( list1 != NULL);
    CHECK_C( list2 != NULL);

    void *e = NULL;
    cc_list_get_first(list1, &e);
    CHECK_C(e == NULL);

    cc_list_get_last(list1, &e);
    CHECK_C(e == NULL);

    CHECK_EQUAL_C_INT(0, cc_list_size(list1));
    CHECK_C(list1 != list2);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListAdd)
{
    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    CHECK_EQUAL_C_INT(CC_OK, cc_list_add(list1, s1));
    CHECK_EQUAL_C_INT(CC_OK, cc_list_add(list1, s2));
    CHECK_EQUAL_C_INT(CC_OK, cc_list_add(list1, s3));
    CHECK_EQUAL_C_INT(CC_OK, cc_list_add(list1, s4));

    void *e;
    cc_list_get_first(list1, &e);
    CHECK_C(e != NULL);

    cc_list_get_last(list1, &e);
    CHECK_C(e != NULL);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListAddLast)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    CHECK_EQUAL_C_INT(4, cc_list_size(list1));

    int *last;
    cc_list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    cc_list_add_last(list1, &append);
    CHECK_EQUAL_C_INT(5, cc_list_size(list1));

    cc_list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_INT(append, *last);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListAddFirst)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    CHECK_EQUAL_C_INT(4, cc_list_size(list1));

    int *first;
    cc_list_get_first(list1, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    cc_list_add_first(list1, &prepend);
    CHECK_EQUAL_C_INT(5, cc_list_size(list1));

    cc_list_get_first(list1, (void*) &first);
    CHECK_EQUAL_C_INT(prepend, *first);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListContains)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    CHECK_EQUAL_C_INT(2, cc_list_contains(list1, &b));
    CHECK_EQUAL_C_INT(1, cc_list_contains(list1, &d));
    CHECK_EQUAL_C_INT(0, cc_list_contains(list1, &e));
};

TEST_C(CC_ListTestsWithDefaults, CC_ListIndexOf)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    size_t idx;
    cc_list_index_of(list1, &a, zero_if_ptr_eq, &idx);
    CHECK_EQUAL_C_INT(0, idx);

    cc_list_index_of(list1, &c, zero_if_ptr_eq, &idx);
    CHECK_EQUAL_C_INT(2, idx);
};


TEST_C(CC_ListTestsWithDefaults, CC_ListSort)
{
    srand(time(NULL));

    /* populate the list with random data */
    int size = 10000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 1000001;
        cc_list_add(list1, e);
    }

    cc_list_sort_in_place(list1, cmp);

    CC_ListIter iter;
    cc_list_iter_init(&iter, list1);

    void *prev;
    void *e;
    cc_list_iter_next(&iter, &prev);
    while (cc_list_iter_next(&iter, &e) != CC_ITER_END) {
        CHECK_C(*((int*)prev) <= *((int*)e));
        prev = e;
    }
};

TEST_C(CC_ListTestsWithDefaults, CC_ListZipIterNext)
{
    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    size_t i = 0;

    void *e1, *e2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            CHECK_EQUAL_C_STRING("a", (char*)e1);
            CHECK_EQUAL_C_STRING("e", (char*)e2);
        }
        if (i == 2) {
            CHECK_EQUAL_C_STRING("c", (char*)e1);
            CHECK_EQUAL_C_STRING("g", (char*)e2);
        }
        i++;
    }
    CHECK_EQUAL_C_INT(3, i);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListZipIterAdd)
{
    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    char *h = "h";
    char *i = "i";

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_list_zip_iter_add(&zip, h, i);
    }

    size_t index;
    CHECK_EQUAL_C_INT(CC_OK, cc_list_index_of(list1, "h", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(2, index);

    CHECK_EQUAL_C_INT(CC_OK, cc_list_index_of(list2, "i", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(2, index);

    CHECK_EQUAL_C_INT(CC_OK, cc_list_index_of(list1, "c", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(3, index);

    CHECK_EQUAL_C_INT(1, cc_list_contains(list1, "h"));
    CHECK_EQUAL_C_INT(1, cc_list_contains(list2, "i"));
    CHECK_EQUAL_C_INT(5, cc_list_size(list1));
    CHECK_EQUAL_C_INT(4, cc_list_size(list2));

    cc_list_zip_iter_init(&zip, list1, list2);
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            cc_list_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    cc_list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);

    cc_list_get_last(list2, (void*) &last);
    CHECK_EQUAL_C_STRING("y", last);
};

TEST_C(CC_ListTestsWithDefaults, CC_ListZipIterRemove)
{
    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_list_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, cc_list_contains(list1, "b"));
    CHECK_EQUAL_C_INT(0, cc_list_contains(list2, "f"));
    CHECK_EQUAL_C_INT(3, cc_list_size(list1));
    CHECK_EQUAL_C_INT(2, cc_list_size(list2));
};


TEST_C(CC_ListTestsWithDefaults, CC_ListZipIterReplace)
{
    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    char *h = "h";
    char *i = "i";

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_list_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    CHECK_EQUAL_C_INT(CC_OK, cc_list_index_of(list1, "h", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(1, index);

    CHECK_EQUAL_C_INT(CC_OK, cc_list_index_of(list2, "i", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, cc_list_contains(list1, "h"));
    CHECK_EQUAL_C_INT(1, cc_list_contains(list2, "i"));
};

TEST_GROUP_C_SETUP(CC_ListTestsCC_ListPrefilled)
{
    cc_list_new(&list1);
    cc_list_new(&list2);

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    cc_list_add(list1, a);
    cc_list_add(list1, b);
    cc_list_add(list1, c);
    cc_list_add(list1, d);

    a = (int*) malloc(sizeof(int));
    b = (int*) malloc(sizeof(int));
    c = (int*) malloc(sizeof(int));
    d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 8;

    cc_list_add(list2, a);
    cc_list_add(list2, b);
    cc_list_add(list2, c);
    cc_list_add(list2, d);
};

TEST_GROUP_C_TEARDOWN(CC_ListTestsCC_ListPrefilled)
{
    cc_list_destroy_cb(list1, free);
    cc_list_destroy(list2);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListAddAllAt)
{
    cc_list_add_all_at(list1, list2, 2);
    CHECK_EQUAL_C_INT(4, cc_list_size(list2));
    CHECK_EQUAL_C_INT(8, cc_list_size(list1));

    int *last;
    cc_list_get_last(list1, (void*) &last);
    int *l1i4;
    cc_list_get_at(list1, 4, (void*) &l1i4);
    int *l2i2;
    cc_list_get_at(list2, 2, (void*) &l2i2);
    CHECK_EQUAL_C_INT(4, *last);
    CHECK_EQUAL_C_INT(*l1i4, *l2i2);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListIterAdd)
{
    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    CC_ListIter iter;
    cc_list_iter_init(&iter, list1);

    int *el;
    while (cc_list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            cc_list_iter_add(&iter, ins);
    }

    CHECK_EQUAL_C_INT(5, cc_list_size(list1));

    int *li3;
    cc_list_get_at(list1, 3, (void*) &li3);

    CHECK_EQUAL_C_INT(*li3, *ins);

    int *li4;
    cc_list_get_at(list1, 4, (void*) &li4);
    CHECK_EQUAL_C_INT(4, *li4);

    cc_list_iter_init(&iter, list1);

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    while (cc_list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4) {
            cc_list_iter_add(&iter, ins);
        }
    }

    void *e;
    cc_list_get_last(list1, &e);
    CHECK_EQUAL_C_INT(*ins, *((int*)e));
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListIterRemove)
{
    int *rm;
    cc_list_get_at(list1, 2, (void*) &rm);

    CC_ListIter iter;
    cc_list_iter_init(&iter, list1);

    int *e;
    while (cc_list_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            cc_list_iter_remove(&iter, NULL);
        }
    }
    CHECK_EQUAL_C_INT(3, cc_list_size(list1));
    CHECK_EQUAL_C_INT(0, cc_list_contains(list1, rm));
    free(rm);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListIterDescRemove)
{
    CC_ListIter iter;
    cc_list_diter_init(&iter, list1);

    int *i;
    while (cc_list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 1 || *i == 3) {
            cc_list_diter_remove(&iter, NULL);
            free(i);
        }
    }
    CHECK_EQUAL_C_INT(2, cc_list_size(list1));

    int *first;
    cc_list_get_first(list1, (void*) &first);

    int *last;
    cc_list_get_last(list1, (void*) &last);

    int *i1;
    cc_list_get_at(list1, 1, (void*) &i1);

    CHECK_EQUAL_C_INT(2, *first);
    CHECK_EQUAL_C_INT(4, *last);
    CHECK_EQUAL_C_INT(4, *i1);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListIterDescAdd)
{
    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    *a = 30;
    *b = 78;

    CC_ListIter iter;
    cc_list_diter_init(&iter, list1);

    int *i;
    while (cc_list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 4) // add to tail
            cc_list_diter_add(&iter, a);

        if (*i == 3)
            cc_list_diter_add(&iter, b);
    }

    CHECK_EQUAL_C_INT(6, cc_list_size(list1));
    int *el;
    cc_list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    cc_list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    cc_list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(*b, *el);

    cc_list_get_at(list1, 4, (void*) &el);
    CHECK_EQUAL_C_INT(*a, *el);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListReverse)
{
    int *last_old;
    cc_list_get_last(list1, (void*) &last_old);

    cc_list_reverse(list1);

    int *el;
    cc_list_get_at(list1, 0, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    cc_list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(2, *el);

    cc_list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_POINTER(el, last_old);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListToArray)
{
    int **array;
    cc_list_to_array(list1, (void*)&array);

    void *e;
    cc_list_get_at(list1, 0, &e);
    CHECK_EQUAL_C_POINTER(array[0], e);

    cc_list_get_at(list1, 2, &e);
    CHECK_EQUAL_C_POINTER(array[2], e);

    cc_list_get_at(list1, 3, &e);
    CHECK_EQUAL_C_POINTER(array[3], e);

    free(array);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListSpliceAt)
{
    cc_list_splice_at(list1, list2, 2);

    CHECK_EQUAL_C_INT(8, cc_list_size(list1));
    CHECK_EQUAL_C_INT(0, cc_list_size(list2));

    int *el;
    cc_list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    cc_list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    cc_list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(5, *el);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListSplice)
{
    cc_list_splice(list1, list2);
    CHECK_EQUAL_C_INT(8, cc_list_size(list1));
    CHECK_EQUAL_C_INT(0, cc_list_size(list2));

    int *el;
    cc_list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(8, *el);

    cc_list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    cc_list_get_at(list1, 4, (void*) &el);
    CHECK_EQUAL_C_INT(5, *el);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListAddAll)
{
    cc_list_add_all(list2, list1);

    CHECK_EQUAL_C_INT(8, cc_list_size(list2));

    int *l1last;
    cc_list_get_last(list1, (void*) &l1last);

    int *l2last;
    cc_list_get_last(list2, (void*) &l2last);
    CHECK_EQUAL_C_INT(*l1last, *l2last);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListRemove)
{
    int *e;
    cc_list_get_at(list1, 1, (void*) &e);
    cc_list_remove(list1, e, NULL);

    CHECK_EQUAL_C_INT(3, cc_list_size(list1));
    CHECK_EQUAL_C_INT(0, cc_list_contains(list1, e));
    free(e);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListRemoveFirst)
{
    int *el;
    cc_list_remove_first(list1, (void*) &el);
    free(el);

    CHECK_EQUAL_C_INT(3, cc_list_size(list1));

    cc_list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(2, *el);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListRemoveLast)
{
    int *el;
    cc_list_remove_last(list1, (void*) &el);
    free(el);
    CHECK_EQUAL_C_INT(3, cc_list_size(list1));

    CHECK_EQUAL_C_INT(CC_OK, cc_list_get_last(list1, (void*) &el));
    CHECK_EQUAL_C_INT(3, *el);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListRemoveAt)
{
    int *e;
    cc_list_remove_at(list1, 2, (void*) &e);
    free(e);

    cc_list_get_at(list1, 2, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
    CHECK_EQUAL_C_INT(3, cc_list_size(list1));
    cc_list_remove_at(list1, 0, (void*) &e);
    free(e);

    cc_list_get_at(list1, 0, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListRemoveAll)
{

    int *e;
    cc_list_get_first(list1, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    cc_list_remove_all_cb(list1, free);
    CHECK_EQUAL_C_INT(0, cc_list_size(list1));

    e = NULL;
    cc_list_get_first(list1, (void*)&e);
    CHECK_C(e == NULL);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListGetLast)
{
    int *e;
    cc_list_get_last(list1, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListGetAt)
{
    int *e;
    cc_list_get_at(list1, 1, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListReplaceAt)
{
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *r;
    cc_list_replace_at(list1, replacement, 2, (void*) &r);
    free(r);

    cc_list_get_at(list1, 2, (void*) &r);
    CHECK_C((int*)r == replacement);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListCopyDeep)
{
    CC_List *cp;
    cc_list_copy_deep(list1, copy, &cp);
    CHECK_EQUAL_C_INT(4, cc_list_size(cp));

    int *e;
    cc_list_get_at(cp, 2, (void*) &e);

    int *le;
    cc_list_get_at(list1, 2, (void*) &le);
    CHECK_EQUAL_C_INT(*e, *le);

    cc_list_get_at(cp, 2, (void*) &e);
    cc_list_get_at(list1, 2, (void*) &le);
    CHECK_C(e != le);

    cc_list_destroy_cb(cp, free);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListSublist)
{
    CC_List *sub;
    cc_list_sublist(list1, 1, 2, &sub);
    CHECK_EQUAL_C_INT(2, cc_list_size(sub));

    int *s1;
    cc_list_get_at(sub, 1, (void*) &s1);

    int *l2;
    cc_list_get_at(list1, 2, (void*) &l2);
    CHECK_EQUAL_C_INT(*s1, *l2);

    cc_list_destroy(sub);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListAddAt)
{
    int *ins = (int*)malloc(sizeof(int));
    *ins = 90;

    cc_list_add_at(list1, ins, 3);
    CHECK_EQUAL_C_INT(5, cc_list_size(list1));

    int *new;
    cc_list_get_at(list1, 3, (void*) &new);
    CHECK_EQUAL_C_INT(*ins, *new);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListCopyShallow)
{
    CC_List *cp;
    cc_list_copy_shallow(list1, &cp);
    CHECK_EQUAL_C_INT(4, cc_list_size(cp));

    void *f1;
    void *f2;
    for(int i=0; i< cc_list_size(list1); i++) {
        cc_list_get_at(cp, i, &f1);
        cc_list_get_at(list1, i, &f2);
        CHECK_EQUAL_C_POINTER(f1, f2);
    }
    cc_list_destroy(cp);
};

void test_cc_list_destroy()
{
     // Nothing to test
}

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListMutFilter1)
{
    CHECK_EQUAL_C_INT(4, cc_list_size(list1));
    cc_list_filter_mut(list1, pred1);
    CHECK_EQUAL_C_INT(0, cc_list_size(list1));

    void *e = NULL;
    cc_list_get_first(list1, &e);
    CHECK_C(e == NULL);

    cc_list_get_last(list1, &e);
    CHECK_C(e == NULL);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListMutFilter2)
{
    CHECK_EQUAL_C_INT(4, cc_list_size(list1));
    cc_list_filter_mut(list1, pred2);
    CHECK_EQUAL_C_INT(1, cc_list_size(list1));

    CC_ListIter iter;
    int *el = NULL;
    int i = 4;
    cc_list_iter_init(&iter, list1);
    while (cc_list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListFilter1)
{
    CC_List *filter = NULL;
    cc_list_filter(list1, pred1, &filter);

    CHECK_EQUAL_C_INT(4, cc_list_size(list1));
    CHECK_EQUAL_C_INT(0, cc_list_size(filter));

    void *e = NULL;
    cc_list_get_first(filter, &e);
    CHECK_C(e == NULL);

    cc_list_get_last(filter, &e);
    CHECK_C(e == NULL);
    free(filter);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListFilter2)
{
    CC_List *filter = NULL;
    cc_list_filter(list1, pred2, &filter);

    CHECK_EQUAL_C_INT(4, cc_list_size(list1));
    CHECK_EQUAL_C_INT(1, cc_list_size(filter));

    CC_ListIter iter;
    int *el = NULL;
    int i = 4;
    cc_list_iter_init(&iter, filter);
    while (cc_list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

    free(filter);
};


TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListReduce1)
{
    int *res;

    cc_list_reduce(list1, sum_reduce, &res);

    CHECK_EQUAL_C_INT(10, *res);
};

TEST_C(CC_ListTestsCC_ListPrefilled, CC_ListReduce2)
{
    bool *res;

    cc_list_reduce(list1, bool_and, &res);

    CHECK_EQUAL_C_BOOL(true, *res);
}