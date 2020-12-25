#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_PQueueTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_PQueueTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_PQueueTestsWithDefaults);
};

TEST_C_WRAPPER(CC_PQueueTestsWithDefaults, PqueuePush);
TEST_C_WRAPPER(CC_PQueueTestsWithDefaults, PqueuePop);
TEST_C_WRAPPER(CC_PQueueTestsWithDefaults, PqueuePopLastTwos);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
