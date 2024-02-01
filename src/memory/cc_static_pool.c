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

#include "memory/cc_static_pool.h"

struct cc_static_pool_s {
    size_t   size;
    uint8_t* block;
    uint8_t* high_ptr;
    uint8_t* low_ptr;
    uint8_t* free_ptr;
};


/**
 * Creates a new CC_StaticPool structure insdie the pool_alloc buffer. The size of the 
 * pool_alloc buffer should be at least <code>cc_static_pool_struct_size(CC_StaticPool)</code>
 * bytes long.
 * 
 * @param[in] size - size of the provided data buffer in bytes
 * @param[in] offset - the offset from the begining of the data buffer in bytes
 * @param[in] data_buf - 
 * @param[in] pool_alloc - buffer into which the pool structure itself is going to be stored.
 * @param[out] out - pointer to the newly created pool structure
 * 
 * @return CC_OK if the creation was successful.
 */
enum cc_stat cc_static_pool_new(
    size_t size,
    size_t offset,
    uint8_t* data_buf,
    uint8_t* pool_alloc,
    CC_StaticPool** out) 
{
    CC_StaticPool *pool = (CC_StaticPool*)pool_alloc;
    pool->block    = data_buf + offset;
    pool->free_ptr = pool->block;
    pool->high_ptr = pool->block;
    pool->low_ptr  = pool->block;
    pool->size     = size;

    *out = pool;

    return CC_OK;
}


/**
 * Returns the size of the CC_StaticPool structure in bytes.
 */
size_t cc_static_pool_struct_size()
{
    return sizeof(CC_StaticPool);
}

/**
 * Resets the pool to its initial state. All previously allocated blocks
 * should be considered invalidated
 * 
 * @param[in] pool - pool to reset
 */
void cc_static_pool_reset(CC_StaticPool* pool)
{
    pool->free_ptr = pool->low_ptr;
    pool->high_ptr = pool->low_ptr;
}

/**
 * Allocates a block memory of <code>size</size> in bytes.
 * 
 * @param[in] size  - number of bytes to be allocated
 * @param[in] pool  - pool inside which the bytes are allocated
 * @return pointer to the newly allocated block.
 */
void* cc_static_pool_malloc(size_t size, CC_StaticPool* pool)
{
    uint8_t* page_max = pool->low_ptr + pool->size;
    uint8_t* new_high = pool->high_ptr + size;

    if (new_high >= page_max) {
        return NULL;
    }
    uint8_t* ptr     = pool->free_ptr;
    pool->high_ptr   = ptr;
    pool->free_ptr   = ptr + size;

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
void* cc_static_pool_calloc(size_t count, size_t size, CC_StaticPool* pool)
{
    uint8_t* ptr = NULL; 
    if ((ptr = cc_static_pool_malloc(count * size, pool))) {
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
void cc_static_pool_free(void* ptr, CC_StaticPool* pool)
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
size_t cc_static_pool_used_bytes(CC_StaticPool* pool)
{
    return pool->free_ptr - pool->low_ptr;
}

/**
 * Returns the number of free bytes inside the pool.
 * 
 * @param[in] pool
 * 
 * @return free bytes
 */
size_t cc_static_pool_free_bytes(CC_StaticPool* pool)
{
    return pool->size - (pool->free_ptr - pool->low_ptr);
}