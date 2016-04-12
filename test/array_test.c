#include <time.h>
#include <stdint.h>

#include "../src/array.h"
#include "test.h"

void test_array_add();
void test_array_add_at();
void test_array_replace_at();
void test_array_remove();
void test_array_remove_at();
void test_array_remove_all();
void test_array_get_at();
void test_array_index_of();
void test_array_subarray();
void test_array_copy_shallow();
void test_array_copy_deep();
void test_array_reverse();
void test_array_trim_capacity();
void test_array_contains();
void test_array_capacity();
void test_array_sort();
void test_array_filter_mut();
void test_array_filter();

void test_array_iter();
void test_array_iter_remove();
void test_array_iter_add();
void test_array_iter_replace();

void test_array_zip_iter();
void test_array_zip_iter_remove();
void test_array_zip_iter_add();
void test_array_zip_iter_next();
void test_array_zip_iter_replace();


int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_array_add();
    test_array_add_at();
    test_array_replace_at();
    test_array_remove();
    test_array_remove_at();
    test_array_remove_all();
    test_array_get_at();
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
    test_array_zip_iter();
    test_array_filter_mut();
    test_array_filter();

    return cc_get_status();
}


void test_array_add()
{
    Array *v;
    int stat = array_new(&v);

    cc_assert(stat == CC_OK,
              cc_msg("array_add: Expected status %d but got %d instead", CC_OK, stat));

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    int *ar;
    int *br;
    int *cr;
    array_get_at(v, 0, (void*) &ar);
    array_get_at(v, 1, (void*) &br);
    array_get_at(v, 2, (void*) &cr);

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

    Array *v;
    array_new_conf(&vc, &v);

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

    int *i0;
    int *i1;
    int *i2;
    int *i3;
    int *i4;

    array_get_at(v, 0, (void*)&i0);
    array_get_at(v, 1, (void*)&i1);
    array_get_at(v, 2, (void*)&i2);
    array_get_at(v, 3, (void*)&i3);
    array_get_at(v, 4, (void*)&i4);

    cc_assert(*i0 == a,
              cc_msg("array_add_at: Expected %d at index 0, but got %d", a, i0));

    cc_assert(*i1 == ins,
              cc_msg("array_add_at: Expected %d at index 1, but got %d", ins, i1));

    cc_assert(*i2 == b,
              cc_msg("array_add_at: Expected %d at index 2, but got %d", b, i2));

    cc_assert(*i3 == c,
              cc_msg("array_add_at: Expected %d at index 3, but got %d", c, i3));

    array_destroy(v);

    Array *ar;
    array_new(&ar);

    size_t s = array_size(ar);

    cc_assert(s == 0,
              cc_msg("array_add_at: Expected size 0, but got %d", s));

    int status = array_add_at(ar, (void*) &a, 1);

    cc_assert(status != CC_OK,
              cc_msg("array_add_at: Expected operation failure"));

    array_destroy(ar);
}


void test_array_replace_at()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;

    int replacement = 11;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);

    array_replace_at(v, (void*)&replacement, 2, NULL);

    int *repl;
    array_get_at(v, 2, (void*) &repl);

    cc_assert(repl != &c,
              cc_msg("array_replace_at: Unique element"
                     "still present after beeing replaced"));

    cc_assert(repl == &replacement,
              cc_msg("array_replace_at: Replacement"
                     "element not at the expected index"));

    array_destroy(v);
}


void test_array_remove()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&e);

    array_remove(v, &c, NULL);

    cc_assert(array_size(v) == 3,
              cc_msg("array_remove: Expected "
                     "size was 3, but got %d",
                     array_size(v)));

    int *r;
    array_get_at(v, 2, (void*) &r);

    cc_assert(r == &e,
              cc_msg("array_remove: Unexpected"
                     " element at index 2"));

    array_destroy(v);
}


void test_array_remove_at()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&e);

    array_remove_at(v, 2, NULL);

    cc_assert(array_size(v) == 3,
              cc_msg("array_remove_at: Expected "
                     "size was 3, but got %d",
                     array_size(v)));

    int *r;
    array_get_at(v, 2, (void*) &r);

    cc_assert(r == &e,
              cc_msg("array_remove_at: Unexpected"
                     " element at index 2"));

    array_destroy(v);
}

void test_array_remove_all()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&e);

    array_remove_all(v);

    cc_assert(array_size(v) == 0,
              cc_msg("array_remove_all: Expected"
                     " size was 0, but got %d", array_size(v)));

    array_destroy(v);
}


void test_array_get_at()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&e);

    int *ar;
    int *cr;
    array_get_at(v, 0, (void*) &ar);
    array_get_at(v, 2, (void*) &cr);

    cc_assert(*ar == a,
              cc_msg("array_get: Expected value was %d, but got %d", a, ar));

    cc_assert(*cr == c,
              cc_msg("array_get: Expected value was %d, but got %d", c, cr));

    array_destroy(v);
}


void test_array_index_of()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);

    size_t ai;
    array_index_of(v, &a, &ai);

    size_t ci;
    array_index_of(v, &c, &ci);

    cc_assert(ai == 0,
              cc_msg("array_index_of: Expected index was 0, but got %d", ai));

    cc_assert(ci == 2,
              cc_msg("array_index_of: Expected index was 2, but got %d", ci));

    array_destroy(v);
}


void test_array_subarray()
{
    Array *v;
    array_new(&v);

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&e);
    array_add(v, (void*)&f);


    Array *sub;
    array_subarray(v, 1, 3, &sub);

    cc_assert(array_size(sub) == 3,
              cc_msg("array_subarray: Expected "
                     "subarray size was 3, but got %d",
                     array_size(sub)));

    int *s0;
    int *s1;
    int *s2;
    array_get_at(sub, 0, (void*)&s0);
    array_get_at(sub, 1, (void*)&s1);
    array_get_at(sub, 2, (void*)&s2);

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
    Array *v;
    array_new(&v);

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);

    Array *cp;
    array_copy_shallow(v, &cp);

    cc_assert(array_size(cp) == array_size(v),
              cc_msg("array_copy_shallow: Array size missmatch"));


    int *ga;
    int *gb;
    array_get_at(v, 2, (void*)&ga);
    array_get_at(cp, 2, (void*)&gb);

    cc_assert(ga == gb,
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
    Array *v;
    array_new(&v);

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);

    Array *cp;
    array_copy_deep(v, copy, &cp);

    cc_assert(array_size(cp) == array_size(v),
              cc_msg("array_copy_deep: Array size missmatch"));

    int *ca;
    array_get_at(cp, 0, (void*) &ca);

    cc_assert(*ca == a,
              cc_msg("array_copy_deep: Value missmatch at index 2"));

    array_destroy(v);
    array_destroy_free(cp);
}


void test_array_reverse()
{
    Array *v;
    array_new(&v);

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);

    array_reverse(v);

    int *i0;
    int *i1;
    int *i2;
    array_get_at(v, 0, (void*)&i0);
    array_get_at(v, 1, (void*)&i1);
    array_get_at(v, 2, (void*)&i2);

    cc_assert(*i0 == c && *i1 == b && *i2 == a,
              cc_msg("array_reverse: Unexpected element order after reverse"));

    array_destroy(v);
}


void test_array_trim_capacity()
{
    ArrayConf vc;
    array_conf_init(&vc);
    vc.capacity = 20;

    Array *v;
    array_new_conf(&vc, &v);

    int a = 5;
    int b = 12;
    int c = 848;

    array_add(v, &a);
    array_add(v, &b);
    array_add(v, &c);

    size_t c1 = array_capacity(v);
    int    op = array_trim_capacity(v);
    size_t c2 = array_capacity(v);

    cc_assert(c1 == 20,
              cc_msg("array_trim_capacity: Expected initial capacity was 20, but got %d", c1));

    cc_assert(c2 == 3,
              cc_msg("array_trim_capacity: Expected trimmed capacity was 3, but got %d", c2));

    array_destroy(v);
}


void test_array_contains()
{
    Array *v;
    array_new(&v);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&c);

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

    Array *v;
    array_new_conf(&vc, &v);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, (void*)&a);

    cc_assert(array_capacity(v) == 1,
              cc_msg("array_capacity: Expected capacity was 1, but got %d",
                     array_capacity(v)));

    array_add(v, (void*)&b);

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
    Array *v;
    array_new(&v);

    srand(time(NULL));

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100;
        array_add(v, (void*)e);
    }

    array_sort(v, comp);

    int *prev;
    array_get_at(v, 0, (void*)&prev);
    for (i = 0; i < size; i++) {
        int *e;
        array_get_at(v, i, (void*)&e);

        cc_assert(*prev <= *e,
                  cc_msg("array_sort: preceding element greater than the current"));

        prev = e;
    }
    array_destroy_free(v);
}


void test_array_iter()
{
     test_array_iter_remove();
     test_array_iter_add();
     test_array_iter_replace();
}

void test_array_iter_remove()
{
    Array *v;
    array_new(&v);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    array_add(v, (void*)&a);
    array_add(v, (void*)&b);
    array_add(v, (void*)&c);
    array_add(v, (void*)&d);

    ArrayIter iter;
    array_iter_init(&iter, v);

    int *e;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            array_iter_remove(&iter, NULL);
    }

    cc_assert(array_contains(v, &c) == 0,
              cc_msg("array_iter_remove: Element still present after removal"));

    array_destroy(v);
}

void test_array_iter_add()
{
    Array *ar;
    array_new(&ar);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    int new = 80;

    array_add(ar, (void*)&a);
    array_add(ar, (void*)&b);
    array_add(ar, (void*)&c);
    array_add(ar, (void*)&d);

    ArrayIter iter;
    array_iter_init(&iter, ar);

    int *e;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3)
            array_iter_add(&iter, (void*) &new);
    }
    cc_assert(array_size(ar) == 5,
              cc_msg("array_iter_add: Expected size"
                     " was 5, but got %d", array_size(ar)));
    void *n;
    array_get_at(ar, 3, &n);

    cc_assert(*((int*)n) == new,
              cc_msg("array_iter_add: Expected element at index 2 was %d"
                     ", but got %d instead", new, *((int*)n)));

    cc_assert(array_contains(ar, &new) == 1,
              cc_msg("array_iter_add: Element not present after being added"));

    array_get_at(ar, 4, (void*) &n);
    cc_assert(*((int*)n) == 4,
              cc_msg("array_iter_add: Expected element at index 4 was 4, but got %d", *((int*)n)));

    array_destroy(ar);
}


void test_array_iter_replace()
{
    Array *ar;
    array_new(&ar);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int replacement = 42;

    array_add(ar, (void*)&a);
    array_add(ar, (void*)&b);
    array_add(ar, (void*)&c);
    array_add(ar, (void*)&d);

    ArrayIter iter;
    array_iter_init(&iter, ar);

    int *e;
    int *old;
    while (array_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == c)
            array_iter_replace(&iter, (void*)&replacement, (void*)&old);
    }

    size_t index;
    array_index_of(ar, (void*) &replacement, &index);

    cc_assert(index == 2,
              cc_msg("array_iter_replace: Expected element %d to be at index 2,"
                     " but was found at index %d instead", replacement, index));

    cc_assert(array_contains(ar, &c) == 0,
              cc_msg("array_iter_replace: Element still present after being replaced"));

    array_destroy(ar);
}


void test_array_zip_iter_next()
{
    Array *a1;
    array_new(&a1);

    array_add(a1, "a");
    array_add(a1, "b");
    array_add(a1, "c");
    array_add(a1, "d");

    Array *a2;
    array_new(&a2);

    array_add(a2, "e");
    array_add(a2, "f");
    array_add(a2, "g");

    ArrayZipIter zip;
    array_zip_iter_init(&zip, a1, a2);

    size_t i = 0;

    void *e1, *e2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            cc_assert(strcmp((char*) e1, "a") == 0,
                      cc_msg("array_zip_iter_next: Expected e1 was \"a\" at index 0, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "e") == 0,
                      cc_msg("array_zip_iter_next: Expected e1 was \"e\" at index 0, but got %s instead",
                             (char*) e2));
        }
        if (i == 2) {
            cc_assert(strcmp((char*) e1, "c") == 0,
                      cc_msg("array_zip_iter_next: Expected e1 was \"a\" at index 2, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "g") == 0,
                      cc_msg("array_zip_iter_next: Expected e1 was \"e\" at index 2, but got %s instead",
                             (char*) e2));
        }
        i++;
    }
    cc_assert(i == 3,
              cc_msg("array_zip_iter_next: Expected 3 iterations, but got %d instead", i));

    array_destroy(a1);
    array_destroy(a2);
}


void test_array_zip_iter_remove()
{
    Array *a1;
    array_new(&a1);

    array_add(a1, "a");
    array_add(a1, "b");
    array_add(a1, "c");
    array_add(a1, "d");

    Array *a2;
    array_new(&a2);

    array_add(a2, "e");
    array_add(a2, "f");
    array_add(a2, "g");

    ArrayZipIter zip;
    array_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            array_zip_iter_remove(&zip, &r1, &r2);
    }
    cc_assert(strcmp((char*) r1, "b") == 0 && strcmp((char*) r2, "f") == 0,
              cc_msg("array_zip_iter_remove: Removed elements don't match expected ones"));

    cc_assert(array_contains(a1, "b") == 0,
              cc_msg("array_zip_iter_remove: Element still present after removal"));

    cc_assert(array_contains(a2, "f") == 0,
              cc_msg("array_zip_iter_remove: Element still present after removal"));

    cc_assert(array_size(a1) == 3,
              cc_msg("array_zip_iter_remove: Expected size 3, but got %d", array_size(a1)));

    cc_assert(array_size(a2) == 2,
              cc_msg("array_zip_iter_remove: Expected size 2, but got %d", array_size(a2)));

    array_destroy(a1);
    array_destroy(a2);
}


void test_array_zip_iter_add()
{
    Array *a1;
    array_new(&a1);

    array_add(a1, "a");
    array_add(a1, "b");
    array_add(a1, "c");
    array_add(a1, "d");

    Array *a2;
    array_new(&a2);

    array_add(a2, "e");
    array_add(a2, "f");
    array_add(a2, "g");

    char *h = "h";
    char *i = "i";

    ArrayZipIter zip;
    array_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            array_zip_iter_add(&zip, h, i);
    }

    size_t index;
    array_index_of(a1, "h", &index);

    cc_assert(index == 2,
              cc_msg("array_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "h", index));

    array_index_of(a1, "i", &index);
    cc_assert(index == 2,
              cc_msg("array_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "i", index));

    array_index_of(a1, "c", &index);
    cc_assert(index == 3,
              cc_msg("array_zip_iter_add: Expected element %s to be at index 3"
                     " but was found at %d", "c", index));

    cc_assert(array_contains(a1, "h") == 1,
              cc_msg("array_zip_iter_add: Element %s not presetn after addition", "h"));

    cc_assert(array_contains(a2, "i") == 1,
              cc_msg("array_zip_iter_add: Element %s not presetn after addition", "i"));

    cc_assert(array_size(a1) == 5,
              cc_msg("array_zip_iter_add: Expected size 5, but got %d", array_size(a1)));

    cc_assert(array_size(a2) == 4,
              cc_msg("array_zip_iter_add: Expected size 4, but got %d", array_size(a2)));

    array_destroy(a1);
    array_destroy(a2);
}


void test_array_zip_iter_replace()
{
    Array *a1;
    array_new(&a1);

    array_add(a1, "a");
    array_add(a1, "b");
    array_add(a1, "c");
    array_add(a1, "d");

    Array *a2;
    array_new(&a2);

    array_add(a2, "e");
    array_add(a2, "f");
    array_add(a2, "g");

    char *h = "h";
    char *i = "i";

    ArrayZipIter zip;
    array_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (array_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            array_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    array_index_of(a1, "h", &index);

    cc_assert(index == 1,
              cc_msg("array_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "h", index));

    array_index_of(a1, "i", &index);
    cc_assert(index == 1,
              cc_msg("array_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "i", index));

    cc_assert(array_contains(a1, "h") == 1,
              cc_msg("array_zip_iter_replace: Element %s not presetn after addition", "h"));

    cc_assert(array_contains(a2, "i") == 1,
              cc_msg("array_zip_iter_replace: Element %s not presetn after addition", "i"));

    array_destroy(a1);
    array_destroy(a2);
}


void test_array_zip_iter()
{
    test_array_zip_iter_remove();
    test_array_zip_iter_add();
    test_array_zip_iter_next();
    test_array_zip_iter_replace();
}


Array *new_filter_array()
{
    Array *ar;
    array_new(&ar);

    array_add(ar, (void*) 0);
    array_add(ar, (void*) 1);
    array_add(ar, (void*) 2);
    array_add(ar, (void*) 0);
    array_add(ar, (void*) 0);
    array_add(ar, (void*) 3);
    array_add(ar, (void*) 4);
    array_add(ar, (void*) 0);
    array_add(ar, (void*) 0);
    array_add(ar, (void*) 5);
    array_add(ar, (void*) 6);
    return ar;
}


bool pred1(const void *e)
{
    return e == 0;
}


bool pred2(const void *e)
{
    return e != 0;
}


void test_array_filter_mut() {
    Array *ar = new_filter_array();

    array_filter_mut(ar, pred1);

    cc_assert(array_size(ar) == 5,
              cc_msg("array_filter_mut: Expected size after filtering was 5,"
                     " but got %d instead", array_size(ar)));

    ArrayIter i;
    array_iter_init(&i, ar);
    void *e;
    while (array_iter_next(&i, &e) != CC_ITER_END) {
        cc_assert(e == 0,
                  cc_msg("array_filter_mut: Expected 0 at index %lu, but got "
                         "%lu instead", array_iter_index(&i), e));
    }
    array_destroy(ar);

    ar = new_filter_array();

    array_filter_mut(ar, pred2);

    cc_assert(array_size(ar) == 6,
              cc_msg("array_filter_mut: Expected size after filtering was 6,"
                     " but got %d instead", array_size(ar)));

    void *e1;
    array_get_at(ar, 0, &e1);

    void *e2;
    array_get_at(ar, 1, &e2);

    void *e3;
    array_get_at(ar, 2, &e3);

    void *e4;
    array_get_at(ar, 3, &e4);

    void *e5;
    array_get_at(ar, 4, &e5);

    void *e6;
    array_get_at(ar, 5, &e6);

    cc_assert((uintptr_t)e1 == 1 && (uintptr_t)e2 == 2 && (uintptr_t)e3 == 3 &&
              (uintptr_t)e4 == 4 && (uintptr_t)e5 == 5 && (uintptr_t)e6 == 6,
              cc_msg("array_filter_mut: Unexpected order after filtering"));

    array_destroy(ar);
}


void test_array_filter() {
    Array *ar = new_filter_array();
    Array *far;
    array_filter(ar, pred1, &far);

    cc_assert(array_size(far) == 5,
              cc_msg("array_filter_mut: Expected size after filtering was 5,"
                     " but got %d instead", array_size(far)));

    ArrayIter i;
    array_iter_init(&i, far);
    void *e;
    while (array_iter_next(&i, &e) != CC_ITER_END) {
        cc_assert(e == 0,
                  cc_msg("array_filter_mut: Expected 0 at index %lu, but got "
                         "%lu instead", array_iter_index(&i), e));
    }
    array_destroy(far);

    array_filter(ar, pred2, &far);

    cc_assert(array_size(far) == 6,
              cc_msg("array_filter_mut: Expected size after filtering was 6,"
                     " but got %d instead", array_size(far)));

    void *e1;
    array_get_at(far, 0, &e1);

    void *e2;
    array_get_at(far, 1, &e2);

    void *e3;
    array_get_at(far, 2, &e3);

    void *e4;
    array_get_at(far, 3, &e4);

    void *e5;
    array_get_at(far, 4, &e5);

    void *e6;
    array_get_at(far, 5, &e6);

    cc_assert((uintptr_t)e1 == 1 && (uintptr_t)e2 == 2 && (uintptr_t)e3 == 3 &&
              (uintptr_t)e4 == 4 && (uintptr_t)e5 == 5 && (uintptr_t)e6 == 6,
              cc_msg("array_filter_mut: Unexpected order after filtering"));

    array_destroy(far);
    array_destroy(ar);
}
