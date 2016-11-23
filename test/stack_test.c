#include <string.h>

#include "stack.h"
#include "CppUTest/TestHarness_c.h"

static Stack *s;
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

    stack_push(s, (void*)&a);
    stack_peek(s, (void*)&p);
    CHECK_EQUAL_C_POINTER(&a, p);

    stack_push(s, (void*) &b);
    stack_peek(s, (void*)&p);
    CHECK_EQUAL_C_POINTER(&b, p);

    stack_push(s, (void*)&c);
    stack_peek(s, (void*)&p);
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
