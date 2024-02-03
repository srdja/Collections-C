#include "munit.h"
#include "cc_array.h"
#include <stdlib.h>


int comp(void const* e1, void const* e2)
{
    int* i = *((int**)e1);
    int* j = *((int**)e2);

    if (*i < *j)
        return -1;
    if (*i > *j)
        return 1;
    return 0;
}

void* copyfn(void* val)
{
    int* N = (int*)malloc(sizeof(int));
    if (N && val) {
        *N = *((int*)val);
    }
    return N;
}

bool pred1(const void* e)
{
    return *(int*)e == 0;
}

bool pred2(const void* e)
{
    return *(int*)e != 0;
}

void reduce_add(void* e1, void* e2, void* result)
{
    int el1 = e1 ? *((int*)e1) : 0;
    int el2 = e2 ? *((int*)e2) : 0;
    *((int*)result) = el1 + el2;
}


/*****************************
 * TESTS
 *****************************/
static MunitResult test_add(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Array *array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    int* ar;
    int* br;
    int* cr;

    cc_array_get_at(array, 0, (void*)&ar);
    cc_array_get_at(array, 1, (void*)&br);
    cc_array_get_at(array, 2, (void*)&cr);

    munit_assert_int(a, == , *ar);
    munit_assert_int(b, == , *br);
    munit_assert_int(c, == , *cr);

    cc_array_destroy(array);

    return MUNIT_OK;
}


static MunitResult test_add_out_of_range(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    munit_assert_size(0, == , cc_array_size(array));
    munit_assert_int(CC_ERR_OUT_OF_RANGE, ==, cc_array_add_at(array, &a, 1));

    cc_array_destroy(array);
    return MUNIT_OK;
}

static MunitResult test_replace_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    cc_array_add(array, &a);
    cc_array_add(array, & b);
    cc_array_add(array, &c);

    cc_array_replace_at(array, &replacement, 2, NULL);

    int* repl;
    cc_array_get_at(array, 2, (void*) &repl);

    munit_assert_int(*repl, !=, c);
    munit_assert_int(*repl, == , replacement);
    cc_array_destroy(array);
    
    return MUNIT_OK;
}

static MunitResult test_remove(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);

    cc_array_remove(array, &c, NULL);

    munit_assert_size(3, == , cc_array_size(array));

    int* r;
    cc_array_get_at(array, 2, (void*)&r);

    munit_assert_ptr_equal(r, &e);
    cc_array_destroy(array);

    return MUNIT_OK;
}


static MunitResult test_remove_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);

    cc_array_remove_at(array, 2, NULL);

    munit_assert_size(3, ==, cc_array_size(array));

    int* r;
    cc_array_get_at(array, 2, (void*)&r);

    munit_assert_ptr_equal(r, &e);
    cc_array_destroy(array);

    return MUNIT_OK;
}


static MunitResult test_remove_all(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);

    cc_array_remove_all(array);

    munit_assert_size(0, ==, cc_array_size(array));
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_get_at(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);

    int* ar;
    int* cr;
    cc_array_get_at(array, 0, (void*)&ar);
    cc_array_get_at(array, 2, (void*)&cr);

    munit_assert_int(a, ==, *ar);
    munit_assert_int(c, ==, *cr);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_index_of(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    size_t ai;
    cc_array_index_of(array, &a, &ai);

    size_t ci;
    cc_array_index_of(array, &c, &ci);

    munit_assert_size(0, ==, ai);
    munit_assert_size(2, ==, ci);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_subarray(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);
    cc_array_add(array, &f);

    CC_Array* subarray;
    cc_array_subarray(array, 1, 3, &subarray);

    munit_assert_size(3, ==, cc_array_size(subarray));

    int* s0;
    int* s1;
    int* s2;
    cc_array_get_at(subarray, 0, (void*)&s0);
    cc_array_get_at(subarray, 1, (void*)&s1);
    cc_array_get_at(subarray, 2, (void*)&s2);

    munit_assert_ptr_equal(&b, s0);
    munit_assert_ptr_equal(&c, s1);
    munit_assert_ptr_equal(&e, s2);

    cc_array_destroy(subarray);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_copy_shallow(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    CC_Array* copyar;
    cc_array_copy_shallow(array, &copyar);

    munit_assert_size(cc_array_size(copyar), ==, cc_array_size(array));

    int* ga;
    int* gb;
    cc_array_get_at(array, 2, (void*) &ga);
    cc_array_get_at(copyar,  2, (void*) &gb);

    munit_assert_int(*ga, == , *gb);

    cc_array_destroy(array);
    cc_array_destroy(copyar);

    return MUNIT_OK;
}

static MunitResult test_copy_deep(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    CC_Array* copyar;
    cc_array_copy_deep(array, copyfn, &copyar);

    munit_assert_size(cc_array_size(array), == , cc_array_size(copyar));

    int* ca;
    cc_array_get_at(copyar, 0, (void*)&ca);

    munit_assert_int(a, ==, *ca);

    cc_array_destroy_cb(copyar, free);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_reverse(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    cc_array_reverse(array);

    int* i0;
    int* i1;
    int* i2;
    cc_array_get_at(array, 0, (void*)&i0);
    cc_array_get_at(array, 1, (void*)&i1);
    cc_array_get_at(array, 2, (void*)&i2);

    munit_assert_int(c, == , *i0);
    munit_assert_int(b, == , *i1);
    munit_assert_int(a, == , *i2);

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_contains(const MunitParameter p[], void* f)
{
    (void)p;
    (void)f;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &c);

    size_t cc = cc_array_contains(array, &c);
    size_t ca = cc_array_contains(array, &a);
    size_t cd = cc_array_contains(array, &d);

    munit_assert_size(2, == , cc);
    munit_assert_size(1, == , ca);
    munit_assert_size(0, == , cd);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_sort(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int* e = (int*)malloc(sizeof(int));
        munit_assert_not_null(e);
        *e = munit_rand_int_range(0, 100);
        cc_array_add(array, (void*)e);
    }
    cc_array_sort(array, comp);

    int* prev;
    cc_array_get_at(array, 0, (void**) &prev);
    for (i = 0; i < size; i++) {
        int* e;
        cc_array_get_at(array, i, (void*)&e);
        munit_assert_int(*prev, <=, *e);
        prev = e;
    }
    cc_array_destroy_cb(array, free);
    return MUNIT_OK;
}

static MunitResult test_iter_remove(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, array);

    int* e;
    while (cc_array_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (*e == c)
            cc_array_iter_remove(&iter, NULL);
    }

    munit_assert_size(0, == , cc_array_contains(array, &c));
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_iter_add(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    int N = 80;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, array);

    int* e;
    while (cc_array_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (*e == 3)
            cc_array_iter_add(&iter, &N);
    }

    munit_assert_size(5, == , cc_array_size(array));

    void* n;
    cc_array_get_at(array, 3, &n);

    munit_assert_int(N, == , *((int*)n));
    munit_assert_size(1, ==, cc_array_contains(array, &N));

    cc_array_get_at(array, 4, (void*)&n);
    munit_assert_int(4, == , *((int*)n));
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_iter_replace(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int replacement = 42;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, array);

    int* e;
    int* old;
    while (cc_array_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (*e == c) {
            cc_array_iter_replace(&iter, (void*)&replacement, (void*)&old);
        }
    }
    size_t index;
    cc_array_index_of(array, (void*)&replacement, &index);

    munit_assert_size(2, == , index);
    munit_assert_size(0, == , cc_array_contains(array, &c));
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    cc_array_add(array, "a");
    cc_array_add(array, "b");
    cc_array_add(array, "c");
    cc_array_add(array, "d");

    CC_Array* array2;
    cc_array_new(&array2);

    cc_array_add(array2, "e");
    cc_array_add(array2, "f");
    cc_array_add(array2, "g");

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, array, array2);

    size_t i = 0;

    void* e1, * e2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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
    munit_assert_size(3, ==, i);

    cc_array_destroy(array);
    cc_array_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_remove(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    cc_array_add(array, "a");
    cc_array_add(array, "b");
    cc_array_add(array, "c");
    cc_array_add(array, "d");

    CC_Array* array2;
    cc_array_new(&array2);

    cc_array_add(array2, "e");
    cc_array_add(array2, "f");
    cc_array_add(array2, "g");

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, array, array2);

    void* e1 = NULL;
    void* e2 = NULL;
    void* r1 = NULL;
    void* r2 = NULL;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0)
            cc_array_zip_iter_remove(&zip, &r1, &r2);
    }
    munit_assert_not_null(r1);
    munit_assert_string_equal("b", (char*)r1);
    munit_assert_size(0, == , cc_array_contains(array, "b"));
    munit_assert_size(0, == , cc_array_contains(array2, "f"));
    munit_assert_size(3, == , cc_array_size(array));
    munit_assert_size(2, == , cc_array_size(array2));

    cc_array_destroy(array);
    cc_array_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_zip_iter_add(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    char* c = "c";

    cc_array_add(array, "a");
    cc_array_add(array, "b");
    cc_array_add(array, c);
    cc_array_add(array, "d");

    CC_Array* array2;
    cc_array_new(&array2);

    cc_array_add(array2, "e");
    cc_array_add(array2, "f");
    cc_array_add(array2, "g");

    char* h = "h";
    char* i = "i";

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, array, array2);

    void* e1, * e2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0) {
            cc_array_zip_iter_add(&zip, h, i);
        }
    }
    size_t index;

    munit_assert_int(CC_OK, == , cc_array_index_of(array, h, &index));
    munit_assert_size(2, == , index);
    munit_assert_int(CC_OK, == , cc_array_index_of(array2, i, &index));
    munit_assert_size(2, == , index);
    munit_assert_int(CC_OK, == , cc_array_index_of(array, c, &index));
    munit_assert_size(3, == , index);

    munit_assert_size(1, == , cc_array_contains(array, h));
    munit_assert_size(1, == , cc_array_contains(array2, i));
    munit_assert_size(5, == , cc_array_size(array));
    munit_assert_size(4, == , cc_array_size(array2));

    cc_array_destroy(array);
    cc_array_destroy(array2);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_replace(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    cc_array_add(array, "a");
    cc_array_add(array, "b");
    cc_array_add(array, "c");
    cc_array_add(array, "d");

    CC_Array* array2;
    cc_array_new(&array2);

    cc_array_add(array2, "e");
    cc_array_add(array2, "f");
    cc_array_add(array2, "g");

    char* h = "h";
    char* i = "i";

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, array, array2);

    void* e1, * e2;
    void* r1, * r2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*)e1, "b") == 0) {
            cc_array_zip_iter_replace(&zip, h, i, &r1, &r2);
        }
    }
    size_t index;

    munit_assert_int(CC_OK, == , cc_array_index_of(array, h, &index));
    munit_assert_size(1, == , index);
    munit_assert_int(CC_OK, == , cc_array_index_of(array2, i, &index));
    munit_assert_size(1, == , index);

    munit_assert_size(1, == , cc_array_contains(array, h));
    munit_assert_size(1, == , cc_array_contains(array2, i));

    cc_array_destroy(array);
    cc_array_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_reduce(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 1;
    int b = 78;
    int c = 12;
    int d = 2;
    int e = 8;

    cc_array_add(array, &a);
    int result;
    cc_array_reduce(array, reduce_add, (void*)&result);

    munit_assert_int(1, == , result);

    cc_array_add(array, &b);
    cc_array_reduce(array, reduce_add, (void*)&result);

    munit_assert_int(79, ==, result);

    cc_array_add(array, &c);
    cc_array_add(array, &d);
    cc_array_add(array, &e);
    cc_array_reduce(array, reduce_add, (void*)&result);

    munit_assert_int(101, == , result);
    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_add_at(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);
    cc_array_add(array, &e);

    cc_array_add_at(array, &ins, 1);

    int* i0;
    int* i1;
    int* i2;
    int* i3;
    int* i4;

    cc_array_get_at(array, 0, (void*)&i0);
    cc_array_get_at(array, 1, (void*)&i1);
    cc_array_get_at(array, 2, (void*)&i2);
    cc_array_get_at(array, 3, (void*)&i3);
    cc_array_get_at(array, 4, (void*)&i4);

    munit_assert_int(a, == , *i0);
    munit_assert_int(ins, == , *i1);
    munit_assert_int(b, == , *i2);
    munit_assert_int(c, == , *i3);
    munit_assert_int(e, == , *i4);

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_trim_capacity(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArrayConf conf;
    cc_array_conf_init(&conf);
    conf.capacity = 20;

    CC_Array* array;
    cc_array_new_conf(&conf, &array);

    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(array, &a);
    cc_array_add(array, &b);
    cc_array_add(array, &c);

    munit_assert_size(20, == , cc_array_capacity(array));
    cc_array_trim_capacity(array);
    munit_assert_size(3, == , cc_array_capacity(array));

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_capacity(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_ArrayConf conf;
    cc_array_conf_init(&conf);
    conf.capacity = 1;

    CC_Array* array;
    cc_array_new_conf(&conf, &array);

    int a = 5;
    int b = 12;

    cc_array_add(array, (void*)&a);
    munit_assert_size(1, == , cc_array_capacity(array));

    cc_array_add(array, (void*)&b);
    munit_assert_size(2, ==, cc_array_capacity(array));

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_filter_mut1(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    for (int i = 0; i < 20; i++) {
        int* v = (int*) malloc(sizeof(int));
        munit_assert_not_null(v);
        *v = i % 2;
        cc_array_add(array, (void*) v);
    }

    enum cc_stat ret = cc_array_filter_mut(array, pred1);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(10, == , cc_array_size(array));

    CC_ArrayIter i;
    cc_array_iter_init(&i, array);
    void* e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        munit_assert_int(0, == , *(int*)e);
    }

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_filter_mut2(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    for (int i = 0; i < 20; i++) {
        int* v = (int*)malloc(sizeof(int));
        munit_assert_not_null(v);
        *v = i % 2;
        cc_array_add(array, (void*)v);
    }

    enum cc_stat ret = cc_array_filter_mut(array, pred2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(10, == , cc_array_size(array));

    CC_ArrayIter i;
    cc_array_iter_init(&i, array);
    void* e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        munit_assert_int(1, == , *(int*)e);
    }

    cc_array_destroy(array);

    return MUNIT_OK;
}

static MunitResult test_filter1(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    for (int i = 0; i < 20; i++) {
        int* v = (int*)malloc(sizeof(int));
        munit_assert_not_null(v);
        *v = i % 2;
        cc_array_add(array, (void*)v);
    }

    CC_Array* array2;
    enum cc_stat ret = cc_array_filter(array, pred1, &array2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(20, == , cc_array_size(array));
    munit_assert_size(10, == , cc_array_size(array2));

    CC_ArrayIter i;
    cc_array_iter_init(&i, array2);
    void* e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        munit_assert_int(0, == , *(int*)e);
    }

    cc_array_destroy(array);
    cc_array_destroy(array2);

    return MUNIT_OK;
}

static MunitResult test_filter2(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array* array;
    cc_array_new(&array);

    for (int i = 0; i < 20; i++) {
        int* v = (int*)malloc(sizeof(int));
        munit_assert_not_null(v);
        *v = i % 2;
        cc_array_add(array, (void*)v);
    }

    CC_Array* array2;
    enum cc_stat ret = cc_array_filter(array, pred2, &array2);
    munit_assert_int(CC_OK, == , ret);
    munit_assert_size(20, == , cc_array_size(array));
    munit_assert_size(10, == , cc_array_size(array2));

    CC_ArrayIter i;
    cc_array_iter_init(&i, array2);
    void* e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        munit_assert_int(1, == , *(int*)e);
    }

    cc_array_destroy(array);
    cc_array_destroy(array2);

    return MUNIT_OK;
}

void map_double(void* e)
{
    int* val = (int*)e;
    *val = *val * 2;
}

static MunitResult test_map(const MunitParameter p[], void* fixture)
{
    (void)p;
    (void)fixture;

    CC_Array *array;
    cc_array_new(&array);

    int a = 1; 
    int b = 2;
    int c = 3;

    cc_array_add(array, (void*)&a);
    cc_array_add(array, (void*)&b);
    cc_array_add(array, (void*)&c);

    cc_array_map(array, map_double);
    int *ra = 0;
    int *rb = 0;
    int *rc = 0;

    cc_array_get_at(array, 0, (void*)&ra);
    cc_array_get_at(array, 1, (void*)&rb);
    cc_array_get_at(array, 2, (void*)&rc);

    munit_assert_int(*ra, == , 2);
    munit_assert_int(*rb, == , 4);
    munit_assert_int(*rc, == , 6);

    cc_array_destroy(array);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
	{(char*)"/array/test_add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_add_out_of_range", test_add_out_of_range, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_replace_at", test_replace_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_remove_at", test_remove_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_remove_all", test_remove_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_get_at", test_get_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_index_of", test_index_of, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_subarray", test_subarray, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_copy_shallow", test_copy_shallow, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_copy_deep", test_copy_deep, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_reverse", test_reverse, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_contains", test_contains, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_sort", test_sort, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_iter_remove", test_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_iter_add", test_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_iter_replace", test_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_zip_iter_next", test_zip_iter_next, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_zip_iter_remove", test_zip_iter_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_zip_iter_add", test_zip_iter_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_zip_iter_replace", test_zip_iter_replace, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_reduce", test_reduce, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_trim_capacity", test_trim_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_capacity", test_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_test_filter_mut1", test_filter_mut1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_test_filter_mut2", test_filter_mut2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_filter1", test_filter1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_filter2", test_filter2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_add_at", test_add_at, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char*)"/array/test_map", test_map, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};


static const MunitSuite test_suite = {
    (char*) "", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
	return munit_suite_main(&test_suite, (void*)"test", argc, argv );
}