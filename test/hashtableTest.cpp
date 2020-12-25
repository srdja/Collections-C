#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_HashTableTestsConf)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTestsConf);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTestsConf);
};

TEST_C_WRAPPER(CC_HashTableTestsConf, CC_HashTableNew);

TEST_GROUP_C_WRAPPER(CC_HashTableTests)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTests);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTests);
};

TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableAdd);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableRemove);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableRemoveAll);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableGet);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableSize);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableContainsKey);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableIterNext);
TEST_C_WRAPPER(CC_HashTableTests, CC_HashTableIterRemove);

TEST_GROUP_C_WRAPPER(CC_HashTableTestsCollision)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTestsCollision);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTestsCollision);
};

TEST_C_WRAPPER(CC_HashTableTestsCollision, CC_HashTableCollisionGet);
TEST_C_WRAPPER(CC_HashTableTestsCollision, CC_HashTableCollisionRemove);

TEST_GROUP_C_WRAPPER(CC_HashTableTestsZeroHash)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTestsZeroHash);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTestsZeroHash);
};

TEST_C_WRAPPER(CC_HashTableTestsZeroHash, CC_HashTableAddStringWithNullKey);
TEST_C_WRAPPER(CC_HashTableTestsZeroHash, CC_HashTableRemoveStringWithNullKey);
TEST_C_WRAPPER(CC_HashTableTestsZeroHash, CC_HashTableGetStringWithNullKey);

TEST_GROUP_C_WRAPPER(CC_HashTableTestsCapacity)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTestsCapacity);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTestsCapacity);
};

TEST_C_WRAPPER(CC_HashTableTestsCapacity, CC_HashTableCapacity);

TEST_GROUP_C_WRAPPER(CC_HashTableTestsCompare)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_HashTableTestsCompare);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_HashTableTestsCompare);
};

TEST_C_WRAPPER(CC_HashTableTestsCompare, CC_HashTableTestsMemoryChunksAsKeys);

int main(int argc, char **argv) {
  return RUN_ALL_TESTS(argc, argv);
}
