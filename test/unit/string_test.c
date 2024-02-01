#include "munit.h"
#include "cc_string.h"
#include <stdlib.h>

static MunitResult test_string_new(const MunitParameter params[], void* fixture)
{


    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/string/new", test_string_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}