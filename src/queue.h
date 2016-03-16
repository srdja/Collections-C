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

#ifndef QUEUE_H_
#define QUEUE_H_

#include "common.h"
#include "deque.h"

/**
 * A FIFO (first in first out) structure. Supports constant time insertion, removal
 * and lookup.
 */
typedef struct queue_s Queue;

/**
 * Queue configuration object.
 */
typedef DequeConf QueueConf;

/**
 * Queue iterator object. Used to iterate over the elements of a queue
 * in an ascending order.
 *
 * @code
 * QueueIter i;
 * queue_iter_init(&i);
 *
 * while (queue_iter_has_next(&i)) {
 *     MyType *e = queue_iter_next(&i);
 * }
 * @endcode
 */
typedef struct queue_iter_s {
    DequeIter i;
} QueueIter;

void   queue_conf_init    (QueueConf *conf);

int    queue_new          (Queue **q);
int    queue_new_conf     (const QueueConf const* conf, Queue **q);
void   queue_destroy      (Queue *queue);
void   queue_destroy_free (Queue *queue);

int    queue_peek         (Queue *queue, void **out);
int    queue_poll         (Queue *queue, void **out);
int    queue_enqueue      (Queue *queue, void *element);

size_t queue_size         (Queue *queue);
void   queue_foreach      (Queue *queue, void (*op) (void*));

void   queue_iter_init    (QueueIter *iter, Queue *queue);
bool   queue_iter_has_next(QueueIter *iter);
void   queue_iter_next    (QueueIter *iter, void **out);
int    queue_iter_replace (QueueIter *iter, void *replacement, void **out);

#endif /* QUEUE_H_ */
