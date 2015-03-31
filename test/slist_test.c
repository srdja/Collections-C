#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../src/slist.h"
#include "test.h"

SList *list_1_to_10();

void test_slist_reverse();
void test_slist_sort();

int main(int argc, char **argv)
{
    cc_set_status(PASS);
    cc_set_exit_on_failure(false);

    test_slist_reverse();
    test_slist_sort();
    
    return cc_get_status();
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

    SListIter *iter    = slist_iter_new(l);
    int        prev    = 0;
    int        current = 0;
    void      *e       = slist_iter_next(iter);

    prev = *((int*) slist_iter_next(iter));
    while (slist_iter_has_next(iter)) {
        e = slist_iter_next(iter);
        current = *((int*) e);
        
        cc_assert(prev <= current,
                  cc_msg("vector_sort: preceding "
                         "element greater than the current"));

        prev = current;
    }
    slist_destroy_free(l);
}

void test_slist_reverse()
{
    SList *l = list_1_to_10();

    slist_foreach(l, p);
    printf("\n");

    slist_reverse(l);

    slist_foreach(l, p);
    printf("\n");
}

SList *list_1_to_10()
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
