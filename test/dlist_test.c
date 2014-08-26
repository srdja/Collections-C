/*
 * dlist_test.c
 *
 *  Created on: Jul 29, 2014
 *      Author: srdja
 */

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "dlist_test.h"
#include "../src/dlist.h"

List *list_1234();
List *list_5677();

void test_dlist_iter_desc_next();
void test_dlist_iter_desc_add();
void test_dlist_iter_desc_remove();

void dlist_test_init()
{
    test_dlist_new();
    test_dlist_destroy();
    test_dlist_add();
    test_dlist_add_last();
    test_dlist_add_first();
    test_dlist_add_all();
    test_dlist_index_of();
    test_dlist_add_all_at();
    test_dlist_add_at();
    test_dlist_remove();
    test_dlist_remove_first();
    test_dlist_remove_last();
    test_dlist_remove_at();
    test_dlist_remove_all();
    test_dlist_get_first();
    test_dlist_get_last();
    test_dlist_get();
    test_dlist_contains();
    test_dlist_replace_at();
    test_dlist_copy_shallow();
    test_dlist_copy_deep();
    test_dlist_sublist();
    test_dlist_sort();
    test_dlist_iter();
    test_dlist_iter_desc();
    test_dlist_splice();
    test_dlist_splice_after();
    test_dlist_splice_before();
    test_dlist_to_array();
    test_dlist_reverse();
}

void test_dlist_iter()
{
    printf("Testing: dlist_iter... ");

    List *dlist = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;

    int ins = 32;

    list_add(dlist, &a);
    list_add(dlist, &b);
    list_add(dlist, &c);
    list_add(dlist, &d);

    /* Test iterator add */
    ListIter *iter = list_iter_new(dlist);
    for (;list_iter_has_next(iter);) {
        int e = *((int*) list_iter_next(iter));
        if (e == c) {
            assert(list_iter_index(iter) == 2);
            list_iter_add(iter, &ins);
        }
    }
    assert(list_size(dlist) == 5);
    assert(list_get(dlist, 3) == &ins);
    assert(list_get(dlist, 4) == &d);
    list_iter_destroy(iter);

    /* Test iterator remove */
    iter = list_iter_new(dlist);
    for(;list_iter_has_next(iter);) {
        int e = *((int*) list_iter_next(iter));
        if (e == ins) {
            assert(list_iter_remove(iter));
            assert(!list_iter_remove(iter));
        }
    }
    assert(list_size(dlist) == 4);
    assert(list_contains(dlist, &ins) == 0);
    list_iter_destroy(iter);

    /* Test iterator replace */
    iter = list_iter_new(dlist);
    for(;list_iter_has_next(iter);) {
        list_iter_next(iter);
        list_iter_replace(iter, &ins);
    }
    list_iter_destroy(iter);

    int i = 0;
    iter = list_iter_new(dlist);
    for (;list_iter_has_next(iter);) {
        int e = *((int*) list_iter_next(iter));
        assert(e == ins);
        i++;
    }
    assert(i == 4);
    list_iter_destroy(iter);

    list_destroy(dlist);

    printf("OK\n");
}

void test_dlist_iter_desc()
{
    test_dlist_iter_desc_next();
    test_dlist_iter_desc_remove();
    test_dlist_iter_desc_add();
}

void test_dlist_iter_desc_next()
{
    printf("Testing: dlist_iter_desc_next... ");

    List *list = list_1234();

    ListDIter *iter = list_diter_new(list);
    int i;
    int li = list_size(list) - 1;
    for (;list_diter_has_next(iter);) {
        i = *((int*) list_diter_next(iter));
        assert(i == *((int*) list_get(list, li)));
        li--;
    }
    list_diter_destroy(iter);
    list_destroy_free(list);
    printf("OK\n");
}

void test_dlist_iter_desc_remove()
{
    printf("Testing: dlist_iter_desc_remove... ");

    List *list = list_1234();

    ListDIter *iter = list_diter_new(list);
    for (;list_diter_has_next(iter);) {
        int i = *((int*)list_diter_next(iter));

        if (i == 1 || i == 3)
            list_diter_remove(iter);
    }
    assert(list_size(list) == 2);
    assert(*((int*)list_get_first(list)) == 2);
    assert(*((int*)list_get_last(list)) == 4);
    assert(*((int*)list_get(list, 0)) == 2);
    assert(*((int*)list_get(list, 1)) == 4);

    list_diter_destroy(iter);
    list_destroy_free(list);
    printf("OK\n");
}

void test_dlist_iter_desc_add()
{
    printf("Testing: dlist_iter_desc_add... ");

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
    assert(list_size(list) == 6);
    assert(*((int*)list_get_first(list)) == 1);
    assert(*((int*)list_get_last(list)) == 4);
    assert(*((int*)list_get(list, 2)) == *b);
    assert(*((int*)list_get(list, 4)) == *a);

    list_diter_destroy(iter);
    list_destroy_free(list);

    printf("OK\n");
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

void test_dlist_reverse()
{
    printf("Testing: dlist_reverse... ");
    List *list = list_1234();

    list_reverse(list);

    int a = *((int*)list_get(list, 0));
    int b = *((int*)list_get(list, 1));
    int c = *((int*)list_get(list, 2));
    int d = *((int*)list_get(list, 3));

    assert(a == 4);
    assert(b == 3);
    assert(c == 2);
    assert(d == 1);

    list_destroy_free(list);
    printf("OK\n");
}

void test_dlist_to_array()
{
    List *list = list_1234();
    int **array = (int**) list_to_array(list);

    assert(list_get(list, 0) == array[0]);
    assert(list_get(list, 1) == array[1]);
    assert(list_get(list, 2) == array[2]);
    assert(list_get(list, 3) == array[3]);

    list_destroy_free(list);
}

void test_dlist_index_of()
{
    printf("Testing: dlist_index_of... ");

    List *dlist = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    list_add(dlist, &a);
    list_add(dlist, &b);
    list_add(dlist, &c);
    list_add(dlist, &d);

    assert(list_index_of(dlist, &a) == 0);
    assert(list_index_of(dlist, &c) == 2);
    assert(list_index_of(dlist, &d) == 3);

    list_destroy(dlist);

    printf("OK\n");
}

void test_dlist_sort()
{
    printf("Testing: dlist_sort... ");

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

    struct timeval t1, t2;
    long mtime, seconds, useconds;

    gettimeofday(&t1, NULL);
    list_sort(list, cmp);
    gettimeofday(&t2, NULL);

    seconds  = t2.tv_sec  - t1.tv_sec;
    useconds = t2.tv_usec - t1.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    //printf("\nElapsed time: %ld milliseconds\n", mtime);

    ListIter *iter = list_iter_new(list);
    int prev;
    for (prev = *((int *) list_iter_next(iter)); list_iter_has_next(iter);) {
        void *e = list_iter_next(iter);
        assert(prev <= *((int*) e));
        prev = *((int *) e);
    }

    list_destroy(list);

    printf("OK\n");
}

void test_dlist_new()
{
    printf("Testing: dlist_new... ");

    List *dlist  = list_new();
    List *dlist2 = list_new();

    assert(dlist != NULL);
    assert(list_get_first(dlist) == NULL); // XXX WHAT DOES IT RETURN?
    assert(list_get_last(dlist) == NULL);
    assert(list_size(dlist) == 0);
    assert(dlist != dlist2);

    list_destroy(dlist);
    list_destroy(dlist2);

    printf("OK\n");
}

void test_dlist_add()
{
    printf("Testing: dlist_add... ");

    List *dlist = list_new();

    char *s1 = "e1", *s2 = "e2", *s3 = "e3", *s4 = "e4";

    bool r1 = list_add(dlist, s1);
    bool r2 = list_add(dlist, s2);
    bool r3 = list_add(dlist, s3);
    bool r4 = list_add(dlist, s4);

    if (r1 && r2 && r3 && r4)
        assert(list_size(dlist) == 4);

    assert(list_get_first(dlist) != NULL);
    assert(list_get_last(dlist) != NULL);

    list_destroy(dlist);
    printf("OK\n");
}

void test_dlist_add_last()
{
    printf("Testing: dlist_append... ");

    List *dlist = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int append = 90;

    list_add(dlist, &a);
    list_add(dlist, &b);
    list_add(dlist, &c);
    list_add(dlist, &d);

    assert(list_size(dlist) == 4);
    assert(list_get_last(dlist) == &d);

    list_add_last(dlist, &append);

    assert(list_size(dlist) == 5);
    assert(list_get_last(dlist) == &append);

    printf("OK\n");
}

void test_dlist_add_first()
{
    printf("Testing: dlist_prepend... ");

    List *dlist = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 1;

    int prepend = 90;

    list_add(dlist, &a);
    list_add(dlist, &b);
    list_add(dlist, &c);
    list_add(dlist, &d);

    assert(list_size(dlist) == 4);
    assert(list_get_first(dlist) == &a);

    list_add_first(dlist, &prepend);

    assert(list_size(dlist) == 5);
    assert(list_get_first(dlist) == &prepend);

    list_destroy(dlist);

    printf("OK\n");
}

void test_dlist_splice_after()
{
    printf("Testing: dlist_splice_after... ");

    List *dlist = list_1234();
    List *dlist2 = list_5677();

    list_splice_after(dlist, dlist2, 1);

    assert(list_size(dlist) == 8);
    assert(list_size(dlist2) == 0);

    assert(*((int*)list_get_first(dlist)) == 1);
    assert(*((int*)list_get_last(dlist)) == 4);
    assert(*((int*)list_get(dlist, 2)) == 5);

    list_destroy(dlist2);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_splice_before()
{
    printf("Testing: dlist_splice_before... ");

    List *dlist = list_1234();
    List *dlist2 = list_5677();

    list_splice_before(dlist, dlist2, 0);

    assert(list_size(dlist) == 8);
    assert(list_size(dlist2) == 0);

    assert(*((int*)list_get_first(dlist)) == 5);
    assert(*((int*)list_get_last(dlist)) == 4);

    list_destroy(dlist2);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_splice()
{
    printf("Testing: dlist_splice... ");

    List *dlist = list_1234();
    List *dlist2 = list_5677();

    list_splice(dlist, dlist2);

    assert(list_size(dlist) == 8);
    assert(list_size(dlist2) == 0);

    assert(*((int*)list_get_last(dlist)) == 7);
    assert(*((int*)list_get_first(dlist)) == 1);
    assert(*((int*)list_get(dlist, 4)) == 5);

    list_destroy(dlist2);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_add_all()
{
    printf("Testing: dlist_add_all... ");

    List *dlist  = list_1234();
    List *dlist2 = list_5677();

    list_add_all_at(dlist, dlist2, 2);

    assert(list_size(dlist2) == 4);
    assert(list_size(dlist) == 8);
    assert(*((int*)list_get_last(dlist)) == 4);
    assert(list_get(dlist, 5) == list_get(dlist2, 2));

    list_destroy(dlist2);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_add_all_at()
{
    printf("Testing: dlist_add_at... ");

    List *dlist = list_1234();
    List *dlist2 = list_5677();

    assert(list_size(dlist2) == 4);

    list_add_all(dlist2, dlist);

    assert(list_size(dlist2) == 8);
    assert(list_get_last(dlist2) == list_get_last(dlist));

    list_destroy(dlist);
    list_destroy_free(dlist2);

    printf("OK\n");
}

void test_dlist_add_at()
{
    printf("Testing: dlist_add_at... ");

    List *dlist = list_1234();

    int ins = 90;

    assert(list_size(dlist) == 4);

    list_add_at(dlist, &ins, 2);

    assert(list_size(dlist) == 5);
    assert(list_get(dlist, 2) == &ins);

    list_remove(dlist, &ins);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_remove()
{
    printf("Testing: dlist_remove... ");

    List *dlist = list_1234();

    assert(list_size(dlist) == 4);

    int *e = list_get(dlist, 1);
    list_remove(dlist, e);

    assert(list_size(dlist) == 3);
    assert(list_contains(dlist, e) == 0);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_remove_first()
{
    printf("Testing: dlist_remove_head... ");

    List *dlist = list_1234();

    assert(list_size(dlist) == 4);

    list_remove_first(dlist);

    assert(list_size(dlist) == 3);
    assert(*((int*)list_get_first(dlist)) == 2);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_remove_last()
{
    printf("Testing: dlist_remove_tail... ");

    List *dlist = list_1234();

    assert(list_size(dlist) == 4);

    list_remove_last(dlist);

    assert(list_size(dlist) == 3);
    assert(*((int*)list_get_last(dlist)) == 3);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_remove_at()
{
    printf("Testing: dlist_remove_at... ");

    List *dlist = list_1234();

    list_remove_at(dlist, 2);

    assert(*((int*)list_get(dlist, 2)) == 4);
    assert(list_size(dlist) == 3);

    list_remove_at(dlist, 0);

    assert(list_size(dlist) == 2);
    assert(*((int*)list_get(dlist, 0)) == 2);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_remove_all()
{
    printf("Testing: dlist_remove_all... ");

    List *dlist = list_1234();
    list_remove_all(dlist);
    assert(list_size(dlist) == 0);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_get_first()
{
    printf("Testing: dlist_get_head... ");

    List *dlist = list_1234();
    assert(*((int*)list_get_first(dlist)) == 1);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_get_last()
{
    printf("Testing: dlist_get_tail... ");

    List *dlist = list_1234();
    assert(*((int*)list_get_last(dlist)) == 4);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_get()
{
    printf("Testing: dlist_get... ");

    List *dlist = list_1234();
    int e = *((int*) list_get(dlist, 1));
    assert(e == 2);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_replace_at()
{
    printf("Testing: dlist_replace_at... ");

    List *dlist = list_1234();
    int *replacement = (int*) malloc(sizeof(int));
    *replacement = 32;

    assert(*((int*)list_get(dlist, 2)) != 2);
    list_replace_at(dlist, replacement, 2);
    assert(list_get(dlist, 2) == replacement);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_contains()
{
    printf("Testing: dlist_contains... ");

    List *dlist = list_new();

    int a = 8;
    int b = 3;
    int c = 20;
    int d = 7;
    int e = 32;

    list_add(dlist, &a);
    list_add(dlist, &b);
    list_add(dlist, &b);
    list_add(dlist, &c);
    list_add(dlist, &d);

    assert(list_contains(dlist, &b) == 2);
    assert(list_contains(dlist, &d) == 1);
    assert(list_contains(dlist, &e) == 0);

    list_destroy(dlist);

    printf("OK\n");
}

void test_dlist_copy_shallow()
{
    printf("Testing: dlist_copy_shallow... ");

    List *dlist = list_1234();
    List *cp = list_copy_shallow(dlist);

    assert(list_size(cp) == 4);
    assert(list_get_first(cp) == list_get_first(dlist));
    assert(list_get_last(cp) == list_get_last(dlist));
    assert(list_get(cp, 3) == list_get(dlist, 3));
    assert(cp != dlist);

    list_destroy_free(cp);
    list_destroy(dlist);

    printf("OK\n");
}

void *copy(void *e1)
{
    int *cp = (int *) malloc(sizeof(int));
    *cp = *((int*) e1);
    return cp;
}

void test_dlist_copy_deep()
{
    printf("Testing: dlist_copy_deep... ");

    List *dlist = list_1234();
    List *cp = list_copy_deep(dlist, copy);

    assert(list_size(cp) == 4);
    assert(list_get_first(cp) != list_get_first(dlist));
    assert(*((int*)list_get_first(cp)) == *((int*)list_get_first(dlist)));
    assert(list_get_last(cp) != list_get_last(dlist));

    list_destroy_free(cp);
    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_sublist()
{
    printf("Testing: dlist_subdlist... ");

    List *dlist = list_1234();

    List *sub = list_sublist(dlist, 1, 2);

    assert(list_size(sub) == 2);
    assert(list_get(sub, 0) == list_get(dlist, 1));
    assert(list_get(sub, 1) == list_get(dlist, 2));

    list_destroy(sub);
    sub = list_sublist(dlist, 0, 3);

    assert(list_size(sub) == 4);
    assert(list_get_last(sub) == list_get_last(dlist));
    assert(list_get_first(sub) == list_get_first(dlist));

    list_destroy(sub);

    list_destroy_free(dlist);

    printf("OK\n");
}

void test_dlist_destroy()
{
    printf("Testing: dlist_destroy... ");

    List *dlist = list_1234();
    assert(list_destroy_free(dlist));

    printf("OK\n");
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
