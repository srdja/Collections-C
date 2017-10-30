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

#ifndef COLLECTIONS_C_ARRAY_H
#define COLLECTIONS_C_ARRAY_H

#include "common.h"

/**
 * A dynamic array that expands automatically as elements are
 * added. The array supports amortized constant time insertion
 * and removal of elements at the end of the array, as well as
 * constant time access.
 */
typedef struct array_s Array;

/**
 * Array configuration structure. Used to initialize a new Array
 * with specific values.
 */
typedef struct array_conf_s {
    /**
     * The initial capacity of the array */
    size_t capacity;

    /**
     * The rate at which the buffer expands (capacity * exp_factor). */
    float  exp_factor;

    /**
     * Memory allocators used to allocate the Array structure and the
     * underlying data buffers. */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
} ArrayConf;

/**
 * Array iterator structure. Used to iterate over the elements of
 * the array in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during
 * iteration.
 */
typedef struct array_iter_s {
    /**
     * The array associated with this iterator */
    Array  *ar;

    /**
     * The current position of the iterator.*/
    size_t  index;

    /**
     * Set to true if the last returned element was removed. */
    bool last_removed;
} ArrayIter;

/**
 * Array zip iterator structure. Used to iterate over the elements of two
 * arrays in lockstep in an ascending order until one of the Arrays is
 * exhausted. The iterator also supports operations for safely adding
 * and removing elements during iteration.
 */
typedef struct array_zip_iter_s {
    Array *ar1;
    Array *ar2;
    size_t index;
    bool last_removed;
} ArrayZipIter;


enum cc_stat  array_new             (Array **out);
enum cc_stat  array_new_conf        (ArrayConf const * const conf, Array **out);
void          array_conf_init       (ArrayConf *conf);

void          array_destroy         (Array *ar);
void          array_destroy_cb      (Array *ar, void (*cb) (void*));

enum cc_stat  array_add             (Array *ar, void *element);
enum cc_stat  array_add_at          (Array *ar, void *element, size_t index);
enum cc_stat  array_replace_at      (Array *ar, void *element, size_t index, void **out);
enum cc_stat  array_swap_at         (Array *ar, size_t index1, size_t index2);

enum cc_stat  array_remove          (Array *ar, void *element, void **out);
enum cc_stat  array_remove_at       (Array *ar, size_t index, void **out);
enum cc_stat  array_remove_last     (Array *ar, void **out);
void          array_remove_all      (Array *ar);
void          array_remove_all_free (Array *ar);

enum cc_stat  array_get_at          (Array *ar, size_t index, void **out);
enum cc_stat  array_get_last        (Array *ar, void **out);

enum cc_stat  array_subarray        (Array *ar, size_t from, size_t to, Array **out);
enum cc_stat  array_copy_shallow    (Array *ar, Array **out);
enum cc_stat  array_copy_deep       (Array *ar, void *(*cp) (void*), Array **out);

void          array_reverse         (Array *ar);
enum cc_stat  array_trim_capacity   (Array *ar);

size_t        array_contains        (Array *ar, void *element);
size_t        array_contains_value  (Array *ar, void *element, int (*cmp) (const void*, const void*));
size_t        array_size            (Array *ar);
size_t        array_capacity        (Array *ar);

enum cc_stat  array_index_of        (Array *ar, void *element, size_t *index);
void          array_sort            (Array *ar, int (*cmp) (const void*, const void*));

void          array_map             (Array *ar, void (*fn) (void*));
void          array_reduce          (Array *ar, void (*fn) (void*, void*, void*), void *result);

enum cc_stat  array_filter_mut      (Array *ar, bool (*predicate) (const void*));
enum cc_stat  array_filter          (Array *ar, bool (*predicate) (const void*), Array **out);

void          array_iter_init       (ArrayIter *iter, Array *ar);
enum cc_stat  array_iter_next       (ArrayIter *iter, void **out);
enum cc_stat  array_iter_remove     (ArrayIter *iter, void **out);
enum cc_stat  array_iter_add        (ArrayIter *iter, void *element);
enum cc_stat  array_iter_replace    (ArrayIter *iter, void *element, void **out);
size_t        array_iter_index      (ArrayIter *iter);


void          array_zip_iter_init   (ArrayZipIter *iter, Array *a1, Array *a2);
enum cc_stat  array_zip_iter_next   (ArrayZipIter *iter, void **out1, void **out2);
enum cc_stat  array_zip_iter_add    (ArrayZipIter *iter, void *e1, void *e2);
enum cc_stat  array_zip_iter_remove (ArrayZipIter *iter, void **out1, void **out2);
enum cc_stat  array_zip_iter_replace(ArrayZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        array_zip_iter_index  (ArrayZipIter *iter);

const void* const* array_get_buffer(Array *ar);


#define ARRAY_FOREACH(val, array, body)         \
    {                                           \
        ArrayIter array_iter_53d46d2a04458e7b;  \
        array_iter_init(&array_iter_53d46d2a04458e7b, array);   \
        void *val;                                              \
        while (array_iter_next(&array_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define ARRAY_FOREACH_ZIP(val1, val2, array1, array2, body)     \
    {                                                           \
        ArrayZipIter array_zip_iter_ea08d3e52f25883b3;                 \
        array_zip_iter_init(&array_zip_iter_ea08d3e52f25883b, array1, array2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (array_zip_iter_next(&array_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#endif /* COLLECTIONS_C_ARRAY_H */
