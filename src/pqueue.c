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

#include "pqueue.h"


#define CC_PARENT(x) (x - 1) / 2
#define CC_LEFT(x)    2 * x + 1
#define CC_RIGHT(x)   2 * x + 2


#define DEFAULT_CAPACITY 8
#define DEFAULT_EXPANSION_FACTOR 2


struct pqueue_s {
    size_t   size;
    size_t   capacity;
    float    exp_factor;
    void   **buffer;

    /* Memory management function pointers */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);

    /*  Comparator function pointer, for compairing the elements of PQueue */
    int   (*cmp) (const void *a, const void *b);
};


static void pqueue_heapify(PQueue *pqueue, size_t index);


/**
 * Initializes the fields of PQueueConf to default values
 *
 * @param[in, out] conf PQueueConf structure that is being initialized
 * @param[in] comp The comparator function required for PQueue
 */
void pqueue_conf_init(PQueueConf *conf, int (*cmp)(const void *, const void *))
{
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
    conf->cmp        = cmp;
    conf->exp_factor = DEFAULT_EXPANSION_FACTOR;
    conf->capacity   = DEFAULT_CAPACITY;
}


/**
 * Creates a new empty pqueue and returns a status code.
 *
 * @param[out] out pointer to where the newly created PQueue is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new PQueue structure failed.
 */
enum cc_stat pqueue_new(PQueue **out, int (*cmp)(const void*, const void*))
{
    PQueueConf conf;
    pqueue_conf_init(&conf, cmp);
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
    float ex;

    /* The expansion factor must be greater than one for the
     * array to grow */
    if (conf->exp_factor <= 1)
        ex = DEFAULT_EXPANSION_FACTOR;
    else
        ex = conf->exp_factor;

    /* Needed to avoid an integer overflow on the first resize and
     * to easily check for any future overflows. */
    if (!conf->capacity || ex >= CC_MAX_ELEMENTS / conf->capacity)
        return CC_ERR_INVALID_CAPACITY;

    PQueue *pq = conf->mem_calloc(1, sizeof(PQueue));

    if (!pq)
        return CC_ERR_ALLOC;

    void **buff = conf->mem_alloc(conf->capacity * sizeof(void*));

    if (!buff) {
        conf->mem_free(pq);
        return CC_ERR_ALLOC;
    }

    pq->mem_alloc  = conf->mem_alloc;
    pq->mem_calloc = conf->mem_calloc;
    pq->mem_free   = conf->mem_free;
    pq->cmp        = conf->cmp;
    pq->buffer     = buff;
    pq->exp_factor = ex;
    pq->capacity   = conf->capacity;

    *out = pq;
    return CC_OK;
}

/**
 * Destroys the specified PQueue structure, while leaving the data it holds
 * intact.
 *
 * @param[in] pq the PQueue to be destroyed
 */
void pqueue_destroy(PQueue *pq)
{
    pq->mem_free(pq->buffer);
    pq->mem_free(pq);
}

/**
 * Destroys the specified priority queue structure along with all the data it holds.
 *
 * @note This function should not be called on a PQueue that has some of its
 * elements allocated on the Stack (stack memory of function calls).
 *
 * @param[in] pq the Priority Queue to be destroyed
 */
void pqueue_destroy_cb(PQueue *pq, void (*cb) (void*))
{
    size_t i;
    for (i = 0; i < pq->size; i++)
        cb(pq->buffer[i]);

    pqueue_destroy(pq);
}

/**
 * Expands the Pqueue capacity. This might fail if the the new buffer
 * cannot be allocated. In case the expansion would overflow the index
 * range, a maximum capacity buffer is allocated instead. If the capacity
 * is already at the maximum capacity, no new buffer is allocated.
 *
 * @param[in] pq pqueue whose capacity is being expanded
 *
 * @return CC_OK if the buffer was expanded successfully, CC_ERR_ALLOC if
 * the memory allocation for the new buffer failed, or CC_ERR_MAX_CAPACITY
 * if the pqueue is already at maximum capacity.
 */
static enum cc_stat expand_capacity(PQueue *pq)
{
    if (pq->capacity == CC_MAX_ELEMENTS)
        return CC_ERR_MAX_CAPACITY;

    size_t new_capacity = pq->capacity * pq->exp_factor;

    /* As long as the capacity is greater that the expansion factor
     * at the point of overflow, this is check is valid. */
    if (new_capacity <= pq->capacity)
        pq->capacity = CC_MAX_ELEMENTS;
    else
        pq->capacity = new_capacity;

    void **new_buff = pq->mem_alloc(new_capacity * sizeof(void*));

    if (!new_buff)
        return CC_ERR_ALLOC;

    memcpy(new_buff, pq->buffer, pq->size * sizeof(void*));

    pq->mem_free(pq->buffer);
    pq->buffer = new_buff;

    return CC_OK;
}

/**
 * Pushes the element in the pqueue
 *
 * @param[in] pq the priority queue in which the element is to be pushed
 * @param[in] element the element which is needed to be pushed
 *
 * @return CC_OK if the element was successfully pushed, or CC_ERR_ALLOC
 * if the memory allocation for the new element failed.
 */
enum cc_stat pqueue_push(PQueue *pq, void *element)
{
    size_t i = pq->size;

    if (i >= pq->capacity) {
        enum cc_stat status = expand_capacity(pq);
        if (status != CC_OK)
            return status;
    }

    pq->buffer[i] = element;
    pq->size++;

    if (i == 0)
        return CC_OK;

    void *child  = pq->buffer[i];
    void *parent = pq->buffer[CC_PARENT(i)];

    while (i != 0 && pq->cmp(child, parent) > 0) {
        void *tmp = pq->buffer[i];
        pq->buffer[i] = pq->buffer[CC_PARENT(i)];
        pq->buffer[CC_PARENT(i)] = tmp;

        i      = CC_PARENT(i);
        child  = pq->buffer[i];
        parent = pq->buffer[CC_PARENT(i)];
    }
    return CC_OK;
}

/**
 * Gets the most prioritized element from the queue without popping it
 * @param[in] pqueue the PQueue structure of which the top element is needed
 * @param[out] out pointer where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if the
 * PQueue is empty.
 */
enum cc_stat pqueue_top(PQueue *pq, void **out)
{
    if (pq->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    *out = pq->buffer[0];
    return CC_OK;
}

/**
 * Removes the most prioritized element from the PQueue
 * @param[in] pq the PQueue structure whose element is needed to be popped
 * @param[out] out the pointer where the removed element will be stored
 *
 * return CC_OK if the element was popped successfully, or CC_ERR_OUT_OF_RANGE
 * if pqueue was empty
 */
enum cc_stat pqueue_pop(PQueue *pq, void **out)
{
    if (pq->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    void *tmp = pq->buffer[0];
    pq->buffer[0] = pq->buffer[pq->size - 1];
    pq->buffer[pq->size - 1] = tmp;

    tmp = pq->buffer[pq->size - 1];
    pq->size--;

    pqueue_heapify(pq, 0);

    if (out)
        *out = tmp;

    return CC_OK;
}

/**
 * Maintains the heap property of the PQueue
 *
 * @param[in] pq the PQueue structure whose heap property is to be maintained
 * @param[in] index the index from where we need to apply this operation
 */
static void pqueue_heapify(PQueue *pq, size_t index)
{
    if (pq->size <= 1)
        return;

    size_t L   = CC_LEFT(index);
    size_t R   = CC_RIGHT(index);
    size_t tmp = index;

    void *left     = pq->buffer[L];
    void *right    = pq->buffer[R];
    void *indexPtr = pq->buffer[index];

    if (L >= pq->size || R >= pq->size)
        return;

    if (pq->cmp(indexPtr, left) < 0) {
        indexPtr = left;
        index = L;
    }

    if (pq->cmp(indexPtr, right) < 0) {
        indexPtr = right;
        index = R;
    }

    if (index != tmp) {
        void *swap_tmp = pq->buffer[tmp];
        pq->buffer[tmp] = pq->buffer[index];
        pq->buffer[index] = swap_tmp;

        pqueue_heapify(pq, index);
    }
}
