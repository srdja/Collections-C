#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_TreeTableTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_TreeTableTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_TreeTableTestsWithDefaults);
};


TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableAdd);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableRemove);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveAll);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableGet);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableSize);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableGetFirst);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableGetLast);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveFirst);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableRemoveLast);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableGetGreaterThan);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableGetLessThan);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableIterNext);
TEST_C_WRAPPER(CC_TreeTableTestsWithDefaults, CC_TreeTableIterRemove);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
