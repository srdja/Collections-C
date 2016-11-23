#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(QueueTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(QueueTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(QueueTestsWithDefaults);
};

TEST_C_WRAPPER(QueueTestsWithDefaults, QueueEnqueue);
TEST_C_WRAPPER(QueueTestsWithDefaults, QueuePoll);
TEST_C_WRAPPER(QueueTestsWithDefaults, QueueIter);
TEST_C_WRAPPER(QueueTestsWithDefaults, QueueZipIterNext);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
