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

#ifndef DEQUE_H_
#define DEQUE_H_

#include "common.h"

typedef struct deque_s      Deque;
typedef struct deque_iter_s DequeIter;

Deque*     deque_new             ();
Deque*     deque_new_capacity    (size_t capacity);

void       deque_destroy         (Deque *deque);
void       deque_destroy_free    (Deque *deque);

bool       deque_add             (Deque *deque, void *element);
bool       deque_add_first       (Deque *deque, void *element);
bool       deque_add_last        (Deque *deque, void *element);

void*      deque_remove_first    (Deque *deque);
void*      deque_remove_last     (Deque *deque);
void       deque_remove_all      (Deque *deque);
void       deque_remove_all_free (Deque *deque);

void*      deque_get             (Deque *deque, size_t index); // XXX
void*      deque_get_first       (Deque *deque);
void*      deque_get_last        (Deque *deque);

Deque*     deque_copy_shallow    (Deque *deque);
Deque*     deque_copy_deep       (Deque *deque, void *(*cp) (void*));

void       deque_trim_capacity   (Deque *deque);
size_t     deque_contains        (Deque *deque, void *element);
size_t     deque_size            (Deque *deque);
size_t     deque_capacity        (Deque *deque);

void       deque_foreach         (Deque *deque, void (*op) (void *));

DequeIter* deque_iter_new        (Deque *deque);
void       deque_iter_destroy    (DequeIter *iter);
bool       deque_iter_has_next   (DequeIter *iter);
void*      deque_iter_next       (DequeIter *iter);
void*      deque_iter_remove     (DequeIter *iter);
void*      deque_iter_add        (DequeIter *iter, void *element);
void*      deque_iter_replace    (DequeIter *iter, void *replacement);

#endif /* DEQUE_H_ */
