#include "munit.h"
#include "cc_stack.h"
#include <stdlib.h>

bool pred1(const void* e)
{
    return *(int*)e == 1 || *(int*)e == 2;
}

bool pred2(const void* e)
{
    return *(int*)e == 1;
}


static MunitResult test_push(const MunitParameter params[], void* fixture)
{
    Stack* s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    int* p;

    stack_push(s, (void*)&a);
    stack_peek(s, (void*)&p);
    munit_assert_ptr_equal(&a, p);

    stack_push(s, (void*)&b);
    stack_peek(s, (void*)&p);
    munit_assert_ptr_equal(&b, p);

    stack_push(s, (void*)&c);
    stack_peek(s, (void*)&p);
    munit_assert_ptr_equal(&c, p);

    stack_destroy(s);
    return MUNIT_OK;
}

static MunitResult test_pop(const MunitParameter params[], void* fixture)
{
    Stack* s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void*)&a);
    stack_push(s, (void*)&b);
    stack_push(s, (void*)&c);

    int* pop;
    int* peek;

    stack_pop(s, (void*)&pop);
    munit_assert_ptr_equal(&c, pop);

    stack_peek(s, (void*)&peek);
    munit_assert_ptr_equal(&b, peek);

    stack_destroy(s);
    return MUNIT_OK;
}

static MunitResult test_filter(const MunitParameter params[], void* fixture)
{
    Stack* s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void*)&a);
    stack_push(s, (void*)&b);
    stack_push(s, (void*)&c);

    Stack* s2;
    stack_filter(s, pred1, &s2);
    munit_assert_size(2, == , stack_size(s2));

    int* peek;
    stack_peek(s2, (void*)&peek);
    munit_assert_int(2, == , *peek);

    stack_destroy(s);
    stack_destroy(s2);
    return MUNIT_OK;
}

static MunitResult test_filter_mut(const MunitParameter params[], void* fixture)
{
    Stack* s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void*)&a);
    stack_push(s, (void*)&b);
    stack_push(s, (void*)&c);

    stack_filter_mut(s, pred2);
    munit_assert_size(1, == , stack_size(s));

    int* peek;
    stack_peek(s, (void*)&peek);
    munit_assert_int(1, ==, *peek);

    stack_destroy(s);
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*)"/stack/test_push", test_push, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/stack/test_pop", test_pop, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/stack/test_filter", test_filter, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/stack/test_filter_mut", test_filter_mut, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}