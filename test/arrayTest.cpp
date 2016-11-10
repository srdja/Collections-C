#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(ArrayTestsWithDefaults)
{
  TEST_GROUP_C_SETUP_WRAPPER(ArrayTestsWithDefaults);
  TEST_GROUP_C_TEARDOWN_WRAPPER(ArrayTestsWithDefaults);
};

TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayAdd);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayAddAt2);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayReplaceAt);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayRemove);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayRemoveAt);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayRemoveAll);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayGetAt);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayIndexOf);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArraySubarray);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayShallowCopy);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayDeepCopy);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayReverse);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayContains);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayIterRemove);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayIterReplace);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayZipIterNext);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayZipIterRemove);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayZipIterAdd);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayZipIterReplace);
TEST_C_WRAPPER(ArrayTestsWithDefaults, ArrayReduce);

TEST_GROUP_C_WRAPPER(ArrayTestsArrayConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(ArrayTestsArrayConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(ArrayTestsArrayConf);
};

TEST_C_WRAPPER(ArrayTestsArrayConf, ArrayAddAt);
TEST_C_WRAPPER(ArrayTestsArrayConf, ArrayCapacity);

TEST_GROUP_C_WRAPPER(ArrayTestsFilter)
{
  TEST_GROUP_C_SETUP_WRAPPER(ArrayTestsFilter);
  TEST_GROUP_C_TEARDOWN_WRAPPER(ArrayTestsFilter);
};

TEST_C_WRAPPER(ArrayTestsFilter, ArrayFilterMut1);
TEST_C_WRAPPER(ArrayTestsFilter, ArrayFilterMut2);
TEST_C_WRAPPER(ArrayTestsFilter, ArrayFilter1);
TEST_C_WRAPPER(ArrayTestsFilter, ArrayFilter2);

int main(int argc, char **argv)
{
  return RUN_ALL_TESTS(argc, argv);
}
