#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/slist.h"
#include "test.h"

SList *slist_1_to_10();
SList *slist_1234();
SList *slist_5677();

void test_slist_new();
void test_slist_add();
void test_slist_add_last();
void test_slist_add_first();
void test_slist_add_all();
void test_slist_add_all_at();
void test_slist_add_at();
void test_slist_remove();
void test_slist_remove_first();
void test_slist_remove_last();
void test_slist_remove_at();
void test_slist_remove_all();
void test_slist_get_first();
void test_slist_get_last();
void test_slist_get();
void test_slist_index_of();
void test_slist_contains();
void test_slist_replace_at();
void test_slist_copy_shallow();
void test_slist_copy_deep();
void test_slist_sublist();
void test_slist_sort();
void test_slist_iter();
void test_slist_reverse();
void test_slist_sort();
void test_slist_splice();
void test_slist_splice_at();
void test_slist_to_array();


int main(int argc, char **argv)
{
    cc_set_status(PASS);
    cc_set_exit_on_failure(false);

    test_slist_new();
    test_slist_add();
    test_slist_add_first();
    test_slist_add_last();
    test_slist_add_at();
    test_slist_add_all();
    test_slist_add_all_at();
    test_slist_remove();
    test_slist_remove_first();
    test_slist_remove_last();
    test_slist_remove_at();
    test_slist_remove_all();
    test_slist_get_first();
    test_slist_get_last();
    test_slist_get();
    test_slist_index_of();
    test_slist_contains();
    test_slist_replace_at();
    test_slist_copy_shallow();
    test_slist_copy_deep();
    test_slist_sublist();
    test_slist_sort();
    test_slist_reverse();
    test_slist_iter();
    test_slist_splice();
    test_slist_splice_at();
    test_slist_to_array();

    return cc_get_status();
}


void test_slist_validate_structure(SList *list, char *test_name)
{
    size_t expected_size = slist_size(list);
    size_t fw_size = 0;

    void *head = slist_get_first(list);
    void *tail = slist_get_last(list);

    SListIter i;
    slist_iter_init(&i, list);

    while (slist_iter_has_next(&i)) {
        void *e = slist_iter_next(&i);
        if (fw_size == 0) {
            cc_assert(head == e,
                      cc_msg("slist_structure_validate: "
                             "head not as expected during"
                             " ascending iteration at %s",
                             test_name));
        }
        fw_size++;

        if (!slist_iter_has_next(&i)) {
            cc_assert(tail == e,
                      cc_msg("slist_structure_validate: "
                             "tail not as expected during"
                             " ascending iteration at %s",
                             test_name));
        }
    }
    cc_assert(fw_size == expected_size,
              cc_msg("slist_structure_validate: "
                     "unexpected forward size at ts",
                     test_name));
}


void test_slist_new()
{
    SList *list  = slist_new();
    SList *list2 = slist_new();

    if (list == NULL || list2 == NULL)
      goto exit;

    cc_assert(slist_get_first(list) == NULL,
              cc_msg("slist_new: Expected NULL first element."));

    cc_assert(slist_get_last(list) == NULL,
              cc_msg("slist_new: Expected NULL last element."));

    cc_assert(slist_size(list) == 0,
              cc_msg("slist_new: Expected size 0, but got %d",
                     slist_size(list)));

    cc_assert(list != list2,
              cc_msg("slist_new: Expected a new instance"));

exit:
    slist_destroy(list);
    slist_destroy(list2);
}


void test_slist_add_last()
{
    SList *list = slist_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    int size = slist_size(list);
    int last = *((int*) slist_get_last(list));

    cc_assert(size == 4,
              cc_msg("slist_add_last: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(last == d,
           cc_msg("slist_add_last: Expected last"
                  " element was %d, but got %d!",
                  d, last));

    slist_add_last(list, &append);

    size = slist_size(list);
    last = *((int*) slist_get_last(list));

    cc_assert(size == 5,
              cc_msg("slist_add_last: Expected size"
                     " was 5, but got %d!", size));

    cc_assert(last == append,
              cc_msg("slist_add_last: Expected last "
                     "element was %d, but got %d!",
                     append, last));

    test_slist_validate_structure(list, "slist_add_last");

    slist_destroy(list);
}


void test_slist_add_first()
{
    SList *list = slist_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    int size = slist_size(list);
    int first = *((int*) slist_get_first(list));

    cc_assert(size == 4,
              cc_msg("slist_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(first == a,
              cc_msg("slist_add_first: Expectd first"
                     " element was %d, but got %d!",
                     prepend, first));

    slist_add_first(list, &prepend);

    size = slist_size(list);
    first = *((int*) slist_get_first(list));

    cc_assert(size == 5,
              cc_msg("slist_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(first == prepend,
              cc_msg("slist_add_first: Expected first"
                     " element was %d, but got %d",
                     prepend, first));

    test_slist_validate_structure(list, "slist_add_first");

    slist_destroy(list);
}


void test_slist_add()
{
    SList *list = slist_new();

    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    bool r1 = slist_add(list, s1);
    bool r2 = slist_add(list, s2);
    bool r3 = slist_add(list, s3);
    bool r4 = slist_add(list, s4);

    if (r1 && r2 && r3 && r4) {
         cc_assert(slist_size(list) == 4,
                   cc_msg("slist_add: Expected "
                          "ssize 4, but got %d",
                          slist_size(list)));
    }
    cc_assert(slist_get_first(list) != NULL,
              cc_msg("slist_add: First element"
                     " not expected to be NULL!"));

    cc_assert(slist_get_last(list) != NULL,
              cc_msg("slist_add: Last element"
                     " not expected to be NULL!"));


    test_slist_validate_structure(list, "slist_add");

    slist_destroy(list);
}


void test_slist_add_at()
{
    SList *list = slist_1234();

    int ins = 90;

    slist_add_at(list, &ins, 3);

    cc_assert(slist_size(list) == 5,
              cc_msg("slist_add_at: Expected size "
                     "was 5, but got %d!", slist_size(list)));

    int new = *((int*) slist_get(list, 3));

    cc_assert(new == ins,
              cc_msg("slist_add_at: Expected element"
                     " at index 2 was %d, but got %d!", ins, new));

    test_slist_validate_structure(list, "slist_add_at");

    slist_remove(list, &ins);
    slist_destroy_free(list);
}


void test_slist_add_all()
{
    SList *list = slist_1234();
    SList *list2 = slist_5677();

    slist_add_all(list2, list);

    cc_assert(slist_size(list2) == 8,
              cc_msg("slist_add_all: Expected size"
                     " was 8, but got %d!", slist_size(list2)));

    int l1last = *((int*) slist_get_last(list));
    int l2last = *((int*) slist_get_last(list2));

    cc_assert(l1last == l2last,
              cc_msg("slist_add_all: Expected last"
                     " element was %d, but got %d!",
                     l1last, l2last));

    test_slist_validate_structure(list, "slist_add_all");

    slist_destroy(list);
    slist_destroy_free(list2);
}


void test_slist_add_all_at()
{
    SList *list  = slist_1234();
    SList *list2 = slist_5677();

    slist_add_all_at(list, list2, 2);

    cc_assert(slist_size(list2) == 4,
              cc_msg("slist_add_all_at: Expected size"
                     " was 4, but got %d!", slist_size(list2)));

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_add_all_at: Expected size"
                     " was 8, but got %d!", slist_size(list)));

    int last = *((int*) slist_get_last(list));
    int l1i5 = *((int*) slist_get(list, 5));
    int l2i2 = *((int*) slist_get(list2, 2));

    cc_assert(last == 4,
              cc_msg("slist_add_all_at: Expected last"
                     " element was 4, but got %d!", last));

    cc_assert(l1i5 == l2i2,
              cc_msg("slist_add_all_at: Expected element"
                     " at index 5 was %d, but got %d!", l1i5, l2i2));

    test_slist_validate_structure(list, "slist_add_at");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_remove()
{
    SList *list = slist_1234();

    int *e = slist_get(list, 1);
    slist_remove(list, e);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove: Expected size"
                     " was 3, but got %d!", slist_size(list)));

    cc_assert(slist_contains(list, e) == 0,
              cc_msg("slist_remove: The list still"
                     " contains the removed element!"));

    test_slist_validate_structure(list, "slist_remove");

    slist_destroy_free(list);
}


void test_slist_remove_first()
{
    SList *list = slist_1234();

    slist_remove_first(list);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_first: Expected "
                     "size was 3, but got %d!",
                     slist_size(list)));

    int first = *((int*) slist_get_first(list));

    cc_assert(first == 2,
              cc_msg("slist_remove_first: Expected "
                     "first element was 2, but got %d!",
                     first));

    test_slist_validate_structure(list, "slist_remove_first");

    slist_destroy_free(list);
}


void test_slist_remove_last()
{
    SList *list = slist_1234();

    slist_remove_last(list);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_last: Expected"
                     " size was 3, but got %d!",
                     slist_size(list)));

    int last = *((int*) slist_get_last(list));

    cc_assert(last == 3,
              cc_msg("slist_remove_last: Expected"
                     " last element was 3, but got %d!",
                     last));

    test_slist_validate_structure(list, "slist_remove_last");

    slist_destroy_free(list);
}


void test_slist_remove_at()
{
    SList *list = slist_1234();

    slist_remove_at(list, 2);

    int e = *((int*) slist_get(list, 2));

    cc_assert(e == 4,
           cc_msg("slist_remove_at: Expected element"
                  " was 4, but got %d!", e));

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_at: Expected size"
                     " was 3, but got %d!", slist_size(list)));

    slist_remove_at(list, 0);

    e = *((int*) slist_get(list, 0));

    cc_assert(e == 2,
              cc_msg("slist_remove_at: Expected element"
                     " was 2, but got %d!", e));

    test_slist_validate_structure(list, "slist_remove_at");

    slist_destroy_free(list);
}


void test_slist_remove_all()
{
    SList *list = slist_1234();
    slist_remove_all(list);

    cc_assert(slist_size(list) == 0,
              cc_msg("slist_remove_all: Expected size"
                     " was 0, but got %d", slist_size(list)));

    cc_assert(slist_get_first(list) == NULL,
              cc_msg("slist_remove_all: First not NULL "
                     "as expected"));

    slist_destroy_free(list);
}


void test_slist_get_first()
{
    SList *list = slist_1234();
    int first = *((int*) slist_get_first(list));

    cc_assert(first == 1,
              cc_msg("slist_get_first: Expected first"
                     " element was 1, but got %d", first));

    slist_destroy_free(list);
}


void test_slist_get_last()
{
    SList *list = slist_1234();
    int last = *((int*) slist_get_last(list));

    cc_assert(last == 4,
              cc_msg("slist_get_last: Expected last"
                     " element was 4, but got %d", last));

    slist_destroy_free(list);
}


void test_slist_get()
{
    SList *list = slist_1234();
    int e = *((int*) slist_get(list, 1));

    cc_assert(e == 2,
              cc_msg("slist_get: Expected element at"
                     " index 1 was 2, but got %d", e));

    slist_destroy_free(list);
}


void test_slist_index_of()
{
    SList *list = slist_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    int i0 = slist_index_of(list, &a);
    int i1 = slist_index_of(list, &c);

    cc_assert(i0 == 0,
              cc_msg("slist_index_of: Expected "
                     "index 0, but got %d", i0));

    cc_assert(i1 == 2,
              cc_msg("slist_index_of: Expected "
                     "index 2, but got %d", i1));

    slist_destroy(list);
}


void test_slist_replace_at()
{
    SList *list = slist_1234();
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    slist_replace_at(list, replacement, 2);

    cc_assert(slist_get(list, 2) == replacement,
              cc_msg("slist_replace_at: Unexpected"
                     " element at index 2"));

    slist_destroy_free(list);
}


void test_slist_contains()
{
    SList *list = slist_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    int c1 = slist_contains(list, &b);
    int c2 = slist_contains(list, &d);
    int c3 = slist_contains(list, &e);

    cc_assert(c1 == 2,
              cc_msg("slist_contains: Expected number"
                     " contained was 2, but got %d", c1));

    cc_assert(c2 == 1,
              cc_msg("slist_contains: Expected number"
                     " contained was 1, but got %d", c2));

    cc_assert(c3 == 0,
              cc_msg("slist_contains: Expected number"
                     " contained was 0, but got %d", c3));

    slist_destroy(list);
}


void test_slist_copy_shallow()
{
    SList *list = slist_1234();
    SList *cp = slist_copy_shallow(list);

    cc_assert(slist_size(cp) == 4,
              cc_msg("slist_copy_shallow: Expected "
                     "size was 4, but got %d", slist_size(cp)));

    cc_assert(slist_get_first(cp) == slist_get_first(list),
              cc_msg("slist_copy_shallow: First element missmatch"));

    cc_assert(slist_get_last(cp) == slist_get_last(list),
              cc_msg("slist_copy_shallow: Last element missmatch"));

    cc_assert(slist_get(cp, 3) == slist_get(list, 3),
              cc_msg("slist_copy_shallow: Element at "
                     "index 3 missmatch"));

    slist_destroy_free(cp);
    slist_destroy(list);
}


void *copy(void *e1)
{
    int *cp = (int *) malloc(sizeof(int));
    *cp = *((int*) e1);
    return cp;
}


void test_slist_copy_deep()
{
    SList *list = slist_1234();
    SList *cp = slist_copy_deep(list, copy);

    cc_assert(slist_size(cp) == 4,
              cc_msg("slist_copy_deep: Expected size"
                     " was 4, but got %d", slist_size(cp)));

    int e = *((int*) slist_get(cp, 2));

    cc_assert(e == *((int*)slist_get(list, 2)),
              cc_msg("slist_copy_deep: Expected element"
                     " at index 2 was 3, but got %d", e));

    cc_assert(slist_get(cp, 2) != slist_get(list, 2),
              cc_msg("slist_copy_deep: Both lists point"
                     " to the same value at index 2"));

    slist_destroy_free(cp);
    slist_destroy_free(list);
}


void test_slist_sublist()
{
    SList *list = slist_1234();

    SList *sub = slist_sublist(list, 1, 2);

    cc_assert(slist_size(sub) == 2,
              cc_msg("slist_sublist: Expected size"
                     " was 2 but got %d", slist_size(sub)));

    int s1 = *((int*)slist_get(sub, 1));
    int l2 = *((int*)slist_get(list, 2));

    cc_assert(s1 == l2,
              cc_msg("slist_sublist: Expected element"
                     " at index 1 was %d, but got %d", l2, s1));

    slist_destroy(sub);
    slist_destroy_free(list);
}


void test_slist_splice()
{
    SList *list = slist_1234();
    SList *list2 = slist_5677();

    slist_splice(list, list2);

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_splice: Expected "
                     "size was 8, but got %d!",
                     slist_size(list)));

    cc_assert(slist_size(list2) == 0,
              cc_msg("slist_splice: Expected "
                     "size was 0, but got %d!",
                     slist_size(list2)));

    int last = *((int*) slist_get_last(list));
    int first = *((int*) slist_get_first(list));
    int i4 = *((int*) slist_get(list, 4));

    cc_assert(last == 7,
              cc_msg("slist_splice: Expected last"
                     " element was 7, but got %d!",
                     last));

    cc_assert(first == 1,
              cc_msg("slist_splice: Expected first"
                     " element was 1, but got %d!",
                     first));

    cc_assert(i4 == 5,
              cc_msg("slist_splice: Expected element"
                     " at index 4 was 5, but got %d!",
                     i4));

    test_slist_validate_structure(list, "slist_splice");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_splice_at()
{
    SList *list = slist_1234();
    SList *list2 = slist_5677();

    slist_splice_at(list, list2, 2);

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_splice_after: Expected"
                     " size was 8, but got %d!", slist_size(list)));

    cc_assert(slist_size(list2) == 0,
              cc_msg("slist_splice_after: Expected"
                     " size was 0, but got %d!", slist_size(list2)));

    int first = *((int*) slist_get_first(list));
    int last  = *((int*) slist_get_last(list));
    int i2    = *((int*) slist_get(list, 2));

    cc_assert(first == 1,
              cc_msg("slist_splice_after: Expected"
                     " first element was 1, but got %d!", first));

    cc_assert(last == 4,
              cc_msg("slist_splice_after: Expected"
                     " last element was 4, but got %d!", last));

    cc_assert(i2 == 5,
              cc_msg("slist_splice_after: Expected"
                     " element at index 2 was 5, but got %d", i2));

    test_slist_validate_structure(list, "slist_splice_at");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_to_array()
{
    SList *list = slist_1234();
    int **array = (int**) slist_to_array(list);

    cc_assert(slist_get(list, 0) == array[0],
              cc_msg("slist_to_array: Array index 0"
                     " does not match list index 0"));

    cc_assert(slist_get(list, 2) == array[2],
              cc_msg("slist_to_array: Array index 2"
                     " does not match list index 2"));

    cc_assert(slist_get(list, 3) == array[3],
              cc_msg("slist_to_array: Array index 3"
                     " does not match list index 3"));

    free(array);
    slist_destroy_free(list);
}

void test_slist_iter_add()
{
    SList *list = slist_1234();
    int ins = 32;

    SListIter iter;
    slist_iter_init(&iter, list);

    for (;slist_iter_has_next(&iter);) {
        int e = *((int*) slist_iter_next(&iter));

        if (e == 3) {
            int i = slist_iter_index(&iter);
            slist_iter_add(&iter, &ins);
        }
    }

    cc_assert(slist_size(list) == 5,
              cc_msg("slist_iter_add: Expected size"
                     " was 5, but got %d", slist_size(list)));

    int li3 = *((int*) slist_get(list, 3));
    int li4 = *((int*) slist_get(list, 4));

    cc_assert(li3 == ins,
              cc_msg("slist_iter_add: Expected element at"
                     " index 3 was %d, but got %d", ins, li3));

    cc_assert(li4 == 4,
              cc_msg("slist_iter_add: Expected element at"
                     " index 4 was 4, but got %d,", li4));

    test_slist_validate_structure(list, "list_iter_add");

    slist_destroy(list);
}


void test_slist_iter_remove()
{
    SList *list = slist_1234();
    int  *rm   = slist_get(list, 2);

    SListIter iter;
    slist_iter_init(&iter, list);

    for(;slist_iter_has_next(&iter);) {
        int e = *((int*) slist_iter_next(&iter));
        if (e == 3) {
             slist_iter_remove(&iter);
        }
    }
    cc_assert(slist_size(list) == 3,
              cc_msg("slist_iter_remove: Expected size"
                     " was 3, but got %d", slist_size(list)));

    cc_assert(slist_contains(list, rm) == 0,
              cc_msg("slist_iter_remove: List contains"
                     " unique element after removal"));

    test_slist_validate_structure(list, "slist_iter_remove");

    slist_destroy(list);
}


void test_slist_iter()
{
    //test_slist_iter_add();
     test_slist_iter_remove();
}


void p(void *e)
{
    int *i = e;
    printf("%d ", *i);
}


int cmp(void const *e1, void const *e2)
{
    int i = *(*((int**) e1));
    int j = *(*((int**) e2));

    if (i < j)
        return -1;
    if (i > j)
        return 1;
    return 0;
}


void test_slist_sort()
{
    SList *l = slist_new();

    srand(time(NULL));

    int size = 1000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 100000;
        slist_add(l, e);
    }
    slist_sort(l, cmp);

    SListIter iter;
    slist_iter_init(&iter, l);

    int        prev    = 0;
    int        current = 0;
    void      *e       = slist_iter_next(&iter);

    prev = *((int*) slist_iter_next(&iter));
    while (slist_iter_has_next(&iter)) {
        e = slist_iter_next(&iter);
        current = *((int*) e);

        cc_assert(prev <= current,
                  cc_msg("slist_sort: preceding "
                         "element greater than the current"));

        prev = current;
    }
    slist_destroy_free(l);
}

void test_slist_reverse()
{
    SList *l = slist_1_to_10();

    slist_foreach(l, p);
    printf("\n");

    slist_reverse(l);

    slist_foreach(l, p);
    printf("\n");
}

SList *slist_1234()
{
    SList *list = slist_new();

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    slist_add(list, a);
    slist_add(list, b);
    slist_add(list, c);
    slist_add(list, d);

    return list;
}


SList *slist_5677()
{
    SList *list = slist_new();

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 7;

    slist_add(list, a);
    slist_add(list, b);
    slist_add(list, c);
    slist_add(list, d);

    return list;
}


SList *slist_1_to_10()
{
    SList *list = slist_new();

    int *e1 = (int*) malloc(sizeof(int));
    int *e2 = (int*) malloc(sizeof(int));
    int *e3 = (int*) malloc(sizeof(int));
    int *e4 = (int*) malloc(sizeof(int));
    int *e5 = (int*) malloc(sizeof(int));
    int *e6 = (int*) malloc(sizeof(int));
    int *e7 = (int*) malloc(sizeof(int));
    int *e8 = (int*) malloc(sizeof(int));
    int *e9 = (int*) malloc(sizeof(int));
    int *e10 = (int*) malloc(sizeof(int));

    *e1 = 1;
    *e2 = 2;
    *e3 = 3;
    *e4 = 4;
    *e5 = 5;
    *e6 = 6;
    *e7 = 7;
    *e8 = 8;
    *e9 = 9;
    *e10 = 10;

    slist_add(list, e1);
    slist_add(list, e2);
    slist_add(list, e3);
    slist_add(list, e4);
    slist_add(list, e5);
    slist_add(list, e6);
    slist_add(list, e7);
    slist_add(list, e8);
    slist_add(list, e9);
    slist_add(list, e10);

    return list;
}
