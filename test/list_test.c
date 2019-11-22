/*
 * dlist_test.c
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"

#include "CppUTest/TestHarness_c.h"

static List *list1;
static List *list2;

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

TEST_GROUP_C_SETUP(ListTestsWithDefaults)
{
    list_new(&list1);
    list_new(&list2);
};

TEST_GROUP_C_TEARDOWN(ListTestsWithDefaults)
{
    list_destroy(list1);
    list_destroy(list2);
};

TEST_C(ListTestsWithDefaults, ListNew)
{
    CHECK_C( list1 != NULL);
    CHECK_C( list2 != NULL);

    void *e = NULL;
    list_get_first(list1, &e);
    CHECK_C(e == NULL);

    list_get_last(list1, &e);
    CHECK_C(e == NULL);

    CHECK_EQUAL_C_INT(0, list_size(list1));
    CHECK_C(list1 != list2);
};

TEST_C(ListTestsWithDefaults, ListAdd)
{
    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    CHECK_EQUAL_C_INT(CC_OK, list_add(list1, s1));
    CHECK_EQUAL_C_INT(CC_OK, list_add(list1, s2));
    CHECK_EQUAL_C_INT(CC_OK, list_add(list1, s3));
    CHECK_EQUAL_C_INT(CC_OK, list_add(list1, s4));

    void *e;
    list_get_first(list1, &e);
    CHECK_C(e != NULL);

    list_get_last(list1, &e);
    CHECK_C(e != NULL);
};

TEST_C(ListTestsWithDefaults, ListAddLast)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    list_add(list1, &a);
    list_add(list1, &b);
    list_add(list1, &c);
    list_add(list1, &d);

    CHECK_EQUAL_C_INT(4, list_size(list1));

    int *last;
    list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    list_add_last(list1, &append);
    CHECK_EQUAL_C_INT(5, list_size(list1));

    list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_INT(append, *last);
};

TEST_C(ListTestsWithDefaults, ListAddFirst)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    list_add(list1, &a);
    list_add(list1, &b);
    list_add(list1, &c);
    list_add(list1, &d);

    CHECK_EQUAL_C_INT(4, list_size(list1));

    int *first;
    list_get_first(list1, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    list_add_first(list1, &prepend);
    CHECK_EQUAL_C_INT(5, list_size(list1));

    list_get_first(list1, (void*) &first);
    CHECK_EQUAL_C_INT(prepend, *first);
};

TEST_C(ListTestsWithDefaults, ListContains)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    list_add(list1, &a);
    list_add(list1, &b);
    list_add(list1, &b);
    list_add(list1, &c);
    list_add(list1, &d);

    CHECK_EQUAL_C_INT(2, list_contains(list1, &b));
    CHECK_EQUAL_C_INT(1, list_contains(list1, &d));
    CHECK_EQUAL_C_INT(0, list_contains(list1, &e));
};

TEST_C(ListTestsWithDefaults, ListIndexOf)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    list_add(list1, &a);
    list_add(list1, &b);
    list_add(list1, &c);
    list_add(list1, &d);

    size_t idx;
    list_index_of(list1, &a, zero_if_ptr_eq, &idx);
    CHECK_EQUAL_C_INT(0, idx);

    list_index_of(list1, &c, zero_if_ptr_eq, &idx);
    CHECK_EQUAL_C_INT(2, idx);
};


TEST_C(ListTestsWithDefaults, ListSort)
{
    srand(time(NULL));

    /* populate the list with random data */
    int size = 10000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 1000001;
        list_add(list1, e);
    }

    list_sort_in_place(list1, cmp);

    ListIter iter;
    list_iter_init(&iter, list1);

    void *prev;
    void *e;
    list_iter_next(&iter, &prev);
    while (list_iter_next(&iter, &e) != CC_ITER_END) {
        CHECK_C(*((int*)prev) <= *((int*)e));
        prev = e;
    }
};

TEST_C(ListTestsWithDefaults, ListZipIterNext)
{
    list_add(list1, "a");
    list_add(list1, "b");
    list_add(list1, "c");
    list_add(list1, "d");

    list_add(list2, "e");
    list_add(list2, "f");
    list_add(list2, "g");

    ListZipIter zip;
    list_zip_iter_init(&zip, list1, list2);

    size_t i = 0;

    void *e1, *e2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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

TEST_C(ListTestsWithDefaults, ListZipIterAdd)
{
    list_add(list1, "a");
    list_add(list1, "b");
    list_add(list1, "c");
    list_add(list1, "d");

    list_add(list2, "e");
    list_add(list2, "f");
    list_add(list2, "g");

    char *h = "h";
    char *i = "i";

    ListZipIter zip;
    list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_add(&zip, h, i);
    }

    size_t index;
    CHECK_EQUAL_C_INT(CC_OK, list_index_of(list1, "h", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(2, index);

    CHECK_EQUAL_C_INT(CC_OK, list_index_of(list2, "i", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(2, index);

    CHECK_EQUAL_C_INT(CC_OK, list_index_of(list1, "c", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(3, index);

    CHECK_EQUAL_C_INT(1, list_contains(list1, "h"));
    CHECK_EQUAL_C_INT(1, list_contains(list2, "i"));
    CHECK_EQUAL_C_INT(5, list_size(list1));
    CHECK_EQUAL_C_INT(4, list_size(list2));

    list_zip_iter_init(&zip, list1, list2);
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            list_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    list_get_last(list1, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);

    list_get_last(list2, (void*) &last);
    CHECK_EQUAL_C_STRING("y", last);
};

TEST_C(ListTestsWithDefaults, ListZipIterRemove)
{
    list_add(list1, "a");
    list_add(list1, "b");
    list_add(list1, "c");
    list_add(list1, "d");

    list_add(list2, "e");
    list_add(list2, "f");
    list_add(list2, "g");

    ListZipIter zip;
    list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, list_contains(list1, "b"));
    CHECK_EQUAL_C_INT(0, list_contains(list2, "f"));
    CHECK_EQUAL_C_INT(3, list_size(list1));
    CHECK_EQUAL_C_INT(2, list_size(list2));
};


TEST_C(ListTestsWithDefaults, ListZipIterReplace)
{
    list_add(list1, "a");
    list_add(list1, "b");
    list_add(list1, "c");
    list_add(list1, "d");

    list_add(list2, "e");
    list_add(list2, "f");
    list_add(list2, "g");

    char *h = "h";
    char *i = "i";

    ListZipIter zip;
    list_zip_iter_init(&zip, list1, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    CHECK_EQUAL_C_INT(CC_OK, list_index_of(list1, "h", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(1, index);

    CHECK_EQUAL_C_INT(CC_OK, list_index_of(list2, "i", zero_if_ptr_eq, &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, list_contains(list1, "h"));
    CHECK_EQUAL_C_INT(1, list_contains(list2, "i"));
};

TEST_GROUP_C_SETUP(ListTestsListPrefilled)
{
    list_new(&list1);
    list_new(&list2);

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    list_add(list1, a);
    list_add(list1, b);
    list_add(list1, c);
    list_add(list1, d);

    a = (int*) malloc(sizeof(int));
    b = (int*) malloc(sizeof(int));
    c = (int*) malloc(sizeof(int));
    d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 8;

    list_add(list2, a);
    list_add(list2, b);
    list_add(list2, c);
    list_add(list2, d);
};

TEST_GROUP_C_TEARDOWN(ListTestsListPrefilled)
{
    list_destroy_cb(list1, free);
    list_destroy(list2);
};

TEST_C(ListTestsListPrefilled, ListAddAllAt)
{
    list_add_all_at(list1, list2, 2);
    CHECK_EQUAL_C_INT(4, list_size(list2));
    CHECK_EQUAL_C_INT(8, list_size(list1));

    int *last;
    list_get_last(list1, (void*) &last);
    int *l1i4;
    list_get_at(list1, 4, (void*) &l1i4);
    int *l2i2;
    list_get_at(list2, 2, (void*) &l2i2);
    CHECK_EQUAL_C_INT(4, *last);
    CHECK_EQUAL_C_INT(*l1i4, *l2i2);
};


TEST_C(ListTestsListPrefilled, ListIterAdd)
{
    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    ListIter iter;
    list_iter_init(&iter, list1);

    int *el;
    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            list_iter_add(&iter, ins);
    }

    CHECK_EQUAL_C_INT(5, list_size(list1));

    int *li3;
    list_get_at(list1, 3, (void*) &li3);

    CHECK_EQUAL_C_INT(*li3, *ins);

    int *li4;
    list_get_at(list1, 4, (void*) &li4);
    CHECK_EQUAL_C_INT(4, *li4);

    list_iter_init(&iter, list1);

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4) {
            list_iter_add(&iter, ins);
        }
    }

    void *e;
    list_get_last(list1, &e);
    CHECK_EQUAL_C_INT(*ins, *((int*)e));
};

TEST_C(ListTestsListPrefilled, ListIterRemove)
{
    int *rm;
    list_get_at(list1, 2, (void*) &rm);

    ListIter iter;
    list_iter_init(&iter, list1);

    int *e;
    while (list_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            list_iter_remove(&iter, NULL);
        }
    }
    CHECK_EQUAL_C_INT(3, list_size(list1));
    CHECK_EQUAL_C_INT(0, list_contains(list1, rm));
    free(rm);
};

TEST_C(ListTestsListPrefilled, ListIterDescRemove)
{
    ListIter iter;
    list_diter_init(&iter, list1);

    int *i;
    while (list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 1 || *i == 3) {
            list_diter_remove(&iter, NULL);
            free(i);
        }
    }
    CHECK_EQUAL_C_INT(2, list_size(list1));

    int *first;
    list_get_first(list1, (void*) &first);

    int *last;
    list_get_last(list1, (void*) &last);

    int *i1;
    list_get_at(list1, 1, (void*) &i1);

    CHECK_EQUAL_C_INT(2, *first);
    CHECK_EQUAL_C_INT(4, *last);
    CHECK_EQUAL_C_INT(4, *i1);
};

TEST_C(ListTestsListPrefilled, ListIterDescAdd)
{
    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    *a = 30;
    *b = 78;

    ListIter iter;
    list_diter_init(&iter, list1);

    int *i;
    while (list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 4) // add to tail
            list_diter_add(&iter, a);

        if (*i == 3)
            list_diter_add(&iter, b);
    }

    CHECK_EQUAL_C_INT(6, list_size(list1));
    int *el;
    list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(*b, *el);

    list_get_at(list1, 4, (void*) &el);
    CHECK_EQUAL_C_INT(*a, *el);
};

TEST_C(ListTestsListPrefilled, ListReverse)
{
    int *last_old;
    list_get_last(list1, (void*) &last_old);

    list_reverse(list1);

    int *el;
    list_get_at(list1, 0, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(2, *el);

    list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_POINTER(el, last_old);
};

TEST_C(ListTestsListPrefilled, ListToArray)
{
    int **array;
    list_to_array(list1, (void*)&array);

    void *e;
    list_get_at(list1, 0, &e);
    CHECK_EQUAL_C_POINTER(array[0], e);

    list_get_at(list1, 2, &e);
    CHECK_EQUAL_C_POINTER(array[2], e);

    list_get_at(list1, 3, &e);
    CHECK_EQUAL_C_POINTER(array[3], e);

    free(array);
};

TEST_C(ListTestsListPrefilled, ListSpliceAt)
{
    list_splice_at(list1, list2, 2);

    CHECK_EQUAL_C_INT(8, list_size(list1));
    CHECK_EQUAL_C_INT(0, list_size(list2));

    int *el;
    list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(4, *el);

    list_get_at(list1, 2, (void*) &el);
    CHECK_EQUAL_C_INT(5, *el);
};

TEST_C(ListTestsListPrefilled, ListSplice)
{
    list_splice(list1, list2);
    CHECK_EQUAL_C_INT(8, list_size(list1));
    CHECK_EQUAL_C_INT(0, list_size(list2));

    int *el;
    list_get_last(list1, (void*) &el);
    CHECK_EQUAL_C_INT(8, *el);

    list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(1, *el);

    list_get_at(list1, 4, (void*) &el);
    CHECK_EQUAL_C_INT(5, *el);
};

TEST_C(ListTestsListPrefilled, ListAddAll)
{
    list_add_all(list2, list1);

    CHECK_EQUAL_C_INT(8, list_size(list2));

    int *l1last;
    list_get_last(list1, (void*) &l1last);

    int *l2last;
    list_get_last(list2, (void*) &l2last);
    CHECK_EQUAL_C_INT(*l1last, *l2last);
};

TEST_C(ListTestsListPrefilled, ListRemove)
{
    int *e;
    list_get_at(list1, 1, (void*) &e);
    list_remove(list1, e, NULL);

    CHECK_EQUAL_C_INT(3, list_size(list1));
    CHECK_EQUAL_C_INT(0, list_contains(list1, e));
    free(e);
};


TEST_C(ListTestsListPrefilled, ListRemoveFirst)
{
    int *el;
    list_remove_first(list1, (void*) &el);
    free(el);

    CHECK_EQUAL_C_INT(3, list_size(list1));

    list_get_first(list1, (void*) &el);
    CHECK_EQUAL_C_INT(2, *el);
};

TEST_C(ListTestsListPrefilled, ListRemoveLast)
{
    int *el;
    list_remove_last(list1, (void*) &el);
    free(el);
    CHECK_EQUAL_C_INT(3, list_size(list1));

    CHECK_EQUAL_C_INT(CC_OK, list_get_last(list1, (void*) &el));
    CHECK_EQUAL_C_INT(3, *el);
};

TEST_C(ListTestsListPrefilled, ListRemoveAt)
{
    int *e;
    list_remove_at(list1, 2, (void*) &e);
    free(e);

    list_get_at(list1, 2, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
    CHECK_EQUAL_C_INT(3, list_size(list1));
    list_remove_at(list1, 0, (void*) &e);
    free(e);

    list_get_at(list1, 0, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};


TEST_C(ListTestsListPrefilled, ListRemoveAll)
{

    int *e;
    list_get_first(list1, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    list_remove_all_cb(list1, free);
    CHECK_EQUAL_C_INT(0, list_size(list1));

    e = NULL;
    list_get_first(list1, (void*)&e);
    CHECK_C(e == NULL);
};

TEST_C(ListTestsListPrefilled, ListGetLast)
{
    int *e;
    list_get_last(list1, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
};

TEST_C(ListTestsListPrefilled, ListGetAt)
{
    int *e;
    list_get_at(list1, 1, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};


TEST_C(ListTestsListPrefilled, ListReplaceAt)
{
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *r;
    list_replace_at(list1, replacement, 2, (void*) &r);
    free(r);

    list_get_at(list1, 2, (void*) &r);
    CHECK_C((int*)r == replacement);
};

TEST_C(ListTestsListPrefilled, ListCopyDeep)
{
    List *cp;
    list_copy_deep(list1, copy, &cp);
    CHECK_EQUAL_C_INT(4, list_size(cp));

    int *e;
    list_get_at(cp, 2, (void*) &e);

    int *le;
    list_get_at(list1, 2, (void*) &le);
    CHECK_EQUAL_C_INT(*e, *le);

    list_get_at(cp, 2, (void*) &e);
    list_get_at(list1, 2, (void*) &le);
    CHECK_C(e != le);

    list_destroy_cb(cp, free);
};


TEST_C(ListTestsListPrefilled, ListSublist)
{
    List *sub;
    list_sublist(list1, 1, 2, &sub);
    CHECK_EQUAL_C_INT(2, list_size(sub));

    int *s1;
    list_get_at(sub, 1, (void*) &s1);

    int *l2;
    list_get_at(list1, 2, (void*) &l2);
    CHECK_EQUAL_C_INT(*s1, *l2);

    list_destroy(sub);
};

TEST_C(ListTestsListPrefilled, ListAddAt)
{
    int *ins = (int*)malloc(sizeof(int));
    *ins = 90;

    list_add_at(list1, ins, 3);
    CHECK_EQUAL_C_INT(5, list_size(list1));

    int *new;
    list_get_at(list1, 3, (void*) &new);
    CHECK_EQUAL_C_INT(*ins, *new);
};

TEST_C(ListTestsListPrefilled, ListCopyShallow)
{
    List *cp;
    list_copy_shallow(list1, &cp);
    CHECK_EQUAL_C_INT(4, list_size(cp));

    void *f1;
    void *f2;
    for(int i=0; i< list_size(list1); i++) {
        list_get_at(cp, i, &f1);
        list_get_at(list1, i, &f2);
        CHECK_EQUAL_C_POINTER(f1, f2);
    }
    list_destroy(cp);
};

void test_list_destroy()
{
     // Nothing to test
}

TEST_C(ListTestsListPrefilled, ListMutFilter1)
{
    CHECK_EQUAL_C_INT(4, list_size(list1));
    list_filter_mut(list1, pred1);
    CHECK_EQUAL_C_INT(0, list_size(list1));

    void *e = NULL;
    list_get_first(list1, &e);
    CHECK_C(e == NULL);

    list_get_last(list1, &e);
    CHECK_C(e == NULL);
};

TEST_C(ListTestsListPrefilled, ListMutFilter2)
{
    CHECK_EQUAL_C_INT(4, list_size(list1));
    list_filter_mut(list1, pred2);
    CHECK_EQUAL_C_INT(1, list_size(list1));

    ListIter iter;
    int *el = NULL;
    int i = 4;
    list_iter_init(&iter, list1);
    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }
};


TEST_C(ListTestsListPrefilled, ListFilter1)
{
    List *filter = NULL;
    list_filter(list1, pred1, &filter);

    CHECK_EQUAL_C_INT(4, list_size(list1));
    CHECK_EQUAL_C_INT(0, list_size(filter));

    void *e = NULL;
    list_get_first(filter, &e);
    CHECK_C(e == NULL);

    list_get_last(filter, &e);
    CHECK_C(e == NULL);
    free(filter);
};

TEST_C(ListTestsListPrefilled, ListFilter2)
{
    List *filter = NULL;
    list_filter(list1, pred2, &filter);

    CHECK_EQUAL_C_INT(4, list_size(list1));
    CHECK_EQUAL_C_INT(1, list_size(filter));

    ListIter iter;
    int *el = NULL;
    int i = 4;
    list_iter_init(&iter, filter);
    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

    free(filter);
};
