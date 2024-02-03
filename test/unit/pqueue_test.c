#include "munit.h"
#include "cc_pqueue.h"
#include <stdlib.h>

static struct Pair
{
    int a, b;
} A, B, C;

static int comp(const void* a, const void* b)
{
    int alpha1 = ((struct Pair*)a)->a, beta1 = ((struct Pair*)a)->b;
    int alpha2 = ((struct Pair*)b)->a, beta2 = ((struct Pair*)b)->b;
    if (alpha1 != alpha2)
        return alpha1 - alpha2;
    else
        return beta1 - beta2;
}

static int comp2(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

struct queues {
    CC_PQueue* p1;
    CC_PQueue* p2;
};


static void* default_pqueue(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;
    struct queues* q = malloc(sizeof(struct queues));
    munit_assert_not_null(q);
    cc_pqueue_new(&q->p1, comp2);
    cc_pqueue_new(&q->p2, comp);
    return (void*) q;
}

static void default_pqueue_teardown(void* fixture)
{
    struct queues* q = (struct queues*)fixture;
    cc_pqueue_destroy(q->p1);
    cc_pqueue_destroy(q->p2);
}

static MunitResult test_push(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    int a = 100, b = -10, c = 11;
    int* ptr;

    cc_pqueue_push(q->p1, (void*)&b);
    cc_pqueue_top(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&b, ptr);

    cc_pqueue_push(q->p1, (void*)&a);
    cc_pqueue_top(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&a, ptr);

    cc_pqueue_push(q->p1, (void*)&c);
    cc_pqueue_top(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&a, ptr);

    struct Pair* ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    cc_pqueue_push(q->p2, (void*)&A);
    cc_pqueue_top(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&A, ptr2);

    cc_pqueue_push(q->p2, (void*)&B);
    cc_pqueue_top(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&A, ptr2);

    cc_pqueue_push(q->p2, (void*)&C);
    cc_pqueue_top(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&C, ptr2);

    return MUNIT_OK;
}

static MunitResult test_pop(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    int a = 100, b = -10, c = 11;
    int* ptr;

    cc_pqueue_push(q->p1, (void*)&b);
    cc_pqueue_push(q->p1, (void*)&a);
    cc_pqueue_push(q->p1, (void*)&c);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&a, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&c, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&b, ptr);

    struct Pair* ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    cc_pqueue_push(q->p2, (void*)&A);
    cc_pqueue_push(q->p2, (void*)&B);
    cc_pqueue_push(q->p2, (void*)&C);

    cc_pqueue_pop(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&C, ptr2);

    cc_pqueue_pop(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&A, ptr2);

    cc_pqueue_pop(q->p2, (void*)&ptr2);
    munit_assert_ptr_equal(&B, ptr2);

    return MUNIT_OK;
}

static MunitResult test_pop_last_two(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    int a = 1, b = 3, c = 5, d = -6, e = 7;
    int* ptr;

    cc_pqueue_push(q->p1, (void*)&a);
    cc_pqueue_push(q->p1, (void*)&b);
    cc_pqueue_push(q->p1, (void*)&c);
    cc_pqueue_push(q->p1, (void*)&d);
    cc_pqueue_push(q->p1, (void*)&e);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&e, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&c, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&b, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&a, ptr);

    cc_pqueue_pop(q->p1, (void*)&ptr);
    munit_assert_ptr_equal(&d, ptr);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"pqueue/test_push", test_push, default_pqueue, default_pqueue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"pqueue/test_pop", test_pop, default_pqueue, default_pqueue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"pqueue/test_pop_last_two", test_pop_last_two, default_pqueue, default_pqueue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}