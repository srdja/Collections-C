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

typedef struct queue_s   Queue;
typedef        DequeConf QueueConf;

typedef struct queue_iter_s {
    DequeIter *i;
} QueueIter;

void   queue_conf_init    (QueueConf *conf);

Queue *queue_new          ();
Queue *queue_new_conf     (QueueConf *conf);
void   queue_destroy      (Queue *queue);
void   queue_destroy_free (Queue *queue);

void  *queue_peek         (Queue *queue);
void  *queue_poll         (Queue *queue);
bool   queue_enqueue      (Queue *queue, void *element);

size_t queue_size         (Queue *queue);
void   queue_foreach      (Queue *queue, void (*op) (void*));

void   queue_iter_init    (QueueIter *iter, Queue *queue);
bool   queue_iter_has_next(QueueIter *iter);
void*  queue_iter_next    (QueueIter *iter);
void*  queue_iter_remove  (QueueIter *iter);
bool   queue_iter_add     (QueueIter *iter, void *element);
void*  queue_iter_replace (QueueIter *iter, void *replacement);

#endif /* QUEUE_H_ */
