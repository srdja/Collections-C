#include "../src/queue.h"
#include "test.h"

void test_queue_poll();
void test_queue_enqueue();
void test_queue_iter();

int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_queue_poll();
    test_queue_enqueue();
    test_queue_iter();

    return cc_get_status();
}


void test_queue_enqueue()
{
    Queue *q;
    queue_new(&q);

    int a = 1;
    int b = 2;
    int c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);

    cc_assert(queue_size(q) == 2,
              cc_msg("queue_enqueue: Qeueu size not as expected"));


    void *p;
    queue_peek(q, &p);
    cc_assert(p == &a,
              cc_msg("queue_enqueue: Front element not as expected."));

    queue_enqueue(q, &c);

    queue_peek(q, &p);
    cc_assert(p == &a,
              cc_msg("queue_enqueue: Front element not as expected."));

    queue_destroy(q);
}

void test_queue_poll()
{
    Queue *q;
    queue_new(&q);

    int a = 1;
    int b = 2;
    int c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);
    queue_enqueue(q, &c);

    void *p;

    queue_poll(q, &p);
    cc_assert(p == &a,
              cc_msg("queue_peek: Front element not as expected."));

    queue_peek(q, &p);
    cc_assert(p == &b,
              cc_msg("queue_peek: Front element not as expected."));

    queue_poll(q, &p);
    cc_assert(p == &b,
              cc_msg("queue_peek: Front element not as expected."));

    queue_peek(q, &p);
    cc_assert(p == &c,
              cc_msg("queue_peek: Front element not as expected."));

    queue_destroy(q);
}

void test_queue_iter()
{
    Queue *q;
    queue_new(&q);

    int a = 1;
    int b = 2;
    int c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);
    queue_enqueue(q, &c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    QueueIter iter;
    queue_iter_init(&iter, q);

    while (queue_iter_has_next(&iter)) {
        int *e;
        queue_iter_next(&iter, (void*) &e);

        if (e == &a)
            x++;

        if (e == &b)
            y++;

        if (e == &c)
            z++;
    }

    cc_assert(x == 1 && y == 1 && z == 1,
              cc_msg("queue_iter: Unexpected number of elments returned "
                     "by the iterator."));

    queue_destroy(q);
}
