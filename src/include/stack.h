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

#ifndef COLLECTIONS_C_STACK_H
#define COLLECTIONS_C_STACK_H

#include "common.h"
#include "array.h"

/**
 * A LIFO (last in first out) structure. Supports constant time
 * insertion, removal and lookup.
 */
typedef struct stack_s Stack;

/**
 * Stack configuration structure. Used to initialize a new Stack
 * with specific attributes.
 */
typedef ArrayConf StackConf;

/**
 * Stack iterator structure. Used to iterate over the elements of
 * the Stack in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during
 * iteration.
 */
typedef struct stack_iter_s {
    ArrayIter i;
} StackIter;

/**
 * Stack zip iterator structure. Used to iterate over the elements
 * of two Stacks in lockstep in an ascending order until one of the
 * Stacks is exhausted. The iterator also supports operations for
 * safely adding and removing elements during iteration.
 */
typedef struct stack_zip_iter_s {
    ArrayZipIter i;
} StackZipIter;


void          stack_conf_init       (StackConf *conf);
enum cc_stat  stack_new             (Stack **out);
enum cc_stat  stack_new_conf        (StackConf const * const conf, Stack **out);
void          stack_destroy         (Stack *stack);
void          stack_destroy_free    (Stack *stack);

enum cc_stat  stack_push            (Stack *stack, void *element);
enum cc_stat  stack_peek            (Stack *stack, void **out);
enum cc_stat  stack_pop             (Stack *stack, void **out);

size_t        stack_size            (Stack *stack);
void          stack_map             (Stack *stack, void (*fn) (void *));

void          stack_iter_init       (StackIter *iter, Stack *s);
enum cc_stat  stack_iter_next       (StackIter *iter, void **out);
enum cc_stat  stack_iter_replace    (StackIter *iter, void *element, void **out);

void          stack_zip_iter_init   (StackZipIter *iter, Stack *a1, Stack *a2);
enum cc_stat  stack_zip_iter_next   (StackZipIter *iter, void **out1, void **out2);
enum cc_stat  stack_zip_iter_replace(StackZipIter *iter, void *e1, void *e2, void **out1, void **out2);



#endif /* COLLECTIONS_C_STACK_H */
