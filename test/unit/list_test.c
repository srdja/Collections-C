#include "munit.h"
#include "cc_list.h"
#include <stdlib.h>
#include <stdbool.h>


int cmp(void const* e1, void const* e2)
{
    int i = *((int*)e1); 
    int j = *((int*)e2);

    if (i < j)
        return -1;
    if (i == j)
        return 0;
    return 1;
}

int zero_if_ptr_eq(void const* e1, void const* e2)
{
    return !(e1 == e2);
}

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
    return *(int*)e > 3;
}

void sum_reduce(void* e1, void* e2, void* res)
{
    int i = *(int*)e1;
    int j = *(int*)e2;

    *(int*)res = i + j;
}

void bool_and(void* e1, void* e2, void* res)
{
    bool p = *(bool*)e1;
    bool q = *(bool*)e2;

    *(bool*)res = p && q;
}

/*****************************
 * TESTS
 *****************************/
static MunitResult test_new(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    CC_List* list2;

    cc_list_new(&list1);
    cc_list_new(&list2);

    munit_assert_ptr_not_null(list1);
    munit_assert_ptr_not_null(list2);

    void* e = NULL;
    cc_list_get_first(list1, &e);
    munit_assert_ptr_null(e);

    cc_list_get_last(list1, &e);
    munit_assert_ptr_null(e);

    munit_assert_size(0, == , cc_list_size(list1));
    munit_assert_ptr_not_equal(list1, list2);

    cc_list_destroy(list1);
    cc_list_destroy(list2);

    return MUNIT_OK;
}

static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    char* s1 = "e1", * s2 = "e2", * s3 = "e3", * s4 = "e4";

    munit_assert_int(CC_OK, == , cc_list_add(list1, s1));
    munit_assert_int(CC_OK, == , cc_list_add(list1, s2));
    munit_assert_int(CC_OK, == , cc_list_add(list1, s3));
    munit_assert_int(CC_OK, == , cc_list_add(list1, s4));

    void* e;
    cc_list_get_first(list1, &e);
    munit_assert_ptr_not_null(e);

    cc_list_get_last(list1, &e);
    munit_assert_ptr_not_null(e);

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_add_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    munit_assert_size(4, ==, cc_list_size(list1));

    int* last;
    cc_list_get_last(list1, (void*)&last);
    munit_assert_int(d, == , *last);

    cc_list_add_last(list1, &append);
    munit_assert_size(5, == , cc_list_size(list1));

    cc_list_get_last(list1, (void*)&last);
    munit_assert_int(append, == , *last);

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_add_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    cc_list_add(list1, &a);
    cc_list_add(list1, &b);
    cc_list_add(list1, &c);
    cc_list_add(list1, &d);

    munit_assert_size(4, == , cc_list_size(list1));

    int* first;
    cc_list_get_first(list1, (void*)&first);
    munit_assert_int(a, == , *first);

    cc_list_add_first(list1, &prepend);
    munit_assert_size(5, == , cc_list_size(list1));

    cc_list_get_first(list1, (void*)&first);
    munit_assert_int(prepend, == , *first);

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_contains(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

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

    munit_assert_size(2, == , cc_list_contains(list1, &b));
    munit_assert_size(1, == , cc_list_contains(list1, &d));
    munit_assert_size(0, == , cc_list_contains(list1, &e));

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_index_of(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

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
    munit_assert_size(0, == , idx);

    cc_list_index_of(list1, &c, zero_if_ptr_eq, &idx);
    munit_assert_size(2, == , idx);

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_sort(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    /* populate the list with random data */
    int size = 10000;
    int i;
    for (i = 0; i < size; i++) {
        int* e = malloc(sizeof(int));
        munit_assert_not_null(e);
        *e = munit_rand_int_range(0, 1000001);
        cc_list_add(list1, e);
    }

    cc_list_sort_in_place(list1, cmp);

    CC_ListIter iter;
    cc_list_iter_init(&iter, list1);

    void* prev;
    void* e;
    cc_list_iter_next(&iter, &prev);
    while (cc_list_iter_next(&iter, &e) != CC_ITER_END) {
        munit_assert_int(*((int*)prev), <= , *((int*)e));
        prev = e;
    }

    cc_list_destroy(list1);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    CC_List* list2;
    cc_list_new(&list2);

    char* a = "a";
    char* e = "e";
    char* c = "c";
    char* g = "g";

    cc_list_add(list1, a);
    cc_list_add(list1, "b");
    cc_list_add(list1, c);
    cc_list_add(list1, "d");

    cc_list_add(list2, e);
    cc_list_add(list2, "f");
    cc_list_add(list2, g);

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    size_t i = 0;

    void* e1, * e2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            munit_assert_string_equal(a, (char*)e1);
            munit_assert_string_equal(e, (char*)e2);
        }
        if (i == 2) {
            munit_assert_string_equal(c, (char*)e1);
            munit_assert_string_equal(g, (char*)e2);
        }
        i++;
    }
    munit_assert_size(3, == , i);

    cc_list_destroy(list1);
    cc_list_destroy(list2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    CC_List* list2;
    cc_list_new(&list2);

    char* a = "a", * b = "b", * c = "c", * d = "d",
        * e = "e", * f = "f", * g = "g";

    cc_list_add(list1, a);
    cc_list_add(list1, b);
    cc_list_add(list1, c);
    cc_list_add(list1, d);

    cc_list_add(list2, e);
    cc_list_add(list2, f);
    cc_list_add(list2, g);

    char* h = "h";
    char* i = "i";

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void* e1, * e2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, b) == 0)
            cc_list_zip_iter_add(&zip, h, i);
    }

    size_t index;
    munit_assert_size(CC_OK, ==, cc_list_index_of(list1, h, zero_if_ptr_eq, &index));
    munit_assert_size(2, ==, index);

    munit_assert_size(CC_OK, ==, cc_list_index_of(list2, i, zero_if_ptr_eq, &index));
    munit_assert_size(2, ==, index);

    munit_assert_size(CC_OK, ==, cc_list_index_of(list1, c, zero_if_ptr_eq, &index));
    munit_assert_size(3, == , index);

    munit_assert_size(1, ==, cc_list_contains(list1, h));
    munit_assert_size(1, == , cc_list_contains(list2, i));
    munit_assert_size(5, == , cc_list_size(list1));
    munit_assert_size(4, == , cc_list_size(list2));

    cc_list_zip_iter_init(&zip, list1, list2);
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e2, g) == 0)
            cc_list_zip_iter_add(&zip, "x", "y");
    }

    char* last;
    cc_list_get_last(list1, (void*)&last);
    munit_assert_string_equal(d, last);

    cc_list_get_last(list2, (void*)&last);
    munit_assert_string_equal("y", last);

    cc_list_destroy(list1);
    cc_list_destroy(list2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    CC_List* list2;
    cc_list_new(&list2);

    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void* e1 = NULL;
    void* e2 = NULL;
    void* r1 = NULL;
    void* r2 = NULL;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_list_zip_iter_remove(&zip, &r1, &r2);
    }
    munit_assert_not_null(r1);
    munit_assert_not_null(r2);
    munit_assert_string_equal("b", r1);
    munit_assert_string_equal("f", r2);
    munit_assert_size(0, == , cc_list_contains(list1, "b"));
    munit_assert_size(0, == , cc_list_contains(list2, "f"));
    munit_assert_size(3, == , cc_list_size(list1));
    munit_assert_size(2, == , cc_list_size(list2));

    cc_list_destroy(list1);
    cc_list_destroy(list2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_replace(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list1;
    cc_list_new(&list1);

    CC_List* list2;
    cc_list_new(&list2);

    cc_list_add(list1, "a");
    cc_list_add(list1, "b");
    cc_list_add(list1, "c");
    cc_list_add(list1, "d");

    cc_list_add(list2, "e");
    cc_list_add(list2, "f");
    cc_list_add(list2, "g");

    char* h = "h";
    char* i = "i";

    CC_ListZipIter zip;
    cc_list_zip_iter_init(&zip, list1, list2);

    void* e1, * e2;
    void* r1, * r2;
    while (cc_list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_list_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    munit_assert_size(CC_OK, ==, cc_list_index_of(list1, h, zero_if_ptr_eq, &index));
    munit_assert_size(1, == , index);

    munit_assert_size(CC_OK, ==, cc_list_index_of(list2, i, zero_if_ptr_eq, &index));
    munit_assert_size(1, == , index);
    munit_assert_size(1, == , cc_list_contains(list1, h));
    munit_assert_size(1, == , cc_list_contains(list2, i));

    cc_list_destroy(list1);
    cc_list_destroy(list2);

    return MUNIT_OK;
}

struct lists {
    CC_List* list1;
    CC_List* list2;
};


static void* pre_filled_lists(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct lists* l = malloc(sizeof(struct lists));

    munit_assert_not_null(l);

    cc_list_new(&(l->list1));
    cc_list_new(&(l->list2));

    int* a = (int*)malloc(sizeof(int));
    int* b = (int*)malloc(sizeof(int));
    int* c = (int*)malloc(sizeof(int));
    int* d = (int*)malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);
    munit_assert_not_null(c);
    munit_assert_not_null(d);

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    cc_list_add(l->list1, a);
    cc_list_add(l->list1, b);
    cc_list_add(l->list1, c);
    cc_list_add(l->list1, d);

    a = (int*)malloc(sizeof(int));
    b = (int*)malloc(sizeof(int));
    c = (int*)malloc(sizeof(int));
    d = (int*)malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);
    munit_assert_not_null(c);
    munit_assert_not_null(d);

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 8;

    cc_list_add(l->list2, a);
    cc_list_add(l->list2, b);
    cc_list_add(l->list2, c);
    cc_list_add(l->list2, d);

    return (void*)l;
}

static void lists_teardown(void* fixture)
{
    struct lists* l = (struct lists*)fixture;
    cc_list_destroy_cb(l->list1, free);
    cc_list_destroy(l->list2);
    free(l);
}

static MunitResult test_add_all_at(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct lists* l = (struct lists*)fixture;

    cc_list_add_all_at(l->list1, l->list2, 2);
    munit_assert_size(4, == , cc_list_size(l->list2));
    munit_assert_size(8, == , cc_list_size(l->list1));

    int* last;
    cc_list_get_last(l->list1, (void*)&last);
    int* l1i4;
    cc_list_get_at(l->list1, 4, (void*)&l1i4);
    int* l2i2;
    cc_list_get_at(l->list2, 2, (void*)&l2i2);

    munit_assert_int(4, == , *last);
    munit_assert_int(*l1i4, == , *l2i2);

    return MUNIT_OK;
}

static MunitResult test_iter_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 32;

    CC_ListIter iter;
    cc_list_iter_init(&iter, l->list1);

    int* el;
    while (cc_list_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        if (*el == 3)
            cc_list_iter_add(&iter, ins);
    }

    munit_assert_size(5, == , cc_list_size(l->list1));

    int* li3;
    cc_list_get_at(l->list1, 3, (void*)&li3);

    munit_assert_int(*li3, == , *ins);

    int* li4;
    cc_list_get_at(l->list1, 4, (void*)&li4);
    munit_assert_int(4, ==, *li4);

    cc_list_iter_init(&iter, l->list1);

    ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 32;

    while (cc_list_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        if (*el == 4) {
            cc_list_iter_add(&iter, ins);
        }
    }

    void* e;
    cc_list_get_last(l->list1, &e);
    munit_assert_int(*ins, == , *((int*)e));

    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;

    struct lists* l = (struct lists*)fixture;

    int* rm;
    cc_list_get_at(l->list1, 2, (void*)&rm);

    CC_ListIter iter;
    cc_list_iter_init(&iter, l->list1);

    int* e;
    while (cc_list_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (*e == 3) {
            cc_list_iter_remove(&iter, NULL);
        }
    }
    munit_assert_size(3, == , cc_list_size(l->list1));
    munit_assert_size(0, ==, cc_list_contains(l->list1, rm));
    free(rm);

    return MUNIT_OK;
}

static MunitResult test_iter_desc_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_ListIter iter;
    cc_list_diter_init(&iter, l->list1);

    int* i = NULL;
    while (cc_list_diter_next(&iter, (void*)&i) != CC_ITER_END) {
        munit_assert_not_null(i);
        if (*i == 1 || *i == 3) {
            cc_list_diter_remove(&iter, NULL);
            free(i);
        }
    }
    munit_assert_size(2, ==, cc_list_size(l->list1));

    int* first;
    cc_list_get_first(l->list1, (void*)&first);

    int* last;
    cc_list_get_last(l->list1, (void*)&last);

    int* i1;
    cc_list_get_at(l->list1, 1, (void*)&i1);

    munit_assert_int(2, == , *first);
    munit_assert_int(4, == , *last);
    munit_assert_int(4, == , *i1);

    return MUNIT_OK;
}

static MunitResult test_iter_desc_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* a = (int*)malloc(sizeof(int));
    int* b = (int*)malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);

    *a = 30;
    *b = 78;

    CC_ListIter iter;
    cc_list_diter_init(&iter, l->list1);

    int* i;
    while (cc_list_diter_next(&iter, (void*)&i) != CC_ITER_END) {
        if (*i == 4) // add to tail
            cc_list_diter_add(&iter, a);

        if (*i == 3)
            cc_list_diter_add(&iter, b);
    }

    munit_assert_size(6, == , cc_list_size(l->list1));
    int* el;
    cc_list_get_first(l->list1, (void*)&el);
    munit_assert_int(1, == , *el);

    cc_list_get_last(l->list1, (void*)&el);
    munit_assert_int(4, == , *el);

    cc_list_get_at(l->list1, 2, (void*)&el);
    munit_assert_int(*b, == , *el);

    cc_list_get_at(l->list1, 4, (void*)&el);
    munit_assert_int(*a, == , *el);

    return MUNIT_OK;
}

static MunitResult test_reverse(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* last_old;
    cc_list_get_last(l->list1, (void*)&last_old);

    cc_list_reverse(l->list1);

    int* el;
    cc_list_get_at(l->list1, 0, (void*)&el);
    munit_assert_int(4, == , *el);

    cc_list_get_at(l->list1, 2, (void*)&el);
    munit_assert_int(2, == , *el);

    cc_list_get_first(l->list1, (void*)&el);
    munit_assert_ptr_equal(el, last_old);

    return MUNIT_OK;
}

static MunitResult test_to_array(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int** array;
    cc_list_to_array(l->list1, (void*)&array);

    void* e;
    cc_list_get_at(l->list1, 0, &e);
    munit_assert_ptr_equal(array[0], e);

    cc_list_get_at(l->list1, 2, &e);
    munit_assert_ptr_equal(array[2], e);

    cc_list_get_at(l->list1, 3, &e);
    munit_assert_ptr_equal(array[3], e);

    free(array);
    return MUNIT_OK;
}

static MunitResult test_splice_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_list_splice_at(l->list1, l->list2, 2);

    munit_assert_size(8, == , cc_list_size(l->list1));
    munit_assert_size(0, == , cc_list_size(l->list2));

    int* el;
    cc_list_get_first(l->list1, (void*)&el);
    munit_assert_int(1, == , *el);

    cc_list_get_last(l->list1, (void*)&el);
    munit_assert_int(4, == , *el);

    cc_list_get_at(l->list1, 2, (void*)&el);
    munit_assert_int(5, == , *el);

    return MUNIT_OK;
}

static MunitResult test_splice(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_list_splice(l->list1, l->list2);
    munit_assert_size(8, == , cc_list_size(l->list1));
    munit_assert_size(0, == , cc_list_size(l->list2));

    int* el;
    cc_list_get_last(l->list1, (void*)&el);
    munit_assert_int(8, == , *el);

    cc_list_get_first(l->list1, (void*)&el);
    munit_assert_int(1, == , *el);

    cc_list_get_at(l->list1, 4, (void*)&el);
    munit_assert_int(5, == , *el);

    return MUNIT_OK;
}

static MunitResult test_add_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    cc_list_add_all(l->list2, l->list1);

    munit_assert_size(8, == , cc_list_size(l->list2));

    int* l1last;
    cc_list_get_last(l->list1, (void*)&l1last);

    int* l2last;
    cc_list_get_last(l->list2, (void*)&l2last);
    munit_assert_int(*l1last, == , *l2last);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    cc_list_get_at(l->list1, 1, (void*)&e);
    cc_list_remove(l->list1, e, NULL);

    munit_assert_size(3, == , cc_list_size(l->list1));
    munit_assert_size(0, ==, cc_list_contains(l->list1, e));
    free(e);

    return MUNIT_OK;
}

static MunitResult test_remove_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* el = NULL;
    cc_list_remove_first(l->list1, (void*)&el);
    munit_assert_not_null(el);
    free(el);

    el = NULL;
    munit_assert_size(3, == , cc_list_size(l->list1));

    cc_list_get_first(l->list1, (void*)&el);
    munit_assert_not_null(el);
    munit_assert_int(2, == , *el);

    return MUNIT_OK;
}

static MunitResult test_remove_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* el = NULL;
    cc_list_remove_last(l->list1, (void*)&el);
    munit_assert_not_null(el);
    free(el);
    munit_assert_size(3, == , cc_list_size(l->list1));

    el = NULL;
    munit_assert_int(CC_OK, == , cc_list_get_last(l->list1, (void*)&el));
    munit_assert_not_null(el);
    munit_assert_int(3, == , *el);

    return MUNIT_OK;
}

static MunitResult test_remove_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e = NULL;
    cc_list_remove_at(l->list1, 2, (void*)&e);
    free(e);

    e = NULL;
    cc_list_get_at(l->list1, 2, (void*)&e);
    munit_assert_not_null(e);
    munit_assert_int(4, == , *e);
    munit_assert_size(3, == , cc_list_size(l->list1));
    cc_list_remove_at(l->list1, 0, (void*)&e);
    free(e);

    e = NULL;
    cc_list_get_at(l->list1, 0, (void*)&e);
    munit_assert_not_null(e);
    munit_assert_int(2, == , *e);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    cc_list_get_first(l->list1, (void*)&e);
    munit_assert_int(1, == , *e);

    cc_list_remove_all_cb(l->list1, free);
    munit_assert_size(0, == , cc_list_size(l->list1));

    e = NULL;
    cc_list_get_first(l->list1, (void*)&e);
    munit_assert_ptr_null(e);

    return MUNIT_OK;
}


static MunitResult test_get_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    cc_list_get_last(l->list1, (void*)&e);
    munit_assert_int(4, == , *e);

    return MUNIT_OK;
}


static MunitResult test_get_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* e;
    cc_list_get_at(l->list1, 1, (void*)&e);
    munit_assert_int(2, == , *e);

    return MUNIT_OK;
}

static MunitResult test_replace_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* replacement = (int*)malloc(sizeof(int));
    munit_assert_not_null(replacement);
    *replacement = 32;

    int* r;
    cc_list_replace_at(l->list1, replacement, 2, (void*)&r);
    free(r);

    cc_list_get_at(l->list1, 2, (void*)&r);
    munit_assert_ptr_equal(r, replacement);

    return MUNIT_OK;
}

static MunitResult test_copy_deep(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_List* cp;
    cc_list_copy_deep(l->list1, copy, &cp);
    munit_assert_size(4, == , cc_list_size(cp));

    int* e;
    cc_list_get_at(cp, 2, (void*)&e);

    int* le;
    cc_list_get_at(l->list1, 2, (void*)&le);
    munit_assert_int(*e, == , *le);

    cc_list_get_at(cp, 2, (void*)&e);
    cc_list_get_at(l->list1, 2, (void*)&le);
    munit_assert_ptr_not_equal(e, le);

    cc_list_destroy_cb(cp, free);

    return MUNIT_OK;
}

static MunitResult test_sublist(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_List* sub;
    cc_list_sublist(l->list1, 1, 2, &sub);
    munit_assert_size(2, == , cc_list_size(sub));

    int* s1;
    cc_list_get_at(sub, 1, (void*)&s1);

    int* l2;
    cc_list_get_at(l->list1, 2, (void*)&l2);
    munit_assert_int(*s1, ==, *l2);

    cc_list_destroy(sub);

    return MUNIT_OK;
}

static MunitResult test_add_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    int* ins = (int*)malloc(sizeof(int));
    munit_assert_not_null(ins);
    *ins = 90;

    cc_list_add_at(l->list1, ins, 3);
    munit_assert_size(5, == , cc_list_size(l->list1));

    int* new;
    cc_list_get_at(l->list1, 3, (void*)&new);
    munit_assert_int(*ins, ==, *new);

    return MUNIT_OK;
}

static MunitResult test_copy_shallow(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_List* cp;
    cc_list_copy_shallow(l->list1, &cp);
    munit_assert_size(4, == , cc_list_size(cp));

    void* f1;
    void* f2;
    for (int i = 0; i < cc_list_size(l->list1); i++) {
        cc_list_get_at(cp, i, &f1);
        cc_list_get_at(l->list1, i, &f2);
        munit_assert_ptr_equal(f1, f2);
    }
    cc_list_destroy(cp);

    return MUNIT_OK;
}

static MunitResult test_mut_filter1(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_list_size(l->list1));
    cc_list_filter_mut(l->list1, pred1);
    munit_assert_size(0, == , cc_list_size(l->list1));

    void* e = NULL;
    cc_list_get_first(l->list1, &e);
    munit_assert_ptr_null(e);

    cc_list_get_last(l->list1, &e);
    munit_assert_ptr_null(e);

    return MUNIT_OK;
}

static MunitResult test_mut_filter2(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    munit_assert_size(4, == , cc_list_size(l->list1));
    cc_list_filter_mut(l->list1, pred2);
    munit_assert_size(1, == , cc_list_size(l->list1));

    CC_ListIter iter;
    int* el = NULL;
    int i = 4;
    cc_list_iter_init(&iter, l->list1);
    while (cc_list_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    return MUNIT_OK;
}

static MunitResult test_filter1(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_List* filter = NULL;
    cc_list_filter(l->list1, pred1, &filter);

    munit_assert_size(4, == , cc_list_size(l->list1));
    munit_assert_size(0, == , cc_list_size(filter));

    void* e = NULL;
    cc_list_get_first(filter, &e);
    munit_assert_ptr_null(e);

    cc_list_get_last(filter, &e);
    munit_assert_ptr_null(e);
    free(filter);

    return MUNIT_OK;
}

static MunitResult test_filter2(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;

    CC_List* filter = NULL;
    cc_list_filter(l->list1, pred2, &filter);

    munit_assert_size(4, == , cc_list_size(l->list1));
    munit_assert_size(1, == , cc_list_size(filter));

    CC_ListIter iter;
    int* el = NULL;
    int i = 4;
    cc_list_iter_init(&iter, filter);
    while (cc_list_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        munit_assert_int(i++, == , *el);
    }

    free(filter);

    return MUNIT_OK;
}

static MunitResult test_reduce1(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct lists* l = (struct lists*)fixture;
    int res;

    cc_list_reduce(l->list1, sum_reduce, &res);

    munit_assert_int(10, == , res);

    return MUNIT_OK;
}

static MunitResult test_reduce2(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_List* list;
    cc_list_new(&list);

    bool* a = (bool*)malloc(sizeof(int));
    bool* b = (bool*)malloc(sizeof(int));
    bool* c = (bool*)malloc(sizeof(int));
    bool* d = (bool*)malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);
    munit_assert_not_null(c);
    munit_assert_not_null(d);

    *a = true;
    *b = true;
    *c = true;
    *d = true;

    cc_list_add(list, a);
    cc_list_add(list, b);
    cc_list_add(list, c);
    cc_list_add(list, d);

    bool res = false;

    cc_list_reduce(list, bool_and, &res);
    munit_assert_true(res);

    cc_list_destroy(list);

    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    {(char*)"/list/test_new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_last", test_add_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_first", test_add_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_contains", test_contains, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_index_of", test_index_of, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_sort", test_sort, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_iter_next", test_zip_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_iter_add", test_zip_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_iter_remove", test_zip_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_zip_iter_replace", test_zip_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_all_at", test_add_all_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_add", test_iter_add, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_remove", test_iter_remove, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_desc_remove", test_iter_desc_remove, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_iter_desc_add", test_iter_desc_add, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_reverse", test_reverse, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_to_array", test_to_array, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_splice_at", test_splice_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_splice", test_splice, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_all", test_add_all, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove", test_remove, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_first", test_remove_first, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_last", test_remove_last, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_at", test_remove_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_remove_all", test_remove_all, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_get_last", test_get_last, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_get_at", test_get_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_replace_at", test_replace_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_copy_deep", test_copy_deep, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_sublist", test_sublist, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_add_at", test_add_at, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_copy_shallow", test_copy_shallow, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_mut_filter1", test_mut_filter1, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_mut_filter2", test_mut_filter2, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter1", test_filter1, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_filter2", test_filter2, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_reduce1", test_reduce1, pre_filled_lists, lists_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/list/test_reduce2", test_reduce2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}