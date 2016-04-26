#include "test.h"
#include "../src/treeset.h"

void test_treeset_add();
void test_treeset_remove();
void test_treeset_remove_all();
void test_treeset_size();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_treeset_add();
    test_treeset_remove();
    test_treeset_remove_all();
    test_treeset_size();

    return cc_get_status();
}

int cmp(const void *k1, const void *k2)
{
    int a = *((int*) k1);
    int b = *((int*) k2);

    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

void test_treeset_add()
{
    TreeSet *set;
    treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    treeset_add(set, &a);
    treeset_add(set, &b);
    treeset_add(set, &c);
    treeset_add(set, &c);

    size_t size = treeset_size(set);

    cc_assert(size == 3,
              cc_msg("treeset_add: Expected size was 3, but got %d",
                     size));

    cc_assert(treeset_contains(set, &a) &&
              treeset_contains(set, &b),
              cc_msg("treeset_add: TreeSet expected to contain elemnts"
                     " %d and %d", a, b));

    treeset_destroy(set);
}

void test_treeset_remove()
{
    TreeSet *set;
    treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    treeset_add(set, &a);
    treeset_add(set, &b);
    treeset_add(set, &c);

    treeset_remove(set, &a, NULL);

    size_t size = treeset_size(set);

    cc_assert(size == 2,
              cc_msg("treeset_add: Expected size was 2, but got %d",
                     size));

    cc_assert(!treeset_contains(set, &a),
              cc_msg("treeset_add: TreeSet not expected to contain "
                     "element %d", a));

    treeset_destroy(set);
}

void test_treeset_remove_all()
{
    TreeSet *set;
    treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    treeset_add(set, &a);
    treeset_add(set, &b);
    treeset_add(set, &c);

    treeset_remove_all(set);

    size_t size = treeset_size(set);

    cc_assert(size == 0,
              cc_msg("treeset_add: Expected size was 0, but got %d",
                     size));

    cc_assert(!treeset_contains(set, &a) &&
              !treeset_contains(set, &c),
              cc_msg("treeset_add: TreeSet not empty after removing"
                     " all elements"));

    treeset_destroy(set);
}

void test_treeset_size()
{
    TreeSet *set;
    treeset_new(cmp, &set);

    int a = 1;
    int b = 2;
    int c = 3;

    treeset_add(set, &a);
    treeset_add(set, &b);
    treeset_add(set, &c);

    cc_assert(treeset_size(set) == 3,
              cc_msg("treeset_size: Unexpected set size"));

    treeset_destroy(set);
}

void test_treeset_iter_next()
{
    TreeSet *t;
    treeset_new(cmp, &t);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treeset_add(t, &a);
    treeset_add(t, &b);
    treeset_add(t, &c);
    treeset_add(t, &d);

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;

    TreeSetIter iter;
    treeset_iter_init(&iter, t);

    void *e;
    while (treeset_iter_next(&iter, &e) != CC_ITER_END) {
        if (*((int*)e) == a)
            one++;

        if (*((int*)e) == b)
            two++;

        if (*((int*)e) == c)
            three++;

        if (*((int*)e) == d)
            four++;
    }

    bool asrt = (one   == 1) && (two  == 1)  &&
                (three == 1) && (four == 1);

    cc_assert(asrt,
              cc_msg("treeset_iter_next: Unexpected number"
                     " of entries returned"));

    treeset_destroy(t);
}

void test_treeset_iter_remove()
{
    TreeSet *t;
    treeset_new(cmp, &t);

    int a = 1;
    int b = 2;
    int c = 3;

    treeset_add(t, &a);
    treeset_add(t, &b);
    treeset_add(t, &c);

    TreeSetIter iter;
    treeset_iter_init(&iter, t);

    void *e;
    while (treeset_iter_next(&iter, &e) != CC_ITER_END) {
        if (*((int*)e) == b)
            treeset_iter_remove(&iter, NULL);
    }

    cc_assert(treeset_size(t) == 2,
              cc_msg("treeset_iter_remove: Expected size 2 but got %d ",
                     treeset_size(t)));

    cc_assert(!treeset_contains(t, &b),
              cc_msg("treeset_iter_remove: Element b  still pressent "
                     "after removal"));

    treeset_destroy(t);
}
