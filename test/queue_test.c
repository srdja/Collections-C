#include "cc_queue.h"
#include "CppUTest/TestHarness_c.h"

static Queue *q;
static Queue *q2;
static int stat;

TEST_GROUP_C_SETUP(QueueTestsWithDefaults)
{
    stat = queue_new(&q);
    queue_new(&q2);
};

TEST_GROUP_C_TEARDOWN(QueueTestsWithDefaults)
{
    queue_destroy(q);
    queue_destroy(q2);
};

TEST_C(QueueTestsWithDefaults, QueueEnqueue)
{
    int a = 1;
    int b = 2;
    int c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);

    CHECK_EQUAL_C_INT(2, queue_size(q));

    void *p;
    queue_peek(q, &p);
    CHECK_EQUAL_C_POINTER(&a, p);

    queue_enqueue(q, &c);

    queue_peek(q, &p);
    CHECK_EQUAL_C_POINTER(&a, p);
};

TEST_C(QueueTestsWithDefaults, QueuePoll)
{
    int a = 1;
    int b = 2;
    int c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);
    queue_enqueue(q, &c);

    void *p;

    queue_poll(q, &p);
    CHECK_EQUAL_C_POINTER(&a, p);

    queue_peek(q, &p);
    CHECK_EQUAL_C_POINTER(&b, p);

    queue_poll(q, &p);
    CHECK_EQUAL_C_POINTER(&b, p);

    queue_peek(q, &p);
    CHECK_EQUAL_C_POINTER(&c, p);
};

TEST_C(QueueTestsWithDefaults, QueueIter)
{
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

    CHECK_EQUAL_C_INT(1, x);
    CHECK_EQUAL_C_INT(1, y);
    CHECK_EQUAL_C_INT(1, z);
};


TEST_C(QueueTestsWithDefaults, QueueZipIterNext)
{
    queue_enqueue(q, "a");
    queue_enqueue(q, "b");
    queue_enqueue(q, "c");
    queue_enqueue(q, "d");

    queue_enqueue(q2, "e");
    queue_enqueue(q2, "f");
    queue_enqueue(q2, "g");

    QueueZipIter zip;
    queue_zip_iter_init(&zip, q, q2);

    size_t i = 0;

    void *e1, *e2;
    while (queue_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            CHECK_EQUAL_C_STRING("d", (char*)e1);
            CHECK_EQUAL_C_STRING("g", (char*)e2);
        }
        if (i == 2) {
            CHECK_EQUAL_C_STRING("b", (char*)e1);
            CHECK_EQUAL_C_STRING("e", (char*)e2);
        }
        i++;
    }
    CHECK_EQUAL_C_INT(3, i);
}
