#include "../src/deque.h"
#include "test.h"

void test_deque_add();
void test_deque_add_first();
void test_deque_add_last();
void test_deque_buffer_expansion();
void test_deque_add_at();
void test_deque_remove_first();
void test_deque_remove_last();
void test_deque_remove_all();
void test_deque_get_at();
void test_deque_get_first();
void test_deque_get_last();
void test_deque_copy_shallow();
void test_deque_copy_deep();
void test_deque_contains();
void test_deque_size();
void test_deque_capacity();
void test_deque_trim_capacity();
void test_deque_reverse();
void test_deque_iterator();

void test_deque_zip_iter();
void test_deque_zip_iter_remove();
void test_deque_zip_iter_add();
void test_deque_zip_iter_next();
void test_deque_zip_iter_replace();


int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_deque_add_first();
    test_deque_add_last();
    test_deque_buffer_expansion();
    test_deque_add_at();
    test_deque_remove_first();
    test_deque_remove_last();
    test_deque_remove_all();
    test_deque_get_at();
    test_deque_get_first();
    test_deque_get_last();
    test_deque_copy_shallow();
    test_deque_copy_deep();
    test_deque_contains();
    test_deque_size();
    test_deque_capacity();
    test_deque_trim_capacity();
    test_deque_reverse();
    test_deque_iterator();
    test_deque_zip_iter();

    return cc_get_status();
}

void *cpy(void *e)
{
    int  o = *((int*) e);
    int *c = malloc(sizeof(int));
    *c = o;
    return (void*) c;
}

void test_deque_add_first()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_first(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);

    cc_assert(deque_size(deque) == 3,
              cc_msg("deque_add_first: Unexpected size."));

    size_t m = deque_capacity(deque);
    const void * const* u = deque_get_buffer(deque);
    const void *e = u[m - 1];

    cc_assert(e == &a,
              cc_msg("deque_add_first: Expected %d at index %d,"
                     " but got %d instead.", a, m - 1, *((int*) e)));

    e = u[m - 2];
    cc_assert(e == &b,
              cc_msg("deque_add_first: Expected %d at index %d,"
                     " but got %d instead.", b, m - 2, *((int*) e)));

    e = u[m - 3];
    cc_assert(e == &c,
              cc_msg("deque_add_first: Expected %d at index %d,"
                     " but got %d instead.", c, m - 3, *((int*) e)));
    deque_destroy(deque);
}


void test_deque_add_last()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);

    cc_assert(deque_size(deque) == 3,
              cc_msg("deque_add_last: Unexpected size."));

    size_t m = deque_capacity(deque);
    const void * const*u = deque_get_buffer(deque);
    const void *e = u[0];

    cc_assert(e == &a,
              cc_msg("deque_add_last: Expected %d at index %d,"
                     " but got %d instead.", a, 0, *((int*) e)));

    e = u[1];
    cc_assert(e == &b,
              cc_msg("deque_add_last: Expected %d at index %d,"
                     " but got %d instead.", b, 1, *((int*) e)));

    e = u[2];
    cc_assert(e == &c,
              cc_msg("deque_add_last: Expected %d at index %d,"
                     " but got %d instead.", c, 2, *((int*) e)));

    deque_destroy(deque);
}


void test_deque_buffer_expansion()
{
    DequeConf conf;
    deque_conf_init(&conf);

    conf.capacity = 4;

    Deque *deque;
    deque_new_conf(&conf, &deque);

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

    cc_assert(capacity == 4,
              cc_msg("deque_buffer_expansion: Expected capacity "
                     "was %d, but got %d", 4, capacity));

    /* Current layout:
       _________________
       | b | d | c | a |
       -----------------
             L   F
     */
    /* This line should trigger the buffer expansion */
    deque_add_first(deque, &e);

    capacity = deque_capacity(deque);
    cc_assert(capacity == 8,
              cc_msg("deque_buffer_expansion: Expected capacity"
                     " was %d, but got %d", 8, capacity));

    /* The expansion should align the elements.*/
    const void * const* buff = deque_get_buffer(deque);
    const int elem = *((int*) buff[0]);

    cc_assert(elem == c,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 0 was %d, but got %d", c, elem));

    const int elem1 = *((int*) buff[1]);
    cc_assert(elem1 == a,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 1 was %d, but got %d", a, elem1));

    const int elem2 = *((int*) buff[2]);
    cc_assert(elem2 == b,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 2 was %d, but got %d", b, elem2));

    const int elem3 = *((int*) buff[3]);
    cc_assert(elem3 == d,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 3 was %d, but got %d", d, elem3));

    const int elem4 = *((int*) buff[7]);
    cc_assert(elem4 == e,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 7 was %d, but got %d", e, elem4));

    deque_add_last(deque, &f);

    const int elem5 = *((int*) buff[4]);
    cc_assert(elem5 == f,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 7 was %d, but got %d", f, elem5));

    deque_destroy(deque);
}


void test_deque_add_at_case1()
{
    /* index < (size / 2) && index_raw > first_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);

    deque_add_at(deque, &g, 4);

    const void * const* buff = deque_get_buffer(deque);
    const void *elem = buff[4];

    cc_assert(buff[4] == &g,
              cc_msg("deque_add_at / case1:"
                     " inserted element expected at index 4"
                     " , but was found", *((int*) (elem))));

    const void *elem1 = buff[5];
    cc_assert(elem1 == &e,
              cc_msg("deque_add_at / case1:"
                     " moved element expected at index"
                     "  5, but got %d", *((int*) (elem1))));

    const void *elem2 = buff[6];
    cc_assert(elem2 == &f,
              cc_msg("deque_add_at / case1:"
                     " last element expected to be %d, "
                     " but got %d", f, *((int*) (elem2))));

    deque_destroy(deque);
}


void test_deque_add_at_case2()
{
    /* index < deque->size / 2 && index_raw > first_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const *buff = deque_get_buffer(deque);
    const void *elem = buff[5];

    cc_assert(elem == &g,
               cc_msg("deque_add_at / case2:"
                      " inserted elment expcted to be at"
                      " index 5, but got %d", *((int*) (elem))));

    const void *elem1 = buff[0];
    cc_assert(elem1 == &a,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 0, but"
                     " got %d instead!", a, *((int*) (elem1))));

    const void *elem2 = buff[7];
    cc_assert(elem2 == &c,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 7, but got"
                     " %d instead!", c, *((int*) (elem2))));

    const void *elem3 = buff[3];
    cc_assert(elem3 == &f,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 3, but got"
                     " %d instead!", f, *((int*) (elem3))));

    deque_destroy(deque);
}


void test_deque_add_at_case3()
{
    /* index >= size / 2 && index_raw > last_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 3);

    const void * const* buff = deque_get_buffer(deque);

    const void *elem = buff[6];
    cc_assert(elem == &g,
               cc_msg("deque_add_at [case 3]:"
                      " inserted elment expcted to be at"
                      " index 6, but got %d", *((int*) (elem))));

    const void *elem1 = buff[0];
    cc_assert(elem1 == &b,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 0, but"
                     " got %d instead!", b, *((int*) (elem1))));

    const void *elem2 = buff[7];
    cc_assert(elem2 == &c,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 7, but got"
                     " %d instead!", c, *((int*) (elem2))));

    const void *elem3 = buff[1];
    cc_assert(elem3 == &a,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 1, but got"
                     " %d instead!", a, *((int*) (elem3))));

    deque_destroy(deque);
}


void test_deque_add_at_case4()
{
    /* index < size / 2 && index_raw < last_raw*/

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const*buff = deque_get_buffer(deque);
    const int elem = *((int*) buff[0]);

    cc_assert(elem == g,
              cc_msg("deque_add_at [case 4]:"
                     " inserted element expected at index 0"
                     " , but %d was found", elem));

    const int elem1 = *((int*) buff[4]);
    cc_assert(elem1 == e,
              cc_msg("deque_add_at [case 4]:"
                     " moved element expected at index"
                     "  4, but got %d", elem1));

    const int elem2 = *((int*) buff[6]);
    cc_assert(elem2 == f,
              cc_msg("deque_add_at [case 4]: "
                     "Expected element %d at index 6, but "
                     "got %d", f, elem2));

    const int elem3 = *((int*) buff[7]);
    cc_assert(elem3 == a,
              cc_msg("deque_add_at [case 4]: "
                     "Expected element %d at index 7, but "
                     "got %d", a, elem3));

    deque_destroy(deque);
}

void test_deque_add_at_case5()
{
    /* f == 0*/

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);

    deque_add_at(deque, &g, 1);

    const void * const* buff = deque_get_buffer(deque);
    const int elem = *((int*) buff[7]);

    cc_assert(elem == a,
              cc_msg("deque_add_at [case 5]: "
                     "Expected element %d at index 7, but "
                     "got %d", a, elem));

    const int elem1 = *((int*) buff[0]);
    cc_assert(elem1 == b,
              cc_msg("deque_add_at [case 5]: "
                     "Expected element %d at index 7, but "
                     "got %d", b, elem1));

    const int elem2 = *((int*) buff[5]);
    cc_assert(elem2 == f,
              cc_msg("deque_add_at [case 5]: "
                     "Expected element %d at index 7, but "
                     "got %d", f, elem2));

    const int elem3 = *((int*) buff[1]);
    cc_assert(elem3 == g,
              cc_msg("deque_add_at [case 5]: "
                     "Expected element %d at index 7, but "
                     "got %d", g, elem3));

    deque_destroy(deque);
}

void test_deque_add_at()
{
    // test all four cases of buffer configuration
    test_deque_add_at_case1();
    test_deque_add_at_case2();
    test_deque_add_at_case3();
    test_deque_add_at_case4();
    test_deque_add_at_case5();
}


void test_deque_remove_first()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    Deque *deque;
    deque_new(&deque);

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);

    int *first;
    deque_get_first(deque, (void*) &first);
    cc_assert(*first == a,
              cc_msg("deque_remove_first: "
                     "Expected first element was %d, but "
                     "got %d", a, *first));

    int *removed;
    deque_remove_first(deque, (void*) &removed);
    cc_assert(*removed == a,
              cc_msg("deque_remove_first: "
                     "Expected removed element was %d, but "
                     "got %d", a, *removed));

    deque_get_first(deque, (void*) &first);
    cc_assert(*first == b,
              cc_msg("deque_remove_first: "
                     "Expected first element was %d, but "
                     "got %d", b, *first));

    deque_destroy(deque);
}


void test_deque_remove_last()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    Deque *deque;
    deque_new(&deque);

    deque_add_first(deque, &a);
    deque_add_first(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);

    int *last;
    deque_get_last(deque, (void*) &last);
    cc_assert(*last == d,
              cc_msg("deque_remove_last: "
                     "Expected last element was %d, but "
                     "got %d", d, *last));

    int *removed;
    deque_remove_last(deque, (void*) &removed);
    cc_assert(*removed == d,
              cc_msg("deque_remove_last: "
                     "Expected removed element was %d, but "
                     "got %d", d, *removed));

    deque_get_last(deque, (void*) &last);
    cc_assert(*last == c,
              cc_msg("deque_remove_last: "
                     "Expected last element was %d, but "
                     "got %d", c, *last));

    deque_destroy(deque);
}


void test_deque_remove_all()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    deque_remove_all(deque);

    void *first;
    int stat1 = deque_get_first(deque, &first);
    void *last;
    int stat2 = deque_get_last(deque, &last);

    cc_assert(stat1 == CC_ERR_OUT_OF_RANGE &&
              stat2 == CC_ERR_OUT_OF_RANGE,
              cc_msg("deque_remove_all: "
                     "First and last elements still reachable "
                     "after the deque has been cleared"));

    cc_assert(deque_size(deque) == 0,
              cc_msg("deque_remove_all: "
                     "Deque size not 0 after the deque has "
                     "been cleared"));

    deque_destroy(deque);
}


void test_deque_get_at()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    void *e;
    deque_get_at(deque, 1, &e);
    void *n;
    int status = deque_get_at(deque, 42, &n);

    cc_assert(*((int*) e) == b,
              cc_msg("deque_get: "
                     "Expected returned element was %d, but"
                     " got %d instead", b, *((int*) e)));

    cc_assert(status == CC_ERR_OUT_OF_RANGE,
              cc_msg("deque_get: "
                     "Expected element from a OOB index was NULL."));

    deque_destroy(deque);
}


void test_deque_get_first()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);

    int *first;
    deque_get_first(deque, (void*) &first);

    cc_assert(*first == c,
              cc_msg("deque_get_first: "
                     "Expected first element was %d, but got %d instead",
                     c, *first));

    deque_destroy(deque);
}


void test_deque_get_last()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_first(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);

    int *last;
    deque_get_last(deque, (void*) &last);

    cc_assert(*last == b,
              cc_msg("deque_get_last: "
                     "Expected last element was %d, but got %d instead",
                     b, *last));

    deque_destroy(deque);
}


void test_deque_copy_shallow()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);

    Deque *copy;
    deque_copy_shallow(deque, &copy);

    int size = deque_size(copy);

    cc_assert(size == 3,
              cc_msg("deque_copy_shallow: "
                     "Unexpected size of the copy. Expected %d,"
                     "but got %d", 3, size));

    int *ca;
    deque_get_at(copy, 0, (void*)&ca);

    int *cb;
    deque_get_at(copy, 1, (void*)&cb);

    int *cc;
    deque_get_at(copy, 2, (void*)&cc);

    cc_assert(*ca == a && *cb == b && *cc == c,
              cc_msg("deque_copy_shallow: "
                     "Structure missmatch"));

    deque_destroy(deque);
    deque_destroy(copy);

}


void test_deque_copy_deep()
{
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add_last(deque, a);
    deque_add_last(deque, b);
    deque_add_last(deque, c);

    Deque *copy;
    deque_copy_deep(deque, cpy, &copy);

    int size = deque_size(copy);

    cc_assert(size == 3,
              cc_msg("deque_copy_deep: "
                     "Unexpected size of the copy. Expected %d,"
                     "but got %d", 3, size));

    int *ca;
    deque_get_at(copy, 0, (void*)&ca);
    int *cb;
    deque_get_at(copy, 1, (void*)&cb);
    int *cc;
    deque_get_at(copy, 2, (void*)&cc);

    cc_assert(*ca == 1 && *cb == 2 && *cc == 3,
              cc_msg("deque_copy_deep: "
                     "Structure missmatch"));

    deque_destroy(deque);
    deque_destroy_free(copy);
    free(a);
    free(b);
    free(c);
}


void test_deque_contains()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    Deque *deque;
    deque_new(&deque);

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);
    deque_add(deque, &e);
    deque_add(deque, &f);
    deque_add(deque, &a);

    cc_assert(deque_contains(deque, &a) == 2,
              cc_msg("deque_contains: ",
                     "Unexpected number of elements found"));

    cc_assert(deque_contains(deque, &g) == 0,
              cc_msg("deque_contains: "
                     "Unexpected number of elements found"));

    cc_assert(deque_contains(deque, &e) == 1,
              cc_msg("deque_contains: "
                     "Unexpected number of elements found"));

    deque_destroy(deque);
}


void test_deque_size()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    Deque *deque;
    deque_new(&deque);

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);
    deque_add(deque, &d);

    size_t size = deque_size(deque);

    cc_assert(size == 4,
              cc_msg("deque_size: Unexpected size. Expected "
                     "%d, but got %d instead", 4, size));

    deque_destroy(deque);
}


void test_deque_capacity()
{
    DequeConf conf;
    deque_conf_init(&conf);

    conf.capacity = 2;

    Deque *deque;
    deque_new_conf(&conf, &deque);

    cc_assert(deque_capacity(deque) == 2,
              cc_msg("deque_capacity: Unexpected capacity. Expected 2"));

    int a = 1;
    int b = 2;
    int c = 3;

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    cc_assert(deque_capacity(deque) == 4,
              cc_msg("deque_capacity: Unexpected capacity. Expected 4"));

    deque_destroy(deque);
}


void test_deque_trim_capacity()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

    deque_add(deque, &a);
    deque_add(deque, &b);
    deque_add(deque, &c);

    size_t capacity = deque_capacity(deque);

    cc_assert(capacity == 8,
              cc_msg("deque_trim_capacity"
                     " Unexpected capacity."
                     " Expected 8, but got %d", capacity));

    deque_trim_capacity(deque);

    capacity = deque_capacity(deque);
    cc_assert(capacity == 4,
              cc_msg("deque_trim_capacity"
                     " Unexpected capacity."
                     " Expected 4, but got %d", capacity));

    deque_destroy(deque);
}


void test_deque_reverse()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque;
    deque_new(&deque);

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

    cc_assert(*ra == c,
              cc_msg("deque_reverse: Expected %d but got %d instead",
                     c, *ra));

    cc_assert(*rb == b,
              cc_msg("deque_reverse: Expected %d but got %d instead",
                     b, *rb));

    cc_assert(*rc == a,
              cc_msg("deque_reverse: Expected %d but got %d instead",
                     a, *rc));

    deque_destroy(deque);
}

void test_deque_iterator_add()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    int g = 7;

    Deque *deque;
    deque_new(&deque);

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
    while (deque_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == d)
            deque_iter_add(&iter, &g);

        if (deque_iter_index(&iter) < 3) {
            cc_assert(deque_size(deque) == 6,
                      cc_msg("deque_iterator: Deque size not 6 as expected"
                             " before insertion."));
        } else {
            cc_assert(deque_size(deque) == 7,
                      cc_msg("deque_iterator: Deque size not 7 as expected"
                             " after insertion."));
        }
        if (i >= 3) {
            cc_assert(deque_iter_index(&iter) - 1 == i,
                      cc_msg("deque_iterator: Iterator index not off by one"
                             " as expected."));
        }
        i++;
    }
    cc_assert(deque_size(deque) == 7,
              cc_msg("deque_iter_add: Expected size"
                     " was 7, but got %d", deque_size(deque)));

    void *ret;
    deque_get_at(deque, 4, &ret);

    cc_assert(*((int*)ret) == g,
              cc_msg("deque_iterator: Expected value at index 3 was %d,"
                     " but got %d instead", g, *((int*)ret)));

    deque_destroy(deque);
}

void test_deque_iterator_remove()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    Deque *deque;
    deque_new(&deque);

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
            cc_assert(deque_size(deque) == 5,
                      cc_msg("deque_iterator remove: Deque size not 5 as expected"
                             " after deletion."));
        } else {
            cc_assert(deque_size(deque) == 6,
                      cc_msg("deque_iterator remove: Deque size not 6 as expected"
                             " before removal."));
        }
        if (i >= 3) {
            cc_assert(deque_iter_index(&iter) == i - 1,
                      cc_msg("deque_iterator remove: Iterator index not off by one"
                             " as expected."));
        }
        i++;
    }
    deque_destroy(deque);
}

void test_deque_iterator_next()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;

    Deque *deque;
    deque_new(&deque);

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
        cc_assert(el == k,
                  cc_msg("deque_iterator: Element returned by "
                         "deque_iter_next not as expected"));
        i++;

        cc_assert(iter.index == i,
                  cc_msg("deque_iterator: Iterator index invalid. "
                         "Expected %d but got %d instead", i, iter.index));
    }
    deque_destroy(deque);
}

void test_deque_iterator()
{
    test_deque_iterator_next();
    test_deque_iterator_add();
    test_deque_iterator_remove();
}


void test_deque_zip_iter()
{
    test_deque_zip_iter_remove();
    test_deque_zip_iter_next();
    test_deque_zip_iter_replace();
    test_deque_zip_iter_add();
}


void test_deque_zip_iter_remove()
{
    Deque *d1;
    deque_new(&d1);

    deque_add(d1, "a");
    deque_add(d1, "b");
    deque_add(d1, "c");
    deque_add(d1, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    DequeZipIter zip;
    deque_zip_iter_init(&zip, d1, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_remove(&zip, &r1, &r2);
    }
    cc_assert(strcmp((char*) r1, "b") == 0 && strcmp((char*) r2, "f") == 0,
              cc_msg("deque_zip_iter_remove: Removed elements don't match expected ones"));

    cc_assert(deque_contains(d1, "b") == 0,
              cc_msg("deque_zip_iter_remove: Element still present after removal"));

    cc_assert(deque_contains(d2, "f") == 0,
              cc_msg("deque_zip_iter_remove: Element still present after removal"));

    cc_assert(deque_size(d1) == 3,
              cc_msg("deque_zip_iter_remove: Expected size 3, but got %d", deque_size(d1)));

    cc_assert(deque_size(d2) == 2,
              cc_msg("deque_zip_iter_remove: Expected size 2, but got %d", deque_size(d2)));

    deque_destroy(d1);
    deque_destroy(d2);
}


void test_deque_zip_iter_add()
{
    Deque *d1;
    deque_new(&d1);

    deque_add(d1, "a");
    deque_add(d1, "b");
    deque_add(d1, "c");
    deque_add(d1, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    DequeZipIter zip;
    deque_zip_iter_init(&zip, d1, d2);

    void *e1, *e2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_add(&zip, h, i);
    }

    size_t index;
    deque_index_of(d1, "h", &index);

    cc_assert(index == 2,
              cc_msg("deque_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "h", index));

    deque_index_of(d1, "i", &index);
    cc_assert(index == 2,
              cc_msg("deque_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "i", index));

    deque_index_of(d1, "c", &index);
    cc_assert(index == 3,
              cc_msg("deque_zip_iter_add: Expected element %s to be at index 3"
                     " but was found at %d", "c", index));

    cc_assert(deque_contains(d1, "h") == 1,
              cc_msg("deque_zip_iter_add: Element %s not presetn after addition", "h"));

    cc_assert(deque_contains(d2, "i") == 1,
              cc_msg("deque_zip_iter_add: Element %s not presetn after addition", "i"));

    cc_assert(deque_size(d1) == 5,
              cc_msg("deque_zip_iter_add: Expected size 5, but got %d", deque_size(d1)));

    cc_assert(deque_size(d2) == 4,
              cc_msg("deque_zip_iter_add: Expected size 4, but got %d", deque_size(d2)));

    deque_destroy(d1);
    deque_destroy(d2);
}


void test_deque_zip_iter_next()
{
    Deque *d1;
    deque_new(&d1);

    deque_add(d1, "a");
    deque_add(d1, "b");
    deque_add(d1, "c");
    deque_add(d1, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    DequeZipIter zip;
    deque_zip_iter_init(&zip, d1, d2);

    size_t i = 0;

    void *e1, *e2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            cc_assert(strcmp((char*) e1, "a") == 0,
                      cc_msg("deque_zip_iter_next: Expected e1 was \"a\" at index 0, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "e") == 0,
                      cc_msg("deque_zip_iter_next: Expected e1 was \"e\" at index 0, but got %s instead",
                             (char*) e2));
        }
        if (i == 2) {
            cc_assert(strcmp((char*) e1, "c") == 0,
                      cc_msg("deque_zip_iter_next: Expected e1 was \"a\" at index 2, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "g") == 0,
                      cc_msg("deque_zip_iter_next: Expected e1 was \"e\" at index 2, but got %s instead",
                             (char*) e2));
        }
        i++;
    }
    cc_assert(i == 3,
              cc_msg("deque_zip_iter_next: Expected 3 iterations, but got %d instead", i));

    deque_destroy(d1);
    deque_destroy(d2);
}


void test_deque_zip_iter_replace()
{
    Deque *d1;
    deque_new(&d1);

    deque_add(d1, "a");
    deque_add(d1, "b");
    deque_add(d1, "c");
    deque_add(d1, "d");

    Deque *d2;
    deque_new(&d2);

    deque_add(d2, "e");
    deque_add(d2, "f");
    deque_add(d2, "g");

    char *h = "h";
    char *i = "i";

    DequeZipIter zip;
    deque_zip_iter_init(&zip, d1, d2);

    void *e1, *e2;
    void *r1, *r2;
    while (deque_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            deque_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    deque_index_of(d1, "h", &index);

    cc_assert(index == 1,
              cc_msg("deque_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "h", index));

    deque_index_of(d1, "i", &index);
    cc_assert(index == 1,
              cc_msg("deque_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "i", index));

    cc_assert(deque_contains(d1, "h") == 1,
              cc_msg("deque_zip_iter_replace: Element %s not present after addition", "h"));

    cc_assert(deque_contains(d2, "i") == 1,
              cc_msg("deque_zip_iter_replace: Element %s not present after addition", "i"));

    deque_destroy(d1);
    deque_destroy(d2);
}
