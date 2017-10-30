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

#ifndef COLLECTIONS_C_DEQUE_H
#define COLLECTIONS_C_DEQUE_H

#include "common.h"

/**
 * A dynamic array that supports amortized constant time insertion
 * and removal at both ends and constant time access.
 */
typedef struct deque_s Deque;

/**
 * Deque configuration structure. Used to initialize a new Deque
 * with specific values.
 */
typedef struct deque_conf_s {
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
} DequeConf;

/**
 * Deque iterator object. Used to iterate over the elements of
 * a deque in an ascending order. The iterator also supports
 * operations for safely  removing and adding elements during
 * iteration.
 */
typedef struct deque_iter_s {
    /**
     * The deque associated with this iterator.*/
    Deque *deque;

    /**
     * The current logical position of the iterator. */
    size_t index;

    bool last_removed;
} DequeIter;

/**
 * Deque zip iterator structure. Used to iterate over two Deques in
 * lockstep in an ascending order until one of the deques is exhausted.
 * The iterator also supports operations for safely adding and removing
 * elements during iteration.
 */
typedef struct deque_zip_iter_s {
    Deque *d1;
    Deque *d2;
    size_t index;
    bool last_removed;
} DequeZipIter;


enum cc_stat  deque_new             (Deque **deque);
enum cc_stat  deque_new_conf        (DequeConf const * const conf, Deque **deque);
void          deque_conf_init       (DequeConf *conf);

void          deque_destroy         (Deque *deque);
void          deque_destroy_cb      (Deque *deque, void (*cb) (void*));

enum cc_stat  deque_add             (Deque *deque, void *element);
enum cc_stat  deque_add_first       (Deque *deque, void *element);
enum cc_stat  deque_add_last        (Deque *deque, void *element);
enum cc_stat  deque_add_at          (Deque *deque, void *element, size_t index);
enum cc_stat  deque_replace_at      (Deque *deque, void *element, size_t index, void **out);

enum cc_stat  deque_remove          (Deque *deque, void *element, void **out);
enum cc_stat  deque_remove_at       (Deque *deque, size_t index, void **out);
enum cc_stat  deque_remove_first    (Deque *deque, void **out);
enum cc_stat  deque_remove_last     (Deque *deque, void **out);
void          deque_remove_all      (Deque *deque);
void          deque_remove_all_cb   (Deque *deque, void (*cb) (void*));

enum cc_stat  deque_get_at          (Deque const * const deque, size_t index, void **out);
enum cc_stat  deque_get_first       (Deque const * const deque, void **out);
enum cc_stat  deque_get_last        (Deque const * const deque, void **out);

enum cc_stat  deque_copy_shallow    (Deque const * const deque, Deque **out);
enum cc_stat  deque_copy_deep       (Deque const * const deque, void *(*cp) (void*), Deque **out);

void          deque_reverse         (Deque *deque);
enum cc_stat  deque_trim_capacity   (Deque *deque);

size_t        deque_contains        (Deque const * const deque, const void *element);
size_t        deque_contains_value  (Deque const * const deque, const void *element, int (*cmp)(const void*, const void*));
size_t        deque_size            (Deque const * const deque);
size_t        deque_capacity        (Deque const * const deque);

enum cc_stat  deque_index_of        (Deque const * const deque, const void *element, size_t *i);

void          deque_foreach         (Deque *deque, void (*fn) (void *));

enum cc_stat  deque_filter_mut      (Deque *deque, bool (*predicate) (const void*));
enum cc_stat  deque_filter          (Deque *deque, bool (*predicate) (const void*), Deque **out);

void          deque_iter_init       (DequeIter *iter, Deque *deque);
enum cc_stat  deque_iter_next       (DequeIter *iter, void **out);
enum cc_stat  deque_iter_remove     (DequeIter *iter, void **out);
enum cc_stat  deque_iter_add        (DequeIter *iter, void *element);
enum cc_stat  deque_iter_replace    (DequeIter *iter, void *replacement, void **out);
size_t        deque_iter_index      (DequeIter *iter);

void          deque_zip_iter_init   (DequeZipIter *iter, Deque *d1, Deque *d2);
enum cc_stat  deque_zip_iter_next   (DequeZipIter *iter, void **out1, void **out2);
enum cc_stat  deque_zip_iter_add    (DequeZipIter *iter, void *e1, void *e2);
enum cc_stat  deque_zip_iter_remove (DequeZipIter *iter, void **out1, void **out2);
enum cc_stat  deque_zip_iter_replace(DequeZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        deque_zip_iter_index  (DequeZipIter *iter);

const void* const* deque_get_buffer (Deque const * const deque);


#define DEQUE_FOREACH(val, deque, body)                                 \
    {                                                                   \
        DequeIter deque_iter_53d46d2a04458e7b;                          \
        deque_iter_init(&deque_iter_53d46d2a04458e7b, deque);           \
        void *val;                                                      \
        while (deque_iter_next(&deque_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define DEQUE_FOREACH_ZIP(val1, val2, deque1, deque2, body)             \
    {                                                                   \
        DequeZipIter deque_zip_iter_ea08d3e52f25883b3;                  \
        deque_zip_iter_init(&deque_zip_iter_ea08d3e52f25883b, deque1, deque2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (deque_zip_iter_next(&deque_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }


#endif /* COLLECTIONS_C_DEQUE_H */
