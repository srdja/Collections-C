/*
 * Collections-C
 * Copyright (C) 2013-2014 Srđan Panić <srdja.panic@gmail.com>
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

#include "array.h"
#include "pqueue.h"

struct pqueue_s {
    Array *v;

    /*
     * Memory management function pointers
     */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);

    /*
     * Comparator function pointer, for compairing the elements of PQueue
     */
    int   (*comparator) (const void *a, const void *b);
};

static int cc_default_comparator(const void *a, const void *b);


/*
 * Initializes the fields of PQueueConf to default values
 * @param[in, out] conf PQueueConf structure that is being initialized
 */


void pqueue_conf_init (PQueueConf *conf)
{
    array_conf_init(&conf->cfg);
    conf->comparator = cc_default_comparator;
}

/**
 * Creates a new empty pqueue and returns a status code.
 *
 * @param[out] out pointer to where the newly created PQueue is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new PQueue structure failed.
 */

enum cc_stat pqueue_new (PQueue **out)
{
    PQueueConf conf;
    pqueue_conf_init(&conf);
    return pqueue_new_conf(&conf, out);
}

/**
 * Creates a new empty PQueue based on the PQueueConf struct and returns a
 * status code.
 *
 * The priority queue is allocated using the allocators specified in the PQueueConf
 * struct. The allocation may fail if the underlying allocator fails. It may also
 * fail if the values of exp_factor and capacity in the ArrayConf
 * structure of the PQueueConf do not meet the following condition:
 * <code>exp_factor < (CC_MAX_ELEMENTS / capacity)</code>.
 *
 * @param[in] conf priority queue configuration structure
 * @param[out] out pointer to where the newly created PQueue is to be stored
 *
 * @return CC_OK if the creation was successful, CC_ERR_INVALID_CAPACITY if
 * the above mentioned condition is not met, or CC_ERR_ALLOC if the memory
 * allocation for the new PQueue structure failed.
 */

enum cc_stat pqueue_new_conf(PQueueConf const * const conf, PQueue **out)
{
    PQueue *pQ = conf->cfg.mem_calloc(1, sizeof(PQueue));
    if (!pQ)
        return CC_ERR_ALLOC;

    pQ->mem_alloc = conf->cfg.mem_alloc;
    pQ->mem_calloc = conf->cfg.mem_calloc;
    pQ->mem_free = conf->cfg.mem_free;

    pQ->comparator = conf->comparator;

    Array *array;
    enum cc_stat status;

    if ((status = array_new_conf(&conf->cfg, &array)) == CC_OK) {
        pQ->v = array;
    } else {
        conf->cfg.mem_free(pQ);
        return status;
    }

    *out = pQ;


    return CC_OK;
}

/**
 * Destroys the specified PQueue structure, while leaving the data it holds
 * intact.
 *
 * @param[in] pqueue the PQueue to be destroyed
 */

void pqueue_destroy(PQueue *pqueue)
{
    array_destroy(pqueue->v);
    pqueue->mem_free(pqueue);
}

/**
 * Destroys the specified priority queue structure along with all the data it holds.
 *
 * @note This function should not be called on a PQueue that has some of its
 * elements allocated on the Stack (stack memory of function calls).
 *
 * @param[in] pqueue the Priority Queue to be destroyed
 */

void pqueue_destroy_free(PQueue *pqueue)
{
    array_destroy_free(pqueue->v);
    pqueue->mem_free(pqueue);
}


/*
 * A default comparator function for the ints.
 * @param[in] two pointers a and b which are needed to be compared, they are
 *     stored in the PQueue structure
 * @return positive value if value at a is greater than at b, 0 if value at a
 *     is equal to at of b, negative value if a is smaller than b.
 */

static int cc_default_comparator(const void *a, const void *b)
{
    return *((int *) a) - *((int *) b);
}
