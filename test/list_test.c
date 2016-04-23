/*
 * dlist_test.c
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/list.h"
#include "test.h"

void list_1234(List **l);
void list_5677(List **l);

void test_list_new();
void test_list_destroy();
void test_list_add();
void test_list_add_last();
void test_list_add_first();
void test_list_add_all();
void test_list_add_all_at();
void test_list_add_at();
void test_list_remove();
void test_list_remove_first();
void test_list_remove_last();
void test_list_remove_at();
void test_list_remove_all();
void test_list_get_first();
void test_list_get_last();
void test_list_get_at();
void test_list_index_of();
void test_list_contains();
void test_list_replace_at();
void test_list_copy_shallow();
void test_list_copy_deep();
void test_list_sublist();
void test_list_sort();
void test_list_iter();
void test_list_iter_desc();
void test_list_reverse();

void test_list_splice();
void test_list_splice_at();
void test_list_to_array();

void test_list_iter_desc_add();
void test_list_iter_desc_remove();

void test_list_iter_add();
void test_list_iter_remove();

void test_list_zip_iter();
void test_list_zip_iter_next();
void test_list_zip_iter_add();
void test_list_zip_iter_remove();
void test_list_zip_iter_replace();


int main(int argc, char **argv)
{
     cc_set_status(PASS);
     cc_set_exit_on_failure(false);

     test_list_destroy();
     test_list_add();
     test_list_add_first();
     test_list_add_last();
     test_list_index_of();
     test_list_add_all_at();
     test_list_add_at();
     test_list_remove();
     test_list_remove_first();
     test_list_remove_last();
     test_list_remove_at();
     test_list_remove_all();
     test_list_get_first();
     test_list_get_last();
     test_list_get_at();
     test_list_contains();
     test_list_replace_at();
     test_list_copy_shallow();
     test_list_copy_deep();
     test_list_sublist();
     test_list_sort();
     test_list_iter();
     test_list_iter_desc();
     test_list_splice();
     test_list_splice_at();
     test_list_to_array();
     test_list_reverse();
     test_list_zip_iter();

     return  cc_get_status();
}

void test_list_validate_structure(List *list, char *test_name)
{
    size_t expected_size = list_size(list);

    size_t bw_size = 0;
    size_t fw_size = 0;

    void *head;
    list_get_first(list, &head);

    void *tail;
    list_get_last(list, &tail);

    ListIter i;
    list_diter_init(&i, list);

    void *e;
    while (list_diter_next(&i, &e) != CC_ITER_END) {
        if (bw_size == 0) {
            cc_assert(tail == e,
                      cc_msg("list_validate_structure: "
                             "tail not as expected during"
                             " descending iteration at %s",
                             test_name));
        }
        bw_size++;
    }

    cc_assert(bw_size == expected_size,
              cc_msg("list_validate_structure: "
                     "uexpected backward size at %s. Expected "
                     "%d, but got %d",
                     test_name, expected_size, bw_size));

    ListIter i2;
    list_iter_init(&i2, list);


    while (list_iter_next(&i2, &e) != CC_ITER_END) {
        if (fw_size == 0) {
            cc_assert(head == e,
                      cc_msg("list_structure_validate: "
                             "head not as expected during"
                             " ascending iteration at %s",
                             test_name));
        }
        fw_size++;
    }

    cc_assert(fw_size == expected_size,
              cc_msg("list_structure_validate: "
                     "unexpected forward size at %s:"
                     " expected %d, but got %d",
                     test_name, expected_size, fw_size));
}


void test_list_new()
{
    List *list;
    list_new(&list);
    List *list2;
    list_new(&list2);

    if (list == NULL || list2 == NULL)
      goto exit;

    void *first = NULL;
    void *last  = NULL;

    list_get_first(list, &first);
    cc_assert(first == NULL,
              cc_msg("list_new: Expected NULL first element."));

    list_get_last(list, &last);
    cc_assert(last == NULL,
              cc_msg("list_new: Expected NULL last element."));

    cc_assert(list_size(list) == 0,
              cc_msg("list_new: Expected size 0, but got %d",
                     list_size(list)));

    cc_assert(list != list2,
              cc_msg("list_new: Expected a new instance"));

exit:
    list_destroy(list);
    list_destroy(list2);
}


void test_list_add()
{
    List *list;
    list_new(&list);

    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    enum cc_stat r1 = list_add(list, s1);
    enum cc_stat r2 = list_add(list, s2);
    enum cc_stat r3 = list_add(list, s3);
    enum cc_stat r4 = list_add(list, s4);

    if (r1 == CC_OK && r2 == CC_OK && r3 == CC_OK && r4 == CC_OK) {
         cc_assert(list_size(list) == 4,
                   cc_msg("list_add: Expected "
                          "size 4, but got %d",
                          list_size(list)));
    }
    void *first;
    list_get_first(list, &first);
    cc_assert(first != NULL,
              cc_msg("list_add: First element"
                     " not expected to be NULL!"));

    void *last;
    list_get_last(list, &last);
    cc_assert(last != NULL,
              cc_msg("list_add: Last element"
                     " not expected to be NULL!"));


    test_list_validate_structure(list, "list_add");

    list_destroy(list);
}


void test_list_add_last()
{
    List *list;
    list_new(&list);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    list_add(list, &a);
    list_add(list, &b);
    list_add(list, &c);
    list_add(list, &d);

    int size = list_size(list);

    int *last;
    list_get_last(list, (void*) &last);

    cc_assert(size == 4,
              cc_msg("list_add_last: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*last == d,
           cc_msg("list_add_last: Expected last"
                  " element was %d, but got %d!",
                  d, *last));

    list_add_last(list, &append);

    size = list_size(list);
    list_get_last(list, (void*) &last);

    cc_assert(size == 5,
              cc_msg("list_add_last: Expected size"
                     " was 5, but got %d!", size));

    cc_assert(*last == append,
              cc_msg("list_add_last: Expected last "
                     "element was %d, but got %d!",
                     append, *last));

    test_list_validate_structure(list, "list_add_last");

    list_destroy(list);
}


void test_list_add_first()
{
    List *list;
    list_new(&list);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    list_add(list, &a);
    list_add(list, &b);
    list_add(list, &c);
    list_add(list, &d);

    int size = list_size(list);
    int *first;
    list_get_first(list, (void*) &first);

    cc_assert(size == 4,
              cc_msg("list_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*first == a,
              cc_msg("list_add_first: Expectd first"
                     " element was %d, but got %d!",
                     prepend, *first));

    list_add_first(list, &prepend);

    size = list_size(list);
    list_get_first(list, (void*) &first);

    cc_assert(size == 5,
              cc_msg("list_add_first: Expected size"
                     " was 4, but got %d!", size));

    cc_assert(*first == prepend,
              cc_msg("list_add_first: Expected first"
                     " element was %d, but got %d",
                     prepend, *first));

    test_list_validate_structure(list, "list_add_first");

    list_destroy(list);
}


void test_list_add_at()
{
    List *list;
    list_1234(&list);

    int ins = 90;

    list_add_at(list, &ins, 3);

    cc_assert(list_size(list) == 5,
              cc_msg("list_add_at: Expected size "
                     "was 5, but got %d!", list_size(list)));

    int *new;
    list_get_at(list, 3, (void*) &new);

    cc_assert(*new == ins,
              cc_msg("list_add_at: Expected element"
                     " at index 2 was %d, but got %d!", ins, *new));

    test_list_validate_structure(list, "list_add_at");

    list_remove(list, &ins, NULL);
    list_destroy_free(list);
}


void test_list_add_all_at()
{
    List *list;
    list_1234(&list);

    List *list2;
    list_5677(&list2);

    list_add_all_at(list, list2, 2);

    cc_assert(list_size(list2) == 4,
              cc_msg("list_add_all_at: Expected size"
                     " was 4, but got %d!", list_size(list2)));

    cc_assert(list_size(list) == 8,
              cc_msg("list_add_all_at: Expected size"
                     " was 8, but got %d!", list_size(list)));

    int *last;
    list_get_last(list, (void*) &last);
    int *l1i5;
    list_get_at(list, 5, (void*) &l1i5);
    int *l2i2;
    list_get_at(list2, 2, (void*) &l2i2);

    cc_assert(*last == 4,
              cc_msg("list_add_all_at: Expected last"
                     " element was 4, but got %d!", *last));

    cc_assert(*l1i5 == *l2i2,
              cc_msg("list_add_all_at: Expected element"
                     " at index 5 was %d, but got %d!", *l1i5, *l2i2));

    test_list_validate_structure(list, "list_add_at");

    list_destroy(list2);
    list_destroy_free(list);
}

void test_list_iter()
{
     test_list_iter_add();
     test_list_iter_remove();
}


void test_list_iter_add()
{
    List *list;
    list_1234(&list);

    int *ins = (int*) malloc(sizeof(int));
    *ins = 32;

    ListIter iter;
    list_iter_init(&iter, list);

    int *el;
    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 3)
            list_iter_add(&iter, ins);
    }

    cc_assert(list_size(list) == 5,
              cc_msg("list_iter_add: Expected size"
                     " was 5, but got %d", list_size(list)));

    int *li3;
    list_get_at(list, 3, (void*) &li3);

    cc_assert(*li3 == *ins,
              cc_msg("list_iter_add: Expected element at"
                     " index 3 was %d, but got %d", *ins, *li3));

    int *li4;
    list_get_at(list, 4, (void*) &li4);

    cc_assert(*li4 == 4,
              cc_msg("list_iter_add: Expected element at"
                     " index 4 was 4, but got %d,", *li4));


    list_iter_init(&iter, list);

    ins = (int*) malloc(sizeof(int));
    *ins = 32;

    while (list_iter_next(&iter, (void*) &el) != CC_ITER_END) {
        if (*el == 4) {
            list_iter_add(&iter, ins);
        }
    }

    void *e;
    list_get_last(list, &e);

    cc_assert(*((int*)e) == *ins,
              cc_msg("list_iter_add: Expected last element"
                     " to be %d, but got %d instead", *ins, *((int*)e)));

    test_list_validate_structure(list, "list_iter_add");

    list_destroy_free(list);
}

void test_list_iter_remove()
{
    List *list;
    list_1234(&list);

    int *rm;
    list_get_at(list, 2, (void*) &rm);

    ListIter iter;
    list_iter_init(&iter, list);

    int *e;
    while (list_iter_next(&iter, (void*) &e) != CC_ITER_END) {
        if (*e == 3) {
            list_iter_remove(&iter, NULL);
            free(e);
        }
    }
    cc_assert(list_size(list) == 3,
              cc_msg("list_iter_remove: Expected size"
                     " was 3, but got %d", list_size(list)));

    cc_assert(list_contains(list, rm) == 0,
              cc_msg("list_iter_remove: List contains"
                     " unique element after removal"));

    test_list_validate_structure(list, "list_iter_remove");

    list_destroy_free(list);
}

void test_list_iter_desc()
{
    test_list_iter_desc_remove();
    test_list_iter_desc_add();
}


void test_list_iter_desc_remove()
{
    List *list;
    list_1234(&list);

    ListIter iter;
    list_diter_init(&iter, list);

    int *i;
    while (list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 1 || *i == 3) {
            list_diter_remove(&iter, NULL);
            free(i);
        }
    }
    int size = list_size(list);

    cc_assert(list_size(list) == 2,
              cc_msg("list_iter_desc_remove:"
                     " expected size 2 but got %d",
                     size));

    int *first;
    list_get_first(list, (void*) &first);

    int *last;
    list_get_last(list, (void*) &last);

    int *i1;
    list_get_at(list, 1, (void*) &i1);

    cc_assert(*first == 2,
              cc_msg("list_iter_desc_remove: expected"
                     " first element was 2, but got %d", *first));

    cc_assert(*last == 4,
              cc_msg("list_iter_desc_remove: expected"
                     " last element was 4, but got %d", *last));

    cc_assert(*i1 == 4,
              cc_msg("list_iter_desc_remove: expected"
                     " element at index 1 was, but got %d", *i1));

    test_list_validate_structure(list, "list_diter_remove");

    list_destroy_free(list);
}


void test_list_iter_desc_add()
{
    List *list;
    list_1234(&list);

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    *a = 30;
    *b = 78;

    ListIter iter;
    list_diter_init(&iter, list);

    int *i;
    while (list_diter_next(&iter, (void*) &i) != CC_ITER_END) {
        if (*i == 4) // add to tail
            list_diter_add(&iter, a);

        if (*i == 3)
            list_diter_add(&iter, b);
    }

    int size  = list_size(list);
    int *first;
    list_get_first(list, (void*) &first);

    int *last;
    list_get_last(list, (void*) &last);

    int *i2;
    list_get_at(list, 2, (void*) &i2);

    int *i4;
    list_get_at(list, 4, (void*) &i4);

    cc_assert(size == 6,
              cc_msg("list_iter_desc_add: Expected"
                     " size 6, but got %d", size));

    cc_assert(*first == 1,
              cc_msg("list_iter_desc_add: Expected"
                     " first 1, but got %d", *first));

    cc_assert(*last == 4,
              cc_msg("list_iter_desc_add: Expected"
                     " last 4, but got %d", *last));

    cc_assert(*i2 == *b,
              cc_msg("list_iter_desc_add: Expected"
                     " element at index 3 to be %d, but got %d",
                     *b, *i2));

    cc_assert(*i4 == *a,
              cc_msg("list_iter_desc_add: Expected"
                     " element at index 4 to be %d, but got %d",
                     *a, *i4));

    test_list_validate_structure(list, "list_diter_add");

    list_destroy_free(list);
}

void test_list_reverse()
{
    List *list;
    list_1234(&list);

    int *last_old;
    list_get_last(list, (void*) &last_old);

    list_reverse(list);

    int *a;
    list_get_at(list, 0, (void*) &a);

    int *b;
    list_get_at(list, 2, (void*) &b);

    cc_assert(*a == 4,
              cc_msg("list_reverse: expected 4,"
                     " but got %d", *a));

    cc_assert(*b == 2,
              cc_msg("list_reverse: expected 2,"
                     " but got %d", *b));

    int *first;
    list_get_first(list, (void*) &first);
    cc_assert(first == last_old,
              cc_msg("list_reverse: unexpected"
                     " list head after reverse"));

    test_list_validate_structure(list, "list_reverse");

    list_destroy_free(list);
}


void test_list_to_array()
{
    List *list;
    list_1234(&list);
    int **array;
    list_to_array(list, (void*)&array);

    void *e;
    list_get_at(list, 0, &e);
    cc_assert(e == array[0],
              cc_msg("list_to_array: Array index 0"
                     " does not match list index 0"));

    list_get_at(list, 2, &e);
    cc_assert(e == array[2],
              cc_msg("list_to_array: Array index 2"
                     " does not match list index 2"));

    list_get_at(list, 3, &e);
    cc_assert(e == array[3],
              cc_msg("list_to_array: Array index 3"
                     " does not match list index 3"));

    free(array);
    list_destroy_free(list);
}


void test_list_index_of()
{
    List *list;
    list_new(&list);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    list_add(list, &a);
    list_add(list, &b);
    list_add(list, &c);
    list_add(list, &d);

    size_t i0;
    list_index_of(list, &a, cc_common_cmp_ptr, &i0);

    size_t i1;
    list_index_of(list, &c, cc_common_cmp_ptr, &i1);

    cc_assert(i0 == 0,
              cc_msg("list_index_of: Expected "
                     "index 0, but got %d", i0));

    cc_assert(i1 == 2,
              cc_msg("list_index_of: Expected "
                     "index 2, but got %d", i1));

    list_destroy(list);
}


int cmp(void const *e1, void const *e2)
{
    int i = *(*((int**) e1));
    int j = *(*((int**) e2));

    if (i < j)
        return -1;
    if (i == j)
        return 0;
    return 1;
}


void test_list_sort()
{
    List *list;
    list_new(&list);

    srand(time(NULL));

    /* populate the list with random data */
    int size = 10000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 1000000;
        list_add(list, e);
    }

    list_sort_in_place(list, cmp);

    ListIter iter;
    list_iter_init(&iter, list);

    void *prev;
    void *e;
    list_iter_next(&iter, &prev);
    while (list_iter_next(&iter, &e) != CC_ITER_END) {
        cc_assert(*((int*)prev) <= *((int*) e),
                  cc_msg("list_sort: preceding elment "
                         "greater than the current"));
        prev = e;
    }

    test_list_validate_structure(list, "list_sort");

    list_destroy_free(list);
}


void test_list_splice_at()
{
    List *list;
    list_1234(&list);
    List *list2;
    list_5677(&list2);

    list_splice_at(list, list2, 2);

    cc_assert(list_size(list) == 8,
              cc_msg("list_splice_after: Expected"
                     " size was 8, but got %d!", list_size(list)));

    cc_assert(list_size(list2) == 0,
              cc_msg("list_splice_after: Expected"
                     " size was 0, but got %d!", list_size(list2)));

    int *first;
    list_get_first(list, (void*) &first);

    int *last;
    list_get_last(list, (void*) &last);

    int *i2;
    list_get_at(list, 2, (void*) &i2);

    cc_assert(*first == 1,
              cc_msg("list_splice_after: Expected"
                     " first element was 1, but got %d!", *first));

    cc_assert(*last == 4,
              cc_msg("list_splice_after: Expected"
                     " last element was 4, but got %d!", *last));

    cc_assert(*i2 == 5,
              cc_msg("list_splice_after: Expected"
                     " element at index 2 was 5, but got %d", *i2));

    test_list_validate_structure(list, "list_splice_at");

    list_destroy(list2);
    list_destroy_free(list);
}

void test_list_splice()
{
    List *list;
    list_1234(&list);

    List *list2;
    list_5677(&list2);

    list_splice(list, list2);

    cc_assert(list_size(list) == 8,
              cc_msg("list_splice: Expected "
                     "size was 8, but got %d!",
                     list_size(list)));

    cc_assert(list_size(list2) == 0,
              cc_msg("list_splice: Expected "
                     "size was 0, but got %d!",
                     list_size(list2)));

    int *last;
    list_get_last(list, (void*) &last);

    int *first;
    list_get_first(list, (void*) &first);

    int *i4;
    list_get_at(list, 4, (void*) &i4);

    cc_assert(*last == 7,
              cc_msg("list_splice: Expected last"
                     " element was 7, but got %d!",
                     *last));

    cc_assert(*first == 1,
              cc_msg("list_splice: Expected first"
                     " element was 1, but got %d!",
                     *first));

    cc_assert(*i4 == 5,
              cc_msg("list_splice: Expected element"
                     " at index 4 was 5, but got %d!",
                     *i4));

    test_list_validate_structure(list, "list_splice");

    list_destroy(list2);
    list_destroy_free(list);
}

void test_list_add_all()
{
    List *list;
    list_1234(&list);
    List *list2;
    list_5677(&list2);

    list_add_all(list2, list);

    cc_assert(list_size(list2) == 8,
              cc_msg("list_add_all: Expected size"
                     " was 8, but got %d!", list_size(list2)));

    int *l1last;
    list_get_last(list, (void*) &l1last);

    int *l2last;
    list_get_last(list2, (void*) &l2last);

    cc_assert(*l1last == *l2last,
              cc_msg("list_add_all: Expected last"
                     " element was %d, but got %d!",
                     *l1last, *l2last));

    test_list_validate_structure(list, "list_add_all");

    list_destroy(list);
    list_destroy_free(list2);
}


void test_list_remove()
{
    List *list;
    list_1234(&list);

    int *e;
    list_get_at(list, 1, (void*) &e);
    list_remove(list, e, NULL);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove: Expected size"
                     " was 3, but got %d!", list_size(list)));

    cc_assert(list_contains(list, e) == 0,
              cc_msg("list_remove: The list still"
                     " contains the removed element!"));

    free(e);

    test_list_validate_structure(list, "list_remove");

    list_destroy_free(list);
}


void test_list_remove_first()
{
    List *list;
    list_1234(&list);

    int *first;
    list_remove_first(list, (void*) &first);
    free(first);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_first: Expected "
                     "size was 3, but got %d!",
                     list_size(list)));

    list_get_first(list, (void*) &first);

    cc_assert(*first == 2,
              cc_msg("list_remove_first: Expected "
                     "first element was 2, but got %d!",
                     *first));

    test_list_validate_structure(list, "list_remove_first");

    list_destroy_free(list);
}


void test_list_remove_last()
{
    List *list;
    list_1234(&list);

    int *last;
    list_remove_last(list, (void*) &last);
    free(last);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_last: Expected"
                     " size was 3, but got %d!",
                     list_size(list)));

    enum cc_stat status = list_get_last(list, (void*) &last);
    cc_assert(status == CC_OK,
              cc_msg("list_remove_last: Status is not CC_OK"));

    cc_assert(*last == 3,
              cc_msg("list_remove_last: Expected"
                     " last element was 3, but got %d!",
                     *last));

    test_list_validate_structure(list, "list_remove_last");

    list_destroy_free(list);
}


void test_list_remove_at()
{
    List *list;
    list_1234(&list);

    int *e;
    list_remove_at(list, 2, (void*) &e);
    free(e);

    list_get_at(list, 2, (void*) &e);

    cc_assert(*e == 4,
           cc_msg("list_remove_at: Expected element"
                  " was 4, but got %d!", *e));

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_at: Expected size"
                     " was 3, but got %d!", list_size(list)));

    list_remove_at(list, 0, (void*) &e);
    free(e);

    list_get_at(list, 0, (void*) &e);

    cc_assert(*e == 2,
              cc_msg("list_remove_at: Expected element"
                     " was 2, but got %d!", *e));

    test_list_validate_structure(list, "list_remove_at");

    list_destroy_free(list);
}


void test_list_remove_all()
{
    List *list;
    list_1234(&list);
    list_remove_all_free(list);

    cc_assert(list_size(list) == 0,
              cc_msg("list_remove_all: Expected size"
                     " was 0, but got %d", list_size(list)));

    void *first = NULL;
    list_get_first(list, &first);
    cc_assert(first == NULL,
              cc_msg("list_remove_all: First not NULL,"
                     "as expected"));

    list_destroy_free(list);
}


void test_list_get_first()
{
    List *list;
    list_1234(&list);

    int *first;
    list_get_first(list, (void*) &first);

    cc_assert(*first == 1,
              cc_msg("list_get_first: Expected first"
                     " element was 1, but got %d", *first));

    list_destroy_free(list);
}


void test_list_get_last()
{
    List *list;
    list_1234(&list);

    int *last;
    list_get_last(list, (void*) &last);

    cc_assert(*last == 4,
              cc_msg("list_get_last: Expected last"
                     " element was 4, but got %d", *last));

    list_destroy_free(list);
}


void test_list_get_at()
{
    List *list;
    list_1234(&list);

    int *e;
    list_get_at(list, 1, (void*) &e);

    cc_assert(*e == 2,
              cc_msg("list_get: Expected element at"
                     " index 1 was 2, but got %d", *e));

    list_destroy_free(list);
}


void test_list_replace_at()
{
    List *list;
    list_1234(&list);

    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    int *r;
    list_replace_at(list, replacement, 2, (void*) &r);
    free(r);

    list_get_at(list, 2, (void*) &r);
    cc_assert((int*) r == replacement,
              cc_msg("list_replace_at: Unexpected"
                     " element at index 2"));

    list_destroy_free(list);
}


void test_list_contains()
{
    List *list;
    list_new(&list);

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    list_add(list, &a);
    list_add(list, &b);
    list_add(list, &b);
    list_add(list, &c);
    list_add(list, &d);

    size_t c1 = list_contains(list, &b);
    size_t c2 = list_contains(list, &d);
    size_t c3 = list_contains(list, &e);

    cc_assert(c1 == 2,
              cc_msg("list_contains: Expected number"
                     " contained was 2, but got %d", c1));

    cc_assert(c2 == 1,
              cc_msg("list_contains: Expected number"
                     " contained was 1, but got %d", c2));

    cc_assert(c3 == 0,
              cc_msg("list_contains: Expected number"
                     " contained was 0, but got %d", c3));

    list_destroy(list);
}


void test_list_copy_shallow()
{
    List *list;
    list_1234(&list);

    List *cp;
    list_copy_shallow(list, &cp);

    cc_assert(list_size(cp) == 4,
              cc_msg("list_copy_shallow: Expected "
                     "size was 4, but got %d", list_size(cp)));

    void *f1;
    void *f2;
    list_get_first(cp, &f1);
    list_get_first(list, &f2);
    cc_assert(f1 == f2,
              cc_msg("list_copy_shallow: First element missmatch"));

    list_get_last(cp, &f1);
    list_get_last(list, &f2);
    cc_assert(f1 == f2,
              cc_msg("list_copy_shallow: Last element missmatch"));

    list_get_at(cp, 3, &f1);
    list_get_at(list, 3, &f2);
    cc_assert(f1 == f2,
              cc_msg("list_copy_shallow: Element at "
                     "index 3 missmatch"));

    list_destroy_free(cp);
    list_destroy(list);
}


void *copy(void *e1)
{
    int *cp = (int *) malloc(sizeof(int));
    *cp = *((int*) e1);
    return cp;
}


void test_list_copy_deep()
{
    List *list;
    list_1234(&list);

    List *cp;
    list_copy_deep(list, copy, &cp);

    cc_assert(list_size(cp) == 4,
              cc_msg("list_copy_deep: Expected size"
                     " was 4, but got %d", list_size(cp)));

    int *e;
    list_get_at(cp, 2, (void*) &e);

    int *le;
    list_get_at(list, 2, (void*) &le);
    cc_assert(*e == *le,
              cc_msg("list_copy_deep: Expected element"
                     " at index 2 was 3, but got %d", *e));

    list_get_at(cp, 2, (void*) &e);
    list_get_at(list, 2, (void*) &le);
    cc_assert(e != le,
              cc_msg("list_copy_deep: Both lists point"
                     " to the same value at index 2"));

    list_destroy_free(cp);
    list_destroy_free(list);
}


void test_list_sublist()
{
    List *list;
    list_1234(&list);

    List *sub;
    list_sublist(list, 1, 2, &sub);

    cc_assert(list_size(sub) == 2,
              cc_msg("list_sublist: Expected size"
                     " was 2 but got %d", list_size(sub)));

    int *s1;
    list_get_at(sub, 1, (void*) &s1);

    int *l2;
    list_get_at(list, 2, (void*) &l2);

    cc_assert(*s1 == *l2,
              cc_msg("list_sublist: Expected element"
                     " at index 1 was %d, but got %d", *l2, *s1));

    list_destroy(sub);
    list_destroy_free(list);
}


void test_list_zip_iter()
{
    test_list_zip_iter_next();
    test_list_zip_iter_add();
    test_list_zip_iter_remove();
    test_list_zip_iter_replace();
}


void test_list_zip_iter_next()
{
    List *a1;
    list_new(&a1);

    list_add(a1, "a");
    list_add(a1, "b");
    list_add(a1, "c");
    list_add(a1, "d");

    List *a2;
    list_new(&a2);

    list_add(a2, "e");
    list_add(a2, "f");
    list_add(a2, "g");

    ListZipIter zip;
    list_zip_iter_init(&zip, a1, a2);

    size_t i = 0;

    void *e1, *e2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            cc_assert(strcmp((char*) e1, "a") == 0,
                      cc_msg("list_zip_iter_next: Expected e1 was \"a\" at index 0, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "e") == 0,
                      cc_msg("list_zip_iter_next: Expected e1 was \"e\" at index 0, but got %s instead",
                             (char*) e2));
        }
        if (i == 2) {
            cc_assert(strcmp((char*) e1, "c") == 0,
                      cc_msg("list_zip_iter_next: Expected e1 was \"a\" at index 2, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "g") == 0,
                      cc_msg("list_zip_iter_next: Expected e1 was \"e\" at index 2, but got %s instead",
                             (char*) e2));
        }
        i++;
    }
    cc_assert(i == 3,
              cc_msg("list_zip_iter_next: Expected 3 iterations, but got %d instead", i));

    list_destroy(a1);
    list_destroy(a2);
}


void test_list_zip_iter_add()
{
    List *a1;
    list_new(&a1);

    list_add(a1, "a");
    list_add(a1, "b");
    list_add(a1, "c");
    list_add(a1, "d");

    List *a2;
    list_new(&a2);

    list_add(a2, "e");
    list_add(a2, "f");
    list_add(a2, "g");

    char *h = "h";
    char *i = "i";

    ListZipIter zip;
    list_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_add(&zip, h, i);
    }

    size_t index;
    list_index_of(a1, "h", cc_common_cmp_ptr, &index);

    cc_assert(index == 2,
              cc_msg("list_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "h", index));

    list_index_of(a1, "i", cc_common_cmp_ptr, &index);
    cc_assert(index == 2,
              cc_msg("list_zip_iter_add: Expected element %s to be at index 2"
                     " but was found at %d", "i", index));

    list_index_of(a1, "c", cc_common_cmp_ptr, &index);
    cc_assert(index == 3,
              cc_msg("list_zip_iter_add: Expected element %s to be at index 3"
                     " but was found at %d", "c", index));

    cc_assert(list_contains(a1, "h") == 1,
              cc_msg("list_zip_iter_add: Element %s not presetn after addition", "h"));

    cc_assert(list_contains(a2, "i") == 1,
              cc_msg("list_zip_iter_add: Element %s not presetn after addition", "i"));

    cc_assert(list_size(a1) == 5,
              cc_msg("list_zip_iter_add: Expected size 5, but got %d", list_size(a1)));

    cc_assert(list_size(a2) == 4,
              cc_msg("list_zip_iter_add: Expected size 4, but got %d", list_size(a2)));

    list_zip_iter_init(&zip, a1, a2);
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e2, "g") == 0)
            list_zip_iter_add(&zip, "x", "y");
    }

    char *last;
    list_get_last(a1, (void*) &last);
    cc_assert(strcmp(last, "d") == 0,
              cc_msg("list_zip_iter_add: Expected last element %s, but got %s instead", "d", last));

    list_get_last(a2, (void*) &last);
    cc_assert(strcmp(last, "y") == 0,
              cc_msg("list_zip_iter_add: Expected last element %s, but got %s instead", "y", last));


    test_list_validate_structure(a1, "list_zip_iter_add (l1)");
    test_list_validate_structure(a2, "list_zip_iter_add (l2)");

    list_destroy(a1);
    list_destroy(a2);
}


void test_list_zip_iter_remove()
{
    List *a1;
    list_new(&a1);

    list_add(a1, "a");
    list_add(a1, "b");
    list_add(a1, "c");
    list_add(a1, "d");

    List *a2;
    list_new(&a2);

    list_add(a2, "e");
    list_add(a2, "f");
    list_add(a2, "g");

    ListZipIter zip;
    list_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_remove(&zip, &r1, &r2);
    }
    cc_assert(strcmp((char*) r1, "b") == 0 && strcmp((char*) r2, "f") == 0,
              cc_msg("list_zip_iter_remove: Removed elements don't match expected ones"));

    cc_assert(list_contains(a1, "b") == 0,
              cc_msg("list_zip_iter_remove: Element still present after removal"));

    cc_assert(list_contains(a2, "f") == 0,
              cc_msg("list_zip_iter_remove: Element still present after removal"));

    cc_assert(list_size(a1) == 3,
              cc_msg("list_zip_iter_remove: Expected size 3, but got %d", list_size(a1)));

    cc_assert(list_size(a2) == 2,
              cc_msg("list_zip_iter_remove: Expected size 2, but got %d", list_size(a2)));

    test_list_validate_structure(a1, "list_zip_iter_remove (l1)");
    test_list_validate_structure(a2, "list_zip_iter_remove (l2)");

    list_destroy(a1);
    list_destroy(a2);
}


void test_list_zip_iter_replace()
{
    List *a1;
    list_new(&a1);

    list_add(a1, "a");
    list_add(a1, "b");
    list_add(a1, "c");
    list_add(a1, "d");

    List *a2;
    list_new(&a2);

    list_add(a2, "e");
    list_add(a2, "f");
    list_add(a2, "g");

    char *h = "h";
    char *i = "i";

    ListZipIter zip;
    list_zip_iter_init(&zip, a1, a2);

    void *e1, *e2;
    void *r1, *r2;
    while (list_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (strcmp((char*) e1, "b") == 0)
            list_zip_iter_replace(&zip, h, i, &r1, &r2);
    }

    size_t index;
    list_index_of(a1, "h", cc_common_cmp_ptr, &index);

    cc_assert(index == 1,
              cc_msg("list_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "h", index));

    list_index_of(a1, "i", cc_common_cmp_ptr, &index);
    cc_assert(index == 1,
              cc_msg("list_zip_iter_replace: Expected element %s to be at index 1"
                     " but was found at %d", "i", index));

    cc_assert(list_contains(a1, "h") == 1,
              cc_msg("list_zip_iter_replace: Element %s not presetn after addition", "h"));

    cc_assert(list_contains(a2, "i") == 1,
              cc_msg("list_zip_iter_replace: Element %s not presetn after addition", "i"));

    list_destroy(a1);
    list_destroy(a2);
}


void test_list_destroy()
{
     // Nothing to test
}


void list_1234(List **l)
{
    List *list;
    list_new(&list);

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    list_add(list, a);
    list_add(list, b);
    list_add(list, c);
    list_add(list, d);

    *l = list;
}


void list_5677(List **l)
{
    List *list;
    list_new(&list);

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    int *c = (int*) malloc(sizeof(int));
    int *d = (int*) malloc(sizeof(int));

    *a = 5;
    *b = 6;
    *c = 7;
    *d = 7;

    list_add(list, a);
    list_add(list, b);
    list_add(list, c);
    list_add(list, d);

    *l = list;
}
