#include "CppUTest/TestHarness_c.h"
#include "deque.h"

static Deque *deque;
static DequeConf conf;
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

TEST_GROUP_C_SETUP(DequeTests)
{
  stat = deque_new(&deque);
};

TEST_GROUP_C_TEARDOWN(DequeTests)
{
  deque_destroy(deque);
};

TEST_C(DequeTests, DequeAddFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add_first(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);

    CHECK_EQUAL_C_INT(3, deque_size(deque));

    size_t m = deque_capacity(deque);
    const void * const* u = deque_get_buffer(deque);
    const void *e = u[m - 1];

    CHECK_EQUAL_C_POINTER(e, &a);

    e = u[m - 2];
    CHECK_EQUAL_C_POINTER(e, &b);

    e = u[m - 3];
    CHECK_EQUAL_C_POINTER(e, &c);
};

TEST_C(DequeTests, DequeAddLast)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);

    CHECK_EQUAL_C_INT(3, deque_size(deque));

    const void * const* u= deque_get_buffer(deque);
    const void *e = u[0];

    CHECK_EQUAL_C_POINTER(e, &a);

    e = u[1];
    CHECK_EQUAL_C_POINTER(e, &b);

    e = u[2];
    CHECK_EQUAL_C_POINTER(e, &c);
};


TEST_C(DequeTests, DequeAddAt1)
{
    /* index < (size / 2) && index_raw > first_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);

    deque_add_at(deque, &g, 4);

    const void * const* buff = deque_get_buffer(deque);

    CHECK_EQUAL_C_POINTER(buff[4], &g);

    CHECK_EQUAL_C_POINTER(buff[5], &e);

    const void *elem = buff[6];
    CHECK_EQUAL_C_POINTER(elem, &f);
};

TEST_C(DequeTests, DequeAddAt2)
{
    /* index < deque->size / 2 && index_raw > first_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const *buff = deque_get_buffer(deque);
    const void *elem = buff[5];

    CHECK_EQUAL_C_POINTER(elem, &g);

    const void *elem1 = buff[0];
    CHECK_EQUAL_C_POINTER(elem1, &a);

    const void *elem2 = buff[7];
    CHECK_EQUAL_C_POINTER(elem2, &c);

    const void *elem3 = buff[3];
    CHECK_EQUAL_C_POINTER(elem3, &f);
};

TEST_C(DequeTests, DequeAddAt3)
{
    /* index >= size / 2 && index_raw > last_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    deque_add_last(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 3);

    const void * const* buff = deque_get_buffer(deque);

    const void *elem = buff[6];
    CHECK_EQUAL_C_POINTER(elem, &g);

    const void *elem1 = buff[0];
    CHECK_EQUAL_C_POINTER(elem1, &b);

    const void *elem2 = buff[7];
    CHECK_EQUAL_C_POINTER(elem2, &c);

    const void *elem3 = buff[1];
    CHECK_EQUAL_C_POINTER(elem3, &a);
};

TEST_C(DequeTests, DequeAddAt4)
{
    /* index < size / 2 && index_raw < last_raw*/

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const*buff = deque_get_buffer(deque);
    const int elem = *((int*) buff[0]);

    CHECK_EQUAL_C_INT(elem, g);

    const int elem1 = *((int*) buff[4]);
    CHECK_EQUAL_C_INT(elem1, e);

    const int elem2 = *((int*) buff[6]);
    CHECK_EQUAL_C_INT(elem2, f);

    const int elem3 = *((int*) buff[7]);
    CHECK_EQUAL_C_INT(elem3, a);
};

TEST_C(DequeTests, DequeAddAt5)
{
    /* f == 0*/

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const* buff = deque_get_buffer(deque);
    const int elem = *((int*) buff[7]);

    CHECK_EQUAL_C_INT(elem, a);

    const int elem1 = *((int*) buff[0]);
    CHECK_EQUAL_C_INT(elem1, b);

    const int elem2 = *((int*) buff[5]);
    CHECK_EQUAL_C_INT(elem2, f);

    const int elem3 = *((int*) buff[1]);
    CHECK_EQUAL_C_INT(elem3, g);
};

TEST_C(DequeTests, DequeRemoveFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);

    int *first;
    deque_get_first(deque, (void*) &first);
    CHECK_EQUAL_C_INT(a, *first);

    int *removed;
    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(a, *removed);

    deque_get_first(deque, (void*) &first);
    CHECK_EQUAL_C_INT(b, *first);
};

TEST_C(DequeTests, DequeRemoveLast)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    deque_add_first(deque, &a);
    deque_add_first(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);

    int *last;
    deque_get_last(deque, (void*) &last);
    CHECK_EQUAL_C_INT(d, *last);

    int *removed;
    deque_remove_last(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(d, *removed);

    deque_get_last(deque, (void*) &last);
    CHECK_EQUAL_C_INT(c, *last);
};

TEST_C(DequeTests, DequeRemoveAll)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    deque_remove_all(deque);

    void *first;
    int stat1 = deque_get_first(deque, &first);
    void *last;
    int stat2 = deque_get_last(deque, &last);

    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, stat1);
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, stat2);

    CHECK_EQUAL_C_INT(0, deque_size(deque));
};

TEST_C(DequeTests, DequeGetAt)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    void *e;
    deque_get_at(deque, 1, &e);
    void *n;
    int status = deque_get_at(deque, 42, &n);

    CHECK_EQUAL_C_INT(b, *(int*)e);
    CHECK_EQUAL_C_INT(CC_ERR_OUT_OF_RANGE, status);
};

TEST_C(DequeTests, DequeGetFirst)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);

    int *first;
    deque_get_first(deque, (void*) &first);

    CHECK_EQUAL_C_INT(c, *first);
};

TEST_C(DequeTests, DequeGetLast)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);

    int *last;
    deque_get_last(deque, (void*) &last);

    CHECK_EQUAL_C_INT(b, *last);
};

TEST_C(DequeTests, DequeCopyShallow)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);

    Deque *copy;
    deque_copy_shallow(deque, &copy);

    int size = deque_size(copy);
    CHECK_EQUAL_C_INT(3, size);

    int *ca;
    deque_get_at(copy, 0, (void*)&ca);

    int *cb;
    deque_get_at(copy, 1, (void*)&cb);

    int *cc;
    deque_get_at(copy, 2, (void*)&cc);

    CHECK_EQUAL_C_INT(a, *ca);
    CHECK_EQUAL_C_INT(b, *cb);
    CHECK_EQUAL_C_INT(c, *cc);
    deque_destroy(copy);
};

TEST_C(DequeTests, DequeCopyDeep)
{
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    deque_add_last(deque, a);
    deque_add_last(deque, b);
    deque_add_last(deque, c);

    Deque *copy;
    deque_copy_deep(deque, cpy, &copy);

    int size = deque_size(copy);
    CHECK_EQUAL_C_INT(3, size);

    int *ca;
    deque_get_at(copy, 0, (void*)&ca);
    int *cb;
    deque_get_at(copy, 1, (void*)&cb);
    int *cc;
    deque_get_at(copy, 2, (void*)&cc);

    CHECK_EQUAL_C_INT(1, *ca);
    CHECK_EQUAL_C_INT(2, *cb);
    CHECK_EQUAL_C_INT(3, *cc);
    deque_destroy_cb(copy, free);
    free(a);
    free(b);
    free(c);
}


TEST_C(DequeTests, DequeContains)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);
    deque_add(deque, &e);
    deque_add(deque, &f);
    deque_add(deque, &a);

    CHECK_EQUAL_C_INT(2, deque_contains(deque, &a));
    CHECK_EQUAL_C_INT(0, deque_contains(deque, &g));
    CHECK_EQUAL_C_INT(1, deque_contains(deque, &e));
};

TEST_C(DequeTests, DequeSize)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);

    size_t size = deque_size(deque);
    CHECK_EQUAL_C_INT(4, size);
};

TEST_C(DequeTests, DequeCapacity)
{
    DequeConf conf;
    deque_conf_init(&conf);

    conf.capacity = 2;

    Deque *deque;
    deque_new_conf(&conf, &deque);

    CHECK_EQUAL_C_INT(2, deque_capacity(deque));

    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    CHECK_EQUAL_C_INT(4, deque_capacity(deque));
}


TEST_C(DequeTests, DequeTrimCapacity)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    CHECK_EQUAL_C_INT(8, deque_capacity(deque));

    deque_trim_capacity(deque);

    CHECK_EQUAL_C_INT(4, deque_capacity(deque));
};

TEST_C(DequeTests, DequeReverse)
{
    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    deque_reverse(deque);

    int *ra;
    deque_get_at(deque, 0, (void*)&ra);
    int *rb;
    deque_get_at(deque, 1, (void*)&rb);
    int *rc;
    deque_get_at(deque, 2, (void*)&rc);

    CHECK_EQUAL_C_INT(c, *ra);
    CHECK_EQUAL_C_INT(b, *rb);
    CHECK_EQUAL_C_INT(a, *rc);
};

TEST_C(DequeTests, DequeIteratorAdd)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    int g = 7;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);
    deque_add(deque, &e);
    deque_add(deque, &f);

    DequeIter iter;
    deque_iter_init(&iter, deque);

    size_t i = 0;

    int *el;

    CHECK_EQUAL_C_INT(6, deque_size(deque));

    while (deque_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == d)
            deque_iter_add(&iter, &g);
        if (i >= 3) {
          CHECK_EQUAL_C_INT(i, deque_iter_index(&iter) - 1);
        }
        i++;
    }
    CHECK_EQUAL_C_INT(7, deque_size(deque));

    void *ret;
    deque_get_at(deque, 4, &ret);
    CHECK_EQUAL_C_INT(g, *(int*)ret);
};

TEST_C(DequeTests, DequeIteratorRemove)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);
    deque_add(deque, &e);
    deque_add(deque, &f);

    DequeIter iter;
    deque_iter_init(&iter, deque);

    size_t i = 0;
    void *el;
    while (deque_iter_next(&iter, &el) != CC_ITER_END) {
        if (i == 3)
            deque_iter_remove(&iter, NULL);

        if (i > 2) {
          CHECK_EQUAL_C_INT(5, deque_size(deque));
        } else {
          CHECK_EQUAL_C_INT(6, deque_size(deque));
        }
        if (i >= 3) {
          CHECK_EQUAL_C_INT(i-1, deque_iter_index(&iter));
        }
        i++;
    }
};

TEST_C(DequeTests, DequeIteratorNext)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);
    deque_add(deque, &e);
    deque_add(deque, &f);

    DequeIter iter;
    deque_iter_init(&iter, deque);

    size_t i = 0;

    void *el;
    while (deque_iter_next(&iter, &el) != CC_ITER_END) {
        void *k;
        deque_get_at(deque, i, &k);
        CHECK_EQUAL_C_POINTER(k, el);
        i++;

        CHECK_EQUAL_C_INT(i, iter.index);
    }
};

TEST_C(DequeTests, DequeZipIterRemove)
{
    deque_add(deque, "a");
    deque_add(deque, "b");
    deque_add(deque, "c");
    deque_add(deque, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    DequeZipIter zip;
    deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_remove(&zip, &r1, &r2);
    }
    CHECK_EQUAL_C_STRING("b", (char*)r1);
    CHECK_EQUAL_C_STRING("f", (char*)r2);
    CHECK_EQUAL_C_INT(0, deque_contains(deque, "b"));
    CHECK_EQUAL_C_INT(0, deque_contains(deque, "f"));
    CHECK_EQUAL_C_INT(3, deque_size(deque));
    CHECK_EQUAL_C_INT(2, deque_size(d2));
    deque_destroy(d2);
};

TEST_C(DequeTests, DequeZipIterAdd)
{
    deque_add(deque, "a");
    deque_add(deque, "b");
    deque_add(deque, "c");
    deque_add(deque, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    DequeZipIter zip;
    deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_add(&zip, h, i);
    }

    size_t index;
    deque_index_of(deque, "h", &index);

    CHECK_EQUAL_C_INT(2, index);

    deque_index_of(deque, "i", &index);
    CHECK_EQUAL_C_INT(2, index);

    deque_index_of(deque, "c", &index);
    CHECK_EQUAL_C_INT(3, index);
    CHECK_EQUAL_C_INT(1, deque_contains(deque, "h"));
    CHECK_EQUAL_C_INT(1, deque_contains(d2, "i"));
    CHECK_EQUAL_C_INT(5, deque_size(deque));
    CHECK_EQUAL_C_INT(4, deque_size(d2));
    deque_destroy(d2);
};


TEST_C(DequeTests, DequeZipIterNext)
{
    deque_add(deque, "a");
    deque_add(deque, "b");
    deque_add(deque, "c");
    deque_add(deque, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    DequeZipIter zip;
    deque_zip_iter_init(&zip, deque, d2);

    size_t i = 0;

    void *e1, *e2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
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
    deque_destroy(d2);
};

TEST_C(DequeTests, DequeZipIterReplace)
{
    deque_add(deque, "a");
    deque_add(deque, "b");
    deque_add(deque, "c");
    deque_add(deque, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    DequeZipIter zip;
    deque_zip_iter_init(&zip, deque, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    deque_index_of(deque, "h", &index);

    CHECK_EQUAL_C_INT(1, index);

    deque_index_of(deque, "i", &index);
    CHECK_EQUAL_C_INT(1, index);
    CHECK_EQUAL_C_INT(1, deque_contains(deque, "h"));
    CHECK_EQUAL_C_INT(1, deque_contains(d2, "i"));
    deque_destroy(d2);
};

TEST_GROUP_C_SETUP(DequeTestsConf)
{
  deque_conf_init(&conf);
  conf.capacity = 4;
  deque_new_conf(&conf, &deque);
};

TEST_GROUP_C_TEARDOWN(DequeTestsConf)
{
  deque_destroy(deque);
};

TEST_C(DequeTestsConf, DequeBufferExpansion)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);
    deque_add_last(deque, &d);

    size_t capacity = deque_capacity(deque);

    CHECK_EQUAL_C_INT(4, capacity);

    /* Current layout:
       _________________
       | b | d | c | a |
       -----------------
             L   F
     */
    /* This line should trigger the buffer expansion */
    deque_add_first(deque, &e);

    capacity = deque_capacity(deque);
    CHECK_EQUAL_C_INT(8, capacity);

    /* The expansion should align the elements.*/
    const void * const* buff = deque_get_buffer(deque);
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

    deque_add_last(deque, &f);

    const int elem5 = *((int*) buff[4]);
    CHECK_EQUAL_C_INT(elem5, f);
};

TEST_C(DequeTests, DequeFilter1)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    Deque *filter = NULL;
    deque_filter(deque, pred1, &filter);
    CHECK_EQUAL_C_INT(3, deque_size(filter));
    const void * const* buff = deque_get_buffer(filter);

    CHECK_EQUAL_C_POINTER(buff[0], &a);
    CHECK_EQUAL_C_POINTER(buff[1], &b);

    const void *elem = buff[2];
    CHECK_EQUAL_C_POINTER(elem, &c);
    free(filter);
};

TEST_C(DequeTests, DequeFilter2)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    Deque *filter = NULL;
    deque_filter(deque, pred2, &filter);
    const void * const* buff = deque_get_buffer(filter);

    CHECK_EQUAL_C_INT(3, deque_size(filter));
    CHECK_EQUAL_C_POINTER(buff[0], &d);
    CHECK_EQUAL_C_POINTER(buff[1], &e);
    CHECK_EQUAL_C_POINTER(buff[2], &f);

    free(filter);
};

TEST_C(DequeTests, DequeFilter3)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    Deque *filter = NULL;
    deque_filter(deque, pred3, &filter);
    const void * const* buff = deque_get_buffer(filter);

    CHECK_EQUAL_C_INT(1, deque_size(filter));
    CHECK_EQUAL_C_POINTER(buff[0], &f);

    free(filter);
};

TEST_C(DequeTests, DequeFilterMut1)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    deque_filter_mut(deque, pred1);
    CHECK_EQUAL_C_INT(3, deque_size(deque));

    int *removed = NULL;
    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(a, *removed);

    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(b, *removed);

    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(c, *removed);
};

TEST_C(DequeTests, DequeFilterMut2)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    deque_filter_mut(deque, pred2);
    CHECK_EQUAL_C_INT(3, deque_size(deque));

    int *removed = NULL;
    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(d, *removed);

    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(e, *removed);

    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(f, *removed);
};


TEST_C(DequeTests, DequeFilterMut3)
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);
    CHECK_EQUAL_C_INT(6, deque_size(deque));

    deque_filter_mut(deque, pred3);
    CHECK_EQUAL_C_INT(1, deque_size(deque));

    int *removed = NULL;
    deque_remove_first(deque, (void*) &removed);
    CHECK_EQUAL_C_INT(f, *removed);
};
