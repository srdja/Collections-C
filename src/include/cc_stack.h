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

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_common.h"
#include "cc_array.h"

/**
 * A LIFO (last in first out) structure. Supports constant time
 * insertion, removal and lookup.
 */
typedef struct cc_stack_s CC_Stack;

/**
 * CC_Stack configuration structure. Used to initialize a new CC_Stack
 * with specific attributes.
 */
typedef CC_ArrayConf CC_StackConf;

/**
 * CC_Stack iterator structure. Used to iterate over the elements of
 * the CC_Stack in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during
 * iteration.
 */
typedef struct cc_stack_iter_s {
    CC_ArrayIter i;
} CC_StackIter;

/**
 * CC_Stack zip iterator structure. Used to iterate over the elements
 * of two Stacks in lockstep in an ascending order until one of the
 * Stacks is exhausted. The iterator also supports operations for
 * safely adding and removing elements during iteration.
 */
typedef struct cc_stack_zip_iter_s {
    CC_ArrayZipIter i;
} CC_StackZipIter;


void          cc_stack_conf_init       (CC_StackConf *conf);
enum cc_stat  cc_stack_new             (CC_Stack **out);
enum cc_stat  cc_stack_new_conf        (CC_StackConf const * const conf, CC_Stack **out);
void          cc_stack_destroy         (CC_Stack *stack);
void          cc_stack_destroy_cb      (CC_Stack *stack, void (*cb) (void*));
size_t        cc_stack_struct_size     ();

enum cc_stat  cc_stack_push            (CC_Stack *stack, void *element);
enum cc_stat  cc_stack_peek            (CC_Stack *stack, void **out);
enum cc_stat  cc_stack_pop             (CC_Stack *stack, void **out);

size_t        cc_stack_size            (CC_Stack *stack);
void          cc_stack_map             (CC_Stack *stack, void (*fn) (void *));
enum cc_stat  cc_stack_filter_mut      (CC_Stack *stack, bool (*predicate) (const void*));
enum cc_stat  cc_stack_filter          (CC_Stack *stack, bool (*predicate)(const void *), CC_Stack **out);

void          cc_stack_iter_init       (CC_StackIter *iter, CC_Stack *s);
enum cc_stat  cc_stack_iter_next       (CC_StackIter *iter, void **out);
enum cc_stat  cc_stack_iter_replace    (CC_StackIter *iter, void *element, void **out);

void          cc_stack_zip_iter_init   (CC_StackZipIter *iter, CC_Stack *a1, CC_Stack *a2);
enum cc_stat  cc_stack_zip_iter_next   (CC_StackZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_stack_zip_iter_replace(CC_StackZipIter *iter, void *e1, void *e2, void **out1, void **out2);


#define CC_STACK_FOREACH(val, stack, body)                                 \
    {                                                                   \
        CC_StackIter stack_iter_53d46d2a04458e7b;                          \
        cc_stack_iter_init(&stack_iter_53d46d2a04458e7b, stack);           \
        void *val;                                                      \
        while (cc_stack_iter_next(&stack_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_STACK_FOREACH_ZIP(val1, val2, stack1, stack2, body)             \
    {                                                                   \
        CC_StackZipIter stack_zip_iter_ea08d3e52f25883b3;                  \
        cc_stack_zip_iter_init(&stack_zip_iter_ea08d3e52f25883b, stack1, stack2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_stack_zip_iter_next(&stack_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#ifdef __cplusplus
}
#endif

#endif /* COLLECTIONS_C_STACK_H */
