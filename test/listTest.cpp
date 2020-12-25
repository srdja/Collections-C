#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(CC_ListTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_ListTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_ListTestsWithDefaults);
};

TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListNew);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListAdd);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListAddLast);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListAddFirst);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListIndexOf);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListSort);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListContains);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListZipIterNext);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListZipIterAdd);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListZipIterRemove);
TEST_C_WRAPPER(CC_ListTestsWithDefaults, CC_ListZipIterReplace);


TEST_GROUP_C_WRAPPER(CC_ListTestsCC_ListPrefilled)
{
    TEST_GROUP_C_SETUP_WRAPPER(CC_ListTestsCC_ListPrefilled);
    TEST_GROUP_C_TEARDOWN_WRAPPER(CC_ListTestsCC_ListPrefilled)
};

TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListAddAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListAddAllAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListIterAdd);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListIterRemove);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListIterDescRemove);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListIterDescAdd);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListReverse);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListToArray);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListSpliceAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListSplice);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListAddAll);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListRemove);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListRemoveFirst);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListRemoveLast);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListRemoveAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListRemoveAll);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListGetLast);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListGetAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListReplaceAt);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListCopyShallow);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListCopyDeep);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListSublist);

TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListFilter1);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListFilter2);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListMutFilter1);
TEST_C_WRAPPER(CC_ListTestsCC_ListPrefilled, CC_ListMutFilter2);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
