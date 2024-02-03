#include "munit.h"
#include "memory/cc_dynamic_pool.h"
#include "cc_list.h"
#include <stdlib.h>


static CC_DynamicPool* pool = NULL;


void pool_free(void* ptr)
{
    cc_dynamic_pool_free(ptr, pool);
}

void *pool_malloc(size_t size)
{
    return cc_dynamic_pool_malloc(size, pool);
}

void *pool_calloc(size_t count, size_t size)
{
    return cc_dynamic_pool_calloc(count, size, pool);
}

#define CONTROL_VAL 0xFFFFFFFFFFFFFFFF

static MunitResult test_alignment(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    uint64_t control_val1 = 0xFFAABBCCDDEE0011;
    uint64_t control_val2 = 0xABCDEFF123456789;
    uint64_t control_val3 = 0x1110011100110011;

    munit_assert_int(CC_OK, ==, cc_dynamic_pool_new(sizeof(uint64_t) * 3, &pool));

    uint64_t* a = pool_malloc(sizeof(uint64_t));
    uint64_t* b = pool_malloc(sizeof(uint64_t));
    uint64_t* c = pool_malloc(sizeof(uint64_t));

    munit_assert_ptr_not_equal(a, b);
    munit_assert_ptr_not_equal(b, c);
    munit_assert_ptr_not_equal(a, c);

    *a = control_val1;
    *b = control_val2;
    *c = control_val3;

    munit_assert_uint64(*a, == , control_val1);
    munit_assert_uint64(*b, == , control_val2);
    munit_assert_uint64(*c, == , control_val3);

    munit_assert_ptr_equal(b, (a+1));
    munit_assert_ptr_equal(c, (a+2));

    cc_dynamic_pool_destroy(pool);

    return MUNIT_OK;
}


static MunitResult test_limit(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    // Only allocate space for 2 x uint64_t
    munit_assert_int(CC_OK, ==, cc_dynamic_pool_new(sizeof(uint64_t) * 2, &pool));
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size(0, == , cc_dynamic_pool_used_bytes(pool));

    // Start filling
    uint64_t* a = pool_malloc(sizeof(uint64_t));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_used_bytes(pool));

    uint64_t* b = pool_malloc(sizeof(uint64_t));
    munit_assert_size(0, == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_used_bytes(pool));

    // No more space from now on
    uint64_t* c = pool_malloc(sizeof(uint64_t)); 

    munit_assert_ptr_not_null(a);
    munit_assert_ptr_not_null(b);
    munit_assert_ptr_null(c);

    cc_dynamic_pool_destroy(pool);

    return MUNIT_OK;
}


static MunitResult test_expansion(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    // Make an expandable pool
    CC_DynamicPoolConf conf;
    cc_dynamic_pool_conf_init(&conf);
    conf.exp_factor = 1;
    conf.is_fixed = false;

    munit_assert_int(CC_OK, ==, cc_dynamic_pool_new_conf(sizeof(uint64_t) * 2, &conf, &pool));
    
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size(0, == , cc_dynamic_pool_used_bytes(pool));

    // Start filling
    uint64_t* a = pool_malloc(sizeof(uint64_t));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(a);

    uint64_t* b = pool_malloc(sizeof(uint64_t));
    munit_assert_size(0, == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(b);

    // This should trigger a resize
    uint64_t* c = pool_malloc(sizeof(uint64_t)); 
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 3), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(c);

    cc_dynamic_pool_destroy(pool);

    return MUNIT_OK;
}

static MunitResult test_reset(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    size_t pool_size = sizeof(uint64_t) * 2;

    // Make an expandable pool
    CC_DynamicPoolConf conf;
    cc_dynamic_pool_conf_init(&conf);
    conf.exp_factor = 1;
    conf.is_fixed = false;

    munit_assert_int(CC_OK, ==, cc_dynamic_pool_new_conf(pool_size, &conf, &pool));
    
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size(0, == , cc_dynamic_pool_used_bytes(pool));

    // Start filling
    uint64_t* a = pool_malloc(sizeof(uint64_t));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(a);

    uint64_t* b = pool_malloc(sizeof(uint64_t));
    munit_assert_size(0, == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(b);

    // This should trigger a resize
    uint64_t* c = pool_malloc(sizeof(uint64_t)); 
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 3), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(c);

    // Reset
    cc_dynamic_pool_reset(pool);

    // Should be initial size
    munit_assert_size(cc_dynamic_pool_free_bytes(pool), == , pool_size);

    // Try putting stuff back in
    a = pool_malloc(sizeof(uint64_t));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 1), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(a);

    b = pool_malloc(sizeof(uint64_t));
    munit_assert_size(0, == , cc_dynamic_pool_free_bytes(pool));
    munit_assert_size((sizeof(uint64_t) * 2), == , cc_dynamic_pool_used_bytes(pool));
    munit_assert_ptr_not_null(b);

    cc_dynamic_pool_destroy(pool);

    return MUNIT_OK;
}


static MunitResult test_with_list(const MunitParameter params[], void* fixture)
{
    (void)params;
    (void)fixture;

    munit_assert_int(CC_OK, ==, cc_dynamic_pool_new(sizeof(size_t) * 100000, &pool));
    
    CC_ListConf conf;
    cc_list_conf_init(&conf);
    conf.mem_alloc  = pool_malloc;
    conf.mem_calloc = pool_calloc;
    conf.mem_free   = pool_free;

    CC_List* list;
    cc_list_new_conf(&conf, &list);

    uint64_t *data = NULL;
    for (int i = 0; i < 50; i++) {
        data  = pool_malloc(sizeof(uint64_t));
        *data = CONTROL_VAL;
        munit_assert_not_null(data);
        munit_assert_int(CC_OK, == , cc_list_add(list, data));
    }

    CC_LIST_FOREACH(ret, list, {
        munit_assert_uint64(*((uint64_t*)ret), == , CONTROL_VAL);
    });

    cc_dynamic_pool_destroy(pool);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { (char*)"/heap_pool/test_alignment", test_alignment, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/heap_pool/test_limit", test_limit, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/heap_pool/test_expansion", test_expansion, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/heap_pool/test_with_list", test_with_list, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { (char*)"/heap_pool/test_reset", test_reset, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void*)"test", argc, argv);
}