/*
 * Collections-C
 * Copyright (C) 2013-2024 Srđan Panić <srdja.panic@gmail.com>
 *
 * This file is part of Collections-C.
 *
 * Collections-C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Collections-C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "memory/cc_dynamic_pool.h"


struct cc_page_info_s {
    void  *previous;
    size_t size;
};

typedef struct cc_page_info_s PageInfo;


struct cc_dynamic_pool_s {
    bool     is_fixed;
    bool     is_packed;
    float    exp_factor;
    size_t   top_page_size;
    size_t   alignment_boundary;
    
    uint8_t* page;
    uint8_t* high_ptr;  /*Start of the highest block*/
    uint8_t* low_ptr;   /*Start of the page*/
    uint8_t* free_ptr;  /*Next free location*/

    void* (*mem_alloc)  (size_t size);
    void* (*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};
 
/**
 * Initializes the config to default values.
 */
void cc_dynamic_pool_conf_init(CC_DynamicPoolConf *conf)
{
    conf->exp_factor         = 1;
    conf->is_fixed           = true;
    conf->is_packed          = true;
    conf->alignment_boundary = 1;
    conf->mem_alloc          = malloc;
    conf->mem_calloc         = calloc;
    conf->mem_free           = free;
}

/**
 * Creates a new pool based on the specified config.
 * 
 * @param[in] size - size of the pool in bytes
 * @param[in] conf - configuration
 * @param[out] out - pointer to where the newly created pool is to be 
 * stored
 * 
 * @return CC_OK if the creation was successful or CC_ERR_ALLOC
 * if the allocation failed.
 */
enum cc_stat cc_dynamic_pool_new_conf(
    size_t size,
    CC_DynamicPoolConf const * const conf, 
    CC_DynamicPool **out)
{
    CC_DynamicPool* pool  = conf->mem_calloc(1, sizeof(CC_DynamicPool));

    if (!pool) {
        return CC_ERR_ALLOC;
    }
    uint8_t* page      = conf->mem_alloc(size + sizeof(PageInfo));

    if (!page) {
        conf->mem_free(pool);
        return CC_ERR_ALLOC;
    }
    PageInfo* page_info = (PageInfo*)page;
    page_info->previous = NULL;
    page_info->size     = size;
    
    pool->exp_factor    = conf->exp_factor;
    pool->is_fixed      = conf->is_fixed;
    pool->is_packed     = conf->is_packed;
    pool->top_page_size = size;    
    pool->page          = page;
    pool->high_ptr      = page + sizeof(PageInfo);
    pool->low_ptr       = pool->high_ptr;
    pool->free_ptr      = pool->high_ptr;

    pool->mem_alloc     = conf->mem_alloc;
    pool->mem_calloc    = conf->mem_calloc;
    pool->mem_free      = conf->mem_free;

    *out = pool;

    return CC_OK;
}

/**
 * Creates a new non-expandable pool of <code>size</code> in bytes.
 * 
 * @param[in] size - size of the pool in bytes
 * @param[out] out pointer to where the newly created CC_DynamicPool is to be stored
 */
enum cc_stat cc_dynamic_pool_new(size_t size, CC_DynamicPool **out)
{
    CC_DynamicPoolConf c;
    cc_dynamic_pool_conf_init(&c);
    return cc_dynamic_pool_new_conf(size, &c, out);
}

/**
 * Destroys the pool and frees all the buffers from memory.
 * 
 * @param[in] pool
 */
void cc_dynamic_pool_destroy(CC_DynamicPool* pool)
{
    PageInfo* p = (PageInfo*)pool->page;
    do {
        PageInfo* del = p;
        p = p->previous;
        pool->mem_free(del);
    } while (p);
    
    pool->mem_free(pool);
}

/**
 * Resets the pool to its initial state. All previously allocated
 * blocks should be considered invalidated.
 * 
 * @param[in] pool - pool to reset
 */
void cc_dynamic_pool_reset(CC_DynamicPool* pool)
{
    PageInfo* top_page = (PageInfo*)pool->page;
    PageInfo* del_page = top_page->previous;
    
    while (del_page) {
        top_page = del_page;
        del_page = top_page->previous;
        pool->mem_free(del_page);
    }
    pool->page          = (uint8_t*) top_page;
    pool->top_page_size = ((PageInfo*)top_page)->size;
    pool->low_ptr       = (uint8_t*) top_page + (sizeof(PageInfo));
    pool->high_ptr      = pool->low_ptr;
    pool->free_ptr      = pool->low_ptr;
}

/**
 * Allocates a block memory of <code>size</size> in bytes.
 * 
 * @param[in] size  - number of bytes to be allocated
 * @param[in] pool  - pool inside which the bytes are allocated
 * @return pointer to the newly allocated block.
 */
void* cc_dynamic_pool_malloc(size_t size, CC_DynamicPool* pool)
{
    if (size >= pool->top_page_size) {
        return NULL;
    }
    uint8_t* page_max = pool->low_ptr + pool->top_page_size;
    uint8_t* new_high = pool->high_ptr + size;

    if (new_high >= page_max) {
        size_t next_max = (size_t) (pool->top_page_size * pool->exp_factor);
        if (pool->is_fixed || size > next_max) {
            return NULL;
        }
        uint8_t*  new_page = pool->mem_alloc(next_max + sizeof(PageInfo));

        if (!new_page) {
            return NULL;
        }
        PageInfo* pinfo     = (PageInfo*)new_page;
        pinfo->previous     = pool->page;
        pinfo->size         = next_max;

        pool->page          = new_page;
        pool->high_ptr      = new_page + sizeof(PageInfo);
        pool->low_ptr       = pool->high_ptr;
        pool->free_ptr      = pool->high_ptr;
        pool->top_page_size = next_max;
    }
    uint8_t* ptr   = pool->free_ptr;
    pool->high_ptr = ptr;

    size_t padding = 0;
    if (!pool->is_packed) {
        padding = size % pool->alignment_boundary;
    }
    pool->free_ptr = ptr + size + padding;
    return ptr;
}

/**
 * Allocates a block of memory for <code>count</code> elements of 
 * <code>size</code> in bytes and sets all the allocated bytes to 0.
 * 
 * @param[in] count - 
 * @param[in] size  - 
 * @param[in] pool  - 
 * 
 * @return Pointer to the newly allocated block.
 */
void* cc_dynamic_pool_calloc(size_t count, size_t size, CC_DynamicPool* pool)
{
    uint8_t* ptr;
    if ((ptr = cc_dynamic_pool_malloc(count * size, pool))) {
        memset(ptr, 0, (count * size));
    }
    return ptr;
}

/**
 * Frees the block only if it is the most recent block allocated. 
 * Otherwise it has no effect.
 * 
 * @param[in] ptr - pointer to the block
 * @param[in] pool - pool from which this block is to be freed
 */
void cc_dynamic_pool_free(void *ptr, CC_DynamicPool *pool)
{
    // Roll back the top entry
    if (ptr == pool->high_ptr) {
        pool->free_ptr = pool->high_ptr;
    }
}

/**
 * Returns the total number of used bytes inside the poool.
 * 
 * @param[in] pool
 * 
 * @return used bytes
 */
size_t cc_dynamic_pool_used_bytes(CC_DynamicPool *pool)
{
    size_t total = pool->free_ptr - pool->low_ptr;
    PageInfo* pi = (PageInfo*)pool->page;

    while (pi->previous) {
        total += pi->size;
        pi     = pi->previous;
    }
    return total;
}

/**
 * Returns the number of free bytes inside the pool.
 * 
 * @param[in] pool
 * 
 * @return free bytes
 */
size_t cc_dynamic_pool_free_bytes(CC_DynamicPool* pool)
{
    return pool->top_page_size - (pool->free_ptr - pool->low_ptr);
}

size_t cc_dynamic_pool_struct_size()
{
    return sizeof(CC_DynamicPool);
}