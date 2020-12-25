#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_HashSetTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_HashSetTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashSetTests);
};

TEST_GROUP_C_WRAPPER(CC_HashSetTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_HashSetTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashSetTestsConf);
};

TEST_C_WRAPPER(CC_HashSetTestsConf, CC_HashSetNew);
TEST_C_WRAPPER(CC_HashSetTests, CC_HashSetAdd);
TEST_C_WRAPPER(CC_HashSetTests, CC_HashSetRemove);
TEST_C_WRAPPER(CC_HashSetTests, CC_HashSetRemoveAll);
TEST_C_WRAPPER(CC_HashSetTests, CC_HashSetIterNext);
TEST_C_WRAPPER(CC_HashSetTests, CC_HashSetIterRemove);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
