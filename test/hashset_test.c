#include "../src/hashset.h"
#include "test.h"
#include <string.h>

void test_hashset_new();
void test_hashset_add();
void test_hashset_remove();
void test_hashset_remove_all();
void test_hashset_iter();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_hashset_new();
    test_hashset_add();
    test_hashset_remove();
    test_hashset_remove_all();
    test_hashset_iter();

    return cc_get_status();
}

void test_hashset_new()
{
    HashSetConf conf;
    hashset_conf_init(&conf);
    conf.initial_capacity = 7;

    HashSet *set;
    hashset_new_conf(&conf, &set);

    cc_assert(hashset_size(set) == 0,
              cc_msg("hashset_new: Initial size not 0"));

    size_t capacity = hashset_capacity(set);

    /* power of 2 rounding */
    cc_assert(capacity == 8,
              cc_msg("hashset_new: Expected capactity was 8, but got %d",
                     capacity));

    hashset_destroy(set);
}

void test_hashset_add()
{
    HashSet *hs;
    hashset_new(&hs);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(hs, a);
    hashset_add(hs, b);
    hashset_add(hs, c);
    hashset_add(hs, d);

    size_t size = hashset_size(hs);

    cc_assert(size == 3,
              cc_msg("hashset_add: Expected size was 3, but got %d",
                     size));

    cc_assert(hashset_contains(hs, a) &&
              hashset_contains(hs, d),
              cc_msg("hashset_add: HashSet expected to contain elemnts"
                     " %s and %s", a, d));

    hashset_destroy(hs);
}


void test_hashset_remove()
{
    HashSet *hs;
    hashset_new(&hs);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(hs, a);
    hashset_add(hs, b);
    hashset_add(hs, c);
    hashset_add(hs, d);

    hashset_remove(hs, "bar", NULL);

    size_t size = hashset_size(hs);

    cc_assert(size == 2,
              cc_msg("hashset_add: Expected size was 2, but got %d",
                     size));

    cc_assert(!hashset_contains(hs, "bar"),
              cc_msg("hashset_add: HashSet not expected to contain "
                     "element %s", "foo"));

    hashset_destroy(hs);
}


void test_hashset_remove_all()
{
    HashSet *hs;
    hashset_new(&hs);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";
    char *d = "foo";

    hashset_add(hs, a);
    hashset_add(hs, b);
    hashset_add(hs, c);
    hashset_add(hs, d);

    hashset_remove_all(hs);

    size_t size = hashset_size(hs);

    cc_assert(size == 0,
              cc_msg("hashset_add: Expected size was 0, but got %d",
                     size));

    cc_assert(!hashset_contains(hs, "bar") &&
              !hashset_contains(hs, c),
              cc_msg("hashset_add: HashSet not empty after removing"
                     " all elements"));

    hashset_destroy(hs);
}

void test_hashset_iter_next()
{
    HashSet *hs;
    hashset_new(&hs);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashset_add(hs, a);
    hashset_add(hs, b);
    hashset_add(hs, c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    HashSetIter iter;
    hashset_iter_init(&iter, hs);

    char *e;
    while (hashset_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (!strcmp(e, "foo"))
            x++;

        if (!strcmp(e, "bar"))
            y++;

        if (!strcmp(e, "baz"))
            z++;
    }

    cc_assert((x == 1) && (y == 1) && (z == 1),
              cc_msg("hashset_iter: Unexpected number of "
                     "elements returned by the iterator"));

    hashset_destroy(hs);
}

void test_hashset_iter_remove()
{
    HashSet *hs;
    hashset_new(&hs);

    char *a = "foo";
    char *b = "bar";
    char *c = "baz";

    hashset_add(hs, a);
    hashset_add(hs, b);
    hashset_add(hs, c);

    HashSetIter iter;
    hashset_iter_init(&iter, hs);

    char *e;
    while (hashset_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (!strcmp(e, "bar"))
            hashset_iter_remove(&iter, NULL);
    }

    cc_assert(hashset_size(hs) == 2,
              cc_msg("hashset_iter: Expected size 2 but got %d ",
                     hashset_size(hs)));

    cc_assert(!hashset_contains(hs, "bar"),
              cc_msg("hashset_iter: Element (%s) still pressent "
                     "after removal", "bar"));

    hashset_destroy(hs);
}

void test_hashset_iter()
{
    test_hashset_iter_next();
    test_hashset_iter_remove();
}
