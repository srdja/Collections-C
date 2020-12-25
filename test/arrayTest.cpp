#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_ArrayTestsWithDefaults)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_ArrayTestsWithDefaults);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_ArrayTestsWithDefaults);
};

TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayAdd);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayAddAt2);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayReplaceAt);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayRemove);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayRemoveAt);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayRemoveAll);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayGetAt);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayIndexOf);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArraySubarray);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayShallowCopy);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayDeepCopy);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayReverse);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayContains);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayIterRemove);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayIterReplace);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayZipIterNext);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayZipIterRemove);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayZipIterAdd);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayZipIterReplace);
TEST_C_WRAPPER(CC_ArrayTestsWithDefaults, CC_ArrayReduce);

TEST_GROUP_C_WRAPPER(CC_ArrayTestsCC_ArrayConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(CC_ArrayTestsCC_ArrayConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(CC_ArrayTestsCC_ArrayConf);
};

TEST_C_WRAPPER(CC_ArrayTestsCC_ArrayConf, CC_ArrayAddAt);
TEST_C_WRAPPER(CC_ArrayTestsCC_ArrayConf, CC_ArrayCapacity);

TEST_GROUP_C_WRAPPER(CC_ArrayTestsFilter)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_ArrayTestsFilter);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_ArrayTestsFilter);
};

TEST_C_WRAPPER(CC_ArrayTestsFilter, CC_ArrayFilterMut1);
TEST_C_WRAPPER(CC_ArrayTestsFilter, CC_ArrayFilterMut2);
TEST_C_WRAPPER(CC_ArrayTestsFilter, CC_ArrayFilter1);
TEST_C_WRAPPER(CC_ArrayTestsFilter, CC_ArrayFilter2);

int main(int argc, char **argv)
{
  return RUN_ALL_TESTS(argc, argv);
}
