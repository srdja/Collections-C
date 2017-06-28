#include "pqueue.h"
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

static PQueue *p1, *p2;



TEST_GROUP_C_SETUP(PQueueTestsWithDefaults)
{
    pqueue_new(&p1, comp2);
    PQueueConf cfg;
    pqueue_conf_init(&cfg, comp);
    pqueue_new_conf(&cfg, &p2);
};

TEST_GROUP_C_TEARDOWN(PQueueTestsWithDefaults)
{
    pqueue_destroy(p1);
    pqueue_destroy(p2);
};

TEST_C(PQueueTestsWithDefaults, PqueuePush)
{
    int a = 100, b = -10, c = 11;
    int *ptr;

    pqueue_push(p1, (void *) &b);
    pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&b, ptr);

    pqueue_push(p1, (void *) &a);
    pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    pqueue_push(p1, (void *) &c);
    pqueue_top(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    struct Pair *ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    pqueue_push(p2, (void *) &A);
    pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);
    
    pqueue_push(p2, (void *) &B);
    pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);

    pqueue_push(p2, (void *) &C);
    pqueue_top(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&C, ptr2);
};

TEST_C(PQueueTestsWithDefaults, PqueuePop)
{
    int a = 100, b = -10, c = 11;
    int *ptr;

    pqueue_push(p1, (void *) &b);

    pqueue_push(p1, (void *) &a);

    pqueue_push(p1, (void *) &c);

    pqueue_pop(p1, (void *) &ptr);
    CHECK_EQUAL_C_POINTER(&a, ptr);

    pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&c, ptr);

    pqueue_pop(p1, (void *)&ptr);
    CHECK_EQUAL_C_POINTER(&b, ptr);

    struct Pair *ptr2;
    A.a = 5, A.b = 10;
    B.a = 2, B.b = 11;
    C.a = 5, C.b = 100;

    pqueue_push(p2, (void *) &A);
    
    pqueue_push(p2, (void *) &B);

    pqueue_push(p2, (void *) &C);

    pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&C, ptr2);

    pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&A, ptr2);

    pqueue_pop(p2, (void *) &ptr2);
    CHECK_EQUAL_C_POINTER(&B, ptr2);
};
