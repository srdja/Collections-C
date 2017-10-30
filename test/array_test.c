#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "CppUTest/TestHarness_c.h"

static Array *v1;
static Array *v2;
static int stat;
static ArrayConf vc;

int comp(void const *e1, void const *e2)
{
    int *i = *((int**) e1);
    int *j = *((int**) e2);

    if (*i < *j)
        return -1;
    if (*i > *j)
        return 1;
    return 0;
}

void *copy(void *val)
{
    int *N = (int*)malloc(sizeof(int));
    *N = *((int*)val);
    return N;
}

bool pred1(const void *e)
{
    return *(int*)e == 0;
}

bool pred2(const void *e)
{
    return *(int*)e != 0;
}

void reduce_add(void *e1, void *e2, void *result)
{
    int el1 = e1 ? *((int*)e1) : 0;
    int el2 = e2 ? *((int*)e2) : 0;
    *((int*)result) = el1 + el2;
}

TEST_GROUP_C_SETUP(ArrayTestsWithDefaults)
{
    stat = array_new(&v1);
}

TEST_GROUP_C_TEARDOWN(ArrayTestsWithDefaults)
{
    array_destroy(v1);
}

TEST_C(ArrayTestsWithDefaults, ArrayAdd)
{
    int a = 5;
    int b = 12;
    int c = 848;
    CHECK_EQUAL_C_INT(CC_OK, stat);

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    int *ar;
    int *br;
    int *cr;

    array_get_at(v1, 0, (void*)&ar);
    array_get_at(v1, 1, (void*)&br);
    array_get_at(v1, 2, (void*)&cr);

    CHECK_EQUAL_C_INT(a, *ar);
    CHECK_EQUAL_C_INT(b, *br);
    CHECK_EQUAL_C_INT(c, *cr);
};

TEST_C(ArrayTestsWithDefaults, ArrayAddAt2)
{
    int a = 5;
    CHECK_EQUAL_C_INT(0, array_size(v1));
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, array_add_at(v1, &a, 1));
};

TEST_C(ArrayTestsWithDefaults, ArrayReplaceAt)
{
    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    array_replace_at(v1, &replacement, 2, NULL);

    int *repl;
    array_get_at(v1, 2, (void*)&repl);

    CHECK_C(*repl != c);
    CHECK_C(*repl == replacement);
};

TEST_C(ArrayTestsWithDefaults, ArrayRemove)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);

    array_remove(v1, &c, NULL);

    CHECK_EQUAL_C_INT(3, array_size(v1));

    int *r;
    array_get_at(v1, 2, (void*)&r);

    CHECK_EQUAL_C_POINTER(r, &e);
};

TEST_C(ArrayTestsWithDefaults, ArrayRemoveAt)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);

    array_remove_at(v1, 2, NULL);

    CHECK_EQUAL_C_INT(3, array_size(v1));

    int *r;
    array_get_at(v1, 2, (void*)&r);

    CHECK_EQUAL_C_POINTER(r, &e);
};

TEST_C(ArrayTestsWithDefaults, ArrayRemoveAll)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);

    array_remove_all(v1);

    CHECK_EQUAL_C_INT(0, array_size(v1));
};

TEST_C(ArrayTestsWithDefaults, ArrayGetAt)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);

    int *ar;
    int *cr;
    array_get_at(v1, 0, (void*)&ar);
    array_get_at(v1, 2, (void*)&cr);

    CHECK_EQUAL_C_INT(a, *ar);
    CHECK_EQUAL_C_INT(c, *cr);
};

TEST_C(ArrayTestsWithDefaults, ArrayIndexOf)
{
    int a = 23;
    int b = 32;
    int c = 55;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    size_t ai;
    array_index_of(v1, &a, &ai);

    size_t ci;
    array_index_of(v1, &c, &ci);

    CHECK_EQUAL_C_INT(0, ai);
    CHECK_EQUAL_C_INT(2, ci);
};

TEST_C(ArrayTestsWithDefaults, ArraySubarray)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);
    array_add(v1, &f);


    Array *sub;
    array_subarray(v1, 1, 3, &sub);

    CHECK_EQUAL_C_INT(3, array_size(sub));

    int *s0;
    int *s1;
    int *s2;
    array_get_at(sub, 0, (void*)&s0);
    array_get_at(sub, 1, (void*)&s1);
    array_get_at(sub, 2, (void*)&s2);

    CHECK_EQUAL_C_POINTER(&b, s0);
    CHECK_EQUAL_C_POINTER(&c, s1);
    CHECK_EQUAL_C_POINTER(&e, s2);

    array_destroy(sub);
};

TEST_C(ArrayTestsWithDefaults, ArrayShallowCopy)
{
    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    // Array *cp;
    array_copy_shallow(v1, &v2);

    CHECK_EQUAL_C_INT(array_size(v2), array_size(v1));

    int *ga;
    int *gb;
    array_get_at(v1, 2, (void*)&ga);
    array_get_at(v2, 2, (void*)&gb);

    CHECK_EQUAL_C_INT(*ga, *gb);

    array_destroy(v2);
};

TEST_C(ArrayTestsWithDefaults, ArrayDeepCopy)
{
    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    array_copy_deep(v1, copy, &v2);

    CHECK_EQUAL_C_INT(array_size(v2), array_size(v1));

    int *ca;
    array_get_at(v2, 0, (void*) &ca);

    CHECK_EQUAL_C_INT(a, *ca);
    array_destroy_cb(v2, free);
};

TEST_C(ArrayTestsWithDefaults, ArrayReverse)
{
    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    array_reverse(v1);

    int *i0;
    int *i1;
    int *i2;
    array_get_at(v1, 0, (void*)&i0);
    array_get_at(v1, 1, (void*)&i1);
    array_get_at(v1, 2, (void*)&i2);

    CHECK_EQUAL_C_INT(c, *i0);
    CHECK_EQUAL_C_INT(b, *i1);
    CHECK_EQUAL_C_INT(a, *i2);
};


TEST_C(ArrayTestsWithDefaults, ArrayContains)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &c);

    int cc = array_contains(v1, &c);
    int ca = array_contains(v1, &a);
    int cd = array_contains(v1, &d);

    CHECK_EQUAL_C_INT(2, cc);
    CHECK_EQUAL_C_INT(1, ca);
    CHECK_EQUAL_C_INT(0, cd);
};


TEST_C(ArrayTestsWithDefaults, ArraySort)
{
    srand(time(NULL));

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int *e = (int*)malloc(sizeof(int));
        *e = rand() % 100;
        array_add(v1, (void*)e);
    }
    array_sort(v1, comp);

    int *prev;
    array_get_at(v1, 0, (void**)&prev);
    for (i = 0; i < size; i++) {
        int *e;
        array_get_at(v1, i, (void*)&e);
        CHECK_C(*prev <= *e);
        prev = e;
    }
};

TEST_C(ArrayTestsWithDefaults, ArrayIterRemove)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &d);

    ArrayIter iter;
    array_iter_init(&iter, v1);

    int *e;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            array_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(0, array_contains(v1, &c));
};

TEST_C(ArrayTestsWithDefaults, ArrayIterAdd)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    int N = 80;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &d);

    ArrayIter iter;
    array_iter_init(&iter, v1);

    int *e;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3)
            array_iter_add(&iter, &N);
    }

    CHECK_EQUAL_C_INT(5, array_size(v1));

    void *n;
    array_get_at(v1, 3, &n);
    CHECK_EQUAL_C_INT(N, *((int*)n));
    CHECK_EQUAL_C_INT(1, array_contains(v1, &N));

    array_get_at(v1, 4, (void*)&n);
    CHECK_EQUAL_C_INT(4, *((int*)n));
};

TEST_C(ArrayTestsWithDefaults, ArrayIterReplace)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int replacement = 42;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &d);

    ArrayIter iter;
    array_iter_init(&iter, v1);

    int *e;
    int *old;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            array_iter_replace(&iter, (void*)&replacement, (void*)&old);
    }

    size_t index;
    array_index_of(v1, (void*) &replacement, &index);

    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(0, array_contains(v1, &c));
};

TEST_C(ArrayTestsWithDefaults, ArrayZipIterNext)
{
    array_add(v1, "a");
    array_add(v1, "b");
    array_add(v1, "c");
    array_add(v1, "d");

    array_new(&v2);

    array_add(v2, "e");
    array_add(v2, "f");
    array_add(v2, "g");

    ArrayZipIter zip;
    array_zip_iter_init(&zip, v1, v2);

    size_t i = 0;

    void *e1, *e2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            CHECK_EQUAL_C_STRING("a", (char*) e1);
            CHECK_EQUAL_C_STRING("e", (char*) e2);
        }
        if (i == 2) {
            CHECK_EQUAL_C_STRING("c", (char*) e1);
            CHECK_EQUAL_C_STRING("g", (char*) e2);
        }
        i++;
    }
    CHECK_EQUAL_C_INT(3, i);
    array_destroy(v2);
};

TEST_C(ArrayTestsWithDefaults, ArrayZipIterRemove)
{
    array_add(v1, "a");
    array_add(v1, "b");
    array_add(v1, "c");
    array_add(v1, "d");

    array_new(&v2);

    array_add(v2, "e");
    array_add(v2, "f");
    array_add(v2, "g");

    ArrayZipIter zip;
    array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    void *r1, *r2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            array_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*) r1);
    CHECK_EQUAL_C_INT(0, array_contains(v1, "b"));
    CHECK_EQUAL_C_INT(0, array_contains(v2, "f"));
    CHECK_EQUAL_C_INT(3, array_size(v1));
    CHECK_EQUAL_C_INT(2, array_size(v2));

    array_destroy(v2);
};

TEST_C(ArrayTestsWithDefaults, ArrayZipIterAdd)
{
    array_add(v1, "a");
    array_add(v1, "b");
    array_add(v1, "c");
    array_add(v1, "d");

    array_new(&v2);

    array_add(v2, "e");
    array_add(v2, "f");
    array_add(v2, "g");

    char *h = "h";
    char *i = "i";

    ArrayZipIter zip;
    array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            array_zip_iter_add(&zip, h, i);
    }

    size_t index;

    CHECK_EQUAL_C_INT(CC_OK, array_index_of(v1, "h", &index));
    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(CC_OK, array_index_of(v2, "i", &index));
    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(CC_OK, array_index_of(v1, "c", &index));
    CHECK_EQUAL_C_INT(3, index);
    CHECK_EQUAL_C_INT(1, array_contains(v1, "h"));
    CHECK_EQUAL_C_INT(1, array_contains(v2, "i"));
    CHECK_EQUAL_C_INT(5, array_size(v1));
    CHECK_EQUAL_C_INT(4, array_size(v2));

    array_destroy(v2);
};

TEST_C(ArrayTestsWithDefaults, ArrayZipIterReplace)
{
    array_add(v1, "a");
    array_add(v1, "b");
    array_add(v1, "c");
    array_add(v1, "d");

    array_new(&v2);

    array_add(v2, "e");
    array_add(v2, "f");
    array_add(v2, "g");

    char *h = "h";
    char *i = "i";

    ArrayZipIter zip;
    array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    void *r1, *r2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0) {
            array_zip_iter_replace(&zip, h, i, &r1, &r2);
        }
    }

    size_t index;

    CHECK_EQUAL_C_INT(CC_OK, array_index_of(v1, "h", &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(CC_OK, array_index_of(v2, i, &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, array_contains(v1, "h"));
    CHECK_EQUAL_C_INT(1, array_contains(v2, "i"));

    array_destroy(v2);
};

TEST_C(ArrayTestsWithDefaults, ArrayReduce)
{
    int a = 1;
    int b = 78;
    int c = 12;
    int d = 2;
    int e = 8;

    array_add(v1, &a);
    int result;
    array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(1, result);

    array_add(v1, &b);
    array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(79, result);

    array_add(v1, &c);
    array_add(v1, &d);
    array_add(v1, &e);
    array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(101, result);
};

TEST_GROUP_C_SETUP(ArrayTestsArrayConf)
{
    array_conf_init(&vc);
    vc.capacity = 20;
    array_new_conf(&vc, &v1);

    ArrayConf vc1;
    array_conf_init(&vc1);
    vc1.capacity = 1;
    array_new_conf(&vc1, &v2);
};

TEST_GROUP_C_TEARDOWN(ArrayTestsArrayConf)
{
    array_destroy(v1);
    array_destroy(v2);
};

TEST_C(ArrayTestsArrayConf, ArrayAddAt)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);
    array_add(v1, &e);

    array_add_at(v1, &ins, 1);

    int *i0;
    int *i1;
    int *i2;
    int *i3;
    int *i4;

    array_get_at(v1, 0, (void*)&i0);
    array_get_at(v1, 1, (void*)&i1);
    array_get_at(v1, 2, (void*)&i2);
    array_get_at(v1, 3, (void*)&i3);
    array_get_at(v1, 4, (void*)&i4);

    CHECK_EQUAL_C_INT(a, *i0);
    CHECK_EQUAL_C_INT(ins, *i1);
    CHECK_EQUAL_C_INT(b, *i2);
    CHECK_EQUAL_C_INT(c, *i3);
};

TEST_C(ArrayTestsArrayConf, ArrayTrimCapacity)
{
    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v1, &a);
    array_add(v1, &b);
    array_add(v1, &c);

    CHECK_EQUAL_C_INT(20, array_capacity(v1));
    array_trim_capacity(v1);
    CHECK_EQUAL_C_INT(3, array_capacity(v1));
};

TEST_C(ArrayTestsArrayConf, ArrayCapacity)
{
    int a = 5;
    int b = 12;

    array_add(v2, (void*)&a);
    CHECK_EQUAL_C_INT(1, array_capacity(v2));

    array_add(v2, (void*)&b);
    CHECK_EQUAL_C_INT(2, array_capacity(v2));
};

TEST_GROUP_C_SETUP(ArrayTestsFilter)
{
    array_new(&v1);
    for(int i=0;i<20; i++) {
        int *v = (int*)malloc(sizeof(int));
        *v = i % 2;
        array_add(v1, (void*)v);
    }
};

TEST_GROUP_C_TEARDOWN(ArrayTestsFilter)
{
    array_destroy(v1);
};

TEST_C(ArrayTestsFilter, ArrayFilterMut1)
{
    int ret = array_filter_mut(v1, pred1);
    CHECK_EQUAL_C_INT(CC_OK, ret);

    CHECK_EQUAL_C_INT(10, array_size(v1));

    ArrayIter i;
    array_iter_init(&i, v1);
    void *e;
    while (array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(0, *(int*)e);
    }
};

TEST_C(ArrayTestsFilter, ArrayFilterMut2)
{
    array_filter_mut(v1, pred2);

    CHECK_EQUAL_C_INT(10, array_size(v1));

    ArrayIter i;
    array_iter_init(&i, v1);
    void *e;
    while(array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(1, *(int*)e);
    }
};

TEST_C(ArrayTestsFilter, ArrayFilter1)
{
    array_filter(v1, pred1, &v2);

    CHECK_EQUAL_C_INT(10, array_size(v2));

    ArrayIter i;
    array_iter_init(&i, v2);
    void *e;
    while (array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(0, *(int*)e);
    }

    array_destroy(v2);
};

TEST_C(ArrayTestsFilter, ArrayFilter2)
{
    CHECK_EQUAL_C_INT(20, array_size(v1));
    array_filter(v1, pred2, &v2);

    CHECK_EQUAL_C_INT(10, array_size(v2));

    ArrayIter i;
    array_iter_init(&i, v2);
    void *e;
    while(array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(1, *(int*)e);
    }

    array_destroy(v2);
};
