#include "munit.h"
#include "cc_slist.h"
#include <stdlib.h>


void* copy(void* e1)
{
    int* cp = (int*)malloc(sizeof(int));
    munit_assert_not_null(cp);
    *cp = *((int*)e1);
    return cp;
}

bool pred1(const void* e)
{
    return *(int*)e == 0;
}

bool pred2(const void* e)
{
    return *(int*)e >= 3;
}

bool pred3(const void* e)
{
    return *(int*)e > 0;
}

int cmp(void const* e1, void const* e2)
{
    int i = *(*((int**)e1));
    int j = *(*((int**)e2));

    if (i < j)
        return -1;
    if (i > j)
        return 1;
    return 0;
}

struct lists {
    CC_SList* list1;
    CC_SList* list2;
};

static void* default_lists(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct lists* l = malloc(sizeof(struct lists));
    munit_assert_not_null(l);

    cc_slist_new(&(l->list1));
    cc_slist_new(&(l->list2));
    return (void*)l;
}

static void default_lists_teardown(void* fixture)
{
    struct lists* l = (struct lists*)fixture;
    cc_slist_destroy(l->list1);
    cc_slist_destroy(l->list2);
    free(l);
}

static void* pre_filled_lists(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct lists* l = malloc(sizeof(struct lists));
    munit_assert_not_null(l);

    cc_slist_new(&(l->list1));
    cc_slist_new(&(l->list2));

    for (int i = 1; i < 5; i++) {
        int* p = (int*)malloc(sizeof(int));
        munit_assert_not_null(p);
        *p = i;
        cc_slist_add(l->list1, p);
    }

    int* a = (int*)malloc(sizeof(int));
    int* b = (int*)malloc(sizeof(int));
    int* c = (int*)malloc(sizeof(int));
    int* d = (int*)malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);
    munit_assert_not_null(c);
    munit_assert_not_null(d);

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 7;

    cc_slist_add(l->list2, a);
    cc_slist_add(l->list2, b);
    cc_slist_add(l->list2, c);
    cc_slist_add(l->list2, d);

    return (void*)l;
}

static void pre_filled_teardown(void* fixture)
{
    struct lists* l = (struct lists*)fixture;
    cc_slist_destroy(l->list1);
    cc_slist_destroy(l->list2);
}

/*****************************
 * TESTS
 *****************************/

static MunitResult test_new(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_SList* list = NULL;
    cc_slist_new(&list);
    munit_assert_ptr_not_null(list);

    void* e = NULL;
    cc_slist_get_first(list, &e);
    munit_assert_ptr_null(e);

    cc_slist_get_last(list, &e);
    munit_assert_ptr_null(e);
    munit_assert_size(0, == , cc_slist_size(list));

    cc_slist_destroy(list);
    
    return MUNIT_OK;
}

static MunitResult test_add_last(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct lists* l = (struct lists*)fixture;

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    cc_slist_add(l->list1, &a);
    cc_slist_add(l->list1, &b);
    cc_slist_add(l->list1, &c);
    cc_slist_add(l->list1, &d);

    munit_assert_size(4, == , cc_slist_size(l->list1));

    int* last;
    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_int(d, == , *last);

    cc_slist_add_last(l->list1, &append);
    munit_assert_size(5, == , cc_slist_size(l->list1));

    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_int(append, == , *last);

    return MUNIT_OK;
}

static MunitResult test_add_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    cc_slist_add(l->list1, &a);
    cc_slist_add(l->list1, &b);
    cc_slist_add(l->list1, &c);
    cc_slist_add(l->list1, &d);

    munit_assert_size(4, == , cc_slist_size(l->list1));

    int* first;
    cc_slist_get_first(l->list1, (void*)&first);
    munit_assert_int(a, == , *first);

    cc_slist_add_first(l->list1, &prepend);

    munit_assert_size(5, == , cc_slist_size(l->list1));
    cc_slist_get_first(l->list1, (void*)&first);

    munit_assert_int(prepend, == , *first);

    return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    char* s1 = "e1", * s2 = "e2", * s3 = "e3", * s4 = "e4";

    munit_assert_int(CC_OK, == , cc_slist_add(l->list1, s1));
    munit_assert_int(CC_OK, == , cc_slist_add(l->list1, s2));
    munit_assert_int(CC_OK, == , cc_slist_add(l->list1, s3));
    munit_assert_int(CC_OK, == , cc_slist_add(l->list1, s4));

    void* e1;
    cc_slist_get_first(l->list1, &e1);
    munit_assert_ptr_not_null(e1);
    munit_assert_ptr_equal(e1, s1);

    void* e2;
    cc_slist_get_last(l->list1, &e2);
    munit_assert_ptr_not_null(e2);
    munit_assert_ptr_equal(e2, s4);

    return MUNIT_OK;
}

static MunitResult test_add_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 90;

    munit_assert_int(CC_OK, == , cc_slist_add_at(l->list1, ins, 2));
    munit_assert_size(5, == , cc_slist_size(l->list1));

    int* newel;
    cc_slist_get_at(l->list1, 2, (void*)&newel);
    munit_assert_int(*ins, == , *newel);
    munit_assert_int(CC_OK, == , cc_slist_add_at(l->list1, ins, 4));
    munit_assert_int(CC_OK, == , cc_slist_add_at(l->list1, ins, 0));

    void* el;
    cc_slist_get_first(l->list1, &el);
    munit_assert_int(*ins, == , *((int*)el));

    return MUNIT_OK;
}

static MunitResult test_add_all(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct lists* l = (struct lists*)fixture;

    cc_slist_add_all(l->list2, l->list1);

    munit_assert_size(8, == , cc_slist_size(l->list2));

    int* l1last;
    cc_slist_get_last(l->list1, (void*)&l1last);

    int* l2last;
    cc_slist_get_last(l->list2, (void*)&l2last);
    munit_assert_int(*l1last, == , *l2last);

    return MUNIT_OK;
}

static MunitResult test_add_all_at(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct lists* l = (struct lists*)fixture;

    cc_slist_add_all_at(l->list1, l->list2, 2);
    munit_assert_size(4, == , cc_slist_size(l->list2));
    munit_assert_size(8, == , cc_slist_size(l->list1));

    int* last;
    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_int(4, == , *last);

    int* l1i5;
    cc_slist_get_at(l->list1, 5, (void*)&l1i5);

    int* l2i2;
    cc_slist_get_at(l->list2, 2, (void*)&l2i2);
    munit_assert_int(*l1i5, == , *l2i2);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    munit_assert_int(CC_OK, == , cc_slist_get_at(l->list1, 1, (void*)&e));

    cc_slist_remove(l->list1, e, NULL);
    munit_assert_size(3, == , cc_slist_size(l->list1));
    munit_assert_size(0, == , cc_slist_contains(l->list1, e));

    free(e);

    return MUNIT_OK;
}

static MunitResult test_remove_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* first = NULL;
    cc_slist_remove_first(l->list1, (void*)&first);
    free(first);
    munit_assert_size(3, == , cc_slist_size(l->list1));

    first = NULL;
    cc_slist_get_first(l->list1, (void*)&first);
    munit_assert_not_null(first);
    munit_assert_int(2, == , *first);

    return MUNIT_OK;
}

static MunitResult test_remove_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* last;
    cc_slist_remove_last(l->list1, (void*)&last);
    free(last);

    last = NULL;
    munit_assert_size(3, == , cc_slist_size(l->list1));

    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_not_null(last);
    munit_assert_int(3, == , *last);

    return MUNIT_OK;
}

static MunitResult test_remove_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    cc_slist_remove_at(l->list1, 2, (void*)&e);
    free(e);

    e = NULL;
    cc_slist_get_at(l->list1, 2, (void*)&e);
    munit_assert_not_null(e);
    munit_assert_int(4, == , *e);
    munit_assert_size(3, == , cc_slist_size(l->list1));

    cc_slist_remove_at(l->list1, 0, (void*)&e);
    free(e);

    e = NULL;
    cc_slist_get_at(l->list1, 0, (void*)&e);
    munit_assert_not_null(e);
    munit_assert_int(2, == , *e);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_slist_remove_all_cb(l->list1, free);
    munit_assert_size(0, == , cc_slist_size(l->list1));

    void* e = NULL;
    cc_slist_get_first(l->list1, &e);
    munit_assert_ptr_null(e);

    return MUNIT_OK;
}

static MunitResult test_get_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* first;
    cc_slist_get_first(l->list1, (void*)&first);
    munit_assert_int(1, == , *first);

    return MUNIT_OK;
}

static MunitResult test_get_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* last;
    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_int(4, == , *last);

    return MUNIT_OK;
}

static MunitResult test_get_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_slist_size(l->list1));

    int* e;
    munit_assert_int(CC_OK, == , cc_slist_get_at(l->list1, 1, (void*)&e));
    munit_assert_int(2, == , *e);

    return MUNIT_OK;
}

static MunitResult test_index_of(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    cc_slist_add(l->list1, &a);
    cc_slist_add(l->list1, &b);
    cc_slist_add(l->list1, &c);
    cc_slist_add(l->list1, &d);

    size_t idx;
    cc_slist_index_of(l->list1, &a, &idx);
    munit_assert_size(0, == , idx);

    cc_slist_index_of(l->list1, &c, &idx);
    munit_assert_size(2, == , idx);

    return MUNIT_OK;
}

static MunitResult test_replace_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* replacement = (int*)malloc(sizeof(int));
    munit_assert_not_null(replacement);
    *replacement = 32;

    int* e;
    cc_slist_replace_at(l->list1, replacement, 2, (void*)&e);
    free(e);

    cc_slist_get_at(l->list1, 2, (void*)&e);
    munit_assert_ptr_equal(replacement, e);

    return MUNIT_OK;
}

static MunitResult test_contains(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    cc_slist_add(l->list1, &a);
    cc_slist_add(l->list1, &b);
    cc_slist_add(l->list1, &b);
    cc_slist_add(l->list1, &c);
    cc_slist_add(l->list1, &d);

    munit_assert_size(2, == , cc_slist_contains(l->list1, &b));
    munit_assert_size(1, == , cc_slist_contains(l->list1, &d));
    munit_assert_size(0, == , cc_slist_contains(l->list1, &e));

    return MUNIT_OK;
}

static MunitResult test_copy_shallow(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* cp;
    cc_slist_copy_shallow(l->list1, &cp);
    munit_assert_size(4, == , cc_slist_size(cp));

    void* e1;
    void* e2;

    cc_slist_get_first(cp, &e1);
    cc_slist_get_first(l->list1, &e2);
    munit_assert_ptr_equal(e1, e2);

    cc_slist_get_last(cp, &e1);
    cc_slist_get_last(l->list1, &e2);
    munit_assert_ptr_equal(e1, e2);

    cc_slist_get_at(cp, 3, &e1);
    cc_slist_get_at(l->list1, 3, &e2);
    munit_assert_ptr_equal(e1, e2);

    cc_slist_destroy(cp);

    return MUNIT_OK;
}

static MunitResult test_copy_deep(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* cp;
    cc_slist_copy_deep(l->list1, copy, &cp);
    munit_assert_size(4, == , cc_slist_size(cp));

    int* e;
    cc_slist_get_at(cp, 2, (void*)&e);

    void* e2;
    cc_slist_get_at(l->list1, 2, &e2);
	munit_assert_int(*e, == , *(int*)e2);

    cc_slist_get_at(l->list1, 2, &e2);
    cc_slist_get_at(cp, 2, (void*)&e);
    munit_assert_ptr_not_equal(e, e2);

    cc_slist_destroy_cb(cp, free);

    return MUNIT_OK;
}

static MunitResult test_sublist(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* sub;
    cc_slist_sublist(l->list1, 1, 2, &sub);
    munit_assert_size(2, == , cc_slist_size(sub));

    int* s1;
    cc_slist_get_at(sub, 1, (void*)&s1);

    int* l2;
    cc_slist_get_at(l->list1, 2, (void*)&l2);
    munit_assert_int(*s1, == , *l2);

    cc_slist_destroy(sub);

    return MUNIT_OK;
}

static MunitResult test_splice(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_slist_splice(l->list1, l->list2);
    munit_assert_size(8, == , cc_slist_size(l->list1));
    munit_assert_size(0, == , cc_slist_size(l->list2));

    int* e;
    cc_slist_get_last(l->list1, (void*)&e);
    munit_assert_int(7, == , *e);

    cc_slist_get_first(l->list1, (void*)&e);
    munit_assert_int(1, == , *e);

    cc_slist_get_at(l->list1, 4, (void*)&e);
    munit_assert_int(5, == , *e);

    return MUNIT_OK;
}

static MunitResult test_splice_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_slist_splice_at(l->list1, l->list2, 2);
    munit_assert_size(8, == , cc_slist_size(l->list1));
    munit_assert_size(0, == , cc_slist_size(l->list2));

    int* e;
    cc_slist_get_first(l->list1, (void*)&e);
    munit_assert_int(1, == , *e);

    cc_slist_get_last(l->list1, (void*)&e);
    munit_assert_int(4, == , *e);

    cc_slist_get_at(l->list1, 2, (void*)&e);
    munit_assert_int(5, == , *e);

    return MUNIT_OK;
}

static MunitResult test_to_array(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int** array;
    cc_slist_to_array(l->list1, (void*)&array);

    void* e;
    cc_slist_get_at(l->list1, 0, &e);
    munit_assert_ptr_equal(e, array[0]);

    cc_slist_get_at(l->list1, 2, &e);
    munit_assert_ptr_equal(e, array[2]);

    cc_slist_get_at(l->list1, 3, &e);
    munit_assert_ptr_equal(e, array[3]);
    free(array);

    return MUNIT_OK;
}

static MunitResult test_iter_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 32;

    CC_SListIter iter;
    cc_slist_iter_init(&iter, l->list1);

    int* el;
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        if (*el == 3)
            cc_slist_iter_add(&iter, ins);
    }
    munit_assert_size(5, == , cc_slist_size(l->list1));

    int* li3;
    cc_slist_get_at(l->list1, 3, (void*)&li3);
    munit_assert_int(*li3, == , *ins);

    int* li4;
    cc_slist_get_at(l->list1, 4, (void*)&li4);
    munit_assert_int(4, == , *li4);

    ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 32;

    cc_slist_iter_init(&iter, l->list1);
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        if (*el == 4)
            cc_slist_iter_add(&iter, ins);
    }

    void* e;
    cc_slist_get_last(l->list1, &e);
    munit_assert_int(*ins, == , *(int*)e);
    munit_assert_size(6, == , cc_slist_size(l->list1));

    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* rm;
    cc_slist_get_at(l->list1, 2, (void*)&rm);

    CC_SListIter iter;
    cc_slist_iter_init(&iter, l->list1);

    int* e = NULL;
    while (cc_slist_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        munit_assert_not_null(e);
        if (*e == 3) {
            cc_slist_iter_remove(&iter, NULL);
            free(e);
        }
    }
    munit_assert_size(3, == , cc_slist_size(l->list1));
    munit_assert_size(0, == , cc_slist_contains(l->list1, rm));

    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_slist_add(l->list1, "a");
    cc_slist_add(l->list1, "b");
    cc_slist_add(l->list1, "c");
    cc_slist_add(l->list1, "d");

    cc_slist_add(l->list2, "e");
    cc_slist_add(l->list2, "f");
    cc_slist_add(l->list2, "g");

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, l->list1, l->list2);

    size_t i = 0;

    void* e1, * e2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            munit_assert_string_equal("a", (char*)e1);
            munit_assert_string_equal("e", (char*)e2);
        }
        if (i == 2) {
            munit_assert_string_equal("c", (char*)e1);
            munit_assert_string_equal("g", (char*)e2);
        }
        i++;
    }
    munit_assert_size(3, == , i);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    char* a = "a", * b = "b", * c = "c",
        * d = "d", * e = "e", * f = "f",
        * g = "g";

    cc_slist_add(l->list1, a);
    cc_slist_add(l->list1, b);
    cc_slist_add(l->list1, c);
    cc_slist_add(l->list1, d);

    cc_slist_add(l->list2, e);
    cc_slist_add(l->list2, f);
    cc_slist_add(l->list2, g);

    char* h = "h";
    char* i = "i";

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, l->list1, l->list2);

    void* e1, * e2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_slist_zip_iter_add(&zip, h, i);
    }

    size_t index;
    cc_slist_index_of(l->list1, h, &index);
    munit_assert_size(2, == , index);

    cc_slist_index_of(l->list1, i, &index);
    munit_assert_size(2, == , index);

    cc_slist_index_of(l->list1, c, &index);
    munit_assert_size(3, == , index);

    munit_assert_size(1, == , cc_slist_contains(l->list1, h));
    munit_assert_size(1, == , cc_slist_contains(l->list2, i));
    munit_assert_size(5, == , cc_slist_size(l->list1));
    munit_assert_size(4, == , cc_slist_size(l->list2));

    char* x = "x", *y = "y";

    cc_slist_zip_iter_init(&zip, l->list1, l->list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e2, "g") == 0)
            cc_slist_zip_iter_add(&zip, x, y);
    }

    char* last;
    cc_slist_get_last(l->list2, (void*)&last);
    munit_assert_string_equal("y", last);

    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_string_equal("d", last);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    char* a = "a", * b = "b", * c = "c",
        * d = "d", * e = "e", * f = "f",
        * g = "g";

    cc_slist_add(l->list1, a);
    cc_slist_add(l->list1, b);
    cc_slist_add(l->list1, c);
    cc_slist_add(l->list1, d);

    cc_slist_add(l->list2, e);
    cc_slist_add(l->list2, f);
    cc_slist_add(l->list2, g);

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, l->list1, l->list2);

    void* e1 = NULL;
    void* e2 = NULL;
    void* r1 = NULL;
    void* r2 = NULL;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }

    munit_assert_not_null(r1);
    munit_assert_not_null(r2);
    munit_assert_string_equal(b, (char*)r1);
    munit_assert_string_equal(f, (char*)r2);

    munit_assert_size(0, ==, cc_slist_contains(l->list1, b));
    munit_assert_size(0, == , cc_slist_contains(l->list2, f));

    munit_assert_size(3, == , cc_slist_size(l->list1));
    munit_assert_size(2, == , cc_slist_size(l->list2));

    cc_slist_zip_iter_init(&zip, l->list1, l->list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e2, "e") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*)e2, "g") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }

    char* first = "";
    char* last = "";

	munit_assert_int(CC_ERR_VALUE_NOT_FOUND, == , cc_slist_get_first(l->list2, (void*)&first));
	munit_assert_int(CC_ERR_VALUE_NOT_FOUND, == , cc_slist_get_last(l->list2, (void*)&last));

    cc_slist_get_first(l->list1, (void*)&first);
    munit_assert_string_equal(d, first);

    cc_slist_get_last(l->list1, (void*)&last);
    munit_assert_string_equal(d, last);

    // consecutive removes
    cc_slist_add(l->list1, a);
    cc_slist_add(l->list1, b);
    cc_slist_add(l->list1, c);
    cc_slist_add(l->list1, d);

    cc_slist_add(l->list2, a);
    cc_slist_add(l->list2, b);
    cc_slist_add(l->list2, c);
    cc_slist_add(l->list2, d);

    cc_slist_zip_iter_init(&zip, l->list1, l->list2);
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*)e1, "c") == 0)
            cc_slist_zip_iter_remove(&zip, &r1, &r2);
    }

    return MUNIT_OK;
}

static MunitResult test_zip_iter_replace(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    char* a = "a", * b = "b", * c = "c",
        * d = "d", * e = "e", * f = "f",
        * g = "g";

    cc_slist_add(l->list1, a);
    cc_slist_add(l->list1, b);
    cc_slist_add(l->list1, c);
    cc_slist_add(l->list1, d);

    cc_slist_add(l->list2, e);
    cc_slist_add(l->list2, f);
    cc_slist_add(l->list2, g);

    char* h = "h";
    char* i = "i";

    CC_SListZipIter zip;
    cc_slist_zip_iter_init(&zip, l->list1, l->list2);

    void* e1, * e2;
    void* r1, * r2;
    while (cc_slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_slist_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    cc_slist_index_of(l->list1, h, &index);
    munit_assert_size(1, == , index);

    cc_slist_index_of(l->list1, i, &index);
    munit_assert_size(1, == , index);
    munit_assert_size(1, == , cc_slist_contains(l->list1, h));
    munit_assert_size(1, == , cc_slist_contains(l->list2, i));

    return MUNIT_OK;
}

static MunitResult test_sort(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int size = 1000;
    int i;
    for (i = 0; i < size; i++) {
        int* e = malloc(sizeof(int));
        munit_assert_not_null(e);
        *e = munit_rand_int_range(0, 10000001);
        cc_slist_add(l->list1, e);
    }
    cc_slist_sort(l->list1, cmp);

    CC_SListIter iter;
    cc_slist_iter_init(&iter, l->list1);

    void* prev = 0;
    void* e;

    cc_slist_iter_next(&iter, &prev);
    while (cc_slist_iter_next(&iter, &e) != CC_ITER_END) {
        munit_assert_int(*(int*)prev, <= , *(int*)e);
        prev = e;
    }

    return MUNIT_OK;
}

static MunitResult test_reverse(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    for (int i = 0; i < 10; i++) {
        e = (int*)malloc(sizeof(int));
        munit_assert_not_null(e);
        *e = i;
        cc_slist_add(l->list1, e);
    }
    cc_slist_reverse(l->list1);

    CC_SListIter i;
    cc_slist_iter_init(&i, l->list1);

    void* el;
    int next = 9;
    while (cc_slist_iter_next(&i, &el) != CC_ITER_END) {
        munit_assert_int(next, == , (*(int*)el));
        next--;
    }

    return MUNIT_OK;
}

static MunitResult test_filter1(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* filter = NULL;
    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter(l->list1, pred1, &filter);

    munit_assert_size(0, == , cc_slist_size(filter));

    void* e = NULL;
    cc_slist_get_first(filter, &e);
    munit_assert_ptr_null(e);

    return MUNIT_OK;
}

static MunitResult test_filter2(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* filter = NULL;
    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter(l->list1, pred2, &filter);
    munit_assert_size(2, == , cc_slist_size(filter));

    CC_SListIter iter;
    int* el = NULL;
    int i = 3;
    cc_slist_iter_init(&iter, filter);
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    return MUNIT_OK;
}

static MunitResult test_filter3(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_SList* filter = NULL;
    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter(l->list1, pred3, &filter);
    munit_assert_size(4, == , cc_slist_size(filter));

    CC_SListIter iter;
    int* el = NULL;
    int i = 1;
    cc_slist_iter_init(&iter, filter);
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    return MUNIT_OK;
}

static MunitResult test_filter_mut1(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter_mut(l->list1, pred1);

    munit_assert_size(0, == , cc_slist_size(l->list1));
    void* e = NULL;
    cc_slist_get_first(l->list1, &e);
    munit_assert_ptr_null(e);

    return MUNIT_OK;
}

static MunitResult test_filter_mut2(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter_mut(l->list1, pred2);
    munit_assert_size(2, == , cc_slist_size(l->list1));

    CC_SListIter iter;
    int* el = NULL;
    int i = 3;
    cc_slist_iter_init(&iter, l->list1);
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    return MUNIT_OK;
}

static MunitResult test_filter_mut3(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_slist_size(l->list1));
    cc_slist_filter_mut(l->list1, pred3);
    munit_assert_size(4, == , cc_slist_size(l->list1));

    CC_SListIter iter;
    int* el = NULL;
    int i = 1;
    cc_slist_iter_init(&iter, l->list1);
    while (cc_slist_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
	{(char*)"/list/test_new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_last", test_add_last, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_first", test_add_first, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add", test_add, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_at", test_add_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_all", test_add_all, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_all_at", test_add_all_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove", test_remove, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_first", test_remove_first, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_last", test_remove_last, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_at", test_remove_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_all", test_remove_all, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_get_first", test_get_first, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_get_last", test_get_last, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_get_at", test_get_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_index_of", test_index_of, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_replace_at", test_replace_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_contains", test_contains, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_copy_shallow", test_copy_shallow, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_copy_deep", test_copy_deep, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_sublist", test_sublist, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_splice", test_splice, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_splice_at", test_splice_at, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_to_array", test_to_array, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_add", test_iter_add, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_remove", test_iter_remove, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_iter_next", test_zip_iter_next, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_add", test_zip_iter_add, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_remove", test_zip_iter_remove, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_replace", test_zip_iter_replace, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_sort", test_sort, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_reverse", test_reverse, default_lists, default_lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter1", test_filter1, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter2", test_filter2, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter3", test_filter3, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter_mut1", test_filter_mut1, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter_mut2", test_filter_mut2, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter_mut3", test_filter_mut3, pre_filled_lists, pre_filled_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};


static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};


int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}