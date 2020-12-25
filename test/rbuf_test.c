/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T13:46:33-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-09T15:24:03-06:00
 */

#include "cc_ring_buffer.h"
#include "CppUTest/TestHarness_c.h"
#include <time.h>

static int stat;
static CC_Rbuf *rbuf;
unsigned long long range = 100;

TEST_GROUP_C_SETUP(CC_RbufTest)
{
    stat = cc_rbuf_new(&rbuf);
}

TEST_GROUP_C_TEARDOWN(CC_RbufTest)
{
    cc_rbuf_destroy(rbuf);
}

TEST_C(CC_RbufTest, CC_RbufEnqueue)
{
    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t)*10);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < 10; i++) {
      uint64_t item = rand() % range;
      cc_rbuf_enqueue(rbuf, item);
      items[i] = item;
    }
    for (int i = 0; i < 10; i++)
        CHECK_EQUAL_C_INT(cc_rbuf_peek(rbuf, i), items[i]);
}

TEST_C(CC_RbufTest, CC_RbufDequeue)
{
     uint64_t items[10];
     memset(items, 0, sizeof(uint64_t)*10);
     srand((unsigned int) time(NULL));
     for (int i = 0; i < 10; i++) {
       uint64_t item = rand() % range + 1;
       items[i] = item;
       cc_rbuf_enqueue(rbuf, item);
     }

     uint64_t out;
     for (int i = 0; i < 10; i++) {
         cc_rbuf_dequeue(rbuf, &out);
         CHECK_EQUAL_C_INT(items[i], out);
       memset(&out, 0, sizeof(uint64_t));
     }
}

TEST_C(CC_RbufTest, CC_RbufEnqueuePastCapacity)
{
    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t)*10);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < 10; i++) {
      uint64_t item = rand() % range + 1;
      items[i] = item;
      cc_rbuf_enqueue(rbuf, item);
    }

    CHECK_EQUAL_C_INT(items[0], cc_rbuf_peek(rbuf, 0));
    CHECK_EQUAL_C_INT(items[1], cc_rbuf_peek(rbuf, 1));

    uint64_t a = rand() % range + 1, b = rand() % range + 1;
    cc_rbuf_enqueue(rbuf, a);
    cc_rbuf_enqueue(rbuf, b);

    CHECK_EQUAL_C_INT(cc_rbuf_peek(rbuf, 0), a);
    CHECK_EQUAL_C_INT(cc_rbuf_peek(rbuf, 1), b);
    uint64_t out;
    cc_rbuf_dequeue(rbuf, &out);
    CHECK_EQUAL_C_INT(items[2], out);
}
