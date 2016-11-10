#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(TreeTableTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(TreeTableTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(TreeTableTestsWithDefaults);
};


TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableAdd);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableRemove);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableRemoveAll);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableGet);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableSize);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableGetFirst);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableGetLast);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableRemoveFirst);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableRemoveLast);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableGetGreaterThan);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableGetLessThan);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableIterNext);
TEST_C_WRAPPER(TreeTableTestsWithDefaults, TreeTableIterRemove);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
