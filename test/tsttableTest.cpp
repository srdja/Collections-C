#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"


TEST_GROUP_C_WRAPPER(TSTTableTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(TSTTableTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(TSTTableTestsConf);
};

TEST_C_WRAPPER(TSTTableTestsConf, TSTTableNew);



TEST_GROUP_C_WRAPPER(TSTTableTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(TSTTableTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(TSTTableTests);
};

TEST_C_WRAPPER(TSTTableTests, TSTTableAdd);
TEST_C_WRAPPER(TSTTableTests, TSTTableReplacingAdd);
TEST_C_WRAPPER(TSTTableTests, TSTTableRemove);
TEST_C_WRAPPER(TSTTableTests, TSTTableRemoveAll);
TEST_C_WRAPPER(TSTTableTests, TSTTableGet);
TEST_C_WRAPPER(TSTTableTests, TSTTableSize);
TEST_C_WRAPPER(TSTTableTests, TSTTableContainsKey);
TEST_C_WRAPPER(TSTTableTests, TSTTableIterNextEmpty);
TEST_C_WRAPPER(TSTTableTests, TSTTableIterNext);
TEST_C_WRAPPER(TSTTableTests, TSTTableIterRemoveEmpty);
TEST_C_WRAPPER(TSTTableTests, TSTTableIterRemove);
TEST_C_WRAPPER(TSTTableTests, TSTTableLoop);


int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
