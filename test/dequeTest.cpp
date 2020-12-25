#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_DequeTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_DequeTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_DequeTests);
};

TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddFirst);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddLast);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddAt1);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddAt2);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddAt3);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddAt4);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeAddAt5);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeRemoveFirst);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeRemoveLast);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeRemoveAll);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeGetAt);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeGetFirst);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeGetLast);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeCopyShallow);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeCopyDeep);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeContains);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeSize);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeCapacity);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeTrimCapacity);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeReverse);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeIteratorAdd);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeIteratorRemove);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeIteratorNext);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeZipIterRemove);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeZipIterAdd);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeZipIterNext);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeZipIterReplace);

TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilter1);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilter2);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilter3);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilterMut1);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilterMut2);
TEST_C_WRAPPER(CC_DequeTests, CC_DequeFilterMut3);

TEST_GROUP_C_WRAPPER(CC_DequeTestsConf)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_DequeTestsConf);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_DequeTestsConf);
};

TEST_C_WRAPPER(CC_DequeTestsConf, CC_DequeBufferExpansion);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
