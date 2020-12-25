#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP_C_WRAPPER(SlistTestsWithDefaults)
{
    TEST_GROUP_C_SETUP_WRAPPER(SlistTestsWithDefaults);
    TEST_GROUP_C_TEARDOWN_WRAPPER(SlistTestsWithDefaults);
};

TEST_GROUP_C_WRAPPER(SlistTestsSlistPrepopulated)
{
    TEST_GROUP_C_SETUP_WRAPPER(SlistTestsSlistPrepopulated);
    TEST_GROUP_C_TEARDOWN_WRAPPER(SlistTestsSlistPrepopulated);
};

TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListNew);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListAddLast);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListAddFirst);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListAdd);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListContains);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListZipIterNext);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListZipIterAdd);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListZipIterRemove);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListZipIterReplace);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListSort);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListReverse);
TEST_C_WRAPPER(SlistTestsWithDefaults, CC_SListIndexOf);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListAddAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListAddAll);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListAddAllAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListRemove);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListRemoveFirst);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListRemoveLast);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListRemoveAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListRemoveAll);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListGetFirst);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListGetLast);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListGet);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListReplaceAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListCopyShallow);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SlistCopyDeep);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListSublist);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListSplice);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListSpliceAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListToArray);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListIterAdd);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListIterRemove);

TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilter1);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilter2);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilter3);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilterMut1);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilterMut2);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, CC_SListFilterMut3);


int main(int argc, char **argv){
    return RUN_ALL_TESTS(argc, argv);
}
