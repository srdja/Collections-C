#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(HashTableTestsConf)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashTableTestsConf);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTestsConf);
};

TEST_C_WRAPPER(HashTableTestsConf, HashTableNew);

TEST_GROUP_C_WRAPPER(HashTableTests)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashTableTests);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTests);
};

TEST_C_WRAPPER(HashTableTests, HashTableAdd);
TEST_C_WRAPPER(HashTableTests, HashTableRemove);
TEST_C_WRAPPER(HashTableTests, HashTableRemoveAll);
TEST_C_WRAPPER(HashTableTests, HashTableGet);
TEST_C_WRAPPER(HashTableTests, HashTableSize);
TEST_C_WRAPPER(HashTableTests, HashTableContainsKey);
TEST_C_WRAPPER(HashTableTests, HashTableIterNext);
TEST_C_WRAPPER(HashTableTests, HashTableIterRemove);

TEST_GROUP_C_WRAPPER(HashTableTestsCollision)
{
    TEST_GROUP_C_SETUP_WRAPPER(HashTableTestsCollision);
    TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTestsCollision);
};

TEST_C_WRAPPER(HashTableTestsCollision, HashTableCollisionGet);
TEST_C_WRAPPER(HashTableTestsCollision, HashTableCollisionRemove);

TEST_GROUP_C_WRAPPER(HashTableTestsZeroHash)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashTableTestsZeroHash);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTestsZeroHash);
};

TEST_C_WRAPPER(HashTableTestsZeroHash, HashTableAddStringWithNullKey);
TEST_C_WRAPPER(HashTableTestsZeroHash, HashTableRemoveStringWithNullKey);
TEST_C_WRAPPER(HashTableTestsZeroHash, HashTableGetStringWithNullKey);

TEST_GROUP_C_WRAPPER(HashTableTestsCapacity)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashTableTestsCapacity);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTestsCapacity);
};

TEST_C_WRAPPER(HashTableTestsCapacity, HashTableCapacity);

TEST_GROUP_C_WRAPPER(HashTableTestsCompare)
{
  TEST_GROUP_C_SETUP_WRAPPER(HashTableTestsCompare);
  TEST_GROUP_C_TEARDOWN_WRAPPER(HashTableTestsCompare);
};

TEST_C_WRAPPER(HashTableTestsCompare, HashTableTestsMemoryChunksAsKeys);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
