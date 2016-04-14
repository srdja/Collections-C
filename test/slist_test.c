#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/slist.h"
#include "test.h"

void slist_1_to_10(SList **list);
void slist_1234(SList **list);
void slist_5677(SList **list);

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

void test_slist_zip_iter();
void test_slist_zip_iter_next();
void test_slist_zip_iter_add();
void test_slist_zip_iter_remove();
void test_slist_zip_iter_replace();


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
    test_slist_zip_iter();

    return cc_get_status();
}


void test_slist_validate_structure(SList *list, char *test_name)
{
    size_t expected_size = slist_size(list);
    size_t fw_size = 0;

    void *head;
    slist_get_first(list, &head);

    void *tail;
    slist_get_last(list, &tail);

    SListIter i;
    slist_iter_init(&i, list);

    void *e;
    while (slist_iter_next(&i, &e) != CC_ITER_END) {
        if (fw_size == 0) {
            cc_assert(head == e,
                      cc_msg("slist_structure_validate: "
                             "head not as expected during"
                             " ascending iteration at %s",
                             test_name));
        }
        fw_size++;
    }
    cc_assert(fw_size == expected_size,
              cc_msg("slist_structure_validate: "
                     "unexpected forward size at %s. Expected %d, but got %d",
                     test_name, expected_size, fw_size));
}


void test_slist_new()
{
    SList *list = NULL;
    slist_new(&list);

    SList *list2 = NULL;
    slist_new(&list2);

    if (list == NULL || list2 == NULL)
      goto exit;


    void *e = NULL;
    slist_get_first(list, &e);
    cc_assert(e == NULL,
              cc_msg("slist_new: Expected NULL first element."));

    slist_get_last(list, &e);
    cc_assert(e == NULL,
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
    SList *list;
    slist_new(&list);

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
    int *last;
    slist_get_last(list, (void*) &last);

    cc_assert(size == 4,
              cc_msg("slist_add_last: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*last == d,
           cc_msg("slist_add_last: Expected last"
                  " element was %d, but got %d!",
                  d, *last));

    slist_add_last(list, &append);

    size = slist_size(list);

    slist_get_last(list, (void*) &last);

    cc_assert(size == 5,
              cc_msg("slist_add_last: Expected size"
                     " was 5, but got %d!", size));

    cc_assert(*last == append,
              cc_msg("slist_add_last: Expected last "
                     "element was %d, but got %d!",
                     append, *last));

    test_slist_validate_structure(list, "slist_add_last");

    slist_destroy(list);
}


void test_slist_add_first()
{
    SList *list;
    slist_new(&list);

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
    int *first;
    slist_get_first(list, (void*) &first);

    cc_assert(size == 4,
              cc_msg("slist_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*first == a,
              cc_msg("slist_add_first: Expectd first"
                     " element was %d, but got %d!",
                     prepend, *first));

    slist_add_first(list, &prepend);

    size = slist_size(list);
    slist_get_first(list, (void*) &first);

    cc_assert(size == 5,
              cc_msg("slist_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*first == prepend,
              cc_msg("slist_add_first: Expected first"
                     " element was %d, but got %d",
                     prepend, *first));

    test_slist_validate_structure(list, "slist_add_first");

    slist_destroy(list);
}


void test_slist_add()
{
    SList *list;
    slist_new(&list);

    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    enum cc_stat r1 = slist_add(list, s1);
    enum cc_stat r2 = slist_add(list, s2);
    enum cc_stat r3 = slist_add(list, s3);
    enum cc_stat r4 = slist_add(list, s4);

    if (r1 == CC_OK && r2 == CC_OK && r3 == CC_OK && r4 == CC_OK) {
         cc_assert(slist_size(list) == 4,
                   cc_msg("slist_add: Expected "
                          "ssize 4, but got %d",
                          slist_size(list)));
    }

    void *e;
    slist_get_first(list, &e);
    cc_assert(e != NULL,
              cc_msg("slist_add: First element"
                     " not expected to be NULL!"));

    slist_get_last(list, &e);
    cc_assert(e != NULL,
              cc_msg("slist_add: Last element"
                     " not expected to be NULL!"));


    test_slist_validate_structure(list, "slist_add");

    slist_destroy(list);
}


void test_slist_add_at()
{
    SList *list;
    slist_1234(&list);

    int ins = 90;

    slist_add_at(list, &ins, 2);

    cc_assert(slist_size(list) == 5,
              cc_msg("slist_add_at: Expected size "
                     "was 5, but got %d!", slist_size(list)));

    int *new;
    slist_get_at(list, 2, (void*)&new);

    cc_assert(*new == ins,
              cc_msg("slist_add_at: Expected element"
                     " at index 2 was %d, but got %d!", ins, *new));

    test_slist_validate_structure(list, "slist_add_at");

    enum cc_stat status = slist_add_at(list, &ins, 4);
    cc_assert(status == CC_OK,
              cc_msg("slist_add_at: Expected error while adding to index 4"));

    status = slist_add_at(list, &ins, 0);
    cc_assert(status == CC_OK,
              cc_msg("slist_add_at: Expected error while adding to index 0"));

    void *first;
    slist_get_first(list, &first);
    cc_assert(*((int*) first) == ins,
              cc_msg("slist_add_at: Expected first to be %d, but got %d instead",
                     ins, *((int*) first)));

    void *last;
    slist_get_first(list, &last);
    cc_assert(*((int*) last) == ins,
              cc_msg("slist_add_at: Expected last to be %d, but got %d instead",
                     ins, *((int*) last)));

    test_slist_validate_structure(list, "slist_add_at");

    slist_remove(list, &ins, NULL);
    slist_remove(list, &ins, NULL);
    slist_remove(list, &ins, NULL);
    slist_destroy_free(list);
}


void test_slist_add_all()
{
    SList *list;
    slist_1234(&list);

    SList *list2;
    slist_5677(&list2);

    slist_add_all(list2, list);

    cc_assert(slist_size(list2) == 8,
              cc_msg("slist_add_all: Expected size"
                     " was 8, but got %d!", slist_size(list2)));

    int *l1last;
    slist_get_last(list, (void*) &l1last);

    int *l2last;
    slist_get_last(list2, (void*) &l2last);

    cc_assert(*l1last == *l2last,
              cc_msg("slist_add_all: Expected last"
                     " element was %d, but got %d!",
                     *l1last, *l2last));

    test_slist_validate_structure(list, "slist_add_all");

    slist_destroy(list);
    slist_destroy_free(list2);
}


void test_slist_add_all_at()
{
    SList *list;
    slist_1234(&list);

    SList *list2;
    slist_5677(&list2);

    slist_add_all_at(list, list2, 2);

    cc_assert(slist_size(list2) == 4,
              cc_msg("slist_add_all_at: Expected size"
                     " was 4, but got %d!", slist_size(list2)));

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_add_all_at: Expected size"
                     " was 8, but got %d!", slist_size(list)));

    int *last;
    slist_get_last(list, (void*) &last);

    int *l1i5;
    slist_get_at(list, 5, (void*) &l1i5);

    int *l2i2;
    slist_get_at(list2, 2, (void*) &l2i2);

    cc_assert(*last == 4,
              cc_msg("slist_add_all_at: Expected last"
                     " element was 4, but got %d!", *last));

    cc_assert(*l1i5 == *l2i2,
              cc_msg("slist_add_all_at: Expected element"
                     " at index 5 was %d, but got %d!", *l1i5, *l2i2));

    test_slist_validate_structure(list, "slist_add_at");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_remove()
{
    SList *list;
    slist_1234(&list);

    int *e;
    slist_get_at(list, 1, (void*) &e);

    slist_remove(list, e, NULL);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove: Expected size"
                     " was 3, but got %d!", slist_size(list)));

    cc_assert(slist_contains(list, e) == 0,
              cc_msg("slist_remove: The list still"
                     " contains the removed element!"));

    free(e);

    test_slist_validate_structure(list, "slist_remove");

    slist_destroy_free(list);
}


void test_slist_remove_first()
{
    SList *list;
    slist_1234(&list);

    int *first;
    slist_remove_first(list, (void*) &first);
    free(first);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_first: Expected "
                     "size was 3, but got %d!",
                     slist_size(list)));

    slist_get_first(list, (void*) &first);

    cc_assert(*first == 2,
              cc_msg("slist_remove_first: Expected "
                     "first element was 2, but got %d!",
                     *first));

    test_slist_validate_structure(list, "slist_remove_first");

    slist_destroy_free(list);
}


void test_slist_remove_last()
{
    SList *list;
    slist_1234(&list);

    int *last;
    slist_remove_last(list, (void*) &last);
    free(last);

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_last: Expected"
                     " size was 3, but got %d!",
                     slist_size(list)));

    slist_get_last(list, (void*) &last);

    cc_assert(*last == 3,
              cc_msg("slist_remove_last: Expected"
                     " last element was 3, but got %d!",
                     *last));

    test_slist_validate_structure(list, "slist_remove_last");

    slist_destroy_free(list);
}


void test_slist_remove_at()
{
    SList *list;
    slist_1234(&list);

    int *e;
    slist_remove_at(list, 2, (void*) &e);
    free(e);

    slist_get_at(list, 2, (void*) &e);

    cc_assert(*e == 4,
           cc_msg("slist_remove_at: Expected element"
                  " was 4, but got %d!", *e));

    cc_assert(slist_size(list) == 3,
              cc_msg("slist_remove_at: Expected size"
                     " was 3, but got %d!", slist_size(list)));

    slist_remove_at(list, 0, (void*) &e);
    free(e);

    slist_get_at(list, 0, (void*) &e);

    cc_assert(*e == 2,
              cc_msg("slist_remove_at: Expected element"
                     " was 2, but got %d!", *e));

    test_slist_validate_structure(list, "slist_remove_at");

    slist_destroy_free(list);
}


void test_slist_remove_all()
{
    SList *list;
    slist_1234(&list);
    slist_remove_all_free(list);

    cc_assert(slist_size(list) == 0,
              cc_msg("slist_remove_all: Expected size"
                     " was 0, but got %d", slist_size(list)));

    void *e = NULL;
    slist_get_first(list, &e);
    cc_assert(e == NULL,
              cc_msg("slist_remove_all: First not NULL "
                     "as expected"));

    slist_destroy_free(list);
}


void test_slist_get_first()
{
    SList *list;
    slist_1234(&list);

    int *first;
    slist_get_first(list, (void*) &first);

    cc_assert(*first == 1,
              cc_msg("slist_get_first: Expected first"
                     " element was 1, but got %d", *first));

    slist_destroy_free(list);
}


void test_slist_get_last()
{
    SList *list;
    slist_1234(&list);

    int *last;
    slist_get_last(list, (void*) &last);

    cc_assert(*last == 4,
              cc_msg("slist_get_last: Expected last"
                     " element was 4, but got %d", *last));

    slist_destroy_free(list);
}


void test_slist_get()
{
    SList *list;
    slist_1234(&list);

    int *e;
    slist_get_at(list, 1, (void*) &e);

    cc_assert(*e == 2,
              cc_msg("slist_get: Expected element at"
                     " index 1 was 2, but got %d", *e));

    slist_destroy_free(list);
}


void test_slist_index_of()
{
    SList *list;
    slist_new(&list);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    slist_add(list, &a);
    slist_add(list, &b);
    slist_add(list, &c);
    slist_add(list, &d);

    size_t i0;
    slist_index_of(list, &a, &i0);
    size_t i1;
    slist_index_of(list, &c, &i1);

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
    SList *list;
    slist_1234(&list);

    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *e;
    slist_replace_at(list, replacement, 2, (void*) &e);
    free(e);

    slist_get_at(list, 2, (void*) &e);
    cc_assert(e == replacement,
              cc_msg("slist_replace_at: Unexpected"
                     " element at index 2"));

    slist_destroy_free(list);
}


void test_slist_contains()
{
    SList *list;
    slist_new(&list);

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

    size_t c1 = slist_contains(list, &b);
    size_t c2 = slist_contains(list, &d);
    size_t c3 = slist_contains(list, &e);

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
    SList *list;
    slist_1234(&list);

    SList *cp;
    slist_copy_shallow(list, &cp);

    cc_assert(slist_size(cp) == 4,
              cc_msg("slist_copy_shallow: Expected "
                     "size was 4, but got %d", slist_size(cp)));

    void *e1;
    void *e2;

    slist_get_first(cp, &e1);
    slist_get_first(list, &e2);
    cc_assert(e1 == e2,
              cc_msg("slist_copy_shallow: First element missmatch"));

    slist_get_last(cp, &e1);
    slist_get_last(list, &e2);
    cc_assert(e1 == e2,
              cc_msg("slist_copy_shallow: Last element missmatch"));

    slist_get_at(cp, 3, &e1);
    slist_get_at(list, 3, &e2);
    cc_assert(e1 == e2,
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
    SList *list;
    slist_1234(&list);

    SList *cp;
    slist_copy_deep(list, copy, &cp);

    cc_assert(slist_size(cp) == 4,
              cc_msg("slist_copy_deep: Expected size"
                     " was 4, but got %d", slist_size(cp)));

    int *e;
    slist_get_at(cp, 2, (void*) &e);

    void *e2;
    slist_get_at(list, 2, &e2);
    cc_assert(*e == *((int*) e2),
              cc_msg("slist_copy_deep: Expected element"
                     " at index 2 was 3, but got %d", e));

    slist_get_at(list, 2, &e2);
    slist_get_at(cp, 2, (void*) &e);

    cc_assert(e != e2,
              cc_msg("slist_copy_deep: Both lists point"
                     " to the same value at index 2"));

    slist_destroy_free(cp);
    slist_destroy_free(list);
}


void test_slist_sublist()
{
    SList *list;
    slist_1234(&list);

    SList *sub;
    slist_sublist(list, 1, 2, &sub);

    cc_assert(slist_size(sub) == 2,
              cc_msg("slist_sublist: Expected size"
                     " was 2 but got %d", slist_size(sub)));

    int *s1;
    slist_get_at(sub, 1, (void*) &s1);

    int *l2;
    slist_get_at(list, 2, (void*) &l2);

    cc_assert(*s1 == *l2,
              cc_msg("slist_sublist: Expected element"
                     " at index 1 was %d, but got %d", *l2, *s1));

    slist_destroy(sub);
    slist_destroy_free(list);
}


void test_slist_splice()
{
    SList *list;
    slist_1234(&list);

    SList *list2;
    slist_5677(&list2);

    slist_splice(list, list2);

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_splice: Expected "
                     "size was 8, but got %d!",
                     slist_size(list)));

    cc_assert(slist_size(list2) == 0,
              cc_msg("slist_splice: Expected "
                     "size was 0, but got %d!",
                     slist_size(list2)));

    int *last;
    slist_get_last(list, (void*) &last);

    int *first;
    slist_get_first(list, (void*) &first);

    int *i4;
    slist_get_at(list, 4, (void*) &i4);

    cc_assert(*last == 7,
              cc_msg("slist_splice: Expected last"
                     " element was 7, but got %d!",
                     *last));

    cc_assert(*first == 1,
              cc_msg("slist_splice: Expected first"
                     " element was 1, but got %d!",
                     *first));

    cc_assert(*i4 == 5,
              cc_msg("slist_splice: Expected element"
                     " at index 4 was 5, but got %d!",
                     *i4));

    test_slist_validate_structure(list, "slist_splice");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_splice_at()
{
    SList *list;
    slist_1234(&list);

    SList *list2;
    slist_5677(&list2);

    slist_splice_at(list, list2, 2);

    cc_assert(slist_size(list) == 8,
              cc_msg("slist_splice_after: Expected"
                     " size was 8, but got %d!", slist_size(list)));

    cc_assert(slist_size(list2) == 0,
              cc_msg("slist_splice_after: Expected"
                     " size was 0, but got %d!", slist_size(list2)));

    int *first;
    slist_get_first(list, (void*) &first);

    int *last;
    slist_get_last(list, (void*) &last);

    int *i2;
    slist_get_at(list, 2, (void*) &i2);

    cc_assert(*first == 1,
              cc_msg("slist_splice_after: Expected"
                     " first element was 1, but got %d!", *first));

    cc_assert(*last == 4,
              cc_msg("slist_splice_after: Expected"
                     " last element was 4, but got %d!", *last));

    cc_assert(*i2 == 5,
              cc_msg("slist_splice_after: Expected"
                     " element at index 2 was 5, but got %d", *i2));

    test_slist_validate_structure(list, "slist_splice_at");

    slist_destroy(list2);
    slist_destroy_free(list);
}


void test_slist_to_array()
{
    SList *list;
    slist_1234(&list);

    int **array;
    slist_to_array(list, (void*) &array);

    void *e;
    slist_get_at(list, 0, &e);
    cc_assert(e == array[0],
              cc_msg("slist_to_array: Array index 0"
                     " does not match list index 0"));

    slist_get_at(list, 2, &e);
    cc_assert(e == array[2],
              cc_msg("slist_to_array: Array index 2"
                     " does not match list index 2"));

    slist_get_at(list, 3, &e);
    cc_assert(e == array[3],
              cc_msg("slist_to_array: Array index 3"
                     " does not match list index 3"));

    free(array);
    slist_destroy_free(list);
}

void test_slist_iter_add()
{
    SList *list;
    slist_1234(&list);
    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    SListIter iter;
    slist_iter_init(&iter, list);

    int *el;
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            slist_iter_add(&iter, ins);
    }

    cc_assert(slist_size(list) == 5,
              cc_msg("slist_iter_add: Expected size"
                     " was 5, but got %d", slist_size(list)));

    int *li3;
    slist_get_at(list, 3, (void*) &li3);

    cc_assert(*li3 == *ins,
              cc_msg("slist_iter_add: Expected element at"
                     " index 3 was %d, but got %d", *ins, *li3));

    int *li4;
    slist_get_at(list, 4, (void*) &li4);

    cc_assert(*li4 == 4,
              cc_msg("slist_iter_add: Expected element at"
                     " index 4 was 4, but got %d,", *li4));

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    slist_iter_init(&iter, list);
    while (slist_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4)
            slist_iter_add(&iter, ins);
    }

    void *e;
    slist_get_last(list, &e);

    cc_assert(*((int*) e) == *ins,
              cc_msg("slist_iter_add: Expected last element to be %d"
                     ", but got %d instead", *ins, *((int*)e)));

    cc_assert(slist_size(list) == 6,
              cc_msg("slist_iter_add: Expected size "
                     "was 6 but got %d", slist_size(list)));

    test_slist_validate_structure(list, "list_iter_add");

    slist_destroy_free(list);
}


void test_slist_iter_remove()
{
    SList *list;
    slist_1234(&list);
    int  *rm;
    slist_get_at(list, 2, (void*) &rm);

    SListIter iter;
    slist_iter_init(&iter, list);

    int *e;
    while (slist_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            slist_iter_remove(&iter, NULL);
            free(e);
        }
    }
    cc_assert(slist_size(list) == 3,
              cc_msg("slist_iter_remove: Expected size"
                     " was 3, but got %d", slist_size(list)));

    cc_assert(slist_contains(list, rm) == 0,
              cc_msg("slist_iter_remove: List contains"
                     " unique element after removal"));

    test_slist_validate_structure(list, "slist_iter_remove");

    slist_destroy_free(list);
}


void test_slist_iter()
{
    test_slist_iter_add();
    test_slist_iter_remove();
}


void test_slist_zip_iter()
{
    test_slist_zip_iter_next();
    test_slist_zip_iter_add();
    test_slist_zip_iter_remove();
    test_slist_zip_iter_replace();
}


void test_slist_zip_iter_next()
{
    SList *a1;
    slist_new(&a1);

    slist_add(a1, "a");
    slist_add(a1, "b");
    slist_add(a1, "c");
    slist_add(a1, "d");

    SList *a2;
    slist_new(&a2);

    slist_add(a2, "e");
    slist_add(a2, "f");
    slist_add(a2, "g");

    SListZipIter zip;
    slist_zip_iter_init(&zip, a1, a2);

    size_t i = 0;

    void *e1, *e2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            cc_assert(strcmp((char*) e1, "a") == 0,
                      cc_msg("slist_zip_iter_next: Expected e1 was \"a\" at index 0, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "e") == 0,
                      cc_msg("slist_zip_iter_next: Expected e1 was \"e\" at index 0, but got %s instead",
                             (char*) e2));
        }
        if (i == 2) {
            cc_assert(strcmp((char*) e1, "c") == 0,
                      cc_msg("slist_zip_iter_next: Expected e1 was \"a\" at index 2, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "g") == 0,
                      cc_msg("slist_zip_iter_next: Expected e1 was \"e\" at index 2, but got %s instead",
                             (char*) e2));
        }
        i++;
    }
    cc_assert(i == 3,
              cc_msg("slist_zip_iter_next: Expected 3 iterations, but got %d instead", i));

    slist_destroy(a1);
    slist_destroy(a2);
}


void test_slist_zip_iter_add()
{
    SList *a1;
    slist_new(&a1);

    slist_add(a1, "a");
    slist_add(a1, "b");
    slist_add(a1, "c");
    slist_add(a1, "d");

    SList *a2;
    slist_new(&a2);

    slist_add(a2, "e");
    slist_add(a2, "f");
    slist_add(a2, "g");

    char *h = "h";
    char *i = "i";

    SListZipIter zip;
    slist_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_add(&zip, h, i);
    }

    size_t index;
    slist_index_of(a1, "h", &index);

    cc_assert(index == 2,
              cc_msg("slist_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "h", index));

    slist_index_of(a1, "i", &index);
    cc_assert(index == 2,
              cc_msg("slist_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "i", index));

    slist_index_of(a1, "c", &index);
    cc_assert(index == 3,
              cc_msg("slist_zip_iter_add: Expected element %s to be at index 3"
                     " but was found at %d", "c", index));

    cc_assert(slist_contains(a1, "h") == 1,
              cc_msg("slist_zip_iter_add: Element %s not presetn after addition", "h"));

    cc_assert(slist_contains(a2, "i") == 1,
              cc_msg("slist_zip_iter_add: Element %s not presetn after addition", "i"));

    cc_assert(slist_size(a1) == 5,
              cc_msg("slist_zip_iter_add: Expected size 5, but got %d", slist_size(a1)));

    cc_assert(slist_size(a2) == 4,
              cc_msg("slist_zip_iter_add: Expected size 4, but got %d", slist_size(a2)));

    slist_zip_iter_init(&zip, a1, a2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            slist_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    slist_get_last(a2, (void*) &last);
    cc_assert(strcmp(last, "y") == 0,
              cc_msg("slist_zip_iter_add: Expected last element to be %s, but got %s instead", "y", last));

    slist_get_last(a1, (void*) &last);
    cc_assert(strcmp(last, "d") == 0,
              cc_msg("slist_zip_iter_add: Expected last element to be %s, but got %s instead", "d", last));

    slist_destroy(a1);
    slist_destroy(a2);
}


void test_slist_zip_iter_remove()
{
    SList *a1;
    slist_new(&a1);

    slist_add(a1, "a");
    slist_add(a1, "b");
    slist_add(a1, "c");
    slist_add(a1, "d");

    SList *a2;
    slist_new(&a2);

    slist_add(a2, "e");
    slist_add(a2, "f");
    slist_add(a2, "g");

    SListZipIter zip;
    slist_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }

    cc_assert(strcmp((char*) r1, "b") == 0 && strcmp((char*) r2, "f") == 0,
              cc_msg("slist_zip_iter_remove: Removed elements don't match expected ones"));

    cc_assert(slist_contains(a1, "b") == 0,
              cc_msg("slist_zip_iter_remove: Element still present after removal"));

    cc_assert(slist_contains(a2, "f") == 0,
              cc_msg("slist_zip_iter_remove: Element still present after removal"));

    cc_assert(slist_size(a1) == 3,
              cc_msg("slist_zip_iter_remove: Expected size 3, but got %d", slist_size(a1)));

    cc_assert(slist_size(a2) == 2,
              cc_msg("slist_zip_iter_remove: Expected size 2, but got %d", slist_size(a2)));

    test_slist_validate_structure(a1, "slist_zip_iter_remove (l1)");
    test_slist_validate_structure(a2, "slist_zip_iter_remove (l2)");

    slist_zip_iter_init(&zip, a1, a2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "e") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e2, "g") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }

    char *first = "";
    char *last  = "";

    enum cc_stat status = slist_get_first(a2, (void*) &first);
    cc_assert(status == CC_ERR_VALUE_NOT_FOUND,
              cc_msg("slist_zip_iter_remove: Expected to not find the first"
                     " element, but got %s instead", first));

    status = slist_get_last(a2, (void*) &last);
    cc_assert(status == CC_ERR_VALUE_NOT_FOUND,
              cc_msg("slist_zip_iter_remove: Expected to not find the last"
                     " element, but got %s instead", last));

    slist_get_first(a1, (void*) &first);
    cc_assert(strcmp(first, "d") == 0,
              cc_msg("slist_zip_iter_remove: Expected first element was %s,"
                     " but got %s instead", "d", first));

    slist_get_last(a1, (void*) &last);
    cc_assert(strcmp(last, "d") == 0,
              cc_msg("slist_zip_iter_remove: Expected last element was %s,"
                     " but got %s instead", "d", last));


    test_slist_validate_structure(a1, "slist_zip_iter_remove (l1)");
    test_slist_validate_structure(a2, "slist_zip_iter_remove (l2)");

    // consecutive removes
    slist_add(a1, "a");
    slist_add(a1, "b");
    slist_add(a1, "c");
    slist_add(a1, "d");

    slist_add(a2, "a");
    slist_add(a2, "b");
    slist_add(a2, "c");
    slist_add(a2, "d");

    slist_zip_iter_init(&zip, a1, a2);
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);

        if (strcmp((char*) e1, "c") == 0)
            slist_zip_iter_remove(&zip, &r1, &r2);
    }

    test_slist_validate_structure(a1, "slist_zip_iter_remove (l1)");
    test_slist_validate_structure(a2, "slist_zip_iter_remove (l2)");

    slist_destroy(a1);
    slist_destroy(a2);
}


void test_slist_zip_iter_replace()
{
    SList *a1;
    slist_new(&a1);

    slist_add(a1, "a");
    slist_add(a1, "b");
    slist_add(a1, "c");
    slist_add(a1, "d");

    SList *a2;
    slist_new(&a2);

    slist_add(a2, "e");
    slist_add(a2, "f");
    slist_add(a2, "g");

    char *h = "h";
    char *i = "i";

    SListZipIter zip;
    slist_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (slist_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            slist_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    slist_index_of(a1, "h", &index);

    cc_assert(index == 1,
              cc_msg("slist_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "h", index));

    slist_index_of(a1, "i", &index);
    cc_assert(index == 1,
              cc_msg("slist_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "i", index));

    cc_assert(slist_contains(a1, "h") == 1,
              cc_msg("slist_zip_iter_replace: Element %s not presetn after addition", "h"));

    cc_assert(slist_contains(a2, "i") == 1,
              cc_msg("slist_zip_iter_replace: Element %s not presetn after addition", "i"));

    slist_destroy(a1);
    slist_destroy(a2);
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
    SList *l;
    slist_new(&l);

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

    void *prev    = 0;
    void *current = 0;
    void *e;

    slist_iter_next(&iter, &prev);
    while (slist_iter_next(&iter, &e) != CC_ITER_END) {
        cc_assert(*((int*)prev) <= *((int*)e),
                  cc_msg("slist_sort: preceding "
                         "element greater than the current"));
        prev = e;
    }
    slist_destroy_free(l);
}

void test_slist_reverse()
{
    SList *l;
    slist_1_to_10(&l);

    slist_reverse(l);

    SListIter i;
    slist_iter_init(&i, l);

    void *e;
    int next = 10;
    while (slist_iter_next(&i, &e) != CC_ITER_END) {
        cc_assert(next == *((int*) e),
                  cc_msg("slist_reverse: Expected %d, but got %d instead", next, *((int*) e)));
        next--;
    }

    slist_destroy_free(l);
}

void slist_1234(SList **out)
{
    SList *list;
    slist_new(&list);

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

    *out = list;
}


void slist_5677(SList **out)
{
    SList *list;
    slist_new(&list);

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

    *out = list;
}


void slist_1_to_10(SList **out)
{
    SList *list;
    slist_new(&list);

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

    *out = list;
}
