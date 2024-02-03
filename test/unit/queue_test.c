#include "munit.h"
#include "cc_queue.h"
#include <stdlib.h>

struct queues {
    CC_Queue* q1;
    CC_Queue* q2;
};

static void* default_queue(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;

    struct queues* q = malloc(sizeof(struct queues));

    munit_assert_not_null(q);

    cc_queue_new(&q->q1);
    cc_queue_new(&q->q2);

    return (void*)q;
}

static void default_queue_teardown(void* fixture)
{
    struct queues* q = (struct queues*)fixture;
    cc_queue_destroy(q->q1);
    cc_queue_destroy(q->q2);
}

static MunitResult test_enqueue(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;
    int a = 1;
    int b = 2;
    int c = 3;

    cc_queue_enqueue(q->q1, &a);
    cc_queue_enqueue(q->q1, &b);

    munit_assert_size(2, == , cc_queue_size(q->q1));

    void* p;
    cc_queue_peek(q->q1, &p);
    munit_assert_ptr_equal(&a, p);

    cc_queue_enqueue(q->q1, &c);

    cc_queue_peek(q->q1, &p);
    munit_assert_ptr_equal(&a, p);
    return MUNIT_OK;
}

static MunitResult test_poll(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    int a = 1;
    int b = 2;
    int c = 3;

    cc_queue_enqueue(q->q1, &a);
    cc_queue_enqueue(q->q1, &b);
    cc_queue_enqueue(q->q1, &c);

    void* p;

    cc_queue_poll(q->q1, &p);
    munit_assert_ptr_equal(&a, p);

    cc_queue_peek(q->q1, &p);
    munit_assert_ptr_equal(&b, p);

    cc_queue_poll(q->q1, &p);
    munit_assert_ptr_equal(&b, p);

    cc_queue_peek(q->q1, &p);
    munit_assert_ptr_equal(&c, p);

    return MUNIT_OK;
}

static MunitResult test_iter(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    int a = 1;
    int b = 2;
    int c = 3;

    cc_queue_enqueue(q->q1, &a);
    cc_queue_enqueue(q->q1, &b);
    cc_queue_enqueue(q->q1, &c);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    CC_QueueIter iter;
    cc_queue_iter_init(&iter, q->q1);

    int* e;
    while (cc_queue_iter_next(&iter, (void*)&e) != CC_ITER_END) {
        if (e == &a) {
            x++;
        }
        if (e == &b) {
            y++;
        }
        if (e == &c) {
            z++;
        }
    }

    munit_assert_size(1, == , x);
    munit_assert_size(1, == , y);
    munit_assert_size(1, == , z);
    return MUNIT_OK;
}

static MunitResult test_zip_iter_next(const MunitParameter params[], void* fixture)
{
    (void)params;
    struct queues* q = (struct queues*)fixture;

    cc_queue_enqueue(q->q1, "a");
    cc_queue_enqueue(q->q1, "b");
    cc_queue_enqueue(q->q1, "c");
    cc_queue_enqueue(q->q1, "d");

    cc_queue_enqueue(q->q2, "e");
    cc_queue_enqueue(q->q2, "f");
    cc_queue_enqueue(q->q2, "g");

    QueueZipIter zip;
    cc_queue_zip_iter_init(&zip, q->q1, q->q2);

    size_t i = 0;

    void* e1, * e2;
    while (cc_queue_zip_iter_next(&zip, &e1, &e2) != CC_ITER_END) {
        if (i == 0) {
            munit_assert_string_equal("d", (char*)e1);
            munit_assert_string_equal("g", (char*)e2);
        }
        if (i == 2) {
            munit_assert_string_equal("b", (char*)e1);
            munit_assert_string_equal("e", (char*)e2);
        }
        i++;
    }
    munit_assert_size(3, == , i);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/queue/test_enqueue", test_enqueue, default_queue, default_queue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/queue/test_poll", test_poll, default_queue, default_queue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/queue/test_iter", test_iter, default_queue, default_queue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/queue/test_zip_iter_next", test_zip_iter_next, default_queue, default_queue_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}