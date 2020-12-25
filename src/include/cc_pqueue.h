/*
 * Collections-C
 * Copyright (C) 2013-2015 Srđan Panić <srdja.panic@gmail.com>
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

#ifndef COLLECTIONS_C_CC_PQUEUE_H
#define COLLECTIONS_C_CC_PQUEUE_H

#include "cc_common.h"
#include "cc_array.h"

/**
 * A Priority Queue data structure. Stores the elements and according to
 * some property associated with the element stored in the pqueue, the elements
 * can be retrieved in a specific order
 **/

typedef struct cc_pqueue_s CC_PQueue;

/**
 * The pqueue initialization configuration structure. Used to initialize the
 * CC_PQueue with the specified attributes
 **/

typedef struct cc_pqueue_conf_s {
    /**
     * The initial capacity of the array */
    size_t capacity;

    /**
     * The rate at which the buffer expands (capacity * exp_factor). */
    float  exp_factor;

    /**
     * comparator, used to hold the address of the function which will
     * be used to compare the elements of the CC_PQueue
     */
    int (*cmp) (const void *a, const void *b);

    /**
     * Memory allocators used to allocate the Array structure and the
     * underlying data buffers. */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
} CC_PQueueConf;

void          cc_pqueue_conf_init       (CC_PQueueConf *conf, int (*)(const void *, const void *));
enum cc_stat  cc_pqueue_new             (CC_PQueue **out, int (*)(const void *, const void *));
enum cc_stat  cc_pqueue_new_conf        (CC_PQueueConf const * const conf, CC_PQueue **out);
void          cc_pqueue_destroy         (CC_PQueue *pqueue);
void          cc_pqueue_destroy_cb      (CC_PQueue *pqueue, void (*cb) (void*));

enum cc_stat  cc_pqueue_push            (CC_PQueue *pqueue, void *element);
enum cc_stat  cc_pqueue_top             (CC_PQueue *pqueue, void **out);
enum cc_stat  cc_pqueue_pop             (CC_PQueue *pqueue, void **out);


#endif
