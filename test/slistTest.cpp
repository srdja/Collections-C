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

TEST_C_WRAPPER(SlistTestsWithDefaults, SListNew);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListAddLast);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListAddFirst);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListAdd);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListContains);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListZipIterNext);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListZipIterAdd);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListZipIterRemove);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListZipIterReplace);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListSort);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListReverse);
TEST_C_WRAPPER(SlistTestsWithDefaults, SListIndexOf);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListAddAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListAddAll);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListAddAllAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListRemove);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListRemoveFirst);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListRemoveLast);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListRemoveAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListRemoveAll);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListGetFirst);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListGetLast);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListGet);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListReplaceAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListCopyShallow);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SlistCopyDeep);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListSublist);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListSplice);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListSpliceAt);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListToArray);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListIterAdd);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListIterRemove);

TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilter1);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilter2);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilter3);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilterMut1);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilterMut2);
TEST_C_WRAPPER(SlistTestsSlistPrepopulated, SListFilterMut3);


int main(int argc, char **argv){
    return RUN_ALL_TESTS(argc, argv);
}
