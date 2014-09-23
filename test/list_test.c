/*
 * dlist_test.c
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../src/list.h"
#include "test.h"

List *list_1234();
List *list_5677();

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
void test_list_get();
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

int main(int argc, char **argv)
{    
     cc_set_status(PASS);
     cc_set_exit_on_failure(false);

     list_new();
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
     test_list_get();
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

     return  cc_get_status();
}

void p(void *e)
{
    printf("%d \n", *((int*) e));
}

void test_list_add_at()
{
    List *list = list_1234();

    int ins = 90;

    list_add_at(list, &ins, 3);

    cc_assert(list_size(list) == 5,
              cc_msg("list_add_at: Expected size was 5, but got %d!", list_size(list)));
    
    int new = *((int*) list_get(list, 3));

    cc_assert(new == ins,
              cc_msg("list_add_at: Expected element at index 2 was %d, but got %d!", ins, new));

    list_remove(list, &ins);
    list_destroy_free(list);
}

void test_list_add_all_at()
{
    List *list  = list_1234();
    List *list2 = list_5677();

    list_add_all_at(list, list2, 2);

    cc_assert(list_size(list2) == 4,
              cc_msg("list_add_all_at: Expected size was 4, but got %d!", list_size(list2)));

    cc_assert(list_size(list) == 8,
              cc_msg("list_add_all_at: Expected size was 8, but got %d!", list_size(list)));
    
    int last = *((int*) list_get_last(list));
    int l1i5 = *((int*) list_get(list, 5));
    int l2i2 = *((int*) list_get(list2, 2));
    
    cc_assert(last == 4,
              cc_msg("list_add_all_at: Expected last element was 4, but got %d!", last));
    
    cc_assert(l1i5 == l2i2,
              cc_msg("list_add_all_at: Expected element at index 5 was %d, but got %d!", l1i5, l2i2));

    list_destroy(list2);
    list_destroy_free(list);
}


void test_list_iter_add()
{
    List *list = list_1234();
    int ins = 32;
    
    ListIter *iter = list_iter_new(list);
    for (;list_iter_has_next(iter);) {
        int e = *((int*) list_iter_next(iter));

        if (e == 3) {
            int i = list_iter_index(iter);
            list_iter_add(iter, &ins);
        }
    }

    cc_assert(list_size(list) == 5,
              cc_msg("list_iter_add: Expected size was 5, but got %d", list_size(list)));
    
    int li3 = *((int*) list_get(list, 3));
    int li4 = *((int*) list_get(list, 4));

    cc_assert(li3 == ins,
              cc_msg("list_iter_add: Expected element at index 3 was %d, but got %d", ins, li3));

    cc_assert(li4 == 4,
              cc_msg("list_iter_add: Expected element at index 4 was 4, but got %d,", li4));

    list_iter_destroy(iter);
    list_destroy(list);
}

void test_list_iter_remove()
{
    List *list = list_1234();
    
    int *rm = list_get(list, 2);

    ListIter *iter = list_iter_new(list);
    for(;list_iter_has_next(iter);) {
        int e = *((int*) list_iter_next(iter));
        if (e == 3) {
             list_iter_remove(iter);
        }
    }
    cc_assert(list_size(list) == 3,
              cc_msg("list_iter_remove: Expected size was 3, but got %d", list_size(list)));
    
    cc_assert(list_contains(list, rm) == 0,
              cc_msg("list_iter_remove: List contains unique element after removal"));

    list_iter_destroy(iter);
    list_destroy(list);
}

void test_list_iter()
{
     test_list_iter_add();
     test_list_iter_remove(); 
}


void test_list_iter_desc()
{
    test_list_iter_desc_remove();
    test_list_iter_desc_add();
}


void test_list_iter_desc_remove()
{
    List *list = list_1234();

    ListDIter *iter = list_diter_new(list);
    for (;list_diter_has_next(iter);) {
        int i = *((int*)list_diter_next(iter));

        if (i == 1 || i == 3)
            list_diter_remove(iter);
    }
    int size = list_size(list);

    cc_assert(list_size(list) == 2, cc_msg("list_iter_desc_remove: expected size 2 but got %d", size));

    int first = *((int*) list_get_first(list));
    int last  = *((int*) list_get_last(list));
    int i1    = *((int*) list_get(list, 1));
    
    cc_assert(first == 2, 
              cc_msg("list_iter_desc_remove: expected first element was 2, but got %d", first));

    cc_assert(last == 4, 
              cc_msg("list_iter_desc_remove: expected last element was 4, but got %d", last));
    
    cc_assert(i1 == 4, 
              cc_msg("list_iter_desc_remove: expected element at index 1 was, but got %d", i1));

    list_diter_destroy(iter);
    list_destroy_free(list);
}


void test_list_iter_desc_add()
{
    List *list = list_1234();

    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));
    *a = 30;
    *b = 78;

    ListDIter *iter = list_diter_new(list);
    for (;list_diter_has_next(iter);) {
        int i = *((int*)list_diter_next(iter));

        if (i == 4)
            list_diter_add(iter, a);

        if (i == 3)
            list_diter_add(iter, b);
    }

    int size  = list_size(list);
    int first = *((int*)list_get_first(list));
    int last  = *((int*)list_get_last(list));
    int i2    = *((int*)list_get(list, 2));
    int i4    = *((int*)list_get(list, 4)); 

    cc_assert(size == 6, 
              cc_msg("list_iter_desc_add: Expected size 6, but got %d", size));

    cc_assert(first == 1, 
              cc_msg("list_iter_desc_add: Expected first 1, but got %d", first));

    cc_assert(last == 4, 
              cc_msg("list_iter_desc_add: Expected last 4, but got %d", last));

    cc_assert(i2 == *b,
              cc_msg("list_iter_desc_add: Expected element at index 2 to be %d, but got %d", *b, i2));

    cc_assert(i4 == *a,
              cc_msg("list_iter_desc_add: Expected element at index 4 to be %d, but got %d", *a, i4));

    list_diter_destroy(iter);
    list_destroy_free(list);
}

void test_list_reverse()
{
    List *list = list_1234();

    int *last_old = list_get_last(list);  

    list_reverse(list);

    int a = *((int*)list_get(list, 0));
    int b = *((int*)list_get(list, 2));

    cc_assert(a == 4, 
              cc_msg("list_reverse: expected 4, but got %d", a));

    cc_assert(b == 2, 
              cc_msg("list_reverse: expected 2, but got %d", b));

    cc_assert(list_get_first(list) == last_old,
              cc_msg("list_reverse: unexpected list head after reverse"));

    list_destroy_free(list);
}


void test_list_to_array()
{
    List *list = list_1234();
    int **array = (int**) list_to_array(list);

    cc_assert(list_get(list, 0) == array[0],
              cc_msg("list_to_array: Array index 0 does not match list index 0"));

    cc_assert(list_get(list, 2) == array[2],
              cc_msg("list_to_array: Array index 2 does not match list index 2"));

    cc_assert(list_get(list, 3) == array[3],
              cc_msg("list_to_array: Array index 3 does not match list index 3"));

    free(array);
    list_destroy_free(list);
}


void test_list_index_of()
{
    List *list = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    list_add(list, &a);
    list_add(list, &b);
    list_add(list, &c);
    list_add(list, &d);
    
    int i0 = list_index_of(list, &a);
    int i1 = list_index_of(list, &c);

    cc_assert(i0 == 0,
              cc_msg("list_index_of: Expected index 0, but got %d", i0));

    cc_assert(i1 == 2,
              cc_msg("list_index_of: Expected index 2, but got %d", i1));

    list_destroy(list);
}


int cmp(void *e1, void *e2)
{
    int *i = (int *) e1;
    int *j = (int *) e2;

    if (*i < *j)
        return -1;
    if (*i == *j)
        return 0;
    return 1;
}


void test_list_sort()
{
    List *list = list_new();

    srand(time(NULL));

    /* populate the list with random data */
    int size = 10000;
    int i;
    for (i = 0; i < size; i++) {
        int *e = malloc(sizeof(int));
        *e = rand() % 1000000;
        list_add(list, e);
    }

    list_sort(list, cmp);

    ListIter *iter = list_iter_new(list);
    int prev;
    for (prev = *((int *) list_iter_next(iter)); list_iter_has_next(iter);) {
        void *e = list_iter_next(iter);

        cc_assert(prev <= *((int*) e), 
                  cc_msg("list_sort: preceding elment greater than the current"));

        prev = *((int *) e);
    }

    list_destroy(list);
}


void test_list_new()
{
    List *list  = list_new();
    List *list2 = list_new();

    if (list == NULL || list2 == NULL) 
      goto exit;

    cc_assert(list_get_first(list) == NULL,
              cc_msg("list_new: Expected NULL first element."));
    
    cc_assert(list_get_last(list) == NULL,
              cc_msg("list_new: Expected NULL last element."));

    cc_assert(list_size(list) == 0,
              cc_msg("list_new: Expected size 0, but got %d", list_size(list)));

    cc_assert(list != list2,
              cc_msg("list_new: Expected a new instance"));
    
exit:
    list_destroy(list);
    list_destroy(list2);
}


void test_list_add()
{
    List *list = list_new();

    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    bool r1 = list_add(list, s1);
    bool r2 = list_add(list, s2);
    bool r3 = list_add(list, s3);
    bool r4 = list_add(list, s4);

    if (r1 && r2 && r3 && r4)
         cc_assert(list_size(list) == 4,
                   cc_msg("list_add: Expected size 4, but got %d", list_size(list)));

    cc_assert(list_get_first(list) != NULL,
              cc_msg("list_add: First element not expected to be NULL!"));

    cc_assert(list_get_last(list) != NULL,
              cc_msg("list_add: Last element not expected to be NULL!"));

    list_destroy(list);
}


void test_list_add_last()
{
    List *list = list_new();

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
    int last = *((int*) list_get_last(list));

    cc_assert(size == 4,
              cc_msg("list_add_last: Expected size was 4, but got %d!", size));
    
    cc_assert(last == d,
           cc_msg("list_add_last: Expected last element was %d, but got %d!", d, last));

    list_add_last(list, &append);

    size = list_size(list);
    last = *((int*) list_get_last(list));

    cc_assert(size == 5,
              cc_msg("list_add_last: Expected size was 5, but got %d!", size));
    
    cc_assert(last == append,
              cc_msg("list_add_last: Expected last element was %d, but got %d!", append, last));
    
    list_destroy(list);
}


void test_list_add_first()
{
    List *list = list_new();

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
    int first = *((int*) list_get_first(list));

    cc_assert(size == 4,
              cc_msg("list_add_first: Expected size was 4, but got %d!", size));
    
    cc_assert(first == a,
              cc_msg("list_add_first: Expectd first element was %d, but got %d!", prepend, first));

    list_add_first(list, &prepend);

    size = list_size(list);
    first = *((int*) list_get_first(list));

    cc_assert(size == 5,
              cc_msg("list_add_first: Expected size was 4, but got %d!", size));

    cc_assert(first == prepend,
              cc_msg("list_add_first: Expected first element was %d, but got %d", prepend, first));

    list_destroy(list);
}


void test_list_splice_at()
{
    List *list = list_1234();
    List *list2 = list_5677();

    list_splice_at(list, list2, 2);

    cc_assert(list_size(list) == 8,
              cc_msg("list_splice_after: Expected size was 8, but got %d!", list_size(list)));

    cc_assert(list_size(list2) == 0,
              cc_msg("list_splice_after: Expected size was 0, but got %d!", list_size(list2)));

    int first = *((int*) list_get_first(list));
    int last  = *((int*) list_get_last(list));
    int i2    = *((int*) list_get(list, 2));

    cc_assert(first == 1,
              cc_msg("list_splice_after: Expected first element was 1, but got %d!", first));

    cc_assert(last == 4,
              cc_msg("list_splice_after: Expected last element was 4, but got %d!", last));

    cc_assert(i2 == 5,
              cc_msg("list_splice_after: Expected element at index 2 was 5, but got %d", i2));

    list_destroy(list2);
    list_destroy_free(list);
}

void test_list_splice()
{
    List *list = list_1234();
    List *list2 = list_5677();

    list_splice(list, list2);

    cc_assert(list_size(list) == 8,
              cc_msg("list_splice: Expected size was 8, but got %d!", list_size(list)));

    cc_assert(list_size(list2) == 0,
              cc_msg("list_splice: Expected size was 0, but got %d!", list_size(list2)));

    int last = *((int*) list_get_last(list));
    int first = *((int*) list_get_first(list));
    int i4 = *((int*) list_get(list, 4));
    
    cc_assert(last == 7,
              cc_msg("list_splice: Expected last element was 7, but got %d!", last));
    
    cc_assert(first == 1,
              cc_msg("list_splice: Expected first element was 1, but got %d!", first));
    
    cc_assert(i4 == 5,
              cc_msg("list_splice: Expected element at index 4 was 5, but got %d!", i4));

    list_destroy(list2);
    list_destroy_free(list);
}

void test_list_add_all()
{
    List *list = list_1234();
    List *list2 = list_5677();

    list_add_all(list2, list);

    cc_assert(list_size(list2) == 8, 
              cc_msg("list_add_all: Expected size was 8, but got %d!", list_size(list2)));
    
    int l1last = *((int*) list_get_last(list));
    int l2last = *((int*) list_get_last(list2));

    cc_assert(l1last == l2last,
              cc_msg("list_add_all: Expected last element was %d, but got %d!", l1last, l2last));

    list_destroy(list);
    list_destroy_free(list2);
}


void test_list_remove()
{
    List *list = list_1234();

    int *e = list_get(list, 1);
    list_remove(list, e);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove: Expected size was 3, but got %d!", list_size(list)));

    cc_assert(list_contains(list, e) == 0,
              cc_msg("list_remove: The list still contains the removed element!"));

    list_destroy_free(list);
}


void test_list_remove_first()
{
    List *list = list_1234();

    list_remove_first(list);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_first: Expected size was 3, but got %d!", list_size(list)));
    
    int first = *((int*) list_get_first(list));

    cc_assert(first == 2,
              cc_msg("list_remove_first: Expected first element was 2, but got %d!", first));

    list_destroy_free(list);
}


void test_list_remove_last()
{
    List *list = list_1234();

    list_remove_last(list);

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_last: Expected size was 3, but got %d!", list_size(list)));
    
    int last = *((int*) list_get_last(list));

    cc_assert(last == 3,
              cc_msg("list_remove_last: Expected last element was 3, but got %d!", last));

    list_destroy_free(list);
}


void test_list_remove_at()
{
    List *list = list_1234();

    list_remove_at(list, 2);

    int e = *((int*) list_get(list, 2));

    cc_assert(e == 4,
           cc_msg("list_remove_at: Expected element was 4, but got %d!", e));

    cc_assert(list_size(list) == 3,
              cc_msg("list_remove_at: Expected size was 3, but got %d!", list_size(list)));

    list_remove_at(list, 0);

    e = *((int*) list_get(list, 0));

    cc_assert(e == 2,
              cc_msg("list_remove_at: Expected element was 2, but got %d!", e));

    list_destroy_free(list);
}


void test_list_remove_all()
{
    List *list = list_1234();
    list_remove_all(list);

    cc_assert(list_size(list) == 0,
              cc_msg("list_remove_all: Expected size was 0, but got %d", list_size(list)));

    list_destroy_free(list);
}


void test_list_get_first()
{
    List *list = list_1234();
    int first = *((int*) list_get_first(list));

    cc_assert(first == 1,
              cc_msg("list_get_first: Expected first element was 1, but got %d", first));

    list_destroy_free(list);
}


void test_list_get_last()
{
    List *list = list_1234();
    int last = *((int*) list_get_last(list));

    cc_assert(last == 4,
              cc_msg("list_get_last: Expected last element was 4, but got %d", last));

    list_destroy_free(list);
}


void test_list_get()
{
    List *list = list_1234();
    int e = *((int*) list_get(list, 1));

    cc_assert(e == 2,
              cc_msg("list_get: Expected element at index 1 was 2, but got %d", e));

    list_destroy_free(list);
}


void test_list_replace_at()
{
    List *list = list_1234();
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;
    
    list_replace_at(list, replacement, 2);

    cc_assert(list_get(list, 2) == replacement,
              cc_msg("list_replace_at: Unexpected element at index 2"));

    list_destroy_free(list);
}


void test_list_contains()
{
    List *list = list_new();

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

    int c1 = list_contains(list, &b);
    int c2 = list_contains(list, &d);
    int c3 = list_contains(list, &e);

    cc_assert(c1 == 2,
              cc_msg("list_contains: Expected number contained was 2, but got %d", c1));

    cc_assert(c2 == 1,
              cc_msg("list_contains: Expected number contained was 1, but got %d", c2));

    cc_assert(c3 == 0,
              cc_msg("list_contains: Expected number contained was 0, but got %d", c3));

    list_destroy(list);
}


void test_list_copy_shallow()
{
    List *list = list_1234();
    List *cp = list_copy_shallow(list);

    cc_assert(list_size(cp) == 4,
              cc_msg("list_copy_shallow: Expected size was 4, but got %d", list_size(cp)));

    cc_assert(list_get_first(cp) == list_get_first(list),
              cc_msg("list_copy_shallow: First element missmatch"));

    cc_assert(list_get_last(cp) == list_get_last(list),
              cc_msg("list_copy_shallow: Last element missmatch"));
    
    cc_assert(list_get(cp, 3) == list_get(list, 3),
              cc_msg("list_copy_shallow: Element at index 3 missmatch"));
    
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
    List *list = list_1234();
    List *cp = list_copy_deep(list, copy);

    cc_assert(list_size(cp) == 4,
              cc_msg("list_copy_deep: Expected size was 4, but got %d", list_size(cp)));

    int e = *((int*) list_get(cp, 2));
    
    cc_assert(e == *((int*)list_get(list, 2)),
              cc_msg("list_copy_deep: Expected element at index 2 was 3, but got %d", e));

    cc_assert(list_get(cp, 2) != list_get(list, 2),
              cc_msg("list_copy_deep: Both lists point to the same value at index 2"));
    
    list_destroy_free(cp);
    list_destroy_free(list);
}


void test_list_sublist()
{
    List *list = list_1234();

    List *sub = list_sublist(list, 1, 2);

    cc_assert(list_size(sub) == 2,
              cc_msg("list_sublist: Expected size was 2 but got %d", list_size(sub)));
    
    int s1 = *((int*)list_get(sub, 1));
    int l2 = *((int*)list_get(list, 2));

    cc_assert(s1 == l2,
              cc_msg("list_sublist: Expected element at index 1 was %d, but got %d", l2, s1));

    list_destroy(sub);
    list_destroy_free(list);
}


void test_list_destroy()
{
     // Nothing to test
}


List *list_1234()
{
    List *list = list_new();

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

    return list;
}


List *list_5677()
{
    List *list = list_new();

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

    return list;
}
