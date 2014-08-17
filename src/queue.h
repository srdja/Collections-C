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

#ifndef QUEUE_H_
#define QUEUE_H_

#include "dlist.h"
#include <stdbool.h>

typedef List Queue;
typedef ListIter QueueIter;

Queue *queue_new();
bool queue_destroy(Queue *queue);
bool queue_destroy_free(Queue *queue);

void *queue_peek(Queue *queue);
void *queue_poll(Queue *queue);
bool queue_enqueue(Queue *queue, void *element);

#endif /* QUEUE_H_ */
