#include "../src/queue.h"
#include "test.h"

void test_queue_poll();
void test_queue_enqueue();
void test_queue_iter();
void test_queue_zip_iter();


int main(int argc, char **argv)
{
    cc_set_exit_on_failure(false);

    test_queue_poll();
    test_queue_enqueue();
    test_queue_iter();
    test_queue_zip_iter();

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

    int *e;
    while (queue_iter_next(&iter, (void*) &e) != CC_ITER_END) {
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


void test_queue_zip_iter_next()
{
    Queue *q1;
    queue_new(&q1);

    queue_enqueue(q1, "a");
    queue_enqueue(q1, "b");
    queue_enqueue(q1, "c");
    queue_enqueue(q1, "d");

    Queue *q2;
    queue_new(&q2);

    queue_enqueue(q2, "e");
    queue_enqueue(q2, "f");
    queue_enqueue(q2, "g");

    QueueZipIter zip;
    queue_zip_iter_init(&zip, q1, q2);

    size_t i = 0;

    void *e1, *e2;
    while (queue_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            cc_assert(strcmp((char*) e1, "d") == 0,
                      cc_msg("queue_zip_iter_next: Expected e1 was \"a\" at index 0, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "g") == 0,
                      cc_msg("queue_zip_iter_next: Expected e1 was \"e\" at index 0, but got %s instead",
                             (char*) e2));
        }
        if (i == 2) {
            cc_assert(strcmp((char*) e1, "b") == 0,
                      cc_msg("queue_zip_iter_next: Expected e1 was \"a\" at index 2, but got %s instead",
                             (char*) e1));
            cc_assert(strcmp((char*) e2, "e") == 0,
                      cc_msg("queue_zip_iter_next: Expected e1 was \"e\" at index 2, but got %s instead",
                             (char*) e2));
        }
        i++;
    }
    cc_assert(i == 3,
              cc_msg("queue_zip_iter_next: Expected 3 iterations, but got %d instead", i));

    queue_destroy(q1);
    queue_destroy(q2);
}


void test_queue_zip_iter()
{
    test_queue_zip_iter_next();
}
