#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cc_slist.h"
#include "CppUTest/TestHarness_c.h"

static CC_SList *list;
static CC_SList *list2;
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
    stat = cc_slist_new(&list);
    cc_slist_new(&list2);
};

TEST_GROUP_C_TEARDOWN(SlistTestsWithDefaults)
{
    cc_slist_destroy(list);
    cc_slist_destroy(list2);
};

TEST_C(SlistTestsWithDefaults, CC_SListNew)
{
    CC_SList *list2 = NULL;
    cc_slist_new(&list2);
    CHECK_C(list != NULL);
    CHECK_C(list2 != NULL);

    void *e = NULL;
    cc_slist_get_first(list, &e);
    CHECK_C(NULL == e);

    cc_slist_get_last(list, &e);
    CHECK_C(NULL == e);
    CHECK_EQUAL_C_INT(0, cc_slist_size(list));
    CHECK_C(list != list2);
};

TEST_C(SlistTestsWithDefaults, CC_SListAddLast)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    cc_slist_add(list, &a);
    cc_slist_add(list, &b);
    cc_slist_add(list, &c);
    cc_slist_add(list, &d);

    cc_slist_size(list);
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));

    int *last;
    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    cc_slist_add_last(list, &append);
    CHECK_EQUAL_C_INT(5, cc_slist_size(list));

    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(append, *last);
};

TEST_C(SlistTestsWithDefaults, CC_SListAddFirst)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    cc_slist_add(list, &a);
    cc_slist_add(list, &b);
    cc_slist_add(list, &c);
    cc_slist_add(list, &d);

    CHECK_EQUAL_C_INT(4, cc_slist_size(list));

    int *first;
    cc_slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    cc_slist_add_first(list, &prepend);

    CHECK_EQUAL_C_INT(5, cc_slist_size(list));
    cc_slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(prepend, *first);
};

TEST_C(SlistTestsWithDefaults, CC_SListAdd)
{
    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add(list, s1));
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add(list, s2));
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add(list, s3));
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add(list, s4));

    void *e;
    cc_slist_get_first(list, &e);
    CHECK_C(e != NULL);

    cc_slist_get_last(list, &e);
    CHECK_C(e != NULL);
};

TEST_GROUP_C_SETUP(SlistTestsSlistPrepopulated)
{
    cc_slist_new(&list);

    for(int i = 1; i < 5; i++) {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        cc_slist_add(list, p);
    }

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 7;

    cc_slist_new(&list2);

    cc_slist_add(list2, a);
    cc_slist_add(list2, b);
    cc_slist_add(list2, c);
    cc_slist_add(list2, d);

};

TEST_GROUP_C_TEARDOWN(SlistTestsSlistPrepopulated)
{
    cc_slist_destroy(list);
    cc_slist_destroy(list2);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListAddAt)
{
    int *ins = (int*)malloc(sizeof(int));
    *ins = 90;

    cc_slist_add_at(list, ins, 2);
    CHECK_EQUAL_C_INT(5, cc_slist_size(list));

    int *new;
    cc_slist_get_at(list, 2, (void*)&new);
    CHECK_EQUAL_C_INT(*ins, *new);
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add_at(list, ins, 4));
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_add_at(list, ins, 0));

    void *el;
    cc_slist_get_first(list, &el);
    CHECK_EQUAL_C_INT(*ins, *((int*)el));

    cc_slist_get_first(list, &el);
    CHECK_EQUAL_C_INT(*ins, *((int*)el));
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListAddAll)
{
    cc_slist_add_all(list2, list);

    CHECK_EQUAL_C_INT(8, cc_slist_size(list2));

    int *l1last;
    cc_slist_get_last(list, (void*) &l1last);

    int *l2last;
    cc_slist_get_last(list2, (void*) &l2last);
    CHECK_EQUAL_C_INT(*l1last, *l2last);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListAddAllAt)
{
    cc_slist_add_all_at(list, list2, 2);
    CHECK_EQUAL_C_INT(4, cc_slist_size(list2));
    CHECK_EQUAL_C_INT(8, cc_slist_size(list));

    int *last;
    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(4, *last);

    int *l1i5;
    cc_slist_get_at(list, 5, (void*) &l1i5);

    int *l2i2;
    cc_slist_get_at(list2, 2, (void*) &l2i2);
    CHECK_EQUAL_C_INT(*l1i5, *l2i2);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListRemove)
{
    int *e;
    CHECK_EQUAL_C_INT(CC_OK, cc_slist_get_at(list, 1, (void*) &e));

    cc_slist_remove(list, e, NULL);
    CHECK_EQUAL_C_INT(3, cc_slist_size(list));
    CHECK_EQUAL_C_INT(0, cc_slist_contains(list, e));
    free(e);
};


TEST_C(SlistTestsSlistPrepopulated, CC_SListRemoveFirst)
{
    int *first;
    cc_slist_remove_first(list, (void*) &first);
    free(first);
    CHECK_EQUAL_C_INT(3, cc_slist_size(list));

    cc_slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(2, *first);
};


TEST_C(SlistTestsSlistPrepopulated, CC_SListRemoveLast)
{
    int *last;
    cc_slist_remove_last(list, (void*) &last);
    free(last);

    CHECK_EQUAL_C_INT(3, cc_slist_size(list));

    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(3, *last);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListRemoveAt)
{
    int *e;
    cc_slist_remove_at(list, 2, (void*) &e);
    free(e);

    cc_slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);
    CHECK_EQUAL_C_INT(3, cc_slist_size(list));

    cc_slist_remove_at(list, 0, (void*) &e);
    free(e);

    cc_slist_get_at(list, 0, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListRemoveAll)
{
    cc_slist_remove_all_cb(list, free);
    CHECK_EQUAL_C_INT(0, cc_slist_size(list));

    void *e = NULL;
    cc_slist_get_first(list, &e);
    CHECK_C(e == NULL);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListGetFirst)
{
    int *first;
    cc_slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_INT(1, *first);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListGetLast)
{
    int *last;
    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_INT(4, *last);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListGet)
{
    int *e;
    cc_slist_get_at(list, 1, (void*) &e);
    CHECK_EQUAL_C_INT(2, *e);
};

TEST_C(SlistTestsWithDefaults, CC_SListIndexOf)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    cc_slist_add(list, &a);
    cc_slist_add(list, &b);
    cc_slist_add(list, &c);
    cc_slist_add(list, &d);

    size_t idx;
    cc_slist_index_of(list, &a, &idx);
    CHECK_EQUAL_C_INT(0, idx);

    cc_slist_index_of(list, &c, &idx);
    CHECK_EQUAL_C_INT(2, idx);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListReplaceAt)
{
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *e;
    cc_slist_replace_at(list, replacement, 2, (void*) &e);
    free(e);

    cc_slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_POINTER(replacement, e);
};

TEST_C(SlistTestsWithDefaults, CC_SListContains)
{
    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    cc_slist_add(list, &a);
    cc_slist_add(list, &b);
    cc_slist_add(list, &b);
    cc_slist_add(list, &c);
    cc_slist_add(list, &d);

    CHECK_EQUAL_C_INT(2, cc_slist_contains(list, &b));
    CHECK_EQUAL_C_INT(1, cc_slist_contains(list, &d));
    CHECK_EQUAL_C_INT(0, cc_slist_contains(list, &e));
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListCopyShallow)
{
    CC_SList *cp;
    cc_slist_copy_shallow(list, &cp);
    CHECK_EQUAL_C_INT(4, cc_slist_size(cp));

    void *e1;
    void *e2;

    cc_slist_get_first(cp, &e1);
    cc_slist_get_first(list, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    cc_slist_get_last(cp, &e1);
    cc_slist_get_last(list, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    cc_slist_get_at(cp, 3, &e1);
    cc_slist_get_at(list, 3, &e2);
    CHECK_EQUAL_C_POINTER(e1, e2);

    cc_slist_destroy(cp);
};

TEST_C(SlistTestsSlistPrepopulated, SlistCopyDeep)
{
    CC_SList *cp;
    cc_slist_copy_deep(list, copy, &cp);
    CHECK_EQUAL_C_INT(4, cc_slist_size(cp));

    int *e;
    cc_slist_get_at(cp, 2, (void*) &e);

    void *e2;
    cc_slist_get_at(list, 2, &e2);
    CHECK_EQUAL_C_INT(*e, *(int*)e2);

    cc_slist_get_at(list, 2, &e2);
    cc_slist_get_at(cp, 2, (void*) &e);
    CHECK_C(e != e2);

    cc_slist_destroy_cb(cp, free);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListSublist)
{
    CC_SList *sub;
    cc_slist_sublist(list, 1, 2, &sub);
    CHECK_EQUAL_C_INT(2, cc_slist_size(sub));

    int *s1;
    cc_slist_get_at(sub, 1, (void*) &s1);

    int *l2;
    cc_slist_get_at(list, 2, (void*) &l2);
    CHECK_EQUAL_C_INT(*s1, *l2);

    cc_slist_destroy(sub);
};


TEST_C(SlistTestsSlistPrepopulated, CC_SListSplice)
{
    cc_slist_splice(list, list2);
    CHECK_EQUAL_C_INT(8, cc_slist_size(list));
    CHECK_EQUAL_C_INT(0, cc_slist_size(list2));

    int *e;
    cc_slist_get_last(list, (void*) &e);
    CHECK_EQUAL_C_INT(7, *e);

    cc_slist_get_first(list, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    cc_slist_get_at(list, 4, (void*) &e);
    CHECK_EQUAL_C_INT(5, *e);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListSpliceAt)
{
    cc_slist_splice_at(list, list2, 2);
    CHECK_EQUAL_C_INT(8, cc_slist_size(list));
    CHECK_EQUAL_C_INT(0, cc_slist_size(list2));

    int *e;
    cc_slist_get_first(list, (void*) &e);
    CHECK_EQUAL_C_INT(1, *e);

    cc_slist_get_last(list, (void*) &e);
    CHECK_EQUAL_C_INT(4, *e);

    cc_slist_get_at(list, 2, (void*) &e);
    CHECK_EQUAL_C_INT(5, *e);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListToArray)
{
    int **array;
    cc_slist_to_array(list, (void*) &array);

    void *e;
    cc_slist_get_at(list, 0, &e);
    CHECK_EQUAL_C_POINTER(e, array[0]);

    cc_slist_get_at(list, 2, &e);
    CHECK_EQUAL_C_POINTER(e, array[2]);

    cc_slist_get_at(list, 3, &e);
    CHECK_EQUAL_C_POINTER(e, array[3]);
    free(array);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListIterAdd)
{
    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    CC_SListIter iter;
    cc_slist_iter_init(&iter, list);

    int *el;
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            cc_slist_iter_add(&iter, ins);
    }
    CHECK_EQUAL_C_INT(5, cc_slist_size(list));

    int *li3;
    cc_slist_get_at(list, 3, (void*) &li3);
    CHECK_EQUAL_C_INT(*li3, *ins);

    int *li4;
    cc_slist_get_at(list, 4, (void*) &li4);
    CHECK_EQUAL_C_INT(4, *li4);

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    cc_slist_iter_init(&iter, list);
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4)
            cc_slist_iter_add(&iter, ins);
    }

    void *e;
    cc_slist_get_last(list, &e);
    CHECK_EQUAL_C_INT(*ins, *(int*)e);
    CHECK_EQUAL_C_INT(6, cc_slist_size(list));
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListIterRemove)
{
    int  *rm;
    cc_slist_get_at(list, 2, (void*) &rm);

    CC_SListIter iter;
    cc_slist_iter_init(&iter, list);

    int *e;
    while (cc_slist_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            cc_slist_iter_remove(&iter, NULL);
            free(e);
        }
    }
    CHECK_EQUAL_C_INT(3, cc_slist_size(list));
    CHECK_EQUAL_C_INT(0, cc_slist_contains(list, rm));
};

TEST_C(SlistTestsWithDefaults, CC_SListZipIterNext)
{
    cc_slist_add(list, "a");
    cc_slist_add(list, "b");
    cc_slist_add(list, "c");
    cc_slist_add(list, "d");

    cc_slist_add(list2, "e");
    cc_slist_add(list2, "f");
    cc_slist_add(list2, "g");

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, list, list2);

    size_t i = 0;

    void *e1, *e2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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

TEST_C(SlistTestsWithDefaults, CC_SListZipIterAdd)
{
    cc_slist_add(list, "a");
    cc_slist_add(list, "b");
    cc_slist_add(list, "c");
    cc_slist_add(list, "d");

    cc_slist_add(list2, "e");
    cc_slist_add(list2, "f");
    cc_slist_add(list2, "g");

    char *h = "h";
    char *i = "i";

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_slist_zip_iter_add(&zip, h, i);
    }

    size_t index;
    cc_slist_index_of(list, "h", &index);
    CHECK_EQUAL_C_INT(2, index);

    cc_slist_index_of(list, "i", &index);
    CHECK_EQUAL_C_INT(2, index);

    cc_slist_index_of(list, "c", &index);
    CHECK_EQUAL_C_INT(3, index);

    CHECK_EQUAL_C_INT(1, cc_slist_contains(list, "h"));
    CHECK_EQUAL_C_INT(1, cc_slist_contains(list2, "i"));
    CHECK_EQUAL_C_INT(5, cc_slist_size(list));
    CHECK_EQUAL_C_INT(4, cc_slist_size(list2));

    cc_slist_zip_iter_init(&zip, list, list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            cc_slist_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    cc_slist_get_last(list2, (void*) &last);
    CHECK_EQUAL_C_STRING("y", last);

    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);
};

TEST_C(SlistTestsWithDefaults, CC_SListZipIterRemove)
{
    cc_slist_add(list, "a");
    cc_slist_add(list, "b");
    cc_slist_add(list, "c");
    cc_slist_add(list, "d");

    cc_slist_add(list2, "e");
    cc_slist_add(list2, "f");
    cc_slist_add(list2, "g");

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }

    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, cc_slist_contains(list, "b"));
    CHECK_EQUAL_C_INT(0, cc_slist_contains(list2, "f"));
    CHECK_EQUAL_C_INT(3, cc_slist_size(list));
    CHECK_EQUAL_C_INT(2, cc_slist_size(list2));

    cc_slist_zip_iter_init(&zip, list, list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "e") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e2, "g") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }

    char *first = "";
    char *last  = "";

    CHECK_EQUAL_C_INT(CC_ERR_VALUE_NOT_FOUND, cc_slist_get_first(list2, (void*) &first));
    CHECK_EQUAL_C_INT(CC_ERR_VALUE_NOT_FOUND, cc_slist_get_last(list2, (void*) &last));

    cc_slist_get_first(list, (void*) &first);
    CHECK_EQUAL_C_STRING("d", first);

    cc_slist_get_last(list, (void*) &last);
    CHECK_EQUAL_C_STRING("d", last);

    // consecutive removes
    cc_slist_add(list, "a");
    cc_slist_add(list, "b");
    cc_slist_add(list, "c");
    cc_slist_add(list, "d");

    cc_slist_add(list2, "a");
    cc_slist_add(list2, "b");
    cc_slist_add(list2, "c");
    cc_slist_add(list2, "d");

    cc_slist_zip_iter_init(&zip, list, list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e1, "c") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }
    // test something here
};

TEST_C(SlistTestsWithDefaults, CC_SListZipIterReplace)
{
    cc_slist_add(list, "a");
    cc_slist_add(list, "b");
    cc_slist_add(list, "c");
    cc_slist_add(list, "d");

    cc_slist_add(list2, "e");
    cc_slist_add(list2, "f");
    cc_slist_add(list2, "g");

    char *h = "h";
    char *i = "i";

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, list, list2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_slist_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    cc_slist_index_of(list, "h", &index);
    CHECK_EQUAL_C_INT(1, index);

    cc_slist_index_of(list, "i", &index);
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, cc_slist_contains(list, "h"));
    CHECK_EQUAL_C_INT(1, cc_slist_contains(list2, "i"));
};

TEST_C(SlistTestsWithDefaults, CC_SListSort)
{
    srand(time(NULL));

    int size = 1000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100000;
        cc_slist_add(list, e);
    }
    cc_slist_sort(list, cmp);

    CC_SListIter iter;
    cc_slist_iter_init(&iter, list);

    void *prev    = 0;
    void *e;

    cc_slist_iter_next(&iter, &prev);
    while (cc_slist_iter_next(&iter, &e) != CC_ITER_END) {
        CHECK_C(*(int*)prev <= *(int*)e);
        prev = e;
    }
};

TEST_C(SlistTestsWithDefaults, CC_SListReverse)
{
    int *e;
    for(int i=0;i<10;i++) {
        e = (int*)malloc(sizeof(int));
        *e = i;
        cc_slist_add(list, e);
    }
    cc_slist_reverse(list);

    CC_SListIter i;
    cc_slist_iter_init(&i, list);

    void *el;
    int next = 9;
    while (cc_slist_iter_next(&i, &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(next, *(int*)el);
        next--;
    }
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilter1)
{
    CC_SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter(list, pred1, &filter);

    CHECK_EQUAL_C_INT(0, cc_slist_size(filter));

    void *e = NULL;
    cc_slist_get_first(filter, &e);
    CHECK_C(e == NULL);
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilter2)
{
    CC_SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter(list, pred2, &filter);
    CHECK_EQUAL_C_INT(2, cc_slist_size(filter));

    CC_SListIter iter;
    int *el = NULL;
    int i = 3;
    cc_slist_iter_init(&iter, filter);
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }
};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilter3)
{
    CC_SList *filter = NULL;
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter(list, pred3, &filter);
    CHECK_EQUAL_C_INT(4, cc_slist_size(filter));

    CC_SListIter iter;
    int *el = NULL;
    int i = 1;
    cc_slist_iter_init(&iter, filter);
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilterMut1)
{
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter_mut(list, pred1);

    CHECK_EQUAL_C_INT(0, cc_slist_size(list));
    void *e = NULL;
    cc_slist_get_first(list, &e);
    CHECK_C(e == NULL);

};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilterMut2)
{
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter_mut(list, pred2);
    CHECK_EQUAL_C_INT(2, cc_slist_size(list));

    CC_SListIter iter;
    int *el = NULL;
    int i = 3;
    cc_slist_iter_init(&iter, list);
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};

TEST_C(SlistTestsSlistPrepopulated, CC_SListFilterMut3)
{
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));
    cc_slist_filter_mut(list, pred3);
    CHECK_EQUAL_C_INT(4, cc_slist_size(list));

    CC_SListIter iter;
    int *el = NULL;
    int i = 1;
    cc_slist_iter_init(&iter, list);
    while (cc_slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(i++, *el);
    }

};
