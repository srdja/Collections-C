#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(ListTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(ListTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(ListTestsWithDefaults);
};

TEST_C_WRAPPER(ListTestsWithDefaults, ListNew);
TEST_C_WRAPPER(ListTestsWithDefaults, ListAdd);
TEST_C_WRAPPER(ListTestsWithDefaults, ListAddLast);
TEST_C_WRAPPER(ListTestsWithDefaults, ListAddFirst);
TEST_C_WRAPPER(ListTestsWithDefaults, ListIndexOf);
TEST_C_WRAPPER(ListTestsWithDefaults, ListSort);
TEST_C_WRAPPER(ListTestsWithDefaults, ListContains);
TEST_C_WRAPPER(ListTestsWithDefaults, ListZipIterNext);
TEST_C_WRAPPER(ListTestsWithDefaults, ListZipIterAdd);
TEST_C_WRAPPER(ListTestsWithDefaults, ListZipIterRemove);
TEST_C_WRAPPER(ListTestsWithDefaults, ListZipIterReplace);


TEST_GROUP_C_WRAPPER(ListTestsListPrefilled)
{
    TEST_GROUP_C_SETUP_WRAPPER(ListTestsListPrefilled);
    TEST_GROUP_C_TEARDOWN_WRAPPER(ListTestsListPrefilled)
};

TEST_C_WRAPPER(ListTestsListPrefilled, ListAddAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListAddAllAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListIterAdd);
TEST_C_WRAPPER(ListTestsListPrefilled, ListIterRemove);
TEST_C_WRAPPER(ListTestsListPrefilled, ListIterDescRemove);
TEST_C_WRAPPER(ListTestsListPrefilled, ListIterDescAdd);
TEST_C_WRAPPER(ListTestsListPrefilled, ListReverse);
TEST_C_WRAPPER(ListTestsListPrefilled, ListToArray);
TEST_C_WRAPPER(ListTestsListPrefilled, ListSpliceAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListSplice);
TEST_C_WRAPPER(ListTestsListPrefilled, ListAddAll);
TEST_C_WRAPPER(ListTestsListPrefilled, ListRemove);
TEST_C_WRAPPER(ListTestsListPrefilled, ListRemoveFirst);
TEST_C_WRAPPER(ListTestsListPrefilled, ListRemoveLast);
TEST_C_WRAPPER(ListTestsListPrefilled, ListRemoveAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListRemoveAll);
TEST_C_WRAPPER(ListTestsListPrefilled, ListGetLast);
TEST_C_WRAPPER(ListTestsListPrefilled, ListGetAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListReplaceAt);
TEST_C_WRAPPER(ListTestsListPrefilled, ListCopyShallow);
TEST_C_WRAPPER(ListTestsListPrefilled, ListCopyDeep);
TEST_C_WRAPPER(ListTestsListPrefilled, ListSublist);

TEST_C_WRAPPER(ListTestsListPrefilled, ListFilter1);
TEST_C_WRAPPER(ListTestsListPrefilled, ListFilter2);
TEST_C_WRAPPER(ListTestsListPrefilled, ListMutFilter1);
TEST_C_WRAPPER(ListTestsListPrefilled, ListMutFilter2);

int main(int argc, char **argv) {
    return RUN_ALL_TESTS(argc, argv);
}
