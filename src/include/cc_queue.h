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

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_common.h"
#include "cc_deque.h"

/**
 * A FIFO (first in first out) structure. Supports constant time
 * insertion, removal and lookup.
 */
typedef struct cc_queue_s CC_Queue;

/**
 * CC_Queue configuration object.
 */
typedef CC_DequeConf CC_QueueConf;

/**
 * CC_Queue iterator object. Used to iterate over the elements of a
 * queue in an ascending order.
 */
typedef struct cc_queue_iter_s {
    CC_DequeIter i;
} CC_QueueIter;

/**
 * CC_Queue zip iterator structure. Used to iterate over the elements of two
 * queues in lockstep in an ascending order until one of the queues is
 * exhausted. The iterator also supports operations for safely adding
 * and removing elements during iteration.
 */
typedef struct queue_zip_iter_s {
    CC_DequeZipIter i;
} QueueZipIter;


void         cc_queue_conf_init       (CC_QueueConf *conf);
enum cc_stat cc_queue_new             (CC_Queue **q);
enum cc_stat cc_queue_new_conf        (CC_QueueConf const * const conf, CC_Queue **q);
void         cc_queue_destroy         (CC_Queue *queue);
void         cc_queue_destroy_cb      (CC_Queue *queue, void (*cb) (void*));
size_t       cc_queue_struct_size     ();

enum cc_stat cc_queue_peek            (CC_Queue const * const queue, void **out);
enum cc_stat cc_queue_poll            (CC_Queue *queue, void **out);
enum cc_stat cc_queue_enqueue         (CC_Queue *queue, void *element);

size_t       cc_queue_size            (CC_Queue const * const queue);
void         cc_queue_foreach         (CC_Queue *queue, void (*op) (void*));

void         cc_queue_iter_init       (CC_QueueIter *iter, CC_Queue *queue);
enum cc_stat cc_queue_iter_next       (CC_QueueIter *iter, void **out);
enum cc_stat cc_queue_iter_replace    (CC_QueueIter *iter, void *replacement, void **out);

void         cc_queue_zip_iter_init   (QueueZipIter *iter, CC_Queue *q1, CC_Queue *q2);
enum cc_stat cc_queue_zip_iter_next   (QueueZipIter *iter, void **out1, void **out2);
enum cc_stat cc_queue_zip_iter_replace(QueueZipIter *iter, void *e1, void *e2, void **out1, void **out2);


#define CC_QUEUE_FOREACH(val, queue, body)                                 \
    {                                                                   \
        CC_QueueIter queue_iter_53d46d2a04458e7b;                          \
        cc_queue_iter_init(&queue_iter_53d46d2a04458e7b, queue);           \
        void *val;                                                      \
        while (cc_queue_iter_next(&queue_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_QUEUE_FOREACH_ZIP(val1, val2, queue1, queue2, body)             \
    {                                                                   \
        QueueZipIter queue_zip_iter_ea08d3e52f25883b3;                  \
        cc_queue_zip_iter_init(&queue_zip_iter_ea08d3e52f25883b, queue1, queue2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_queue_zip_iter_next(&queue_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#ifdef __cplusplus
}
#endif

#endif /* COLLECTIONS_C_QUEUE_H */
