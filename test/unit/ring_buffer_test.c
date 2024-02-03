#include "munit.h"
#include "cc_ring_buffer.h"
#include <stdlib.h>


static MunitResult test_enqueue(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Rbuf* rbuf;
    int range = 100;
    cc_rbuf_new(&rbuf);

    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t) * 10);

    for (int i = 0; i < 10; i++) {
        uint64_t item = (uint64_t) munit_rand_int_range(0, range);
        cc_rbuf_enqueue(rbuf, item);
        items[i] = item;
    }

    for (int i = 0; i < 10; i++) {
        munit_assert_uint64(cc_rbuf_peek(rbuf, i), == , items[i]);
    }

    cc_rbuf_destroy(rbuf);
    return MUNIT_OK;
}

static MunitResult test_dequeue(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Rbuf* rbuf;
    int range = 100;
    cc_rbuf_new(&rbuf);

    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t) * 10);

    for (int i = 0; i < 10; i++) {
        uint64_t item = munit_rand_int_range(0, range + 1);
        items[i] = item;
        cc_rbuf_enqueue(rbuf, item);
    }

    uint64_t out;
    for (int i = 0; i < 10; i++) {
        cc_rbuf_dequeue(rbuf, &out);
        munit_assert_uint64(items[i], == , out);
        memset(&out, 0, sizeof(uint64_t));
    }

    cc_rbuf_destroy(rbuf);
    return MUNIT_OK;
}

static MunitResult test_enqueue_past_capacity(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    CC_Rbuf* rbuf;
    int range = 100;
    cc_rbuf_new(&rbuf);

    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t) * 10);

    for (int i = 0; i < 10; i++) {
        uint64_t item = munit_rand_int_range(0, range + 1);
        items[i] = item;
        cc_rbuf_enqueue(rbuf, item);
    }

    munit_assert_uint64(items[0], == , cc_rbuf_peek(rbuf, 0));
    munit_assert_uint64(items[1], == , cc_rbuf_peek(rbuf, 1));

    uint64_t a = munit_rand_int_range(0, range + 1);
    uint64_t b = munit_rand_int_range(0, range + 1);

    cc_rbuf_enqueue(rbuf, a);
    cc_rbuf_enqueue(rbuf, b);

    munit_assert_uint64(cc_rbuf_peek(rbuf, 0), == , a);
    munit_assert_uint64(cc_rbuf_peek(rbuf, 1), == , b);
   
    uint64_t out;
    cc_rbuf_dequeue(rbuf, &out);
    munit_assert_uint64(items[2], == , out);

    cc_rbuf_destroy(rbuf);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/ring_buffer/test_enqueue", test_enqueue, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/ring_buffer/test_dequeue", test_dequeue, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/ring_buffer/test_enqueue_past_capacity", test_enqueue_past_capacity, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}