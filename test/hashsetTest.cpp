#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(HashSetTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashSetTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashSetTests);
};

TEST_GROUP_C_WRAPPER(HashSetTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashSetTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashSetTestsConf);
};

TEST_C_WRAPPER(HashSetTestsConf, HashSetNew);
TEST_C_WRAPPER(HashSetTests, HashSetAdd);
TEST_C_WRAPPER(HashSetTests, HashSetRemove);
TEST_C_WRAPPER(HashSetTests, HashSetRemoveAll);
TEST_C_WRAPPER(HashSetTests, HashSetIterNext);
TEST_C_WRAPPER(HashSetTests, HashSetIterRemove);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
