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

#ifndef STACK_H_
#define STACK_H_

#include "common.h"
#include "array.h"

/**
 * A LIFO (last in first out) structure. Supports constant time time insertion, removal
 * and lookup.
 */
typedef struct stack_s Stack;

/**
 * Stack configuration object.
 */
typedef ArrayConf StackConf;

void          stack_conf_init   (StackConf *conf);
enum cc_stat  stack_new         (Stack **out);
enum cc_stat  stack_new_conf    (StackConf const * const conf, Stack **out);
void          stack_destroy     (Stack *stack);
void          stack_destroy_free(Stack *stack);

enum cc_stat  stack_push        (Stack *stack, void *element);
enum cc_stat  stack_peek        (Stack *stack, void **out);
enum cc_stat  stack_pop         (Stack *stack, void **out);

size_t        stack_size        (Stack *stack);
void          stack_map         (Stack *stack, void (*fn) (void *));

#endif /* STACK_H_ */
