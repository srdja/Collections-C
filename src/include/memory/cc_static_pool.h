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

#ifndef _CC_BLOCK_POOL_H_
#define _CC_BLOCK_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_common.h"

typedef struct cc_static_pool_s CC_StaticPool;

enum cc_stat cc_static_pool_new        (size_t size, size_t offset, uint8_t* buf, uint8_t* pool_alloc, CC_StaticPool** out);
size_t       cc_static_pool_struct_size();

void         cc_static_pool_reset      (CC_StaticPool* pool);
size_t       cc_static_pool_used_bytes (CC_StaticPool* pool);
size_t       cc_static_pool_free_bytes (CC_StaticPool* pool);

void*        cc_static_pool_malloc     (size_t size, CC_StaticPool *pool);
void*        cc_static_pool_calloc     (size_t count, size_t size, CC_StaticPool *pool);
void         cc_static_pool_free       (void* ptr, CC_StaticPool* pool);


#ifdef __cplusplus
}
#endif /*_cplusplus_*/

#endif /*_CC_BLOCK_POOL_H_*/