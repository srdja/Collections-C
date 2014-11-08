#include "../src/vector.h"
#include "test.h"

void test_vector_add();
void test_vector_add_at();
void test_vector_replace_at();
void test_vector_remove();
void test_vector_remove_at();
void test_vector_remove_all();
void test_vector_get();
void test_vector_index_of();
void test_vector_subvector();
void test_vector_copy_shallow();
void test_vector_copy_deep();
void test_vector_reverse();
void test_vector_trim_capacity();
void test_vector_contains();
void test_vector_capacity();
void test_vector_sort();

void test_vector_iter();
void test_vector_iter_remove();
void test_vector_iter_add();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);
    
    test_vector_add();
    test_vector_add_at();
    test_vector_replace_at();
    test_vector_remove();
    test_vector_remove_at();
    test_vector_remove_all();
    test_vector_get();
    test_vector_index_of();
    test_vector_subvector();
    test_vector_copy_shallow();
    test_vector_copy_deep();
    test_vector_reverse();
    test_vector_trim_capacity();
    test_vector_contains();
    test_vector_capacity();
    test_vector_sort();
    test_vector_iter();

    return cc_get_status();
}


void test_vector_add()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    
    int *ar = vector_get(v, 0);
    int *br = vector_get(v, 1);
    int *cr = vector_get(v, 2);
    
    cc_assert(*ar == a,
              cc_msg("vector_add: Added element not in the expected position"));

    cc_assert(*br == b,
              cc_msg("vector_add: Added element not in the expected position"));

    cc_assert(*cr == c,
              cc_msg("vector_add: Added element not in the expected position"));
    
    vector_destroy(v);
}


void test_vector_add_at()
{
    Vector *v = vector_new_capacity(20);
    
    int a = 5;
    int b = 12;
    int c = 848;
    int e = 233;

    int ins = 123;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);

    vector_add_at(v, &ins, 1);

    int i0 = *((int*) vector_get(v, 0));
    int i1 = *((int*) vector_get(v, 1));
    int i2 = *((int*) vector_get(v, 2));
    int i3 = *((int*) vector_get(v, 3));
    int i4 = *((int*) vector_get(v, 4));

    cc_assert(i0 == a,
              cc_msg("vector_add_at: Expected %d at index 0, but got %d", a, i0));

    cc_assert(i1 == ins,
              cc_msg("vector_add_at: Expected %d at index 1, but got %d", ins, i1));

    cc_assert(i2 == b,
              cc_msg("vector_add_at: Expected %d at index 2, but got %d", b, i2));

    cc_assert(i3 == c,
              cc_msg("vector_add_at: Expected %d at index 3, but got %d", c, i3));

    vector_destroy(v);
}


void test_vector_replace_at()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    
    int replacement = 11;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);

    vector_replace_at(v, &replacement, 2);

    cc_assert(vector_get(v, 2) != &c,
              cc_msg("vector_replace_at: Unique element"
                     "still present after beeing replaced"));

    cc_assert(vector_get(v, 2) == &replacement,
              cc_msg("vector_replace_at: Replacement"
                     "element not at the expected index"));
    
    vector_destroy(v);
}


void test_vector_remove()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);

    vector_remove(v, &c);

    cc_assert(vector_size(v) == 3,
              cc_msg("vector_remove: Expected "
                     "size was 3, but got %d", 
                     vector_size(v)));

    cc_assert(vector_get(v, 2) == &e,
              cc_msg("vector_remove: Unexpected"
                     " element at index 2"));
    
    vector_destroy(v);
}


void test_vector_remove_at()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);

    vector_remove_at(v, 2);

    cc_assert(vector_size(v) == 3,
              cc_msg("vector_remove_at: Expected "
                     "size was 3, but got %d", 
                     vector_size(v)));

    cc_assert(vector_get(v, 2) == &e,
              cc_msg("vector_remove_at: Unexpected"
                     " element at index 2"));
    
    vector_destroy(v);
}

void test_vector_remove_all()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);

    vector_remove_all(v);

    cc_assert(vector_size(v) == 0,
              cc_msg("vector_remove_all: Expected"
                     " size was 0, but got %d", vector_size(v)));

    vector_destroy(v);
}


void test_vector_get()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);

    int ar = *((int*) vector_get(v, 0));
    int cr = *((int*) vector_get(v, 2));

    cc_assert(ar == a,
              cc_msg("vector_get: Expected value was %d, but got %d", a, ar));

    cc_assert(cr == c,
              cc_msg("vector_get: Expected value was %d, but got %d", c, cr));

    vector_destroy(v);
}


void test_vector_index_of()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    
    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    
    int ai = vector_index_of(v, &a);
    int ci = vector_index_of(v, &c);

    cc_assert(ai == 0, 
              cc_msg("vector_index_of: Expected index was 0, but got %d", ai));

     cc_assert(ci == 2, 
              cc_msg("vector_index_of: Expected index was 2, but got %d", ci));

    vector_destroy(v);
}


void test_vector_subvector()
{
    Vector *v = vector_new();

    int a = 23;
    int b = 32;
    int c = 55;
    int e = 11;
    int f = 43;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &e);
    vector_add(v, &f);

    Vector *sub = vector_subvector(v, 1, 3);
    
    cc_assert(vector_size(sub) == 3,
              cc_msg("vector_subvector: Expected "
                     "subvector size was 3, but got %d", 
                     vector_size(sub)));

    int *s0 = vector_get(sub, 0);
    int *s1 = vector_get(sub, 1);
    int *s2 = vector_get(sub, 2);
    
    cc_assert(s0 == &b,
              cc_msg("vector_subvector: Expected element at index 0 was %d, but got %d", b, *s0));
    
    cc_assert(s1 == &c,
              cc_msg("vector_subvector: Expected element at index 1 was %d, but got %d", c, *s1));
    
    cc_assert(s2 == &e,
              cc_msg("vector_subvector: Expected element at index 2 was %d, but got %d", e, *s2));

    vector_destroy(v);
    vector_destroy(sub);
}


void test_vector_copy_shallow()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);

    Vector *cp = vector_copy_shallow(v);

    cc_assert(vector_size(cp) == vector_size(v),
              cc_msg("vector_copy_shallow: Vector size missmatch"));

    cc_assert(vector_get(v, 2) == vector_get(cp, 2),
              cc_msg("vector_copy_shallow: Element missmatch at index 2"));

    vector_destroy(v);
    vector_destroy(cp);
}


void *copy(void *val) 
{
    int v = *((int*) val);
    int *new = malloc(sizeof(int));
    *new = v;
    return new;
}


void test_vector_copy_deep()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);

    Vector *cp = vector_copy_deep(v, copy);

    cc_assert(vector_size(cp) == vector_size(v),
              cc_msg("vector_copy_deep: Vector size missmatch"));

    int ca = *((int*) vector_get(cp, 0));

    cc_assert(ca == a,
              cc_msg("vector_copy_deep: Value missmatch at index 2"));

    vector_destroy(v);
    vector_destroy_free(cp);
}


void test_vector_reverse()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    
    vector_reverse(v);

    int *i0 = vector_get(v, 0);
    int *i1 = vector_get(v, 1);
    int *i2 = vector_get(v, 2);

    cc_assert(*i0 == c && *i1 == b && *i2 == a,
              cc_msg("vector_reverse: Unexpected element order after reverse"));

    vector_destroy(v);
}


void test_vector_trim_capacity()
{
    Vector *v = vector_new_capacity(20);

    int a = 5;
    int b = 12;
    int c = 848;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);

    size_t c1 = vector_capacity(v);

    vector_trim_capacity(v);

    size_t c2 = vector_capacity(v);

    cc_assert(c1 == 20,
              cc_msg("vector_trim_capacity: Expected initial capacity was 20, but got %d", c1));

    cc_assert(c2 == 3,
              cc_msg("vector_trim_capacity: Expected trimmed capacity was 3, but got %d", c2));

    vector_destroy(v);
}


void test_vector_contains()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &c);

    int cc = vector_contains(v, &c);
    int ca = vector_contains(v, &a);
    int cd = vector_contains(v, &d);

    cc_assert(cc == 2,
              cc_msg("vector_contains: Expected number of elements was 2, but got %d", cc));
    
    cc_assert(ca == 1,
              cc_msg("vector_contains: Expected number of elements was 1, but got %d", ca));

    cc_assert(cd == 0,
              cc_msg("vector_contains: Expected number of elements was 0, but got %d", cd));

    vector_destroy(v);
}


void test_vector_capacity()
{
    Vector *v = vector_new_capacity(1);

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    vector_add(v, &a);
    
    cc_assert(vector_capacity(v) == 1,
              cc_msg("vector_capacity: Expected capacity was 1, but got %d", vector_capacity(v)));

    vector_add(v, &b);

    cc_assert(vector_capacity(v) == 2,
              cc_msg("vector_capacity: Expected capacity was 2, but got %d", vector_capacity(v)));
    
    vector_destroy(v);
}

int comp(void const *e1, void const *e2)
{
    int *i = *((int*) e1);
    int *j = *((int*) e2);

    if (*i < *j)
        return -1;
    if (*i > *j)
        return 1;
    return 0;
}

void test_vector_sort()
{
    Vector *v = vector_new();

    srand(time(NULL));

    int size = 10;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100;
        vector_add(v, e);
    }

    vector_sort(v, comp);

    int prev = *((int*) vector_get(v, 0));
    for (i = 0; i < size; i++) {
        int e = *((int*) vector_get(v, i));

        cc_assert(prev <= e,
                  cc_msg("vector_sort: preceding element greater than the current"));

        prev = e;
    }
    vector_destroy_free(v);
}


void test_vector_iter()
{
     test_vector_iter_remove();
     test_vector_iter_add();
}

void test_vector_iter_remove()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &d);

    VectorIter *iter = vector_iter_new(v);
    for (;vector_iter_has_next(iter);) {
        int *e = vector_iter_next(iter);
        
        if (*e == c) {
            vector_iter_remove(iter);
        }
    }

    cc_assert(vector_contains(v, &c) == 0,
              cc_msg("vector_iter_remove: Element still present after removal"));

    vector_iter_destroy(iter);
    vector_destroy(v);
}

void test_vector_iter_add()
{
    Vector *v = vector_new();

    int a = 5;
    int b = 12;
    int c = 848;
    int d = 23;

    int new = 80;

    vector_add(v, &a);
    vector_add(v, &b);
    vector_add(v, &c);
    vector_add(v, &d);

    VectorIter *iter = vector_iter_new(v);
    for (;vector_iter_has_next(iter);) {
        int *e = vector_iter_next(iter);
        
        if (vector_iter_index(iter) == 2)
            vector_iter_add(iter, &new);      
    }

    cc_assert(vector_contains(v, &new) == 1,
              cc_msg("vector_iter_add: Element not present after being added"));

    vector_iter_destroy(iter);
    vector_destroy(v);
}

