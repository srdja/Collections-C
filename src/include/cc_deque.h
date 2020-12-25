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

#ifndef COLLECTIONS_C_CC_DEQUE_H
#define COLLECTIONS_C_CC_DEQUE_H

#include "cc_common.h"

/**
 * A dynamic array that supports amortized constant time insertion
 * and removal at both ends and constant time access.
 */
typedef struct cc_deque_s CC_Deque;

/**
 * CC_Deque configuration structure. Used to initialize a new CC_Deque
 * with specific values.
 */
typedef struct cc_deque_conf_s {
    /**
     * The initial capacity of the deque. Must be a power of two.
     * if a non power of two is passed, it will be rounded to the
     * closest upper power of two */
    size_t capacity;

    /**
     * Memory allocators used to allocate the Vector structure and the
     * underlying data buffers. */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
} CC_DequeConf;

/**
 * CC_Deque iterator object. Used to iterate over the elements of
 * a deque in an ascending order. The iterator also supports
 * operations for safely  removing and adding elements during
 * iteration.
 */
typedef struct cc_deque_iter_s {
    /**
     * The deque associated with this iterator.*/
    CC_Deque *deque;

    /**
     * The current logical position of the iterator. */
    size_t index;

    bool last_removed;
} CC_DequeIter;

/**
 * CC_Deque zip iterator structure. Used to iterate over two CC_Deques in
 * lockstep in an ascending order until one of the deques is exhausted.
 * The iterator also supports operations for safely adding and removing
 * elements during iteration.
 */
typedef struct cc_deque_zip_iter_s {
    CC_Deque *d1;
    CC_Deque *d2;
    size_t index;
    bool last_removed;
} CC_DequeZipIter;


enum cc_stat  cc_deque_new             (CC_Deque **deque);
enum cc_stat  cc_deque_new_conf        (CC_DequeConf const * const conf, CC_Deque **deque);
void          cc_deque_conf_init       (CC_DequeConf *conf);

void          cc_deque_destroy         (CC_Deque *deque);
void          cc_deque_destroy_cb      (CC_Deque *deque, void (*cb) (void*));

enum cc_stat  cc_deque_add             (CC_Deque *deque, void *element);
enum cc_stat  cc_deque_add_first       (CC_Deque *deque, void *element);
enum cc_stat  cc_deque_add_last        (CC_Deque *deque, void *element);
enum cc_stat  cc_deque_add_at          (CC_Deque *deque, void *element, size_t index);
enum cc_stat  cc_deque_replace_at      (CC_Deque *deque, void *element, size_t index, void **out);

enum cc_stat  cc_deque_remove          (CC_Deque *deque, void *element, void **out);
enum cc_stat  cc_deque_remove_at       (CC_Deque *deque, size_t index, void **out);
enum cc_stat  cc_deque_remove_first    (CC_Deque *deque, void **out);
enum cc_stat  cc_deque_remove_last     (CC_Deque *deque, void **out);
void          cc_deque_remove_all      (CC_Deque *deque);
void          cc_deque_remove_all_cb   (CC_Deque *deque, void (*cb) (void*));

enum cc_stat  cc_deque_get_at          (CC_Deque const * const deque, size_t index, void **out);
enum cc_stat  cc_deque_get_first       (CC_Deque const * const deque, void **out);
enum cc_stat  cc_deque_get_last        (CC_Deque const * const deque, void **out);

enum cc_stat  cc_deque_copy_shallow    (CC_Deque const * const deque, CC_Deque **out);
enum cc_stat  cc_deque_copy_deep       (CC_Deque const * const deque, void *(*cp) (void*), CC_Deque **out);

void          cc_deque_reverse         (CC_Deque *deque);
enum cc_stat  cc_deque_trim_capacity   (CC_Deque *deque);

size_t        cc_deque_contains        (CC_Deque const * const deque, const void *element);
size_t        cc_deque_contains_value  (CC_Deque const * const deque, const void *element, int (*cmp)(const void*, const void*));
size_t        cc_deque_size            (CC_Deque const * const deque);
size_t        cc_deque_capacity        (CC_Deque const * const deque);

enum cc_stat  cc_deque_index_of        (CC_Deque const * const deque, const void *element, size_t *i);

void          cc_deque_foreach         (CC_Deque *deque, void (*fn) (void *));

enum cc_stat  cc_deque_filter_mut      (CC_Deque *deque, bool (*predicate) (const void*));
enum cc_stat  cc_deque_filter          (CC_Deque *deque, bool (*predicate) (const void*), CC_Deque **out);

void          cc_deque_iter_init       (CC_DequeIter *iter, CC_Deque *deque);
enum cc_stat  cc_deque_iter_next       (CC_DequeIter *iter, void **out);
enum cc_stat  cc_deque_iter_remove     (CC_DequeIter *iter, void **out);
enum cc_stat  cc_deque_iter_add        (CC_DequeIter *iter, void *element);
enum cc_stat  cc_deque_iter_replace    (CC_DequeIter *iter, void *replacement, void **out);
size_t        cc_deque_iter_index      (CC_DequeIter *iter);

void          cc_deque_zip_iter_init   (CC_DequeZipIter *iter, CC_Deque *d1, CC_Deque *d2);
enum cc_stat  cc_deque_zip_iter_next   (CC_DequeZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_deque_zip_iter_add    (CC_DequeZipIter *iter, void *e1, void *e2);
enum cc_stat  cc_deque_zip_iter_remove (CC_DequeZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_deque_zip_iter_replace(CC_DequeZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        cc_deque_zip_iter_index  (CC_DequeZipIter *iter);

const void* const* cc_deque_get_buffer (CC_Deque const * const deque);


#define CC_DEQUE_FOREACH(val, deque, body)                              \
    {                                                                   \
        CC_DequeIter cc_deque_iter_53d46d2a04458e7b;                    \
        cc_deque_iter_init(&cc_deque_iter_53d46d2a04458e7b, deque);     \
        void *val;                                                      \
        while (cc_deque_iter_next(&cc_deque_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_DEQUE_FOREACH_ZIP(val1, val2, deque1, deque2, body)          \
    {                                                                   \
        CC_DequeZipIter cc_deque_zip_iter_ea08d3e52f25883b3;            \
        cc_deque_zip_iter_init(&cc_deque_zip_iter_ea08d3e52f25883b, deque1, deque2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_deque_zip_iter_next(&cc_deque_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }


#endif /* COLLECTIONS_C_CC_DEQUE_H */
