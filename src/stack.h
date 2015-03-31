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

typedef struct stack_s Stack;

Stack  *stack_new     ();
void    stack_destroy (Stack *stack);

bool    stack_push    (Stack *stack, void *element);
void   *stack_peek    (Stack *stack);
void   *stack_pop     (Stack *stack);

size_t  stack_size    (Stack *stack);

void    stack_foreach (Stack *stack, void (*op) (void *));

#endif /* STACK_H_ */
