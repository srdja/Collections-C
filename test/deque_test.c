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
void test_deque_remove_all_free();
void test_deque_get();
void test_deque_get_first();
void test_deque_get_last();
void test_deque_copy_shallow();
void test_deque_copy_deep();
void test_deque_contains();
void test_deque_size();
void test_deque_capacity();
void test_deque_foreach();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_deque_add_first();
    test_deque_add_last();
    test_deque_buffer_expansion();
    test_deque_add_at();
/*    test_deque_remove_first();
    test_deque_remove_last();
    test_deque_remove_all();
    test_deque_remove_all_free();
    test_deque_get();
    test_deque_get_first();
    test_deque_get_last();
    test_deque_copy_shallow();
    test_deque_copy_deep();
    test_deque_contains();
    test_deque_size();
    test_deque_capacity();
    test_deque_foreach(); */

    return cc_get_status();
}

void test_deque_add_first()
{
    int a = 1;
    int b = 2;
    int c = 3;

    Deque *deque = deque_new();

    deque_add_first(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);

    cc_assert(deque_size(deque) == 3,
              cc_msg("deque_add_first: Unexpected size."));

    size_t m = deque_capacity(deque);
    void **u = deque_get_buffer(deque);
    void  *e = u[m - 1];

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

    Deque *deque = deque_new();

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);

    cc_assert(deque_size(deque) == 3,
              cc_msg("deque_add_last: Unexpected size."));

    size_t m = deque_capacity(deque);
    void **u = deque_get_buffer(deque);
    void  *e = u[0];

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

    Deque *deque = deque_new_conf(&conf);

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
    void **buff = deque_get_buffer(deque);
    int    elem = *((int*) buff[0]);

    cc_assert(elem == c,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 0 was %d, but got %d", c, elem));

    elem = *((int*) buff[1]);
    cc_assert(elem == a,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 1 was %d, but got %d", a, elem));

    elem = *((int*) buff[2]);
    cc_assert(elem == b,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 2 was %d, but got %d", b, elem));

    elem = *((int*) buff[3]);
    cc_assert(elem == d,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 3 was %d, but got %d", d, elem));

    elem = *((int*) buff[7]);
    cc_assert(elem == e,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 7 was %d, but got %d", e, elem));

    deque_add_last(deque, &f);

    elem = *((int*) buff[4]);
    cc_assert(elem == f,
              cc_msg("deque_buffer_expansion: Expected element "
                     "at index 7 was %d, but got %d", f, elem));

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

    Deque *deque = deque_new();

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_last(deque, &f);

    deque_add_at(deque, &g, 4);

    void **buff = deque_get_buffer(deque);
    void  *elem = buff[4];

    cc_assert(buff[4] == &g,
              cc_msg("deque_add_at / case1:"
                     " inserted element expected at index 4"
                     " , but was found", *((int*) (elem))));

    elem = buff[5];
    cc_assert(elem == &e,
              cc_msg("deque_add_at / case1:"
                     " moved element expected at index"
                     "  5, but got %d", *((int*) (elem))));

    elem = buff[6];
    cc_assert(elem == &f,
              cc_msg("deque_add_at / case1:"
                     " last element expected to be %d, "
                     " but got %d", f, *((int*) (elem))));

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

    Deque *deque = deque_new();

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    void **buff = deque_get_buffer(deque);
    void  *elem = buff[5];

    cc_assert(elem == &g,
               cc_msg("deque_add_at / case2:"
                      " inserted elment expcted to be at"
                      " index 5, but got %d", *((int*) (elem))));

    elem = buff[0];
    cc_assert(elem == &a,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 0, but"
                     " got %d instead!", a, *((int*) (elem))));

    elem = buff[7];
    cc_assert(elem == &c,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 7, but got"
                     " %d instead!", c, *((int*) (elem))));

    elem = buff[3];
    cc_assert(elem == &f,
              cc_msg("deque_add_at / case2: "
                     "expected element %d at index 3, but got"
                     " %d instead!", f, *((int*) (elem))));

    deque_destroy(deque);
}


void test_deque_add_at_case3()
{
    /* index >= deque->size && index_raw > last_raw */

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 999;

    Deque *deque = deque_new();

    deque_add_last(deque, &a);
    deque_add_first(deque, &b);
    deque_add_first(deque, &c);
    deque_add_first(deque, &d);
    deque_add_first(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 3);

    void **buff = deque_get_buffer(deque);

    void *elem = buff[6];
    cc_assert(elem == &g,
               cc_msg("deque_add_at [case 3]:"
                      " inserted elment expcted to be at"
                      " index 6, but got %d", *((int*) (elem))));

    elem = buff[0];
    cc_assert(elem == &b,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 0, but"
                     " got %d instead!", b, *((int*) (elem))));

    elem = buff[7];
    cc_assert(elem == &c,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 7, but got"
                     " %d instead!", c, *((int*) (elem))));

    elem = buff[1];
    cc_assert(elem == &a,
              cc_msg("deque_add_at [case 3]: "
                     "expected element %d at index 1, but got"
                     " %d instead!", a, *((int*) (elem))));

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

    Deque *deque = deque_new();

    deque_add_last(deque, &a);
    deque_add_last(deque, &b);
    deque_add_last(deque, &c);
    deque_add_last(deque, &d);
    deque_add_last(deque, &e);
    deque_add_first(deque, &f);

    deque_add_at(deque, &g, 1);

    void **buff = deque_get_buffer(deque);
    int    elem = *((int*) buff[0]);

    cc_assert(elem == g,
              cc_msg("deque_add_at [case 4]:"
                     " inserted element expected at index 0"
                     " , but %d was found", elem));

    elem = *((int*) buff[4]);
    cc_assert(elem == e,
              cc_msg("deque_add_at [case 4]:"
                     " moved element expected at index"
                     "  4, but got %d", elem));

    elem = *((int*) buff[6]);
    cc_assert(elem == f,
              cc_msg("deque_add_at [case 4]: "
                     "Expected element %d at index 6, but "
                     "got %d", f, elem));

    elem = *((int*) buff[7]);
    cc_assert(elem == a,
              cc_msg("deque_add_at [case 4]: "
                     "Expected element %d at index 7, but "
                     "got %d", a, elem));

    deque_destroy(deque);
}

void test_deque_add_at_case5()
{
    /* f == 0*/
}

void test_deque_add_at_case6()
{
    /*   l == c */
}

void test_deque_add_at()
{
    // test all four cases of buffer configuration
    test_deque_add_at_case1();
    test_deque_add_at_case2();
    test_deque_add_at_case3();
    test_deque_add_at_case4();
    test_deque_add_at_case5();
    test_deque_add_at_case6();
}


void test_deque_remove_first()
{

}


void test_deque_remove_last()
{

}


void test_deque_remove_all()
{

}


void test_deque_remove_all_free()
{

}


void test_deque_get()
{

}


void test_deque_get_first()
{

}


void test_deque_get_last()
{

}


void test_deque_copy_shallow()
{

}


void test_deque_copy_deep()
{

}


void test_deque_contains()
{

}


void test_deque_size()
{

}


void test_deque_capacity()
{

}


void test_deque_foreach()
{

}
