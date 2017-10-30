#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "slist.h"
#include "CppUTest/TestHarness_c.h"

static SList *list;
static SList *list2;
static int stat;

void *copy(void *e1)
{
    int *cp = (int *) malloc(sizeof(int));
    *cp = *((int*) e1);
    return cp;
}

void p(void *e)
{
    int *i = e;
    printf("%d ", *i);
}

bool pred1(const void *e)
{
    return *(int*)e == 0;
}

bool pred2(const void *e)
{
    return *(int*)e >= 3;
}

bool pred3(const void *e)
{
    return *(int*)e > 0;
}

int cmp(void const *e1, void const *e2)
{
    int i = *(*((int**) e1));
    int j = *(*((int**) e2));

    if (i < j)
        return -1;
    if (i > j)
        return 1;
    return 0;
}

TEST_GROUP_C_SETUP(SlistTestsWithDefaults)
{
    stat = slist_new(&list);
    slist_new(&list2);
};

TEST_GROUP_C_TEARDOWN(SlistTestsWithDefaults)
{
    slist_destroy(list);
    slist_destroy(list2);
};

TEST_C(SlistTestsWithDefaults, SListNew)
{
    SList *list2 = NULL;
    slist_new(&list2);
    CHECK_C(list != NULL);
    CHECK_C(list2 != NULL);

    void *e = NULL;
    slist_get_first(list, &e);
    CHECK_C(NULL == e);

    slist_get_last(list, &e);
    CHECK_C(NULL == e);
    CHECK_EQUAL_C_INT(0, slist_size(list));
    CHECK_C(list != list2);
};

TEST_C(SlistTestsWithDefaults, SListAddLast)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    slist_size(list);
    CHECK_EQUAL_C_INT(4, slist_size(list));

    int *last;
    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    slist_add_last(list, &append);
    CHECK_EQUAL_C_INT(5, slist_size(list));

    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(append, *last);
};

TEST_C(SlistTestsWithDefaults, SListAddFirst)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    CHECK_EQUAL_C_INT(4, slist_size(list));

    int *first;
    slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    slist_add_first(list, &prepend);

    CHECK_EQUAL_C_INT(5, slist_size(list));
    slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(prepend, *first);
};

TEST_C(SlistTestsWithDefaults, SListAdd)
{
    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    CHECK_EQUAL_C_INT(CC_OK, slist_add(list, s1));
    CHECK_EQUAL_C_INT(CC_OK, slist_add(list, s2));
    CHECK_EQUAL_C_INT(CC_OK, slist_add(list, s3));
    CHECK_EQUAL_C_INT(CC_OK, slist_add(list, s4));

    void *e;
    slist_get_first(list, &e);
    CHECK_C(e != NULL);

    slist_get_last(list, &e);
    CHECK_C(e != NULL);
};

TEST_GROUP_C_SETUP(SlistTestsSlistPrepopulated)
{
    slist_new(&list);

    for(int i = 1; i < 5; i++) {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        slist_add(list, p);
    }

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 7;

    slist_new(&list2);

    slist_add(list2, a);
    slist_add(list2, b);
    slist_add(list2, c);
    slist_add(list2, d);

};

TEST_GROUP_C_TEARDOWN(SlistTestsSlistPrepopulated)
{
    slist_destroy(list);
    slist_destroy(list2);
};

TEST_C(SlistTestsSlistPrepopulated, SListAddAt)
{
    int *ins = (int*)malloc(sizeof(int));
    *ins = 90;

    slist_add_at(list, ins, 2);
    CHECK_EQUAL_C_INT(5, slist_size(list));

    int *new;
    slist_get_at(list, 2, (void*)&new);
    CHECK_EQUAL_C_INT(*ins, *new);
    CHECK_EQUAL_C_INT(CC_OK, slist_add_at(list, ins, 4));
    CHECK_EQUAL_C_INT(CC_OK, slist_add_at(list, ins, 0));

    void *el;
    slist_get_first(list, &el);
    CHECK_EQUAL_C_INT(*ins, *((int*)el));

    slist_get_first(list, &el);
    CHECK_EQUAL_C_INT(*ins, *((int*)el));
};

TEST_C(SlistTestsSlistPrepopulated, SListAddAll)
{
    slist_add_all(list2, list);

    CHECK_EQUAL_C_INT(8, slist_size(list2));

    int *l1last;
    slist_get_last(list, (void*) &l1last);

    int *l2last;
    slist_get_last(list2, (void*) &l2last);
    CHECK_EQUAL_C_INT(*l1last, *l2last);
};

TEST_C(SlistTestsSlistPrepopulated, SListAddAllAt)
{
    slist_add_all_at(list, list2, 2);
    CHECK_EQUAL_C_INT(4, slist_size(list2));
    CHECK_EQUAL_C_INT(8, slist_size(list));

    int *last;
    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(4, *last);

    int *l1i5;
    slist_get_at(list, 5, (void*) &l1i5);

    int *l2i2;
    slist_get_at(list2, 2, (void*) &l2i2);
    CHECK_EQUAL_C_INT(*l1i5, *l2i2);
};

TEST_C(SlistTestsSlistPrepopulated, SListRemove)
{
    int *e;
    CHECK_EQUAL_C_INT(CC_OK, slist_get_at(list, 1, (void*) &e));

    slist_remove(list, e, NULL);
    CHECK_EQUAL_C_INT(3, slist_size(list));
    CHECK_EQUAL_C_INT(0, slist_contains(list, e));
    free(e);
};


TEST_C(SlistTestsSlistPrepopulated, SListRemoveFirst)
{
    int *first;
    slist_remove_first(list, (void*) &first);
    free(first);
    CHECK_EQUAL_C_INT(3, slist_size(list));

    slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(2, *first);
};


TEST_C(SlistTestsSlistPrepopulated, SListRemoveLast)
{
    int *last;
    slist_remove_last(list, (void*) &last);
    free(last);

    CHECK_EQUAL_C_INT(3, slist_size(list));

    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(3, *last);
};

TEST_C(SlistTestsSlistPrepopulated, SListRemoveAt)
{
    int *e;
    slist_remove_at(list, 2, (void*) &e);
    free(e);

    slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
    CHECK_EQUAL_C_INT(3, slist_size(list));

    slist_remove_at(list, 0, (void*) &e);
    free(e);

    slist_get_at(list, 0, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};

TEST_C(SlistTestsSlistPrepopulated, SListRemoveAll)
{
    slist_remove_all_cb(list, free);
    CHECK_EQUAL_C_INT(0, slist_size(list));

    void *e = NULL;
    slist_get_first(list, &e);
    CHECK_C(e == NULL);
};

TEST_C(SlistTestsSlistPrepopulated, SListGetFirst)
{
    int *first;
    slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(1, *first);
};

TEST_C(SlistTestsSlistPrepopulated, SListGetLast)
{
    int *last;
    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(4, *last);
};

TEST_C(SlistTestsSlistPrepopulated, SListGet)
{
    int *e;
    slist_get_at(list, 1, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};

TEST_C(SlistTestsWithDefaults, SListIndexOf)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    size_t idx;
    slist_index_of(list, &a, &idx);
    CHECK_EQUAL_C_INT(0, idx);

    slist_index_of(list, &c, &idx);
    CHECK_EQUAL_C_INT(2, idx);
};

TEST_C(SlistTestsSlistPrepopulated, SListReplaceAt)
{
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *e;
    slist_replace_at(list, replacement, 2, (void*) &e);
    free(e);

    slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_POINTER(replacement, e);
};

TEST_C(SlistTestsWithDefaults, SListContains)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    CHECK_EQUAL_C_INT(2, slist_contains(list, &b));
    CHECK_EQUAL_C_INT(1, slist_contains(list, &d));
    CHECK_EQUAL_C_INT(0, slist_contains(list, &e));
};

TEST_C(SlistTestsSlistPrepopulated, SListCopyShallow)
{
    SList *cp;
    slist_copy_shallow(list, &cp);
    CHECK_EQUAL_C_INT(4, slist_size(cp));

    void *e1;
    void *e2;

    slist_get_first(cp, &e1);
    slist_get_first(list, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    slist_get_last(cp, &e1);
    slist_get_last(list, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    slist_get_at(cp, 3, &e1);
    slist_get_at(list, 3, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    slist_destroy(cp);
};

TEST_C(SlistTestsSlistPrepopulated, SlistCopyDeep)
{
    SList *cp;
    slist_copy_deep(list, copy, &cp);
    CHECK_EQUAL_C_INT(4, slist_size(cp));

    int *e;
    slist_get_at(cp, 2, (void*) &e);

    void *e2;
    slist_get_at(list, 2, &e2);
    CHECK_EQUAL_C_INT(*e, *(int*)e2);

    slist_get_at(list, 2, &e2);
    slist_get_at(cp, 2, (void*) &e);
    CHECK_C(e != e2);

    slist_destroy_cb(cp, free);
};

TEST_C(SlistTestsSlistPrepopulated, SListSublist)
{
    SList *sub;
    slist_sublist(list, 1, 2, &sub);
    CHECK_EQUAL_C_INT(2, slist_size(sub));

    int *s1;
    slist_get_at(sub, 1, (void*) &s1);

    int *l2;
    slist_get_at(list, 2, (void*) &l2);
    CHECK_EQUAL_C_INT(*s1, *l2);

    slist_destroy(sub);
};


TEST_C(SlistTestsSlistPrepopulated, SListSplice)
{
    slist_splice(list, list2);
    CHECK_EQUAL_C_INT(8, slist_size(list));
    CHECK_EQUAL_C_INT(0, slist_size(list2));

    int *e;
    slist_get_last(list, (void*) &e);
    CHECK_EQUAL_C_INT(7, *e);

    slist_get_first(list, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    slist_get_at(list, 4, (void*) &e);
    CHECK_EQUAL_C_INT(5, *e);
};

TEST_C(SlistTestsSlistPrepopulated, SListSpliceAt)
{
    slist_splice_at(list, list2, 2);
    CHECK_EQUAL_C_INT(8, slist_size(list));
    CHECK_EQUAL_C_INT(0, slist_size(list2));

    int *e;
    slist_get_first(list, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    slist_get_last(list, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);

    slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_INT(5, *e);
};

TEST_C(SlistTestsSlistPrepopulated, SListToArray)
{
    int **array;
    slist_to_array(list, (void*) &array);

    void *e;
    slist_get_at(list, 0, &e);
    CHECK_EQUAL_C_POINTER(e, array[0]);

    slist_get_at(list, 2, &e);
    CHECK_EQUAL_C_POINTER(e, array[2]);

    slist_get_at(list, 3, &e);
    CHECK_EQUAL_C_POINTER(e, array[3]);
    free(array);
};

TEST_C(SlistTestsSlistPrepopulated, SListIterAdd)
{
    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    SListIter iter;
    slist_iter_init(&iter, list);

    int *el;
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            slist_iter_add(&iter, ins);
    }
    CHECK_EQUAL_C_INT(5, slist_size(list));

    int *li3;
    slist_get_at(list, 3, (void*) &li3);
    CHECK_EQUAL_C_INT(*li3, *ins);

    int *li4;
    slist_get_at(list, 4, (void*) &li4);
    CHECK_EQUAL_C_INT(4, *li4);

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    slist_iter_init(&iter, list);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4)
            slist_iter_add(&iter, ins);
    }

    void *e;
    slist_get_last(list, &e);
    CHECK_EQUAL_C_INT(*ins, *(int*)e);
    CHECK_EQUAL_C_INT(6, slist_size(list));
};

TEST_C(SlistTestsSlistPrepopulated, SListIterRemove)
{
    int  *rm;
    slist_get_at(list, 2, (void*) &rm);

    SListIter iter;
    slist_iter_init(&iter, list);

    int *e;
    while (slist_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            slist_iter_remove(&iter, NULL);
            free(e);
        }
    }
    CHECK_EQUAL_C_INT(3, slist_size(list));
    CHECK_EQUAL_C_INT(0, slist_contains(list, rm));
};

TEST_C(SlistTestsWithDefaults, SListZipIterNext)
{
    slist_add(list, "a");
    slist_add(list, "b");
    slist_add(list, "c");
    slist_add(list, "d");

    slist_add(list2, "e");
    slist_add(list2, "f");
    slist_add(list2, "g");

    SListZipIter zip;
    slist_zip_iter_init(&zip, list, list2);

    size_t i = 0;

    void *e1, *e2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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

TEST_C(SlistTestsWithDefaults, SListZipIterAdd)
{
    slist_add(list, "a");
    slist_add(list, "b");
    slist_add(list, "c");
    slist_add(list, "d");

    slist_add(list2, "e");
    slist_add(list2, "f");
    slist_add(list2, "g");

    char *h = "h";
    char *i = "i";

    SListZipIter zip;
    slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_add(&zip, h, i);
    }

    size_t index;
    slist_index_of(list, "h", &index);
    CHECK_EQUAL_C_INT(2, index);

    slist_index_of(list, "i", &index);
    CHECK_EQUAL_C_INT(2, index);

    slist_index_of(list, "c", &index);
    CHECK_EQUAL_C_INT(3, index);

    CHECK_EQUAL_C_INT(1, slist_contains(list, "h"));
    CHECK_EQUAL_C_INT(1, slist_contains(list2, "i"));
    CHECK_EQUAL_C_INT(5, slist_size(list));
    CHECK_EQUAL_C_INT(4, slist_size(list2));

    slist_zip_iter_init(&zip, list, list2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            slist_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    slist_get_last(list2, (void*) &last);
    CHECK_EQUAL_C_STRING("y", last);

    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);
};

TEST_C(SlistTestsWithDefaults, SListZipIterRemove)
{
    slist_add(list, "a");
    slist_add(list, "b");
    slist_add(list, "c");
    slist_add(list, "d");

    slist_add(list2, "e");
    slist_add(list2, "f");
    slist_add(list2, "g");

    SListZipIter zip;
    slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }

    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, slist_contains(list, "b"));
    CHECK_EQUAL_C_INT(0, slist_contains(list2, "f"));
    CHECK_EQUAL_C_INT(3, slist_size(list));
    CHECK_EQUAL_C_INT(2, slist_size(list2));

    slist_zip_iter_init(&zip, list, list2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "e") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e2, "g") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }

    char *first = "";
    char *last  = "";

    CHECK_EQUAL_C_INT(CC_ERR_VALUE_NOT_FOUND, slist_get_first(list2, (void*) &first));
    CHECK_EQUAL_C_INT(CC_ERR_VALUE_NOT_FOUND, slist_get_last(list2, (void*) &last));

    slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_STRING("d", first);

    slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);

    // consecutive removes
    slist_add(list, "a");
    slist_add(list, "b");
    slist_add(list, "c");
    slist_add(list, "d");

    slist_add(list2, "a");
    slist_add(list2, "b");
    slist_add(list2, "c");
    slist_add(list2, "d");

    slist_zip_iter_init(&zip, list, list2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e1, "c") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }
    // test something here
};

TEST_C(SlistTestsWithDefaults, SListZipIterReplace)
{
    slist_add(list, "a");
    slist_add(list, "b");
    slist_add(list, "c");
    slist_add(list, "d");

    slist_add(list2, "e");
    slist_add(list2, "f");
    slist_add(list2, "g");

    char *h = "h";
    char *i = "i";

    SListZipIter zip;
    slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    slist_index_of(list, "h", &index);
    CHECK_EQUAL_C_INT(1, index);

    slist_index_of(list, "i", &index);
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, slist_contains(list, "h"));
    CHECK_EQUAL_C_INT(1, slist_contains(list2, "i"));
};

TEST_C(SlistTestsWithDefaults, SListSort)
{
    srand(time(NULL));

    int size = 1000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100000;
        slist_add(list, e);
    }
    slist_sort(list, cmp);

    SListIter iter;
    slist_iter_init(&iter, list);

    void *prev    = 0;
    void *e;

    slist_iter_next(&iter, &prev);
    while (slist_iter_next(&iter, &e) != CC_ITER_END) {
        CHECK_C(*(int*)prev <= *(int*)e);
        prev = e;
    }
};

TEST_C(SlistTestsWithDefaults, SListReverse)
{
    int *e;
    for(int i=0;i<10;i++) {
        e = (int*)malloc(sizeof(int));
        *e = i;
        slist_add(list, e);
    }
    slist_reverse(list);

    SListIter i;
    slist_iter_init(&i, list);

    void *el;
    int next = 9;
    while (slist_iter_next(&i, &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(next, *(int*)el);
        next--;
    }
};

TEST_C(SlistTestsSlistPrepopulated, SListFilter1)
{
    SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter(list, pred1, &filter);

    CHECK_EQUAL_C_INT(0, slist_size(filter));

    void *e = NULL;
    slist_get_first(filter, &e);
    CHECK_C(e == NULL);
};

TEST_C(SlistTestsSlistPrepopulated, SListFilter2)
{
    SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter(list, pred2, &filter);
    CHECK_EQUAL_C_INT(2, slist_size(filter));

    SListIter iter;
    int *el = NULL;
    int i = 3;
    slist_iter_init(&iter, filter);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }
};

TEST_C(SlistTestsSlistPrepopulated, SListFilter3)
{
    SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter(list, pred3, &filter);
    CHECK_EQUAL_C_INT(4, slist_size(filter));

    SListIter iter;
    int *el = NULL;
    int i = 1;
    slist_iter_init(&iter, filter);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};

TEST_C(SlistTestsSlistPrepopulated, SListFilterMut1)
{
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter_mut(list, pred1);

    CHECK_EQUAL_C_INT(0, slist_size(list));
    void *e = NULL;
    slist_get_first(list, &e);
    CHECK_C(e == NULL);

};

TEST_C(SlistTestsSlistPrepopulated, SListFilterMut2)
{
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter_mut(list, pred2);
    CHECK_EQUAL_C_INT(2, slist_size(list));

    SListIter iter;
    int *el = NULL;
    int i = 3;
    slist_iter_init(&iter, list);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};

TEST_C(SlistTestsSlistPrepopulated, SListFilterMut3)
{
    CHECK_EQUAL_C_INT(4, slist_size(list));
    slist_filter_mut(list, pred3);
    CHECK_EQUAL_C_INT(4, slist_size(list));

    SListIter iter;
    int *el = NULL;
    int i = 1;
    slist_iter_init(&iter, list);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};
