#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(DequeTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(DequeTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(DequeTests);
};

TEST_C_WRAPPER(DequeTests, DequeAddFirst);
TEST_C_WRAPPER(DequeTests, DequeAddLast);
TEST_C_WRAPPER(DequeTests, DequeAddAt1);
TEST_C_WRAPPER(DequeTests, DequeAddAt2);
TEST_C_WRAPPER(DequeTests, DequeAddAt3);
TEST_C_WRAPPER(DequeTests, DequeAddAt4);
TEST_C_WRAPPER(DequeTests, DequeAddAt5);
TEST_C_WRAPPER(DequeTests, DequeRemoveFirst);
TEST_C_WRAPPER(DequeTests, DequeRemoveLast);
TEST_C_WRAPPER(DequeTests, DequeRemoveAll);
TEST_C_WRAPPER(DequeTests, DequeGetAt);
TEST_C_WRAPPER(DequeTests, DequeGetFirst);
TEST_C_WRAPPER(DequeTests, DequeGetLast);
TEST_C_WRAPPER(DequeTests, DequeCopyShallow);
TEST_C_WRAPPER(DequeTests, DequeCopyDeep);
TEST_C_WRAPPER(DequeTests, DequeContains);
TEST_C_WRAPPER(DequeTests, DequeSize);
TEST_C_WRAPPER(DequeTests, DequeCapacity);
TEST_C_WRAPPER(DequeTests, DequeTrimCapacity);
TEST_C_WRAPPER(DequeTests, DequeReverse);
TEST_C_WRAPPER(DequeTests, DequeIteratorAdd);
TEST_C_WRAPPER(DequeTests, DequeIteratorRemove);
TEST_C_WRAPPER(DequeTests, DequeIteratorNext);
TEST_C_WRAPPER(DequeTests, DequeZipIterRemove);
TEST_C_WRAPPER(DequeTests, DequeZipIterAdd);
TEST_C_WRAPPER(DequeTests, DequeZipIterNext);
TEST_C_WRAPPER(DequeTests, DequeZipIterReplace);

TEST_C_WRAPPER(DequeTests, DequeFilter1);
TEST_C_WRAPPER(DequeTests, DequeFilter2);
TEST_C_WRAPPER(DequeTests, DequeFilter3);
TEST_C_WRAPPER(DequeTests, DequeFilterMut1);
TEST_C_WRAPPER(DequeTests, DequeFilterMut2);
TEST_C_WRAPPER(DequeTests, DequeFilterMut3);

TEST_GROUP_C_WRAPPER(DequeTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(DequeTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(DequeTestsConf);
};

TEST_C_WRAPPER(DequeTestsConf, DequeBufferExpansion);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
