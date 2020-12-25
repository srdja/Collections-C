#include "CppUTest/TestHarness_c.h"
#include "cc_deque.h"

static CC_Deque *deque;
static CC_DequeConf conf;
int stat;

void *cpy(void *e)
{
    int  o = *((int*) e);
    int *c = malloc(sizeof(int));
    *c = o;
    return (void*) c;
}

bool pred1(const void *e)
{
    return *(int*)e <= 3;
}

bool pred2(const void *e)
{
    return *(int*)e > 3;
}

bool pred3(const void *e)
{
    return *(int*)e > 5;
}

TEST_GROUP_C_SETUP(CC_DequeTests)
{
  stat = cc_deque_new(&deque);
};

TEST_GROUP_C_TEARDOWN(CC_DequeTests)
{
  cc_deque_destroy(deque);
};

TEST_C(CC_DequeTests, CC_DequeAddFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_first(deque, &b);
    cc_deque_add_first(deque, &c);

    CHECK_EQUAL_C_INT(3, cc_deque_size(deque));

    size_t m = cc_deque_capacity(deque);
    const void * const* u = cc_deque_get_buffer(deque);
    const void *e = u[m - 1];

    CHECK_EQUAL_C_POINTER(e, &a);

    e = u[m - 2];
    CHECK_EQUAL_C_POINTER(e, &b);

    e = u[m - 3];
    CHECK_EQUAL_C_POINTER(e, &c);
};

TEST_C(CC_DequeTests, CC_DequeAddLast)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);

    CHECK_EQUAL_C_INT(3, cc_deque_size(deque));

    const void * const* u= cc_deque_get_buffer(deque);
    const void *e = u[0];

    CHECK_EQUAL_C_POINTER(e, &a);

    e = u[1];
    CHECK_EQUAL_C_POINTER(e, &b);

    e = u[2];
    CHECK_EQUAL_C_POINTER(e, &c);
};


TEST_C(CC_DequeTests, CC_DequeAddAt1)
{
    /* index < (size / 2) && index_raw > first_raw */

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

    const void * const* buff = cc_deque_get_buffer(deque);

    CHECK_EQUAL_C_POINTER(buff[4], &g);

    CHECK_EQUAL_C_POINTER(buff[5], &e);

    const void *elem = buff[6];
    CHECK_EQUAL_C_POINTER(elem, &f);
};

TEST_C(CC_DequeTests, CC_DequeAddAt2)
{
    /* index < deque->size / 2 && index_raw > first_raw */

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

    const void * const *buff = cc_deque_get_buffer(deque);
    const void *elem = buff[5];

    CHECK_EQUAL_C_POINTER(elem, &g);

    const void *elem1 = buff[0];
    CHECK_EQUAL_C_POINTER(elem1, &a);

    const void *elem2 = buff[7];
    CHECK_EQUAL_C_POINTER(elem2, &c);

    const void *elem3 = buff[3];
    CHECK_EQUAL_C_POINTER(elem3, &f);
};

TEST_C(CC_DequeTests, CC_DequeAddAt3)
{
    /* index >= size / 2 && index_raw > last_raw */

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

    const void * const* buff = cc_deque_get_buffer(deque);

    const void *elem = buff[6];
    CHECK_EQUAL_C_POINTER(elem, &g);

    const void *elem1 = buff[0];
    CHECK_EQUAL_C_POINTER(elem1, &b);

    const void *elem2 = buff[7];
    CHECK_EQUAL_C_POINTER(elem2, &c);

    const void *elem3 = buff[1];
    CHECK_EQUAL_C_POINTER(elem3, &a);
};

TEST_C(CC_DequeTests, CC_DequeAddAt4)
{
    /* index < size / 2 && index_raw < last_raw*/

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

    const void * const*buff = cc_deque_get_buffer(deque);
    const int elem = *((int*) buff[0]);

    CHECK_EQUAL_C_INT(elem, g);

    const int elem1 = *((int*) buff[4]);
    CHECK_EQUAL_C_INT(elem1, e);

    const int elem2 = *((int*) buff[6]);
    CHECK_EQUAL_C_INT(elem2, f);

    const int elem3 = *((int*) buff[7]);
    CHECK_EQUAL_C_INT(elem3, a);
};

TEST_C(CC_DequeTests, CC_DequeAddAt5)
{
    /* f == 0*/

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

    const void * const* buff = cc_deque_get_buffer(deque);
    const int elem = *((int*) buff[7]);

    CHECK_EQUAL_C_INT(elem, a);

    const int elem1 = *((int*) buff[0]);
    CHECK_EQUAL_C_INT(elem1, b);

    const int elem2 = *((int*) buff[5]);
    CHECK_EQUAL_C_INT(elem2, f);

    const int elem3 = *((int*) buff[1]);
    CHECK_EQUAL_C_INT(elem3, g);
};

TEST_C(CC_DequeTests, CC_DequeRemoveFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);

    int *first;
    cc_deque_get_first(deque, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    int *removed;
    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(a, *removed);

    cc_deque_get_first(deque, (void*) &first);
    CHECK_EQUAL_C_INT(b, *first);
};

TEST_C(CC_DequeTests, CC_DequeRemoveLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add_first(deque, &a);
    cc_deque_add_first(deque, &b);
    cc_deque_add_last(deque, &c);
    cc_deque_add_last(deque, &d);

    int *last;
    cc_deque_get_last(deque, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    int *removed;
    cc_deque_remove_last(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(d, *removed);

    cc_deque_get_last(deque, (void*) &last);
    CHECK_EQUAL_C_INT(c, *last);
};

TEST_C(CC_DequeTests, CC_DequeRemoveAll)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    cc_deque_remove_all(deque);

    void *first;
    int stat1 = cc_deque_get_first(deque, &first);
    void *last;
    int stat2 = cc_deque_get_last(deque, &last);

    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, stat1);
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, stat2);

    CHECK_EQUAL_C_INT(0, cc_deque_size(deque));
};

TEST_C(CC_DequeTests, CC_DequeGetAt)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    void *e;
    cc_deque_get_at(deque, 1, &e);
    void *n;
    int status = cc_deque_get_at(deque, 42, &n);

    CHECK_EQUAL_C_INT(b, *(int*)e);
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, status);
};

TEST_C(CC_DequeTests, CC_DequeGetFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);

    int *first;
    cc_deque_get_first(deque, (void*) &first);

    CHECK_EQUAL_C_INT(c, *first);
};

TEST_C(CC_DequeTests, CC_DequeGetLast)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_first(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_first(deque, &c);

    int *last;
    cc_deque_get_last(deque, (void*) &last);

    CHECK_EQUAL_C_INT(b, *last);
};

TEST_C(CC_DequeTests, CC_DequeCopyShallow)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add_last(deque, &a);
    cc_deque_add_last(deque, &b);
    cc_deque_add_last(deque, &c);

    CC_Deque *copy;
    cc_deque_copy_shallow(deque, &copy);

    int size = cc_deque_size(copy);
    CHECK_EQUAL_C_INT(3, size);

    int *ca;
    cc_deque_get_at(copy, 0, (void*)&ca);

    int *cb;
    cc_deque_get_at(copy, 1, (void*)&cb);

    int *cc;
    cc_deque_get_at(copy, 2, (void*)&cc);

    CHECK_EQUAL_C_INT(a, *ca);
    CHECK_EQUAL_C_INT(b, *cb);
    CHECK_EQUAL_C_INT(c, *cc);
    cc_deque_destroy(copy);
};

TEST_C(CC_DequeTests, CC_DequeCopyDeep)
{
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    cc_deque_add_last(deque, a);
    cc_deque_add_last(deque, b);
    cc_deque_add_last(deque, c);

    CC_Deque *copy;
    cc_deque_copy_deep(deque, cpy, &copy);

    int size = cc_deque_size(copy);
    CHECK_EQUAL_C_INT(3, size);

    int *ca;
    cc_deque_get_at(copy, 0, (void*)&ca);
    int *cb;
    cc_deque_get_at(copy, 1, (void*)&cb);
    int *cc;
    cc_deque_get_at(copy, 2, (void*)&cc);

    CHECK_EQUAL_C_INT(1, *ca);
    CHECK_EQUAL_C_INT(2, *cb);
    CHECK_EQUAL_C_INT(3, *cc);
    cc_deque_destroy_cb(copy, free);
    free(a);
    free(b);
    free(c);
}


TEST_C(CC_DequeTests, CC_DequeContains)
{
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

    CHECK_EQUAL_C_INT(2, cc_deque_contains(deque, &a));
    CHECK_EQUAL_C_INT(0, cc_deque_contains(deque, &g));
    CHECK_EQUAL_C_INT(1, cc_deque_contains(deque, &e));
};

TEST_C(CC_DequeTests, CC_DequeSize)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);
    cc_deque_add(deque, &d);

    size_t size = cc_deque_size(deque);
    CHECK_EQUAL_C_INT(4, size);
};

TEST_C(CC_DequeTests, CC_DequeCapacity)
{
    CC_DequeConf conf;
    cc_deque_conf_init(&conf);

    conf.capacity = 2;

    CC_Deque *deque;
    cc_deque_new_conf(&conf, &deque);

    CHECK_EQUAL_C_INT(2, cc_deque_capacity(deque));

    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    CHECK_EQUAL_C_INT(4, cc_deque_capacity(deque));
}


TEST_C(CC_DequeTests, CC_DequeTrimCapacity)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    CHECK_EQUAL_C_INT(8, cc_deque_capacity(deque));

    cc_deque_trim_capacity(deque);

    CHECK_EQUAL_C_INT(4, cc_deque_capacity(deque));
};

TEST_C(CC_DequeTests, CC_DequeReverse)
{
    int a = 1;
    int b = 2;
    int c = 3;

    cc_deque_add(deque, &a);
    cc_deque_add(deque, &b);
    cc_deque_add(deque, &c);

    cc_deque_reverse(deque);

    int *ra;
    cc_deque_get_at(deque, 0, (void*)&ra);
    int *rb;
    cc_deque_get_at(deque, 1, (void*)&rb);
    int *rc;
    cc_deque_get_at(deque, 2, (void*)&rc);

    CHECK_EQUAL_C_INT(c, *ra);
    CHECK_EQUAL_C_INT(b, *rb);
    CHECK_EQUAL_C_INT(a, *rc);
};

TEST_C(CC_DequeTests, CC_DequeIteratorAdd)
{
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

    int *el;

    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    while (cc_deque_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == d)
            cc_deque_iter_add(&iter, &g);
        if (i >= 3) {
          CHECK_EQUAL_C_INT(i, cc_deque_iter_index(&iter) - 1);
        }
        i++;
    }
    CHECK_EQUAL_C_INT(7, cc_deque_size(deque));

    void *ret;
    cc_deque_get_at(deque, 4, &ret);
    CHECK_EQUAL_C_INT(g, *(int*)ret);
};

TEST_C(CC_DequeTests, CC_DequeIteratorRemove)
{
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
    void *el;
    while (cc_deque_iter_next(&iter, &el) != CC_ITER_END) {
        if (i == 3)
            cc_deque_iter_remove(&iter, NULL);

        if (i > 2) {
          CHECK_EQUAL_C_INT(5, cc_deque_size(deque));
        } else {
          CHECK_EQUAL_C_INT(6, cc_deque_size(deque));
        }
        if (i >= 3) {
          CHECK_EQUAL_C_INT(i-1, cc_deque_iter_index(&iter));
        }
        i++;
    }
};

TEST_C(CC_DequeTests, CC_DequeIteratorNext)
{
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

    void *el;
    while (cc_deque_iter_next(&iter, &el) != CC_ITER_END) {
        void *k;
        cc_deque_get_at(deque, i, &k);
        CHECK_EQUAL_C_POINTER(k, el);
        i++;

        CHECK_EQUAL_C_INT(i, iter.index);
    }
};

TEST_C(CC_DequeTests, CC_DequeZipIterRemove)
{
    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque *d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_deque_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, cc_deque_contains(deque, "b"));
    CHECK_EQUAL_C_INT(0, cc_deque_contains(deque, "f"));
    CHECK_EQUAL_C_INT(3, cc_deque_size(deque));
    CHECK_EQUAL_C_INT(2, cc_deque_size(d2));
    cc_deque_destroy(d2);
};

TEST_C(CC_DequeTests, CC_DequeZipIterAdd)
{
    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque *d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_deque_zip_iter_add(&zip, h, i);
    }

    size_t index;
    cc_deque_index_of(deque, "h", &index);

    CHECK_EQUAL_C_INT(2, index);

    cc_deque_index_of(deque, "i", &index);
    CHECK_EQUAL_C_INT(2, index);

    cc_deque_index_of(deque, "c", &index);
    CHECK_EQUAL_C_INT(3, index);
    CHECK_EQUAL_C_INT(1, cc_deque_contains(deque, "h"));
    CHECK_EQUAL_C_INT(1, cc_deque_contains(d2, "i"));
    CHECK_EQUAL_C_INT(5, cc_deque_size(deque));
    CHECK_EQUAL_C_INT(4, cc_deque_size(d2));
    cc_deque_destroy(d2);
};


TEST_C(CC_DequeTests, CC_DequeZipIterNext)
{
    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque *d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    size_t i = 0;

    void *e1, *e2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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
    cc_deque_destroy(d2);
};

TEST_C(CC_DequeTests, CC_DequeZipIterReplace)
{
    cc_deque_add(deque, "a");
    cc_deque_add(deque, "b");
    cc_deque_add(deque, "c");
    cc_deque_add(deque, "d");

    CC_Deque *d2;
    cc_deque_new(&d2);

    cc_deque_add(d2, "e");
    cc_deque_add(d2, "f");
    cc_deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    CC_DequeZipIter zip;
    cc_deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (cc_deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            cc_deque_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    cc_deque_index_of(deque, "h", &index);

    CHECK_EQUAL_C_INT(1, index);

    cc_deque_index_of(deque, "i", &index);
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, cc_deque_contains(deque, "h"));
    CHECK_EQUAL_C_INT(1, cc_deque_contains(d2, "i"));
    cc_deque_destroy(d2);
};

TEST_GROUP_C_SETUP(CC_DequeTestsConf)
{
  cc_deque_conf_init(&conf);
  conf.capacity = 4;
  cc_deque_new_conf(&conf, &deque);
};

TEST_GROUP_C_TEARDOWN(CC_DequeTestsConf)
{
  cc_deque_destroy(deque);
};

TEST_C(CC_DequeTestsConf, CC_DequeBufferExpansion)
{
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

    CHECK_EQUAL_C_INT(4, capacity);

    /* Current layout:
       _________________
       | b | d | c | a |
       -----------------
             L   F
     */
    /* This line should trigger the buffer expansion */
    cc_deque_add_first(deque, &e);

    capacity = cc_deque_capacity(deque);
    CHECK_EQUAL_C_INT(8, capacity);

    /* The expansion should align the elements.*/
    const void * const* buff = cc_deque_get_buffer(deque);
    const int elem = *((int*) buff[0]);

    CHECK_EQUAL_C_INT(elem, c);

    const int elem1 = *((int*) buff[1]);
    CHECK_EQUAL_C_INT(elem1, a);

    const int elem2 = *((int*) buff[2]);
    CHECK_EQUAL_C_INT(elem2, b);

    const int elem3 = *((int*) buff[3]);
    CHECK_EQUAL_C_INT(elem3, d);

    const int elem4 = *((int*) buff[7]);
    CHECK_EQUAL_C_INT(elem4, e);

    cc_deque_add_last(deque, &f);

    const int elem5 = *((int*) buff[4]);
    CHECK_EQUAL_C_INT(elem5, f);
};

TEST_C(CC_DequeTests, CC_DequeFilter1)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    CC_Deque *filter = NULL;
    cc_deque_filter(deque, pred1, &filter);
    CHECK_EQUAL_C_INT(3, cc_deque_size(filter));
    const void * const* buff = cc_deque_get_buffer(filter);

    CHECK_EQUAL_C_POINTER(buff[0], &a);
    CHECK_EQUAL_C_POINTER(buff[1], &b);

    const void *elem = buff[2];
    CHECK_EQUAL_C_POINTER(elem, &c);
    free(filter);
};

TEST_C(CC_DequeTests, CC_DequeFilter2)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    CC_Deque *filter = NULL;
    cc_deque_filter(deque, pred2, &filter);
    const void * const* buff = cc_deque_get_buffer(filter);

    CHECK_EQUAL_C_INT(3, cc_deque_size(filter));
    CHECK_EQUAL_C_POINTER(buff[0], &d);
    CHECK_EQUAL_C_POINTER(buff[1], &e);
    CHECK_EQUAL_C_POINTER(buff[2], &f);

    free(filter);
};

TEST_C(CC_DequeTests, CC_DequeFilter3)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    CC_Deque *filter = NULL;
    cc_deque_filter(deque, pred3, &filter);
    const void * const* buff = cc_deque_get_buffer(filter);

    CHECK_EQUAL_C_INT(1, cc_deque_size(filter));
    CHECK_EQUAL_C_POINTER(buff[0], &f);

    free(filter);
};

TEST_C(CC_DequeTests, CC_DequeFilterMut1)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred1);
    CHECK_EQUAL_C_INT(3, cc_deque_size(deque));

    int *removed = NULL;
    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(a, *removed);

    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(b, *removed);

    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(c, *removed);
};

TEST_C(CC_DequeTests, CC_DequeFilterMut2)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred2);
    CHECK_EQUAL_C_INT(3, cc_deque_size(deque));

    int *removed = NULL;
    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(d, *removed);

    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(e, *removed);

    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(f, *removed);
};


TEST_C(CC_DequeTests, CC_DequeFilterMut3)
{
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
    CHECK_EQUAL_C_INT(6, cc_deque_size(deque));

    cc_deque_filter_mut(deque, pred3);
    CHECK_EQUAL_C_INT(1, cc_deque_size(deque));

    int *removed = NULL;
    cc_deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(f, *removed);
};
