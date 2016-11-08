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

#ifndef COLLECTIONS_C_QUEUE_H
#define COLLECTIONS_C_QUEUE_H

#include "common.h"
#include "deque.h"

/**
 * A FIFO (first in first out) structure. Supports constant time
 * insertion, removal and lookup.
 */
typedef struct queue_s Queue;

/**
 * Queue configuration object.
 */
typedef DequeConf QueueConf;

/**
 * Queue iterator object. Used to iterate over the elements of a
 * queue in an ascending order.
 */
typedef struct queue_iter_s {
    DequeIter i;
} QueueIter;

/**
 * Queue zip iterator structure. Used to iterate over the elements of two
 * queues in lockstep in an ascending order until one of the queues is
 * exhausted. The iterator also supports operations for safely adding
 * and removing elements during iteration.
 */
typedef struct queue_zip_iter_s {
    DequeZipIter i;
} QueueZipIter;


void         queue_conf_init       (QueueConf *conf);
enum cc_stat queue_new             (Queue **q);
enum cc_stat queue_new_conf        (QueueConf const * const conf, Queue **q);
void         queue_destroy         (Queue *queue);
void         queue_destroy_free    (Queue *queue);

enum cc_stat queue_peek            (Queue const * const queue, void **out);
enum cc_stat queue_poll            (Queue *queue, void **out);
enum cc_stat queue_enqueue         (Queue *queue, void *element);

size_t       queue_size            (Queue const * const queue);
void         queue_foreach         (Queue *queue, void (*op) (void*));

void         queue_iter_init       (QueueIter *iter, Queue *queue);
enum cc_stat queue_iter_next       (QueueIter *iter, void **out);
enum cc_stat queue_iter_replace    (QueueIter *iter, void *replacement, void **out);

void         queue_zip_iter_init   (QueueZipIter *iter, Queue *q1, Queue *q2);
enum cc_stat queue_zip_iter_next   (QueueZipIter *iter, void **out1, void **out2);
enum cc_stat queue_zip_iter_replace(QueueZipIter *iter, void *e1, void *e2, void **out1, void **out2);

#endif /* COLLECTIONS_C_QUEUE_H */
