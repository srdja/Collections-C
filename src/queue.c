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
Queue *queue_new()
{
    QueueConf conf;
    queue_conf_init(&conf);
    return queue_new_conf(&conf);
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
Queue *queue_new_conf(QueueConf *conf)
{
    Queue *queue = conf->mem_calloc(1, sizeof(Queue));
    queue->d = deque_new_conf(conf);

    return queue;
}

/**
 * Destroys the queue structure, but leaves the data it used to hold intact.
 *
 * @param[in] queue the queue that is to be destroyed
 */
void queue_destroy(Queue *queue)
{
    deque_destroy(queue->d);
    free(queue);
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
void *queue_peek(Queue *queue)
{
    return deque_get_last(queue->d);
}

/**
 * Returns and removes the element at the front of the queue.
 *
 * @param[in] queue the queue on which this operation is performed
 *
 * @return the element that was at the front of the queue
 */
void *queue_poll(Queue *queue)
{
    return deque_remove_last(queue->d);
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
bool queue_enqueue(Queue *queue, void *element)
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
 * Checks whether or not the iterator has reached the end of the queue.
 *
 * @param[in] iter iterator whose position is being checked
 *
 * @return true if the are more elements to be iterated over
 */
bool queue_iter_has_next(QueueIter *iter)
{
    return deque_iter_has_next(&(iter->i));
}

/**
 * Retruns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
void* queue_iter_next(QueueIter *iter)
{
    return deque_iter_next(&(iter->i));
}

/**
 * Replaces the last returned element by the specified iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] replacement the replacement element
 *
 * @return the old element that was replaced
 */
void* queue_iter_replace(QueueIter *iter, void *replacement)
{
    return deque_iter_replace(&(iter->i), replacement);
}
