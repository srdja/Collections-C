#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../src/treetable.h"
#include "../src/deque.h"
#include "test.h"

void test_treetable_add();
void test_treetable_remove();
void test_treetable_remove_all();
void test_treetable_get();
void test_treetable_size();
void test_treetable_get_first();
void test_treetable_get_last();
void test_treetable_remove_first();
void test_treetable_remove_last();
void test_treetable_get_greater_than();
void test_treetable_get_lesser_than();
void test_treetable_foreach();
void test_treetable_iter();

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
    test_treetable_get_first();
    test_treetable_get_last();
    test_treetable_remove_first();
    test_treetable_remove_last();
    test_treetable_get_greater_than();
    test_treetable_get_lesser_than();
    test_treetable_iter();

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

void test_treetable_add()
{
    TreeTable *table;
    treetable_new(cmp, &table);

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
    TreeTable *table;
    treetable_new(cmp, &table);

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

    treetable_remove(table, &f, NULL);

    cc_assert(!treetable_contains_key(table, &f),
              cc_msg("treetable_remove: Removed key still present"));

    treetable_destroy(table);

}

void test_treetable_remove_all()
{
    TreeTable *table;
    treetable_new(cmp, &table);

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
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(table, &a, "a");
    treetable_add(table, &b, "b");
    treetable_add(table, &c, "c");

    char *ra;
    char *rb;
    char *rc;
    treetable_get(table, &a, (void*) &ra);
    treetable_get(table, &b, (void*) &rb);
    treetable_get(table, &c, (void*) &rc);

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
    TreeTable *table;
    treetable_new(cmp, &table);

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

void test_treetable_get_first()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *first;
    treetable_get_first_key(table, (void*) &first);

    cc_assert(*first == a,
              cc_msg("treetable_get_first_key: first %d, expected %d instead",
                     *first, a));

    treetable_destroy(table);
}

void test_treetable_get_last()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *last;
    treetable_get_last_key(table, (void*) &last);

    cc_assert(*last == d,
              cc_msg("treetable_get_last_key: last %d, expected %d instead",
                     *last, d));

    treetable_destroy(table);
}

void test_treetable_remove_first()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    treetable_remove_first(table, NULL);

    cc_assert(!treetable_contains_key(table, &a),
              cc_msg("treetable_remove_first: Key still present after removal"));

    treetable_destroy(table);
}

void test_treetable_remove_last()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    treetable_remove_last(table, NULL);

    cc_assert(!treetable_contains_key(table, &d),
              cc_msg("treetable_remove_last: Key still present after removal"));

    treetable_destroy(table);
}

void test_treetable_get_greater_than()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *g;
    treetable_get_greater_than(table, &b, (void*) &g);

    cc_assert(*g == c,
              cc_msg("treetable_get_greater_than: Expected key was %d, but got "
                     "%d instead", c, *g));

    treetable_destroy(table);
}

void test_treetable_get_lesser_than()
{
    TreeTable *table;
    treetable_new(cmp, &table);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(table, &c, "a");
    treetable_add(table, &d, "b");
    treetable_add(table, &b, "c");
    treetable_add(table, &a, "d");

    int *g;
    treetable_get_lesser_than(table, &b, (void*) &g);

    cc_assert(*g == a,
              cc_msg("treetable_get_lesser_than: Expected key was %d, but got "
                     "%d instead", a, *g));

    treetable_destroy(table);
}


void test_treetable_iter_next()
{
    TreeTable *t;
    treetable_new(cmp, &t);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    treetable_add(t, &a, "1");
    treetable_add(t, &b, "2");
    treetable_add(t, &c, "3");
    treetable_add(t, &d, "5");

    int one   = 0;
    int two   = 0;
    int three = 0;
    int four  = 0;

    TreeTableIter iter;
    treetable_iter_init(&iter, t);

    TreeTableEntry entry;
    while (treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const *key = entry.key;

        if (*key == a)
            one++;

        if (*key == b)
            two++;

        if (*key == c)
            three++;

        if (*key == d)
            four++;
    }

    bool asrt = (one   == 1) && (two  == 1)  &&
                (three == 1) && (four == 1);

    cc_assert(asrt,
              cc_msg("treetable_iter_next: Unexpected number"
                     " of entries returned"));

    treetable_destroy(t);
}

void test_treetable_iter_remove()
{
    TreeTable *t;
    treetable_new(cmp, &t);

    int a = 1;
    int b = 2;
    int c = 3;

    treetable_add(t, &a, "a");
    treetable_add(t, &b, "a");
    treetable_add(t, &c, "a");

    TreeTableIter iter;
    treetable_iter_init(&iter, t);

    TreeTableEntry entry;
    while (treetable_iter_next(&iter, &entry) != CC_ITER_END) {
        int const *key = entry.key;

        if (*key == b) {
            enum cc_stat s;
            s = treetable_iter_remove(&iter, NULL);

            cc_assert(s == CC_OK,
                      cc_msg("treetable_iter_remove: Expected remove success, but"
                             " got failure instead"));

            s = treetable_iter_remove(&iter, NULL);

            cc_assert(s == CC_ERR_KEY_NOT_FOUND,
                      cc_msg("treetable_iter_remove: Expected remove failure, but"
                             " got something else instead"));
        }
    }

    cc_assert(treetable_size(t) == 2,
              cc_msg("treetable_iter_remove: Expected size 2 but got %d ",
                     treetable_size(t)));

    cc_assert(!treetable_contains_key(t, &b),
              cc_msg("treetable_iter_remove: Element b  still pressent "
                     "after removal"));

    treetable_destroy(t);
}

void test_treetable_iter()
{
    test_treetable_iter_next();
    test_treetable_iter_remove();
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
    default:
	sprintf(msg, "UNKNOWN_ERR_CODE: %d", code);
        return msg;
    }
}


void test_rb_structure()
{
    /* Thows a whole bunch of random data at the tree to make sure that  */
    /* the Red Black properties always hold. */

    /* fill an array so that we don't have to malloc all these ints */
    int nkeys = 1000;
    int rkeys[nkeys];

    srand(time(NULL));

    char *dummy = "dummy";

    /* Easy tracking of how many keys are available after removal */
    Deque *keys;
    deque_new(&keys);
    TreeTable *tree;
    treetable_new(cmp, &tree);

    /* Populate the tree and assert RB validity on each insert */
    int i;
    for (i = 0; i < nkeys; i++) {
        rkeys[i] = (rand() % 10000);
        deque_add(keys, &(rkeys[i]));
        treetable_add(tree, &(rkeys[i]), dummy);

        int status = treetable_assert_rb_rules(tree);
        char *msg = error_code_to_string(status);
        cc_assert(status == RB_ERROR_OK,
                  cc_msg("Red Black tree validation (insertion) (i=%d): %s", i, msg));
        free(msg);
    }

    /* Remove keys at random until all keys are removed and assert Red Black */
    /* validity on each remove. */
    for (i = 0; i < nkeys; i++) {
        int *key;
        deque_get_at(keys, (rand() % (deque_size(keys)) -1), (void*)&key);

        if (key != NULL) {
            treetable_remove(tree, key, NULL);
            deque_remove(keys, key, NULL);
        }
        int status = treetable_assert_rb_rules(tree);
        char *msg = error_code_to_string(status);
        cc_assert(status  == RB_ERROR_OK,
                  cc_msg("Red Black tree validation (removal) (i=%d): %s", i, msg));
        free(msg);
    }

    deque_destroy(keys);
    treetable_destroy(tree);
}
#endif
