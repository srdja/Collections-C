#include "cc_pqueue.h"
#include "CppUTest/TestHarness_c.h"

static struct Pair
{
    int a, b;
} A, B, C;

static int comp(const void *a, const void *b)
{
    int alpha1 = ((struct Pair*) a)->a, beta1 = ((struct Pair *) a)->b;
    int alpha2 = ((struct Pair*) b)->a, beta2 = ((struct Pair *) b)->b;
    if(alpha1 != alpha2)
        return alpha1 - alpha2;
    else
        return beta1 - beta2;
}

static int comp2(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}

static CC_PQueue *p1, *p2;



TEST_GROUP_C_SETUP(CC_PQueueTestsWithDefaults)
{
    cc_pqueue_new(&p1, comp2);
    CC_PQueueConf cfg;
    cc_pqueue_conf_init(&cfg, comp);
    cc_pqueue_new_conf(&cfg, &p2);
};

TEST_GROUP_C_TEARDOWN(CC_PQueueTestsWithDefaults)
{
    cc_pqueue_destroy(p1);
    cc_pqueue_destroy(p2);
};

TEST_C(CC_PQueueTestsWithDefaults, PqueuePush)
{
    int a = 100, b = -10, c = 11;
    int *ptr;

    cc_pqueue_push(p1, (void *) &b);
    cc_pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&b, ptr);

    cc_pqueue_push(p1, (void *) &a);
    cc_pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    cc_pqueue_push(p1, (void *) &c);
    cc_pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    struct Pair *ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    cc_pqueue_push(p2, (void *) &A);
    cc_pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);

    cc_pqueue_push(p2, (void *) &B);
    cc_pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);

    cc_pqueue_push(p2, (void *) &C);
    cc_pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&C, ptr2);
};

TEST_C(CC_PQueueTestsWithDefaults, PqueuePop)
{
    int a = 100, b = -10, c = 11;
    int *ptr;

    cc_pqueue_push(p1, (void *) &b);

    cc_pqueue_push(p1, (void *) &a);

    cc_pqueue_push(p1, (void *) &c);

    cc_pqueue_pop(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&c, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&b, ptr);

    struct Pair *ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    cc_pqueue_push(p2, (void *) &A);

    cc_pqueue_push(p2, (void *) &B);

    cc_pqueue_push(p2, (void *) &C);

    cc_pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&C, ptr2);

    cc_pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);

    cc_pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&B, ptr2);
};

TEST_C(CC_PQueueTestsWithDefaults, PqueuePopLastTwos)
{
    int a = 1, b = 3, c = 5, d = -6, e = 7;
    int *ptr;

    cc_pqueue_push(p1, (void *) &a);

    cc_pqueue_push(p1, (void *) &b);

    cc_pqueue_push(p1, (void *) &c);

    cc_pqueue_push(p1, (void *) &d);

    cc_pqueue_push(p1, (void *) &e);

    cc_pqueue_pop(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&e, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&c, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&b, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    cc_pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&d, ptr);
};
