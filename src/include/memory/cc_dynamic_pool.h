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

#ifndef _CC_DYNAMIC_POOL_H_
#define _CC_DYNAMIC_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_common.h"

/**
A memory pool allocated on the heap. By default its size
is fixed, but it can be optinally set to expand once it's 
full.
*/
typedef struct cc_dynamic_pool_s CC_DynamicPool;

typedef struct cc_dynamic_pool_conf_s {
    /**
     * The rate at which the pool expands (initial_size * exp_factor). 
     * Must be a positive. Only has an effect if the the pool expansion 
     * isn't fixed.
     * 
     * Example: 
     * - if (exp_factor == 1) then linear growth
     * - if (exp_factor >  1) then accelarated growth
     * - if (exp_factor <  1) then decaying growth 
     **/
    float  exp_factor;

    /**
    * If true, the pool will not try to expand when full. */
    bool   is_fixed;

    bool   is_packed;
    size_t alignment_boundary;

    /**
     * Memory allocators used to allocate the Array structure and the
     * underlying data buffers. */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
} CC_DynamicPoolConf;


enum cc_stat cc_dynamic_pool_new       (size_t initial_size, CC_DynamicPool** out);
enum cc_stat cc_dynamic_pool_new_conf  (size_t initial_size, CC_DynamicPoolConf const * const conf, CC_DynamicPool **out);
void         cc_dynamic_pool_conf_init (CC_DynamicPoolConf *conf);

void         cc_dynamic_pool_destroy   (CC_DynamicPool* pool);
void         cc_dynamic_pool_reset     (CC_DynamicPool* pool);
size_t       cc_dynamic_pool_used_bytes(CC_DynamicPool* pool);
size_t       cc_dynamic_pool_free_bytes(CC_DynamicPool* pool);

void*        cc_dynamic_pool_malloc    (size_t size, CC_DynamicPool *pool);
void*        cc_dynamic_pool_calloc    (size_t count, size_t size, CC_DynamicPool *pool);
//void*        cc_dynamic_pool_realloc   (void* ptr, size_t size, CC_DynamicPool* pool);
void         cc_dynamic_pool_free      (void* ptr, CC_DynamicPool* pool);
size_t       cc_dynamic_pool_struct_size();

#ifdef __cplusplus
}
#endif /*_cplusplus_*/

#endif /*_CC_DYNAMIC_POOL_H_*/