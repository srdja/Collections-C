#include <string.h>
#include <stdlib.h>
#include "../src/treetable.h"
#include "../src/deque.h"
#include "test.h"


void test_treetable_add();
void test_treetable_remove();
void test_treetable_remove_all();
void test_treetable_get();
void test_treetable_size();

#ifdef DEBUG
void test_rb_structure();
char *error_code_to_string(int code);
#endif

int main(int argc, char **argv)
{
#ifdef DEBUG
    cc_set_exit_on_failure(true);
    test_rb_structure();
#endif
    cc_set_exit_on_failure(false);
    test_treetable_add();
    test_treetable_remove();
    test_treetable_remove_all();
    test_treetable_get();
    test_treetable_size();

    return cc_get_status();
}

int cmp(void *k1, void *k2)
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

void test_treetable_add()
{
    TreeTable *table = treetable_new(cmp);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");
    treetable_add(table, &e, "e");
    treetable_add(table, &f, "f");
    treetable_add(table, &g, "g");
    treetable_add(table, &h, "h");

    cc_assert(treetable_contains_key(table, &a),
              cc_msg("treetable_add: Added key not found"));

    cc_assert(treetable_contains_key(table, &e),
              cc_msg("treetable_add: Added key not found"));

    treetable_destroy(table);
}

void test_treetable_remove()
{
    TreeTable *table = treetable_new(cmp);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;
    int g = 7;
    int h = 8;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");
    treetable_add(table, &e, "e");
    treetable_add(table, &f, "f");
    treetable_add(table, &g, "g");
    treetable_add(table, &h, "h");

    treetable_remove(table, &f);

    cc_assert(!treetable_contains_key(table, &f),
              cc_msg("treetable_remove: Removed key still present"));

    treetable_destroy(table);

}

void test_treetable_remove_all()
{
    TreeTable *table = treetable_new(cmp);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");
    treetable_add(table, &d, "d");

    treetable_remove_all(table);

    cc_assert(treetable_size(table) == 0,
              cc_msg("treetable_remove_all: Table size not 0"));

    cc_assert(!treetable_contains_key(table, &a),
              cc_msg("treetable_remove_all: Removed key still present"));

    cc_assert(!treetable_contains_key(table, &b),
              cc_msg("treetable_remove_all: Removed key still present"));

    cc_assert(!treetable_contains_key(table, &c),
              cc_msg("treetable_remove_all: Removed key still present"));

    cc_assert(!treetable_contains_key(table, &d),
              cc_msg("treetable_remove_all: Removed key still present"));

    treetable_destroy(table);
}

void test_treetable_get()
{
    TreeTable *table = treetable_new(cmp);

    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");

    char *ra = treetable_get(table, &a);
    char *rb = treetable_get(table, &b);
    char *rc = treetable_get(table, &c);

    cc_assert(strcmp(ra, "a") == 0,
              cc_msg("treetable_get: Retrieved value not as expected"));

    cc_assert(strcmp(rb, "b") == 0,
              cc_msg("treetable_get: Retrieved value not as expected"));

    cc_assert(strcmp(rc, "c") == 0,
              cc_msg("treetable_get: Retrieved value not as expected"));

    treetable_destroy(table);
}

void test_treetable_size()
{
    TreeTable *table = treetable_new(cmp);

    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");

    cc_assert(treetable_size(table) == 3,
              cc_msg("treetable_size: Unexpected table size"));

    treetable_destroy(table);
}

#ifdef DEBUG
char *error_code_to_string(int code)
{
    char *msg = malloc(sizeof(char) * 50);
    char *m;

    switch (code) {
    case RB_ERROR_OK:
        m = "RB_ERROR_OK";
        return strcpy(msg, m);
    case RB_ERROR_CONSECUTIVE_RED:
        m = "RB_ERROR_CONSECUTIVE_RED";
        return strcpy(msg, m);
    case RB_ERROR_BLACK_HEIGHT:
        m = "RB_ERROR_BLACK_HEIGHT";
        return strcpy(msg, m);
    case RB_ERROR_TREE_STRUCTURE:
        m = "RB_ERROR_TREE_STRUCTURE";
        return strcpy(msg, m);
    }
}


void test_rb_structure()
{
    /* Thows a whole bunch of random data at the tree to make sure that  */
    /* the Red Black properties always hold. */

    /* fill an array so that we don't have to malloc all these ints */
    int nkeys = 10000;
    int rkeys[nkeys];

    srand(time(NULL));

    char *dummy = "dummy";

    /* Easy tracking of how many keys are available after removal */
    Deque     *keys = deque_new();
    TreeTable *tree = treetable_new(cmp);

    /* Populate the tree and assert RB validity on each insert */
    int i;
    for (i = 0; i < nkeys; i++) {
        rkeys[i] = (rand() % 10000);
        deque_add(keys, &(rkeys[i]));
        treetable_add(tree, &(rkeys[i]), dummy);

        int status = treetable_assert_rb_rules(tree);
        cc_assert(status == RB_ERROR_OK,
                  cc_msg("Red Black tree validation (insertion) (i=%d): %s", i,
                         error_code_to_string(status)));
    }

    /* Remove keys at random until all keys are removed and assert Red Black */
    /* validity on each remove. */
    for (i = 0; i < nkeys; i++) {
        int *key = deque_get(keys, (rand() % (deque_size(keys)) -1));

        if (key != NULL) {
            printf("Removing %d\n", *key);
            treetable_remove(tree, key);
            deque_remove(keys, key);
        }
        int status = treetable_assert_rb_rules(tree);
        cc_assert(status  == RB_ERROR_OK,
                  cc_msg("Red Black tree validation (removal) (i=%d): %s", i,
                         error_code_to_string(status)));
    }

    deque_destroy(keys);
    treetable_destroy(tree);
}
#endif
