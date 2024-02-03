#include "munit.h"
#include "cc_deque.h"
#include <stdlib.h>


void* cpy(void* e)
{
    int  o = *((int*)e);
    int* c = malloc(sizeof(int));
    munit_assert_not_null(c);
    *c = o;
    return (void*)c;
}

bool pred1(const void* e)
{
    return *(int*)e <= 3;
}

bool pred2(const void* e)
{
    return *(int*)e > 3;
}

bool pred3(const void* e)
{
    return *(int*)e > 5;
}


/*****************************
 * TESTS
 *****************************/
static MunitResult test_add_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_first(deque, &b);
    cc_deque_add_first(deque, &c);

    munit_assert_size(3, == , cc_deque_size(deque));

    size_t m = cc_deque_capacity(deque);
    const void* const* u = cc_deque_get_buffer(deque);
    const void* e = u[m - 1];

    munit_assert_ptr_equal(e, &a);

    e = u[m - 2];
    munit_assert_ptr_equal(e, &b);

    e = u[m - 3];
    munit_assert_ptr_equal(e, &c);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_add_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);

    munit_assert_size(3, == , cc_deque_size(deque));

    const void* const* u = cc_deque_get_buffer(deque);
    const void* e = u[0];

    munit_assert_ptr_equal(e, &a);

    e = u[1];
    munit_assert_ptr_equal(e, &b);

    e = u[2];
    munit_assert_ptr_equal(e, &c);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_add_at1(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);

    cc_deque_add_at(deque, &g, 4);

    const void* const* buff = cc_deque_get_buffer(deque);

    munit_assert_ptr_equal(buff[4], &g);

    munit_assert_ptr_equal(buff[5], &e);

    const void* elem = buff[6];
    munit_assert_ptr_equal(elem, &f);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_add_at2(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);
    cc_deque_add_first(deque, &d);
    cc_deque_add_first(deque, &e);
    cc_deque_add_first(deque, &f);

    cc_deque_add_at(deque, &g, 1);

    const void* const* buff = cc_deque_get_buffer(deque);
    const void* elem = buff[5];

    munit_assert_ptr_equal(elem, &g);

    const void* elem1 = buff[0];
    munit_assert_ptr_equal(elem1, &a);

    const void* elem2 = buff[7];
    munit_assert_ptr_equal(elem2, &c);

    const void* elem3 = buff[3];
    munit_assert_ptr_equal(elem3, &f);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_add_at3(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    cc_deque_add_last(deque, &a);
    cc_deque_add_first(deque, &b);
    cc_deque_add_first(deque, &c);
    cc_deque_add_first(deque, &d);
    cc_deque_add_first(deque, &e);
    cc_deque_add_first(deque, &f);

    cc_deque_add_at(deque, &g, 3);

    const void* const* buff = cc_deque_get_buffer(deque);

    const void* elem = buff[6];
    munit_assert_ptr_equal(elem, &g);

    const void* elem1 = buff[0];
    munit_assert_ptr_equal(elem1, &b);

    const void* elem2 = buff[7];
    munit_assert_ptr_equal(elem2, &c);

    const void* elem3 = buff[1];
    munit_assert_ptr_equal(elem3, &a);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_add_at4(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_first(deque, &f);

    cc_deque_add_at(deque, &g, 1);

    const void* const* buff = cc_deque_get_buffer(deque);
    const int elem = *((int*)buff[0]);

    munit_assert_int(elem, == , g);

    const int elem1 = *((int*)buff[4]);
    munit_assert_int(elem1, == , e);

    const int elem2 = *((int*)buff[6]);
    munit_assert_int(elem2, == , f);

    const int elem3 = *((int*)buff[7]);
    munit_assert_int(elem3, ==, a);

    cc_deque_destroy(deque);
    return MUNIT_OK;
}

static MunitResult test_add_at5(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);

    cc_deque_add_at(deque, &g, 1);

    const void* const* buff = cc_deque_get_buffer(deque);
    const int elem = *((int*)buff[7]);

    munit_assert_int(elem, ==, a);

    const int elem1 = *((int*)buff[0]);
    munit_assert_int(elem1, == , b);

    const int elem2 = *((int*)buff[5]);
    munit_assert_int(elem2, ==, f);

    const int elem3 = *((int*)buff[1]);
    munit_assert_int(elem3, == , g);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_remove_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);

    int* first;
    cc_deque_get_first(deque, (void*)&first);
    munit_assert_int(a, == , *first);

    int* removed;
    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(a, ==, *removed);

    cc_deque_get_first(deque, (void*)&first);
    munit_assert_int(b, == , *first);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_remove_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add_first(deque, &a);
    cc_deque_add_first(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);

    int* last;
    cc_deque_get_last(deque, (void*)&last);
    munit_assert_int(d, == , *last);

    int* removed;
    cc_deque_remove_last(deque, (void*)&removed);
    munit_assert_int(d, == , *removed);

    cc_deque_get_last(deque, (void*)&last);
    munit_assert_int(c, == , *last);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    cc_deque_remove_all(deque);

    void* first;
    int stat1 = cc_deque_get_first(deque, &first);
    void* last;
    int stat2 = cc_deque_get_last(deque, &last);

    munit_assert_int(CC_ERR_OUT_OF_RANGE, ==, stat1);
    munit_assert_int(CC_ERR_OUT_OF_RANGE, == , stat2);

    munit_assert_size(0, == , cc_deque_size(deque));

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_get_at(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    void* e;
    cc_deque_get_at(deque, 1, &e);
    void* n;
    int status = cc_deque_get_at(deque, 42, &n);

    munit_assert_int(b, == , *(int*)e);
    munit_assert_int(CC_ERR_OUT_OF_RANGE, == , status);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_get_first(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);

    int* first;
    cc_deque_get_first(deque, (void*)&first);

    munit_assert_int(c, == , *first);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_get_last(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);

    int* last;
    cc_deque_get_last(deque, (void*)&last);

    munit_assert_int(b, == , *last);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_copy_shallow(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);

    CC_Deque* copy;
    cc_deque_copy_shallow(deque, &copy);

    size_t size = cc_deque_size(copy);
    munit_assert_size(3, == , size);

    int* ca;
    cc_deque_get_at(copy, 0, (void*)&ca);

    int* cb;
    cc_deque_get_at(copy, 1, (void*)&cb);

    int* cc;
    cc_deque_get_at(copy, 2, (void*)&cc);

    munit_assert_int(a, == , *ca);
    munit_assert_int(b, == , *cb);
    munit_assert_int(c, == , *cc);

    cc_deque_destroy(copy);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_copy_deep(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));

    munit_assert_not_null(a);
    munit_assert_not_null(b);
    munit_assert_not_null(c);

    *a = 1;
    *b = 2;
    *c = 3;

    cc_deque_add_last(deque, a);
    cc_deque_add_last(deque, b);
    cc_deque_add_last(deque, c);

    CC_Deque* copy;
    cc_deque_copy_deep(deque, cpy, &copy);

    size_t size = cc_deque_size(copy);
    munit_assert_size(size, == , 3);

    int* ca;
    cc_deque_get_at(copy, 0, (void*)&ca);
    int* cb;
    cc_deque_get_at(copy, 1, (void*)&cb);
    int* cc;
    cc_deque_get_at(copy, 2, (void*)&cc);

    munit_assert_int(1, == , *ca);
    munit_assert_int(2, == , *cb);
    munit_assert_int(3, == , *cc);

    cc_deque_destroy(deque);
    cc_deque_destroy_cb(copy, free);
    free(a);
    free(b);
    free(c);
    return MUNIT_OK;
}

static MunitResult test_contains(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);
    cc_deque_add(deque, &e);
    cc_deque_add(deque, &f);
    cc_deque_add(deque, &a);

    munit_assert_size(2, == , cc_deque_contains(deque, &a));
    munit_assert_size(0, == , cc_deque_contains(deque, &g));
    munit_assert_size(1, == , cc_deque_contains(deque, &e));

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_size(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);

    size_t size = cc_deque_size(deque);
    munit_assert_size(4, == , size);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_capacity(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_DequeConf conf;
    cc_deque_conf_init(&conf);

    conf.capacity = 2;

    CC_Deque* deque;
    cc_deque_new_conf(&conf, &deque);

    munit_assert_size(2, ==, cc_deque_capacity(deque));

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    munit_assert_size(4, == , cc_deque_capacity(deque));

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_trim_capacity(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    munit_assert_size(8, ==, cc_deque_capacity(deque));

    cc_deque_trim_capacity(deque);

    munit_assert_size(4, ==, cc_deque_capacity(deque));

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_reverse(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    cc_deque_reverse(deque);

    int* ra;
    cc_deque_get_at(deque, 0, (void*)&ra);
    int* rb;
    cc_deque_get_at(deque, 1, (void*)&rb);
    int* rc;
    cc_deque_get_at(deque, 2, (void*)&rc);

    munit_assert_int(c, == , *ra);
    munit_assert_int(b, == , *rb);
    munit_assert_int(a, == , *rc);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_iterator_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    int g = 7;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);
    cc_deque_add(deque, &e);
    cc_deque_add(deque, &f);

    CC_DequeIter iter;
    cc_deque_iter_init(&iter, deque);

    size_t i = 0;

    int* el = NULL;

    munit_assert_size(6, == , cc_deque_size(deque));

    while (cc_deque_iter_next(&iter, (void*)&el) != CC_ITER_END) {
        if (*el == d)
            cc_deque_iter_add(&iter, &g);
        if (i >= 3) {
            munit_assert_size(i, == , cc_deque_iter_index(&iter) - 1);
        }
        i++;
    }
    munit_assert_size(7, == , cc_deque_size(deque));

    void* ret;
    cc_deque_get_at(deque, 4, &ret);
    munit_assert_int(g, == , *(int*)ret);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_iterator_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);
    cc_deque_add(deque, &e);
    cc_deque_add(deque, &f);

    CC_DequeIter iter;
    cc_deque_iter_init(&iter, deque);

    size_t i = 0;
    void* el = NULL;
    int* removed = NULL;
    while (cc_deque_iter_next(&iter, &el) != CC_ITER_END) {
        if (i == 3)
            cc_deque_iter_remove(&iter, (void**)&removed);

        if (i > 2) {
            munit_assert_size(5, == , cc_deque_size(deque));
        }
        else {
            munit_assert_size(6, == , cc_deque_size(deque));
        }
        if (i >= 3) {
            munit_assert_size(i - 1, ==, cc_deque_iter_index(&iter));
        }
        i++;
    }
    munit_assert_not_null(removed);
    munit_assert_int(d, == , *removed);

    cc_deque_destroy(deque);
    return MUNIT_OK;
}

static MunitResult test_iterator_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);
    cc_deque_add(deque, &e);
    cc_deque_add(deque, &f);

    CC_DequeIter iter;
    cc_deque_iter_init(&iter, deque);

    size_t i = 0;

    void* el;
    while (cc_deque_iter_next(&iter, &el) != CC_ITER_END) {
        void* k;
        cc_deque_get_at(deque, i, &k);
        munit_assert_ptr_equal(k, el);
        i++;

        munit_assert_size(i, == , iter.index);
    }
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_remove(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque* d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void* e1 = NULL;
    void* e2 = NULL;
    void* r1 = NULL;
    void* r2 = NULL;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_deque_zip_iter_remove(&zip, &r1, &r2);
    }
    munit_assert_not_null(r1);
    munit_assert_not_null(r2);
    munit_assert_string_equal("b", (char*)r1);
    munit_assert_string_equal("f", (char*)r2);
    munit_assert_size(0, == , cc_deque_contains(deque, "b"));
    munit_assert_size(0, == , cc_deque_contains(deque, "f"));
    munit_assert_size(3, ==, cc_deque_size(deque));
    munit_assert_size(2, == , cc_deque_size(d2));

    cc_deque_destroy(d2);
    cc_deque_destroy(deque);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    char* a = "a";
    char* b = "b";
    char* c = "c";
    char* d = "d";

    cc_deque_add(deque, a);
    cc_deque_add(deque, b);
    cc_deque_add(deque, c);
    cc_deque_add(deque, d);

    CC_Deque* d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    char* h = "h";
    char* i = "i";

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void* e1, * e2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_deque_zip_iter_add(&zip, h, i);
    }

    size_t index;
    cc_deque_index_of(deque, h, &index);

    munit_assert_size(2, == , index);

    cc_deque_index_of(deque, i, &index);
    munit_assert_size(2, == , index);

    cc_deque_index_of(deque, c, &index);
    munit_assert_size(3, == , index);

    munit_assert_size(1, == , cc_deque_contains(deque, h));
    munit_assert_size(1, == , cc_deque_contains(d2, i));
    munit_assert_size(5, == , cc_deque_size(deque));
    munit_assert_size(4, == , cc_deque_size(d2));

    cc_deque_destroy(d2);
    cc_deque_destroy(deque);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque* d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    size_t i = 0;

    void* e1, * e2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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

    cc_deque_destroy(d2);
    cc_deque_destroy(deque);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_replace(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque* d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    char* h = "h";
    char* i = "i";

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void* e1, * e2;
    void* r1, * r2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0) {
            cc_deque_zip_iter_replace(&zip, h, i, &r1, &r2);
        }
    }
    size_t index;
    cc_deque_index_of(deque, h, &index);
    munit_assert_size(1, == , index);

    cc_deque_index_of(d2, i, &index);
    munit_assert_size(1, ==, index);

    munit_assert_size(1, == , cc_deque_contains(deque, h));
    munit_assert_size(1, == , cc_deque_contains(d2, i));

    cc_deque_destroy(d2);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_deque_buffer_expansion(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_DequeConf conf;
    cc_deque_conf_init(&conf);
    conf.capacity = 4;

    CC_Deque* deque;
    cc_deque_new_conf(&conf, &deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);
    cc_deque_add_last(deque, &d);

    size_t capacity = cc_deque_capacity(deque);

    munit_assert_size(4, == , capacity);

    /* Current layout:
       _________________
       | b | d | c | a |
       -----------------
             L   F
     */
     /* This line should trigger the buffer expansion */
    cc_deque_add_first(deque, &e);

    capacity = cc_deque_capacity(deque);
    munit_assert_size(8, == , capacity);

    /* The expansion should align the elements.*/
    const void* const* buff = cc_deque_get_buffer(deque);
    const int elem = *((int*)buff[0]);

    munit_assert_int(elem, ==, c);

    const int elem1 = *((int*)buff[1]);
    munit_assert_int(elem1, == , a);

    const int elem2 = *((int*)buff[2]);
    munit_assert_int(elem2, == , b);

    const int elem3 = *((int*)buff[3]);
    munit_assert_int(elem3, == , d);

    const int elem4 = *((int*)buff[7]);
    munit_assert_int(elem4, == , e);

    cc_deque_add_last(deque, &f);

    const int elem5 = *((int*)buff[4]);
    munit_assert_int(elem5, == , f);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter1(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, == , cc_deque_size(deque));

    CC_Deque* filter = NULL;
    cc_deque_filter(deque, pred1, &filter);
    munit_assert_size(3, == , cc_deque_size(filter));

    const void* const* buff = cc_deque_get_buffer(filter);

    munit_assert_ptr_equal(buff[0], &a);
    munit_assert_ptr_equal(buff[1], &b);

    const void* elem = buff[2];
    munit_assert_ptr_equal(elem, &c);

    cc_deque_destroy(filter);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter2(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, ==, cc_deque_size(deque));

    CC_Deque* filter = NULL;
    cc_deque_filter(deque, pred2, &filter);
    const void* const* buff = cc_deque_get_buffer(filter);

    munit_assert_size(3, == , cc_deque_size(filter));

    munit_assert_ptr_equal(buff[0], &d);
    munit_assert_ptr_equal(buff[1], &e);
    munit_assert_ptr_equal(buff[2], &f);

    cc_deque_destroy(filter);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter3(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, == , cc_deque_size(deque));

    CC_Deque* filter = NULL;
    cc_deque_filter(deque, pred3, &filter);
    const void* const* buff = cc_deque_get_buffer(filter);

    munit_assert_size(1, == , cc_deque_size(filter));
    munit_assert_ptr_equal(buff[0], &f);

    cc_deque_destroy(filter);
    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter_mut1(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, == , cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred1);
    munit_assert_size(3, == , cc_deque_size(deque));

    int* removed = NULL;
    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(a, == , *removed);

    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(b, == , *removed);

    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(c, ==, *removed);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter_mut2(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, == , cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred2);
    munit_assert_size(3, == , cc_deque_size(deque));

    int* removed = NULL;
    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(d, == , *removed);

    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(e, == , *removed);

    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(f, == , *removed);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitResult test_filter_mut3(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Deque* deque;
    cc_deque_new(&deque);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);
    cc_deque_add_last(deque, &e);
    cc_deque_add_last(deque, &f);
    munit_assert_size(6, == , cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred3);
    munit_assert_size(1, == , cc_deque_size(deque));

    int* removed = NULL;
    cc_deque_remove_first(deque, (void*)&removed);
    munit_assert_int(f, == , *removed);

    cc_deque_destroy(deque);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {(char*)"/deque/test_add_first", test_add_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_last", test_add_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_at1", test_add_at1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_at2", test_add_at2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_at3", test_add_at3, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_at4", test_add_at4, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_add_at5", test_add_at5, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_remove_first", test_remove_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_remove_last", test_remove_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_get_at", test_get_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_get_first", test_get_first, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_get_last", test_get_last, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_copy_shallow", test_copy_shallow, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_copy_deep", test_copy_deep, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_contains", test_contains, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_size", test_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_capacity", test_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_trim_capacity", test_trim_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_reverse", test_reverse, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_iterator_add", test_iterator_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_iterator_remove", test_iterator_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_iterator_next", test_iterator_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_zip_iter_remove", test_zip_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_zip_iter_add", test_zip_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_zip_iter_next", test_zip_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_zip_iter_replace", test_zip_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_deque_buffer_expansion", test_deque_buffer_expansion, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter1", test_filter1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter2", test_filter2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter3", test_filter3, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter_mut1", test_filter_mut1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter_mut2", test_filter_mut2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/deque/test_filter_mut3", test_filter_mut3, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}