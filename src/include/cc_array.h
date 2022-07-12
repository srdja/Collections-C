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

#include "cc_common.h"

/**
 * A dynamic array that expands automatically as elements are
 * added. The array supports amortized constant time insertion
 * and removal of elements at the end of the array, as well as
 * constant time access.
 */
typedef struct cc_array_s CC_Array;

/**
 * Array configuration structure. Used to initialize a new Array
 * with specific values.
 */
typedef struct cc_array_conf_s {
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
} CC_ArrayConf;

/**
 * Array iterator structure. Used to iterate over the elements of
 * the array in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during
 * iteration.
 */
typedef struct cc_array_iter_s {
    /**
     * The array associated with this iterator */
    CC_Array  *ar;

    /**
     * The current position of the iterator.*/
    size_t  index;

    /**
     * Set to true if the last returned element was removed. */
    bool last_removed;
} CC_ArrayIter;

/**
 * Array zip iterator structure. Used to iterate over the elements of two
 * arrays in lockstep in an ascending order until one of the Arrays is
 * exhausted. The iterator also supports operations for safely adding
 * and removing elements during iteration.
 */
typedef struct array_zip_iter_s {
    CC_Array *ar1;
    CC_Array *ar2;
    size_t    index;
    bool      last_removed;
} CC_ArrayZipIter;


enum cc_stat  cc_array_new             (CC_Array **out);
enum cc_stat  cc_array_new_conf        (CC_ArrayConf const * const conf, CC_Array **out);
void          cc_array_conf_init       (CC_ArrayConf *conf);

void          cc_array_destroy         (CC_Array *ar);
void          cc_array_destroy_cb      (CC_Array *ar, void (*cb) (void*));

enum cc_stat  cc_array_add             (CC_Array *ar, void *element);
enum cc_stat  cc_array_add_at          (CC_Array *ar, void *element, size_t index);
enum cc_stat  cc_array_replace_at      (CC_Array *ar, void *element, size_t index, void **out);
enum cc_stat  cc_array_swap_at         (CC_Array *ar, size_t index1, size_t index2);

enum cc_stat  cc_array_remove          (CC_Array *ar, void *element, void **out);
enum cc_stat  cc_array_remove_at       (CC_Array *ar, size_t index, void **out);
enum cc_stat  cc_array_remove_last     (CC_Array *ar, void **out);
void          cc_array_remove_all      (CC_Array *ar);
void          cc_array_remove_all_free (CC_Array *ar);

enum cc_stat  cc_array_get_at          (CC_Array *ar, size_t index, void **out);
enum cc_stat  cc_array_get_last        (CC_Array *ar, void **out);

enum cc_stat  cc_array_subarray        (CC_Array *ar, size_t from, size_t to, CC_Array **out);
enum cc_stat  cc_array_copy_shallow    (CC_Array *ar, CC_Array **out);
enum cc_stat  cc_array_copy_deep       (CC_Array *ar, void *(*cp) (void*), CC_Array **out);

void          cc_array_reverse         (CC_Array *ar);
enum cc_stat  cc_array_trim_capacity   (CC_Array *ar);

size_t        cc_array_contains        (CC_Array *ar, void *element);
size_t        cc_array_contains_value  (CC_Array *ar, void *element, int (*cmp) (const void*, const void*));
size_t        cc_array_size            (CC_Array *ar);
size_t        cc_array_capacity        (CC_Array *ar);

enum cc_stat  cc_array_index_of        (CC_Array *ar, void *element, size_t *index);
void          cc_array_sort            (CC_Array *ar, int (*cmp) (const void*, const void*));

void          cc_array_map             (CC_Array *ar, void (*fn) (void*));
void          cc_array_reduce          (CC_Array *ar, void (*fn) (void*, void*, void*), void *result);

enum cc_stat  cc_array_filter_mut      (CC_Array *ar, bool (*predicate) (const void*));
enum cc_stat  cc_array_filter          (CC_Array *ar, bool (*predicate) (const void*), CC_Array **out);

void          cc_array_iter_init       (CC_ArrayIter *iter, CC_Array *ar);
enum cc_stat  cc_array_iter_next       (CC_ArrayIter *iter, void **out);
enum cc_stat  cc_array_iter_remove     (CC_ArrayIter *iter, void **out);
enum cc_stat  cc_array_iter_add        (CC_ArrayIter *iter, void *element);
enum cc_stat  cc_array_iter_replace    (CC_ArrayIter *iter, void *element, void **out);
size_t        cc_array_iter_index      (CC_ArrayIter *iter);


void          cc_array_zip_iter_init   (CC_ArrayZipIter *iter, CC_Array *a1, CC_Array *a2);
enum cc_stat  cc_array_zip_iter_next   (CC_ArrayZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_array_zip_iter_add    (CC_ArrayZipIter *iter, void *e1, void *e2);
enum cc_stat  cc_array_zip_iter_remove (CC_ArrayZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_array_zip_iter_replace(CC_ArrayZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        cc_array_zip_iter_index  (CC_ArrayZipIter *iter);

const void* const* cc_array_get_buffer(CC_Array *ar);


#define CC_ARRAY_FOREACH(val, array, body)                      \
    {                                                           \
        CC_ArrayIter cc_array_iter_53d46d2a04458e7b;            \
        cc_array_iter_init(&cc_array_iter_53d46d2a04458e7b, array);     \
        void *val;                                                      \
        while (cc_array_iter_next(&cc_array_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_ARRAY_FOREACH_ZIP(val1, val2, array1, array2, body)         \
    {                                                                  \
        CC_ArrayZipIter cc_array_zip_iter_ea08d3e52f25883b3;            \
        cc_array_zip_iter_init(&cc_array_zip_iter_ea08d3e52f25883b3, array1, array2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_array_zip_iter_next(&cc_array_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#endif /* COLLECTIONS_C_ARRAY_H */
