#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_TreeSetTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_TreeSetTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_TreeSetTestsWithDefaults);
};

TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetAdd);
TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetRemove);
TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetRemoveAll);
TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetSetSize);
TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetIterNext);
TEST_C_WRAPPER(CC_TreeSetTestsWithDefaults, CC_TreeSetIterRemove);

int main(int argc, char **argv){
    return RUN_ALL_TESTS(argc, argv);
}
