#include "../src/stack.h"
#include "test.h"
#include <string.h>

void test_stack_push();
void test_stack_pop();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_stack_push();
    test_stack_pop();

    return cc_get_status();
}

void test_stack_push()
{
    Stack *s = stack_new();

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, &a);

    cc_assert(stack_peek(s) == &a,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_push(s, &b);

    cc_assert(stack_peek(s) == &b,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_push(s, &c);

    cc_assert(stack_peek(s) == &c,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_destroy(s);

}

void test_stack_pop()
{
    Stack *s = stack_new();

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, &a);
    stack_push(s, &b);
    stack_push(s, &c);

    cc_assert(stack_pop(s) == &c,
              cc_msg("stack_pop: Top stack element not as expected"));

    cc_assert(stack_peek(s) == &b,
              cc_msg("stack_pop: Top stack element not as expected"));

    stack_destroy(s);
}
