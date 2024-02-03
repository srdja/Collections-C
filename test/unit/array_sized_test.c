#include "munit.h"
#include "sized/cc_array_sized.h"
#include <stdlib.h>


/*****************************
 * TESTS
 *****************************/
static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_ArraySized *array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_sized_add(array, (uint8_t*) & a);
    cc_array_sized_add(array, (uint8_t*) & b);
    cc_array_sized_add(array, (uint8_t*) & c);

    int ar;
    int br;
    int cr;

    cc_array_sized_get_at(array, 0, (uint8_t*)&ar);
    cc_array_sized_get_at(array, 1, (uint8_t*)&br);
    cc_array_sized_get_at(array, 2, (uint8_t*)&cr);

    munit_assert_int(a, == , ar);
    munit_assert_int(b, == , br);
    munit_assert_int(c, == , cr);

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_add_out_of_range(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    munit_assert_size(0, == , cc_array_sized_size(array));
    munit_assert_int(CC_ERR_OUT_OF_RANGE, ==, cc_array_sized_add_at(array, (uint8_t*) & a, 1));

    cc_array_sized_destroy(array);
    return MUNIT_OK;
}

static MunitResult test_replace_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    cc_array_sized_add(array, (uint8_t*) & a);
    cc_array_sized_add(array, (uint8_t*) & b);
    cc_array_sized_add(array, (uint8_t*) & c);

    int out;
    cc_array_sized_replace_at(array, (uint8_t*) & replacement, 2, (uint8_t*)&out);
    munit_assert_int(out, == , c);

    int repl;
    cc_array_sized_get_at(array, 2, (uint8_t*) &repl);

    munit_assert_int(repl, !=, c);
    munit_assert_int(repl, == , replacement);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_index_of(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;

    cc_array_sized_add(array, (uint8_t*) & a);
    cc_array_sized_add(array, (uint8_t*) & b);
    cc_array_sized_add(array, (uint8_t*) & c);

    size_t ai;
    cc_array_sized_index_of(array, (uint8_t*) & a, &ai);

    size_t ci;
    cc_array_sized_index_of(array, (uint8_t*) & c, &ci);

    munit_assert_size(0, ==, ai);
    munit_assert_size(2, ==, ci);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);

    munit_assert_int(CC_OK, == , cc_array_sized_remove(array, (uint8_t*)&c));

    munit_assert_size(3, == , cc_array_sized_size(array));

    int r;
    cc_array_sized_get_at(array, 2, (uint8_t*)&r);

    munit_assert_int(r, ==, e);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_remove_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);

    int r;
    munit_assert_int(CC_OK, ==, cc_array_sized_remove_at(array, 2, (uint8_t*)&r));
    munit_assert_int(r, == , c);
    munit_assert_size(3, ==, cc_array_sized_size(array));

    cc_array_sized_get_at(array, 2, (uint8_t*)&r);

    munit_assert_int(r, ==, e);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_remove_all(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);

    cc_array_sized_remove_all(array);

    munit_assert_size(0, ==, cc_array_sized_size(array));
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_get_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);

    int ar;
    int cr;
    cc_array_sized_get_at(array, 0, (uint8_t*)&ar);
    cc_array_sized_get_at(array, 2, (uint8_t*)&cr);

    munit_assert_int(a, ==, ar);
    munit_assert_int(c, ==, cr);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_subarray(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);
    cc_array_sized_add(array, (uint8_t*)&f);

    CC_ArraySized* subarray;
    cc_array_sized_subarray(array, 1, 3, &subarray);

    munit_assert_size(3, ==, cc_array_sized_size(subarray));

    int s0;
    int s1;
    int s2;
    cc_array_sized_get_at(subarray, 0, (uint8_t*)&s0);
    cc_array_sized_get_at(subarray, 1, (uint8_t*)&s1);
    cc_array_sized_get_at(subarray, 2, (uint8_t*)&s2);

    munit_assert_int(b, ==, s0);
    munit_assert_int(c, ==, s1);
    munit_assert_int(e, ==, s2);

    cc_array_sized_destroy(subarray);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_copy(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);

    CC_ArraySized* copyar;
    cc_array_sized_copy(array, &copyar);

    munit_assert_size(cc_array_sized_size(array), == , cc_array_sized_size(copyar));

    int ca;
    cc_array_sized_get_at(copyar, 0, (uint8_t*)&ca);

    munit_assert_int(a, ==, ca);

    cc_array_sized_destroy(copyar);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_reverse(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);

    int tmp;
    cc_array_sized_reverse(array, (uint8_t*) & tmp);

    int i0;
    int i1;
    int i2;
    cc_array_sized_get_at(array, 0, (uint8_t*)&i0);
    cc_array_sized_get_at(array, 1, (uint8_t*)&i1);
    cc_array_sized_get_at(array, 2, (uint8_t*)&i2);

    munit_assert_int(c, == , i0);
    munit_assert_int(b, == , i1);
    munit_assert_int(a, == , i2);

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_contains(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_sized_add(array, (uint8_t*) & a);
    cc_array_sized_add(array, (uint8_t*) & b);
    cc_array_sized_add(array, (uint8_t*) & c);
    cc_array_sized_add(array, (uint8_t*) & c);

    size_t cc = cc_array_sized_contains(array, (uint8_t*) & c);
    size_t ca = cc_array_sized_contains(array, (uint8_t*) & a);
    size_t cd = cc_array_sized_contains(array, (uint8_t*) & d);

    munit_assert_size(2, == , cc);
    munit_assert_size(1, == , ca);
    munit_assert_size(0, == , cd);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

int comp(void const* e1, void const* e2)
{
    int i = *((int*)e1);
    int j = *((int*)e2);

    if (i < j)
        return -1;
    if (i > j)
        return 1;
    return 0;
}

static MunitResult test_sort(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int e = munit_rand_int_range(0, 100);
        cc_array_sized_add(array, (uint8_t*)&e);
    }
    cc_array_sized_sort(array, comp);

    int prev;
    cc_array_sized_get_at(array, 0, (uint8_t*) &prev);
    
    int e;
    for (i = 0; i < size; i++) {
        cc_array_sized_get_at(array, i, (uint8_t*)&e);
        munit_assert_int(prev, <=, e);
        prev = e;
    }
    cc_array_sized_destroy(array);
    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySizedIter iter;
    cc_array_sized_iter_init(&iter, array);

    int *e;
    while (cc_array_sized_iter_next(&iter, (uint8_t**)&e) != CC_ITER_END) {
        if (*e == c)
            cc_array_sized_iter_remove(&iter, NULL);
    }

    munit_assert_size(0, == , cc_array_sized_contains(array, (uint8_t*)&c));
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_iter_add(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    int N = 80;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySizedIter iter;
    cc_array_sized_iter_init(&iter, array);

    int* e;
    while (cc_array_sized_iter_next(&iter, (uint8_t**)&e) != CC_ITER_END) {
        if (*e == 3)
            cc_array_sized_iter_add(&iter, (uint8_t*)&N);
    }

    munit_assert_size(5, == , cc_array_sized_size(array));

    int n;
    cc_array_sized_get_at(array, 3, (uint8_t*)&n);

    munit_assert_int(N, == , n);
    munit_assert_size(1, == , cc_array_sized_contains(array, (uint8_t*)&N));

    cc_array_sized_get_at(array, 4, (uint8_t*)&n);
    munit_assert_int(4, == , n);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_iter_replace(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int replacement = 42;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySizedIter iter;
    cc_array_sized_iter_init(&iter, array);

    int* e;
    int old;
    while (cc_array_sized_iter_next(&iter, (uint8_t**)&e) != CC_ITER_END) {
        if (*e == c) {
            cc_array_sized_iter_replace(&iter, (uint8_t*)&replacement, (uint8_t*)&old);
        }
    }
    size_t index;
    cc_array_sized_index_of(array, (uint8_t*)&replacement, &index);

    munit_assert_size(2, == , index);
    munit_assert_size(0, == , cc_array_sized_contains(array, (uint8_t*) & c));
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySized* array2;
    cc_array_sized_new(sizeof(int), &array2);

    cc_array_sized_add(array2, (uint8_t*)&e);
    cc_array_sized_add(array2, (uint8_t*)&f);
    cc_array_sized_add(array2, (uint8_t*)&g);

    CC_ArraySizedZipIter zip;
    cc_array_sized_zip_iter_init(&zip, array, array2);

    size_t i = 0;

    int *e1;
    int *e2;
    while (cc_array_sized_zip_iter_next(&zip, (uint8_t**)&e1, (uint8_t**)&e2) != CC_ITER_END) {
        if (i == 0) {
            munit_assert_int(*e1, == , a);
            munit_assert_int(*e2, == , e);
        }
        if (i == 2) {
            munit_assert_int(*e1, == , c);
            munit_assert_int(*e2, == , g);
        }
        i++;
    }
    munit_assert_size(3, ==, i);

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_remove(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySized* array2;
    cc_array_sized_new(sizeof(int), & array2);

    cc_array_sized_add(array2, (uint8_t*)&e);
    cc_array_sized_add(array2, (uint8_t*)&f);
    cc_array_sized_add(array2, (uint8_t*)&g);

    CC_ArraySizedZipIter zip;
    cc_array_sized_zip_iter_init(&zip, array, array2);

    int* e1 = NULL;
    int* e2 = NULL;
    int r1  = 0;
    int r2  = 0;
    while (cc_array_sized_zip_iter_next(&zip, (uint8_t**)&e1, (uint8_t**)&e2) != CC_ITER_END) {
        if (*e1 == b) {
            cc_array_sized_zip_iter_remove(&zip, (uint8_t*)&r1, (uint8_t*)&r2);
        }
    }
    munit_assert_int(r1, == , b);
    munit_assert_size(0, == , cc_array_sized_contains(array, (uint8_t*)&b));
    munit_assert_size(0, == , cc_array_sized_contains(array2, (uint8_t*)&f));
    munit_assert_size(3, == , cc_array_sized_size(array));
    munit_assert_size(2, == , cc_array_sized_size(array2));

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_add(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySized* array2;
    cc_array_sized_new(sizeof(int), & array2);

    cc_array_sized_add(array2, (uint8_t*)&e);
    cc_array_sized_add(array2, (uint8_t*)&f);
    cc_array_sized_add(array2, (uint8_t*)&g);

    int h = 9;
    int i = 8;

    CC_ArraySizedZipIter zip;
    cc_array_sized_zip_iter_init(&zip, array, array2);

    int* e1;
    int* e2;
    while (cc_array_sized_zip_iter_next(&zip, (uint8_t**)&e1, (uint8_t**)&e2) != CC_ITER_END) {
        if (*e1 == b) {
            cc_array_sized_zip_iter_add(&zip, (uint8_t*)&h, (uint8_t*)&i);
        }
    }
    size_t index;

    munit_assert_int(CC_OK, == , cc_array_sized_index_of(array, (uint8_t*)&h, &index));
    munit_assert_size(2, == , index);
    munit_assert_int(CC_OK, == , cc_array_sized_index_of(array2, (uint8_t*)&i, &index));
    munit_assert_size(2, == , index);
    munit_assert_int(CC_OK, == , cc_array_sized_index_of(array, (uint8_t*)&c, &index));
    munit_assert_size(3, == , index);

    munit_assert_size(1, == , cc_array_sized_contains(array, (uint8_t*)&h));
    munit_assert_size(1, == , cc_array_sized_contains(array2, (uint8_t*)&i));
    munit_assert_size(5, == , cc_array_sized_size(array));
    munit_assert_size(4, == , cc_array_sized_size(array2));

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_replace(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);

    CC_ArraySized* array2;
    cc_array_sized_new(sizeof(int), & array2);

    cc_array_sized_add(array2, (uint8_t*)&e);
    cc_array_sized_add(array2, (uint8_t*)&f);
    cc_array_sized_add(array2, (uint8_t*)&g);

    int h = 9;
    int i = 8;

    CC_ArraySizedZipIter zip;
    cc_array_sized_zip_iter_init(&zip, array, array2);
 
    int* e1;
    int *e2;
    int r1;
    int r2;
    while (cc_array_sized_zip_iter_next(&zip, (uint8_t**)&e1, (uint8_t**)&e2) != CC_ITER_END) {
        if (*e1 == b) {
            cc_array_sized_zip_iter_replace(&zip, (uint8_t*)&h, (uint8_t*)&i, (uint8_t*)&r1, (uint8_t*)&r2);
        }
    }
    size_t index;

    munit_assert_int(CC_OK, == , cc_array_sized_index_of(array, (uint8_t*)&h, &index));
    munit_assert_size(1, == , index);
    munit_assert_int(CC_OK, == , cc_array_sized_index_of(array2, (uint8_t*)&i, &index));
    munit_assert_size(1, == , index);

    munit_assert_size(1, == , cc_array_sized_contains(array, (uint8_t*)&h));
    munit_assert_size(1, == , cc_array_sized_contains(array2, (uint8_t*)&i));

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);

    return MUNIT_OK;
}


void reduce_add(uint8_t* e1, uint8_t* e2, uint8_t* result)
{
    int el1 = e1 ? *((int*)e1) : 0;
    int el2 = e2 ? *((int*)e2) : 0;
    *((int*)result) = el1 + el2;
}

static MunitResult test_reduce(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int a = 1;
    int b = 78;
    int c = 12;
    int d = 2;
    int e = 8;

    cc_array_sized_add(array, (uint8_t*) & a);
    int result;
    cc_array_sized_reduce(array, reduce_add, (uint8_t*)&result);

    munit_assert_int(1, == , result);

    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_reduce(array, reduce_add, (uint8_t*)&result);

    munit_assert_int(79, ==, result);

    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&d);
    cc_array_sized_add(array, (uint8_t*)&e);
    cc_array_sized_reduce(array, reduce_add, (uint8_t*)&result);

    munit_assert_int(101, == , result);
    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_add_at(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);
    cc_array_sized_add(array, (uint8_t*)&e);

    cc_array_sized_add_at(array, (uint8_t*)&ins, 1);

    int i0;
    int i1;
    int i2;
    int i3;
    int i4;

    cc_array_sized_get_at(array, 0, (uint8_t*)&i0);
    cc_array_sized_get_at(array, 1, (uint8_t*)&i1);
    cc_array_sized_get_at(array, 2, (uint8_t*)&i2);
    cc_array_sized_get_at(array, 3, (uint8_t*)&i3);
    cc_array_sized_get_at(array, 4, (uint8_t*)&i4);

    munit_assert_int(a, == , i0);
    munit_assert_int(ins, == , i1);
    munit_assert_int(b, == , i2);
    munit_assert_int(c, == , i3);
    munit_assert_int(e, == , i4);

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_trim_capacity(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySizedConf conf;
    cc_array_sized_conf_init(&conf);
    conf.capacity = 20;

    CC_ArraySized* array;
    cc_array_sized_new_conf(sizeof(int), &conf, &array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);

    munit_assert_size(20, == , cc_array_sized_capacity(array));
    cc_array_sized_trim_capacity(array);
    munit_assert_size(3, == , cc_array_sized_capacity(array));

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

bool pred1(const uint8_t* e)
{
    return *(int*)e == 0;
}

bool pred2(const uint8_t* e)
{
    return *(int*)e != 0;
}


static MunitResult test_filter_mut1(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    int v;
    for (int i = 0; i < 20; i++) {
        v = i % 2;
        cc_array_sized_add(array, (uint8_t*)&v);
    }

    enum cc_stat ret = cc_array_sized_filter_mut(array, pred1);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(10, == , cc_array_sized_size(array));

    CC_ArraySizedIter i;
    cc_array_sized_iter_init(&i, array);
    int* e;
    while (cc_array_sized_iter_next(&i, (uint8_t**)&e) != CC_ITER_END) {
        munit_assert_int(0, == , *e);
    }

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_filter_mut2(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    int v;
    for (int i = 0; i < 20; i++) {
        v = i % 2;
        cc_array_sized_add(array, (uint8_t*)&v);
    }

    enum cc_stat ret = cc_array_sized_filter_mut(array, pred2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(10, == , cc_array_sized_size(array));

    CC_ArraySizedIter i;
    cc_array_sized_iter_init(&i, array);
    int* e;
    while (cc_array_sized_iter_next(&i, (uint8_t**)&e) != CC_ITER_END) {
        munit_assert_int(1, == , *e);
    }

    cc_array_sized_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_filter1(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), &array);

    int v;
    for (int i = 0; i < 20; i++) {
        v = i % 2;
        cc_array_sized_add(array, (uint8_t*)&v);
    }

    CC_ArraySized* array2;
    enum cc_stat ret = cc_array_sized_filter(array, pred1, &array2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(20, == , cc_array_sized_size(array));
    munit_assert_size(10, == , cc_array_sized_size(array2));

    CC_ArraySizedIter i;
    cc_array_sized_iter_init(&i, array2);
    int* e;
    while (cc_array_sized_iter_next(&i, (uint8_t**)&e) != CC_ITER_END) {
        munit_assert_int(0, == , *e);
    }

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_filter2(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized* array;
    cc_array_sized_new(sizeof(int), & array);

    int v;
    for (int i = 0; i < 20; i++) {
        v = i % 2;
        cc_array_sized_add(array, (uint8_t*)&v);
    }

    CC_ArraySized* array2;
    enum cc_stat ret = cc_array_sized_filter(array, pred2, &array2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(20, == , cc_array_sized_size(array));
    munit_assert_size(10, == , cc_array_sized_size(array2));

    CC_ArraySizedIter i;
    cc_array_sized_iter_init(&i, array2);
    int* e;
    while (cc_array_sized_iter_next(&i, (uint8_t**)&e) != CC_ITER_END) {
        munit_assert_int(1, == , *e);
    }

    cc_array_sized_destroy(array);
    cc_array_sized_destroy(array2);

    return MUNIT_OK;
}

void map_double(uint8_t* e)
{
    int* val = (int*)e;
    *val = *val * 2;
}

static MunitResult test_map(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArraySized *array;
    cc_array_sized_new(sizeof(int), &array);

    int a = 1; 
    int b = 2;
    int c = 3;

    cc_array_sized_add(array, (uint8_t*)&a);
    cc_array_sized_add(array, (uint8_t*)&b);
    cc_array_sized_add(array, (uint8_t*)&c);

    cc_array_sized_map(array, map_double);
    int ra = 0;
    int rb = 0;
    int rc = 0;

    cc_array_sized_get_at(array, 0, (uint8_t*)&ra);
    cc_array_sized_get_at(array, 1, (uint8_t*)&rb);
    cc_array_sized_get_at(array, 2, (uint8_t*)&rc);

    munit_assert_int(ra, == , 2);
    munit_assert_int(rb, == , 4);
    munit_assert_int(rc, == , 6);

    cc_array_sized_destroy(array);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {(char*)"/array_sized/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_add_out_of_range", test_add_out_of_range, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_replace_at", test_replace_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_index_of", test_index_of, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_remove_at", test_remove_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_get_at", test_get_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_subarray", test_subarray, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_copy", test_copy, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_reverse", test_reverse, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_contains", test_contains, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_sort", test_sort, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_iter_add", test_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_iter_replace", test_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_zip_iter_next", test_zip_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_zip_iter_remove", test_zip_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_zip_iter_add", test_zip_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_zip_iter_replace", test_zip_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_reduce", test_reduce, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_add_at", test_add_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_trim_capacity", test_trim_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_filter_mut1", test_filter_mut1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_filter_mut2", test_filter_mut2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_filter1", test_filter1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_filter2", test_filter2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array_sized/test_map", test_map, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*) "", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
	return munit_suite_main(&test_suite, (void*)"test", argc, argv );
}