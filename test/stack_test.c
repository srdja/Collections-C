#include <string.h>

#include "cc_stack.h"
#include "CppUTest/TestHarness_c.h"

static Stack *s;
static Stack *s2;

bool pred1(const void *e)
{
    return *(int *)e == 1 || *(int *)e == 2;
}

bool pred2(const void *e)
{
    return *(int *)e == 1;
}

TEST_GROUP_C_SETUP(StackTestsWithDefaults)
{
    stack_new(&s);
};

TEST_GROUP_C_TEARDOWN(StackTestsWithDefaults)
{
    stack_destroy(s);
};

TEST_C(StackTestsWithDefaults, StackPush)
{
    int a = 1;
    int b = 2;
    int c = 3;

    int *p;

    stack_push(s, (void *)&a);
    stack_peek(s, (void *)&p);
    CHECK_EQUAL_C_POINTER(&a, p);

    stack_push(s, (void *)&b);
    stack_peek(s, (void *)&p);
    CHECK_EQUAL_C_POINTER(&b, p);

    stack_push(s, (void *)&c);
    stack_peek(s, (void *)&p);
    CHECK_EQUAL_C_POINTER(&c, p);
};

TEST_C(StackTestsWithDefaults, StackPop)
{
    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void*)&a);
    stack_push(s, (void*)&b);
    stack_push(s, (void*)&c);

    int *pop;
    int *peek;

    stack_pop(s, (void*)&pop);
    CHECK_EQUAL_C_POINTER(&c, pop);

    stack_peek(s, (void*)&peek);
    CHECK_EQUAL_C_POINTER(&b, peek);
};

TEST_GROUP_C_SETUP(StackTestsFilter)
{
    stack_new(&s);
    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void *)&a);
    stack_push(s, (void *)&b);
    stack_push(s, (void *)&c);
};

TEST_GROUP_C_TEARDOWN(StackTestsFilter)
{
    stack_destroy(s);
    stack_destroy(s2);
};

TEST_C(StackTestsFilter, StackFilter)
{
    stack_filter(s, pred1, &s2);
    CHECK_EQUAL_C_INT(2, stack_size(s2));

    int *peek;
    stack_peek(s2, (void *)&peek);
    CHECK_EQUAL_C_INT(2, *peek);
}

TEST_C(StackTestsFilter, StackFilterMut)
{
    stack_filter_mut(s, pred2);
    CHECK_EQUAL_C_INT(1, stack_size(s));

    int *peek;
    stack_peek(s, (void *)&peek);
    CHECK_EQUAL_C_INT(1, *peek);
}