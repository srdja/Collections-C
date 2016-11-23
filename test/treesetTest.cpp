#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(TreeSetTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(TreeSetTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(TreeSetTestsWithDefaults);
};

TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetAdd);
TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetRemove);
TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetRemoveAll);
TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetSetSize);
TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetIterNext);
TEST_C_WRAPPER(TreeSetTestsWithDefaults, TreeSetIterRemove);

int main(int argc, char **argv){
    return RUN_ALL_TESTS(argc, argv);
}
