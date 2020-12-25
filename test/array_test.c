#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include "cc_array.h"
#include "CppUTest/TestHarness_c.h"

static CC_Array *v1;
static CC_Array *v2;
static int stat;
static CC_ArrayConf vc;

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

TEST_GROUP_C_SETUP(CC_ArrayTestsWithDefaults)
{
    stat = cc_array_new(&v1);
}

TEST_GROUP_C_TEARDOWN(CC_ArrayTestsWithDefaults)
{
    cc_array_destroy(v1);
}

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayAdd)
{
    int a = 5;
    int b = 12;
    int c = 848;
    CHECK_EQUAL_C_INT(CC_OK, stat);

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    int *ar;
    int *br;
    int *cr;

    cc_array_get_at(v1, 0, (void*)&ar);
    cc_array_get_at(v1, 1, (void*)&br);
    cc_array_get_at(v1, 2, (void*)&cr);

    CHECK_EQUAL_C_INT(a, *ar);
    CHECK_EQUAL_C_INT(b, *br);
    CHECK_EQUAL_C_INT(c, *cr);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayAddAt2)
{
    int a = 5;
    CHECK_EQUAL_C_INT(0, cc_array_size(v1));
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, cc_array_add_at(v1, &a, 1));
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayReplaceAt)
{
    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    cc_array_replace_at(v1, &replacement, 2, NULL);

    int *repl;
    cc_array_get_at(v1, 2, (void*)&repl);

    CHECK_C(*repl != c);
    CHECK_C(*repl == replacement);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayRemove)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);

    cc_array_remove(v1, &c, NULL);

    CHECK_EQUAL_C_INT(3, cc_array_size(v1));

    int *r;
    cc_array_get_at(v1, 2, (void*)&r);

    CHECK_EQUAL_C_POINTER(r, &e);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayRemoveAt)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);

    cc_array_remove_at(v1, 2, NULL);

    CHECK_EQUAL_C_INT(3, cc_array_size(v1));

    int *r;
    cc_array_get_at(v1, 2, (void*)&r);

    CHECK_EQUAL_C_POINTER(r, &e);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayRemoveAll)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);

    cc_array_remove_all(v1);

    CHECK_EQUAL_C_INT(0, cc_array_size(v1));
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayGetAt)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);

    int *ar;
    int *cr;
    cc_array_get_at(v1, 0, (void*)&ar);
    cc_array_get_at(v1, 2, (void*)&cr);

    CHECK_EQUAL_C_INT(a, *ar);
    CHECK_EQUAL_C_INT(c, *cr);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayIndexOf)
{
    int a = 23;
    int b = 32;
    int c = 55;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    size_t ai;
    cc_array_index_of(v1, &a, &ai);

    size_t ci;
    cc_array_index_of(v1, &c, &ci);

    CHECK_EQUAL_C_INT(0, ai);
    CHECK_EQUAL_C_INT(2, ci);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArraySubarray)
{
    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);
    cc_array_add(v1, &f);


    CC_Array *sub;
    cc_array_subarray(v1, 1, 3, &sub);

    CHECK_EQUAL_C_INT(3, cc_array_size(sub));

    int *s0;
    int *s1;
    int *s2;
    cc_array_get_at(sub, 0, (void*)&s0);
    cc_array_get_at(sub, 1, (void*)&s1);
    cc_array_get_at(sub, 2, (void*)&s2);

    CHECK_EQUAL_C_POINTER(&b, s0);
    CHECK_EQUAL_C_POINTER(&c, s1);
    CHECK_EQUAL_C_POINTER(&e, s2);

    cc_array_destroy(sub);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayShallowCopy)
{
    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    // CC_Array *cp;
    cc_array_copy_shallow(v1, &v2);

    CHECK_EQUAL_C_INT(cc_array_size(v2), cc_array_size(v1));

    int *ga;
    int *gb;
    cc_array_get_at(v1, 2, (void*)&ga);
    cc_array_get_at(v2, 2, (void*)&gb);

    CHECK_EQUAL_C_INT(*ga, *gb);

    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayDeepCopy)
{
    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    cc_array_copy_deep(v1, copy, &v2);

    CHECK_EQUAL_C_INT(cc_array_size(v2), cc_array_size(v1));

    int *ca;
    cc_array_get_at(v2, 0, (void*) &ca);

    CHECK_EQUAL_C_INT(a, *ca);
    cc_array_destroy_cb(v2, free);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayReverse)
{
    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    cc_array_reverse(v1);

    int *i0;
    int *i1;
    int *i2;
    cc_array_get_at(v1, 0, (void*)&i0);
    cc_array_get_at(v1, 1, (void*)&i1);
    cc_array_get_at(v1, 2, (void*)&i2);

    CHECK_EQUAL_C_INT(c, *i0);
    CHECK_EQUAL_C_INT(b, *i1);
    CHECK_EQUAL_C_INT(a, *i2);
};


TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayContains)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &c);

    int cc = cc_array_contains(v1, &c);
    int ca = cc_array_contains(v1, &a);
    int cd = cc_array_contains(v1, &d);

    CHECK_EQUAL_C_INT(2, cc);
    CHECK_EQUAL_C_INT(1, ca);
    CHECK_EQUAL_C_INT(0, cd);
};


TEST_C(CC_ArrayTestsWithDefaults, CC_ArraySort)
{
    srand(time(NULL));

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int *e = (int*)malloc(sizeof(int));
        *e = rand() % 100;
        cc_array_add(v1, (void*)e);
    }
    cc_array_sort(v1, comp);

    int *prev;
    cc_array_get_at(v1, 0, (void**)&prev);
    for (i = 0; i < size; i++) {
        int *e;
        cc_array_get_at(v1, i, (void*)&e);
        CHECK_C(*prev <= *e);
        prev = e;
    }
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayIterRemove)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, v1);

    int *e;
    while (cc_array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            cc_array_iter_remove(&iter, NULL);
    }

    CHECK_EQUAL_C_INT(0, cc_array_contains(v1, &c));
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayIterAdd)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    int N = 80;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, v1);

    int *e;
    while (cc_array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3)
            cc_array_iter_add(&iter, &N);
    }

    CHECK_EQUAL_C_INT(5, cc_array_size(v1));

    void *n;
    cc_array_get_at(v1, 3, &n);
    CHECK_EQUAL_C_INT(N, *((int*)n));
    CHECK_EQUAL_C_INT(1, cc_array_contains(v1, &N));

    cc_array_get_at(v1, 4, (void*)&n);
    CHECK_EQUAL_C_INT(4, *((int*)n));
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayIterReplace)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int replacement = 42;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &d);

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, v1);

    int *e;
    int *old;
    while (cc_array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            cc_array_iter_replace(&iter, (void*)&replacement, (void*)&old);
    }

    size_t index;
    cc_array_index_of(v1, (void*) &replacement, &index);

    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(0, cc_array_contains(v1, &c));
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayZipIterNext)
{
    cc_array_add(v1, "a");
    cc_array_add(v1, "b");
    cc_array_add(v1, "c");
    cc_array_add(v1, "d");

    cc_array_new(&v2);

    cc_array_add(v2, "e");
    cc_array_add(v2, "f");
    cc_array_add(v2, "g");

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, v1, v2);

    size_t i = 0;

    void *e1, *e2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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
    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayZipIterRemove)
{
    cc_array_add(v1, "a");
    cc_array_add(v1, "b");
    cc_array_add(v1, "c");
    cc_array_add(v1, "d");

    cc_array_new(&v2);

    cc_array_add(v2, "e");
    cc_array_add(v2, "f");
    cc_array_add(v2, "g");

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_array_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*) r1);
    CHECK_EQUAL_C_INT(0, cc_array_contains(v1, "b"));
    CHECK_EQUAL_C_INT(0, cc_array_contains(v2, "f"));
    CHECK_EQUAL_C_INT(3, cc_array_size(v1));
    CHECK_EQUAL_C_INT(2, cc_array_size(v2));

    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayZipIterAdd)
{
    cc_array_add(v1, "a");
    cc_array_add(v1, "b");
    cc_array_add(v1, "c");
    cc_array_add(v1, "d");

    cc_array_new(&v2);

    cc_array_add(v2, "e");
    cc_array_add(v2, "f");
    cc_array_add(v2, "g");

    char *h = "h";
    char *i = "i";

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_array_zip_iter_add(&zip, h, i);
    }

    size_t index;

    CHECK_EQUAL_C_INT(CC_OK, cc_array_index_of(v1, "h", &index));
    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(CC_OK, cc_array_index_of(v2, "i", &index));
    CHECK_EQUAL_C_INT(2, index);
    CHECK_EQUAL_C_INT(CC_OK, cc_array_index_of(v1, "c", &index));
    CHECK_EQUAL_C_INT(3, index);
    CHECK_EQUAL_C_INT(1, cc_array_contains(v1, "h"));
    CHECK_EQUAL_C_INT(1, cc_array_contains(v2, "i"));
    CHECK_EQUAL_C_INT(5, cc_array_size(v1));
    CHECK_EQUAL_C_INT(4, cc_array_size(v2));

    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayZipIterReplace)
{
    cc_array_add(v1, "a");
    cc_array_add(v1, "b");
    cc_array_add(v1, "c");
    cc_array_add(v1, "d");

    cc_array_new(&v2);

    cc_array_add(v2, "e");
    cc_array_add(v2, "f");
    cc_array_add(v2, "g");

    char *h = "h";
    char *i = "i";

    CC_ArrayZipIter zip;
    cc_array_zip_iter_init(&zip, v1, v2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0) {
            cc_array_zip_iter_replace(&zip, h, i, &r1, &r2);
        }
    }

    size_t index;

    CHECK_EQUAL_C_INT(CC_OK, cc_array_index_of(v1, "h", &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(CC_OK, cc_array_index_of(v2, i, &index));
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, cc_array_contains(v1, "h"));
    CHECK_EQUAL_C_INT(1, cc_array_contains(v2, "i"));

    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsWithDefaults, CC_ArrayReduce)
{
    int a = 1;
    int b = 78;
    int c = 12;
    int d = 2;
    int e = 8;

    cc_array_add(v1, &a);
    int result;
    cc_array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(1, result);

    cc_array_add(v1, &b);
    cc_array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(79, result);

    cc_array_add(v1, &c);
    cc_array_add(v1, &d);
    cc_array_add(v1, &e);
    cc_array_reduce(v1, reduce_add, (void*)&result);

    CHECK_EQUAL_C_INT(101, result);
};

TEST_GROUP_C_SETUP(CC_ArrayTestsCC_ArrayConf)
{
    cc_array_conf_init(&vc);
    vc.capacity = 20;
    cc_array_new_conf(&vc, &v1);

    CC_ArrayConf vc1;
    cc_array_conf_init(&vc1);
    vc1.capacity = 1;
    cc_array_new_conf(&vc1, &v2);
};

TEST_GROUP_C_TEARDOWN(CC_ArrayTestsCC_ArrayConf)
{
    cc_array_destroy(v1);
    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsCC_ArrayConf, CC_ArrayAddAt)
{
    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);
    cc_array_add(v1, &e);

    cc_array_add_at(v1, &ins, 1);

    int *i0;
    int *i1;
    int *i2;
    int *i3;
    int *i4;

    cc_array_get_at(v1, 0, (void*)&i0);
    cc_array_get_at(v1, 1, (void*)&i1);
    cc_array_get_at(v1, 2, (void*)&i2);
    cc_array_get_at(v1, 3, (void*)&i3);
    cc_array_get_at(v1, 4, (void*)&i4);

    CHECK_EQUAL_C_INT(a, *i0);
    CHECK_EQUAL_C_INT(ins, *i1);
    CHECK_EQUAL_C_INT(b, *i2);
    CHECK_EQUAL_C_INT(c, *i3);
};

TEST_C(CC_ArrayTestsCC_ArrayConf, CC_ArrayTrimCapacity)
{
    int a = 5;
    int b = 12;
    int c = 848;

    cc_array_add(v1, &a);
    cc_array_add(v1, &b);
    cc_array_add(v1, &c);

    CHECK_EQUAL_C_INT(20, cc_array_capacity(v1));
    cc_array_trim_capacity(v1);
    CHECK_EQUAL_C_INT(3, cc_array_capacity(v1));
};

TEST_C(CC_ArrayTestsCC_ArrayConf, CC_ArrayCapacity)
{
    int a = 5;
    int b = 12;

    cc_array_add(v2, (void*)&a);
    CHECK_EQUAL_C_INT(1, cc_array_capacity(v2));

    cc_array_add(v2, (void*)&b);
    CHECK_EQUAL_C_INT(2, cc_array_capacity(v2));
};

TEST_GROUP_C_SETUP(CC_ArrayTestsFilter)
{
    cc_array_new(&v1);
    for(int i=0;i<20; i++) {
        int *v = (int*)malloc(sizeof(int));
        *v = i % 2;
        cc_array_add(v1, (void*)v);
    }
};

TEST_GROUP_C_TEARDOWN(CC_ArrayTestsFilter)
{
    cc_array_destroy(v1);
};

TEST_C(CC_ArrayTestsFilter, CC_ArrayFilterMut1)
{
    int ret = cc_array_filter_mut(v1, pred1);
    CHECK_EQUAL_C_INT(CC_OK, ret);

    CHECK_EQUAL_C_INT(10, cc_array_size(v1));

    CC_ArrayIter i;
    cc_array_iter_init(&i, v1);
    void *e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(0, *(int*)e);
    }
};

TEST_C(CC_ArrayTestsFilter, CC_ArrayFilterMut2)
{
    cc_array_filter_mut(v1, pred2);

    CHECK_EQUAL_C_INT(10, cc_array_size(v1));

    CC_ArrayIter i;
    cc_array_iter_init(&i, v1);
    void *e;
    while(cc_array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(1, *(int*)e);
    }
};

TEST_C(CC_ArrayTestsFilter, CC_ArrayFilter1)
{
    cc_array_filter(v1, pred1, &v2);

    CHECK_EQUAL_C_INT(10, cc_array_size(v2));

    CC_ArrayIter i;
    cc_array_iter_init(&i, v2);
    void *e;
    while (cc_array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(0, *(int*)e);
    }

    cc_array_destroy(v2);
};

TEST_C(CC_ArrayTestsFilter, CC_ArrayFilter2)
{
    CHECK_EQUAL_C_INT(20, cc_array_size(v1));
    cc_array_filter(v1, pred2, &v2);

    CHECK_EQUAL_C_INT(10, cc_array_size(v2));

    CC_ArrayIter i;
    cc_array_iter_init(&i, v2);
    void *e;
    while(cc_array_iter_next(&i, &e) != CC_ITER_END) {
        CHECK_EQUAL_C_INT(1, *(int*)e);
    }

    cc_array_destroy(v2);
};
