#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(PQueueTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(PQueueTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(PQueueTestsWithDefaults);
};

TEST_C_WRAPPER(PQueueTestsWithDefaults, PqueuePush);
TEST_C_WRAPPER(PQueueTestsWithDefaults, PqueuePop);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
