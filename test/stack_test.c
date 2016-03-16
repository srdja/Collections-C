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
    Stack *s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    int *p;

    stack_push(s, (void*)&a);
    stack_peek(s, (void*)&p);
    cc_assert(p == &a,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_push(s, (void*) &b);
    stack_peek(s, (void*)&p);
    cc_assert(p == &b,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_push(s, (void*)&c);
    stack_peek(s, (void*)&p);
    cc_assert(p == &c,
              cc_msg("stack_push: Top stack element not as expected"));

    stack_destroy(s);

}

void test_stack_pop()
{
    Stack *s;
    stack_new(&s);

    int a = 1;
    int b = 2;
    int c = 3;

    stack_push(s, (void*)&a);
    stack_push(s, (void*)&b);
    stack_push(s, (void*)&c);

    int *pop;
    int *peek;

    stack_pop(s, (void*)&pop);
    cc_assert(pop == &c,
              cc_msg("stack_pop: Top stack element not as expected"));

    stack_peek(s, (void*)&peek);
    cc_assert(peek == &b,
              cc_msg("stack_pop: Top stack element not as expected"));

    stack_destroy(s);
}
