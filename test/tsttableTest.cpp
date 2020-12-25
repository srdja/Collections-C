#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"


TEST_GROUP_C_WRAPPER(CC_TSTTableTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_TSTTableTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_TSTTableTestsConf);
};

TEST_C_WRAPPER(CC_TSTTableTestsConf, CC_TSTTableNew);



TEST_GROUP_C_WRAPPER(CC_TSTTableTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_TSTTableTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_TSTTableTests);
};

TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableAdd);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableAddSubKeys);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableReplacingAdd);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableRemove);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableRemoveAll);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableGet);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableSize);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableContainsKey);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableIterNextEmpty);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableIterNext);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableIterRemoveEmpty);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableIterRemove);
TEST_C_WRAPPER(CC_TSTTableTests, CC_TSTTableLoop);


int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
