#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "memory/cc_dynamic_pool.h"
#include "cc_list.h"
#include "cc_slist.h"

// 500 MB
#define POOL_SIZE 500000000

void bench_malloc()
{
    int val = 100;

    printf("Runing system malloc test...\n");
    printf("Populating list... ");
    clock_t t_start = clock();

    CC_SList* list;
    cc_slist_new(&list);

    for (int i = 0; i < 10000000; i++) {
        cc_slist_add(list, (void*) & val);
    }
    clock_t t_end = clock();
    double t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;
    printf("done in %Lf sec.\n", t_delta);

    printf("Reading list... ");
    t_start = clock();

    CC_SLIST_FOREACH(v, list, {
        *((int*)v) += 1;
    });
    t_end = clock();
    t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;

    cc_slist_destroy(list);
    printf("done in %Lf sec.\n\n", t_delta);
}

CC_DynamicPool* pool;


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


void bench_pool_packed()
{
    cc_dynamic_pool_new(POOL_SIZE, &pool);
    int val = 100;

    printf("Runing packed pool test...\n");
    printf("Populating list... ");
    clock_t t_start = clock();

    CC_SList* list;
    CC_SListConf conf;
    conf.mem_alloc = pool_malloc;
    conf.mem_calloc = pool_calloc;
    conf.mem_free = pool_free;
    cc_slist_new_conf(&conf, &list);

    for (int i = 0; i < 10000000; i++) {
        cc_slist_add(list, (void*)&val);
    }
    clock_t t_end = clock();
    double t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;
    printf("done in %Lf sec.\n", t_delta);

    printf("Reading list... ");
    t_start = clock();

    CC_SLIST_FOREACH(v, list, {
        *((int*)v) += 1;
    });
    t_end = clock();
    t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;

    printf("done in %f sec.\n\n", t_delta);

    cc_dynamic_pool_destroy(pool);
}


void bench_pool_aligned()
{
    CC_DynamicPoolConf pconf;
    cc_dynamic_pool_conf_init(&pconf);
    pconf.alignment_boundary = false;
    pconf.alignment_boundary = sizeof(size_t);
    cc_dynamic_pool_new_conf(POOL_SIZE, &pconf, &pool);
    int val = 100;

    printf("Runing aligned pool test...\n");
    printf("Populating list... ");
    clock_t t_start = clock();

    CC_SList* list;
    CC_SListConf conf;
    conf.mem_alloc = pool_malloc;
    conf.mem_calloc = pool_calloc;
    conf.mem_free = pool_free;
    cc_slist_new_conf(&conf, &list);

    for (int i = 0; i < 10000000; i++) {
        cc_slist_add(list, (void*)&val);
    }
    clock_t t_end = clock();
    double t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;
    printf("done in %Lf sec.\n", t_delta);

    printf("Reading list... ");
    t_start = clock();

    CC_SLIST_FOREACH(v, list, {
        *((int*)v) += 1;
    });
    t_end = clock();
    t_delta = (double)(t_end - t_start)/CLOCKS_PER_SEC;

    printf("done in %f sec.\n\n", t_delta);

    cc_dynamic_pool_destroy(pool);
}


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    bench_pool_aligned();
    bench_pool_packed();
    bench_malloc();

    return 0;
}