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

#include "queue.h"

struct queue_s {
    Deque *d;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

/**
 * Initializes the fields of the QueueConf struct to default values.
 *
 * @param[in, out] conf the configuration struct that is being initialized
 */
void queue_conf_init(QueueConf *conf)
{
    deque_conf_init(conf);
}

/**
 * Returns a new empty queue, or NULL if the allocation fails.
 *
 * @return a new queue if the allocation was successful, or NULL.
 */
enum cc_stat queue_new(Queue **queue)
{
    QueueConf conf;
    queue_conf_init(&conf);
    return queue_new_conf(&conf, queue);
}

/**
 * Returns a new empty queue based on the specified QueueConf object.
 * The queue is allocated using the allocatord specified by the Queue
 * conf object. The allocation may fail if the underlying allocator
 * fails.
 *
 * @param[in] conf Queue configuration object. All firlds must be initialized.
 *
 * @return a new queue if the allocation was successful, or NULL.
 */
enum cc_stat queue_new_conf(QueueConf const * const conf, Queue **q)
{
    Queue *queue = conf->mem_calloc(1, sizeof(Queue));

    if (!queue)
        return CC_ERR_ALLOC;

    Deque *deque;
    deque_new_conf(conf, &deque);

    if (!deque) {
        conf->mem_free(queue);
        return CC_ERR_ALLOC;
    }

    queue->d          = deque;
    queue->mem_alloc  = conf->mem_alloc;
    queue->mem_calloc = conf->mem_calloc;
    queue->mem_free   = conf->mem_free;

    *q = queue;

    return CC_OK;
}

/**
 * Destroys the queue structure, but leaves the data it used to hold intact.
 *
 * @param[in] queue the queue that is to be destroyed
 */
void queue_destroy(Queue *queue)
{
    deque_destroy(queue->d);
    queue->mem_free(queue);
}

/**
 * Destroys the queue structure along with all the data it holds.
 *
 * @note
 * This function should not be called on a queue that has some of its elements
 * allocated on the stack.
 *
 * @param[in] queue the queue that is to be destroyed.
 */
void queue_destroy_free(Queue *queue)
{
    deque_destroy_free(queue->d);
    free(queue);
}

/**
 * Returns the element at the front of the queue.
 *
 * @param[in] queue the queue whose element is being returned
 *
 * @return the element at the front of the queue
 */
enum cc_stat queue_peek(Queue *queue, void **out)
{
    return deque_get_last(queue->d, out);
}

/**
 * Returns and removes the element at the front of the queue.
 *
 * @param[in] queue the queue on which this operation is performed
 *
 * @return the element that was at the front of the queue
 */
enum cc_stat queue_poll(Queue *queue, void **out)
{
    return deque_remove_last(queue->d, out);
}

/**
 * Appends an element to the back of the queue. This operation may
 * fail if the memory allocation for the new elmenet fails.
 *
 * @param[in] queue the queue on which this operation is performed
 * @param[in] element the element being enqueued
 *
 * @return true if the operation was successful
 */
enum cc_stat queue_enqueue(Queue *queue, void *element)
{
    return deque_add_first(queue->d, element);
}

/**
 * Returns the size of the specified queue. The size of the queue is
 * the number of elements contained within the queue.
 *
 * @param[in] queue the queue whose size is being returned
 *
 * @return the number of elements within the queue
 */
size_t queue_size(Queue *queue)
{
    return deque_size(queue->d);
}

/**
 * A 'foreach loop' function that invokes the specified function on each element
 * in the queue.
 *
 * @param[in] queue the queue on which this operation is performed
 * @param[in] op the operation function thatis to be invoked on each queue element
 */
void queue_foreach(Queue *queue, void (*op) (void*))
{
    deque_foreach(queue->d, op);
}

/**
 * Initializs the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] queue the queue to iterate over
 */
void queue_iter_init(QueueIter *iter, Queue *queue)
{
    deque_iter_init(&(iter->i), queue->d);
}

/**
 * Retruns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
enum cc_stat queue_iter_next(QueueIter *iter, void **out)
{
    return deque_iter_next(&(iter->i), out);
}

/**
 * Replaces the last returned element by the specified iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] replacement the replacement element
 *
 * @return the old element that was replaced
 */
enum cc_stat queue_iter_replace(QueueIter *iter, void *replacement, void **out)
{
    return deque_iter_replace(&(iter->i), replacement, out);
}
