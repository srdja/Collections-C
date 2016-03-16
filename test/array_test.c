#include <time.h>

#include "../src/array.h"
#include "test.h"

void test_array_add();
void test_array_add_at();
void test_array_replace_at();
void test_array_remove();
void test_array_remove_at();
void test_array_remove_all();
void test_array_get();
void test_array_index_of();
void test_array_subarray();
void test_array_copy_shallow();
void test_array_copy_deep();
void test_array_reverse();
void test_array_trim_capacity();
void test_array_contains();
void test_array_capacity();
void test_array_sort();

void test_array_iter();
void test_array_iter_remove();
void test_array_iter_add();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_array_add();
    test_array_add_at();
    test_array_replace_at();
    test_array_remove();
    test_array_remove_at();
    test_array_remove_all();
    test_array_get();
    test_array_index_of();
    test_array_subarray();
    test_array_copy_shallow();
    test_array_copy_deep();
    test_array_reverse();
    test_array_trim_capacity();
    test_array_contains();
    test_array_capacity();
    test_array_sort();
    test_array_iter();

    return cc_get_status();
}


void test_array_add()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    int *ar = array_get(v, 0);
    int *br = array_get(v, 1);
    int *cr = array_get(v, 2);

    cc_assert(*ar == a,
              cc_msg("array_add: Added element not in the expected position"));

    cc_assert(*br == b,
              cc_msg("array_add: Added element not in the expected position"));

    cc_assert(*cr == c,
              cc_msg("array_add: Added element not in the expected position"));

    array_destroy(v);
}


void test_array_add_at()
{
    ArrayConf vc;
    array_conf_init(&vc);
    vc.capacity = 20;

    Array *v = array_new_conf(&vc);

    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);

    array_add_at(v, &ins, 1);

    int i0 = *((int*) array_get(v, 0));
    int i1 = *((int*) array_get(v, 1));
    int i2 = *((int*) array_get(v, 2));
    int i3 = *((int*) array_get(v, 3));
    int i4 = *((int*) array_get(v, 4));

    cc_assert(i0 == a,
              cc_msg("array_add_at: Expected %d at index 0, but got %d", a, i0));

    cc_assert(i1 == ins,
              cc_msg("array_add_at: Expected %d at index 1, but got %d", ins, i1));

    cc_assert(i2 == b,
              cc_msg("array_add_at: Expected %d at index 2, but got %d", b, i2));

    cc_assert(i3 == c,
              cc_msg("array_add_at: Expected %d at index 3, but got %d", c, i3));

    array_destroy(v);


    Array *ar = array_new();
    size_t s  = array_size(ar);

    cc_assert(s == 0,
              cc_msg("array_add_at: Expected size 0, but got %d", s));

    bool status = array_add_at(ar, &a, 1);

    cc_assert(!status,
              cc_msg("array_add_at: Expected operation failure"));

    array_destroy(ar);
}


void test_array_replace_at()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    array_replace_at(v, &replacement, 2);

    cc_assert(array_get(v, 2) != &c,
              cc_msg("array_replace_at: Unique element"
                     "still present after beeing replaced"));

    cc_assert(array_get(v, 2) == &replacement,
              cc_msg("array_replace_at: Replacement"
                     "element not at the expected index"));

    array_destroy(v);
}


void test_array_remove()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);

    array_remove(v, &c);

    cc_assert(array_size(v) == 3,
              cc_msg("array_remove: Expected "
                     "size was 3, but got %d",
                     array_size(v)));

    cc_assert(array_get(v, 2) == &e,
              cc_msg("array_remove: Unexpected"
                     " element at index 2"));

    array_destroy(v);
}


void test_array_remove_at()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);

    array_remove_at(v, 2);

    cc_assert(array_size(v) == 3,
              cc_msg("array_remove_at: Expected "
                     "size was 3, but got %d",
                     array_size(v)));

    cc_assert(array_get(v, 2) == &e,
              cc_msg("array_remove_at: Unexpected"
                     " element at index 2"));

    array_destroy(v);
}

void test_array_remove_all()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);

    array_remove_all(v);

    cc_assert(array_size(v) == 0,
              cc_msg("array_remove_all: Expected"
                     " size was 0, but got %d", array_size(v)));

    array_destroy(v);
}


void test_array_get()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);

    int ar = *((int*) array_get(v, 0));
    int cr = *((int*) array_get(v, 2));

    cc_assert(ar == a,
              cc_msg("array_get: Expected value was %d, but got %d", a, ar));

    cc_assert(cr == c,
              cc_msg("array_get: Expected value was %d, but got %d", c, cr));

    array_destroy(v);
}


void test_array_index_of()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    int ai = array_index_of(v, &a);
    int ci = array_index_of(v, &c);

    cc_assert(ai == 0,
              cc_msg("array_index_of: Expected index was 0, but got %d", ai));

     cc_assert(ci == 2,
              cc_msg("array_index_of: Expected index was 2, but got %d", ci));

    array_destroy(v);
}


void test_array_subarray()
{
    Array *v = array_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &e);
    array_add(v, &f);

    Array *sub = array_subarray(v, 1, 3);

    cc_assert(array_size(sub) == 3,
              cc_msg("array_subarray: Expected "
                     "subarray size was 3, but got %d",
                     array_size(sub)));

    int *s0 = array_get(sub, 0);
    int *s1 = array_get(sub, 1);
    int *s2 = array_get(sub, 2);

    cc_assert(s0 == &b,
              cc_msg("array_subarray: Expected element at index 0 was %d, but got %d", b, *s0));

    cc_assert(s1 == &c,
              cc_msg("array_subarray: Expected element at index 1 was %d, but got %d", c, *s1));

    cc_assert(s2 == &e,
              cc_msg("array_subarray: Expected element at index 2 was %d, but got %d", e, *s2));

    array_destroy(v);
    array_destroy(sub);
}


void test_array_copy_shallow()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    Array *cp = array_copy_shallow(v);

    cc_assert(array_size(cp) == array_size(v),
              cc_msg("array_copy_shallow: Array size missmatch"));

    cc_assert(array_get(v, 2) == array_get(cp, 2),
              cc_msg("array_copy_shallow: Element missmatch at index 2"));

    array_destroy(v);
    array_destroy(cp);
}


void *copy(void *val)
{
    int v = *((int*) val);
    int *new = malloc(sizeof(int));
    *new = v;
    return new;
}


void test_array_copy_deep()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    Array *cp = array_copy_deep(v, copy);

    cc_assert(array_size(cp) == array_size(v),
              cc_msg("array_copy_deep: Array size missmatch"));

    int ca = *((int*) array_get(cp, 0));

    cc_assert(ca == a,
              cc_msg("array_copy_deep: Value missmatch at index 2"));

    array_destroy(v);
    array_destroy_free(cp);
}


void test_array_reverse()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    array_reverse(v);

    int *i0 = array_get(v, 0);
    int *i1 = array_get(v, 1);
    int *i2 = array_get(v, 2);

    cc_assert(*i0 == c && *i1 == b && *i2 == a,
              cc_msg("array_reverse: Unexpected element order after reverse"));

    array_destroy(v);
}


void test_array_trim_capacity()
{
    ArrayConf vc;
    array_conf_init(&vc);
    vc.capacity = 20;

    Array *v = array_new_conf(&vc);

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    size_t c1 = array_capacity(v);
    bool   op = array_trim_capacity(v);
    size_t c2 = array_capacity(v);

    cc_assert(c1 == 20,
              cc_msg("array_trim_capacity: Expected initial capacity was 20, but got %d", c1));

    cc_assert(c2 == 3,
              cc_msg("array_trim_capacity: Expected trimmed capacity was 3, but got %d", c2));

    array_destroy(v);
}


void test_array_contains()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &c);

    int cc = array_contains(v, &c);
    int ca = array_contains(v, &a);
    int cd = array_contains(v, &d);

    cc_assert(cc == 2,
              cc_msg("array_contains: Expected number of elements was 2, but got %d", cc));

    cc_assert(ca == 1,
              cc_msg("array_contains: Expected number of elements was 1, but got %d", ca));

    cc_assert(cd == 0,
              cc_msg("array_contains: Expected number of elements was 0, but got %d", cd));

    array_destroy(v);
}


void test_array_capacity()
{
    ArrayConf vc;
    array_conf_init(&vc);
    vc.capacity = 1;

    Array *v = array_new_conf(&vc);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, &a);

    cc_assert(array_capacity(v) == 1,
              cc_msg("array_capacity: Expected capacity was 1, but got %d",
                     array_capacity(v)));

    array_add(v, &b);

    cc_assert(array_capacity(v) == 2,
              cc_msg("array_capacity: Expected capacity was 2, but got %d",
                     array_capacity(v)));

    array_destroy(v);
}

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

void test_array_sort()
{
    Array *v = array_new();

    srand(time(NULL));

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100;
        array_add(v, e);
    }

    array_sort(v, comp);

    int prev = *((int*) array_get(v, 0));
    for (i = 0; i < size; i++) {
        int e = *((int*) array_get(v, i));

        cc_assert(prev <= e,
                  cc_msg("array_sort: preceding element greater than the current"));

        prev = e;
    }
    array_destroy_free(v);
}


void test_array_iter()
{
     test_array_iter_remove();
     test_array_iter_add();
}

void test_array_iter_remove()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &d);

    ArrayIter iter;
    array_iter_init(&iter, v);

    for (;array_iter_has_next(&iter);) {
        int *e = array_iter_next(&iter);

        if (*e == c) {
            array_iter_remove(&iter);
        }
    }

    cc_assert(array_contains(v, &c) == 0,
              cc_msg("array_iter_remove: Element still present after removal"));

    array_destroy(v);
}

void test_array_iter_add()
{
    Array *v = array_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int new = 80;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);
    array_add(v, &d);

    ArrayIter iter;
    array_iter_init(&iter, v);

    for (;array_iter_has_next(&iter);) {
        int *e = array_iter_next(&iter);

        if (array_iter_index(&iter) == 2)
            array_iter_add(&iter, &new);
    }

    cc_assert(array_contains(v, &new) == 1,
              cc_msg("array_iter_add: Element not present after being added"));

    array_destroy(v);
}
