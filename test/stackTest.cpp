#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(StackTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(StackTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(StackTestsWithDefaults);
};

TEST_C_WRAPPER(StackTestsWithDefaults, StackPush);
TEST_C_WRAPPER(StackTestsWithDefaults, StackPop);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
