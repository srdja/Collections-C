/*
 * Collections-C
 * Copyright (C) 2013-2024 Srđan Panić <srdja.panic@gmail.com>
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

#ifndef _COLLECTIONS_C_ARRAY_SIZED_H_
#define _COLLECTIONS_C_ARRAY_SIZED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_common.h"

/**
 * A dynamic array that expands automatically as elements are
 * added. The array supports amortized constant time insertion
 * and removal of elements at the end of the array, as well as
 * constant time access.
 */
typedef struct cc_array_sized_s CC_ArraySized;

/**
 * ArraySized configuration structure. Used to initialize a new ArraySized
 * with specific values.
 */
typedef struct cc_array_sized_conf_s {
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
} CC_ArraySizedConf;

/**
 * ArraySized iterator structure. Used to iterate over the elements of
 * the array in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during
 * iteration.
 */
typedef struct cc_array_sized_iter_s {
    /**
     * The array associated with this iterator */
    CC_ArraySized  *ar;

    /**
     * The current position of the iterator.*/
    size_t  index;

    /**
     * Set to true if the last returned element was removed. */
    bool last_removed;
} CC_ArraySizedIter;

/**
 * ArraySized zip iterator structure. Used to iterate over the elements of two
 * arrays in lockstep in an ascending order until one of the Arrays is
 * exhausted. The iterator also supports operations for safely adding
 * and removing elements during iteration.
 */
typedef struct array_sized_zip_iter_s {
    CC_ArraySized *ar1;
    CC_ArraySized *ar2;
    size_t         index;
    bool           last_removed;
} CC_ArraySizedZipIter;


enum cc_stat  cc_array_sized_new             (size_t element_size, CC_ArraySized **out);
enum cc_stat  cc_array_sized_new_reserved    (size_t element_size, size_t n_reserved, CC_ArraySized** out);
enum cc_stat  cc_array_sized_new_conf        (size_t element_size, CC_ArraySizedConf const * const conf, CC_ArraySized **out);
void          cc_array_sized_conf_init       (CC_ArraySizedConf *conf);
size_t        cc_array_sized_struct_size     ();

void          cc_array_sized_destroy         (CC_ArraySized *ar);

enum cc_stat  cc_array_sized_add             (CC_ArraySized *ar, uint8_t *element);
enum cc_stat  cc_array_sized_add_at          (CC_ArraySized *ar, uint8_t *element, size_t index);
enum cc_stat  cc_array_sized_replace_at      (CC_ArraySized *ar, uint8_t *element, size_t index, uint8_t *out);
enum cc_stat  cc_array_sized_swap_at         (CC_ArraySized *ar, size_t index1, size_t index2);

enum cc_stat  cc_array_sized_remove          (CC_ArraySized *ar, uint8_t *element);
enum cc_stat  cc_array_sized_remove_at       (CC_ArraySized *ar, size_t index, uint8_t *out);
enum cc_stat  cc_array_sized_remove_last     (CC_ArraySized *ar, uint8_t *out);
void          cc_array_sized_remove_all      (CC_ArraySized *ar);

enum cc_stat  cc_array_sized_peek            (CC_ArraySized* ar, size_t index, uint8_t **out);

enum cc_stat  cc_array_sized_get_at          (CC_ArraySized *ar, size_t index, uint8_t *out);
enum cc_stat  cc_array_sized_get_last        (CC_ArraySized *ar, uint8_t *out);

enum cc_stat  cc_array_sized_subarray        (CC_ArraySized *ar, size_t from, size_t to, CC_ArraySized **out);
enum cc_stat  cc_array_sized_copy            (CC_ArraySized *ar, CC_ArraySized **out);

void          cc_array_sized_reverse         (CC_ArraySized *ar, uint8_t *tmp);
enum cc_stat  cc_array_sized_trim_capacity   (CC_ArraySized *ar);

size_t        cc_array_sized_contains        (CC_ArraySized *ar, uint8_t *element);
size_t        cc_array_sized_size            (CC_ArraySized *ar);
size_t        cc_array_sized_capacity        (CC_ArraySized *ar);

enum cc_stat  cc_array_sized_index_of        (CC_ArraySized *ar, uint8_t *element, size_t *index);
void          cc_array_sized_sort            (CC_ArraySized* ar, int (*cmp) (const void*, const void*));

void          cc_array_sized_map             (CC_ArraySized* ar, void (*fn) (uint8_t*));
void          cc_array_sized_reduce          (CC_ArraySized *ar, void (*fn) (uint8_t*, uint8_t*, uint8_t*), uint8_t *result);

enum cc_stat  cc_array_sized_filter_mut      (CC_ArraySized *ar, bool (*predicate) (const uint8_t*));
enum cc_stat  cc_array_sized_filter          (CC_ArraySized *ar, bool (*predicate) (const uint8_t*), CC_ArraySized **out);

void          cc_array_sized_iter_init       (CC_ArraySizedIter *iter, CC_ArraySized *ar);
enum cc_stat  cc_array_sized_iter_next       (CC_ArraySizedIter *iter, uint8_t **out);
enum cc_stat  cc_array_sized_iter_remove     (CC_ArraySizedIter *iter, uint8_t *out);
enum cc_stat  cc_array_sized_iter_add        (CC_ArraySizedIter *iter, uint8_t *element);
enum cc_stat  cc_array_sized_iter_replace    (CC_ArraySizedIter *iter, uint8_t *element, uint8_t *out);
size_t        cc_array_sized_iter_index      (CC_ArraySizedIter *iter);


void          cc_array_sized_zip_iter_init   (CC_ArraySizedZipIter *iter, CC_ArraySized *a1, CC_ArraySized *a2);
enum cc_stat  cc_array_sized_zip_iter_next   (CC_ArraySizedZipIter *iter, uint8_t **out1, uint8_t **out2);
enum cc_stat  cc_array_sized_zip_iter_add    (CC_ArraySizedZipIter *iter, uint8_t *e1, uint8_t *e2);
enum cc_stat  cc_array_sized_zip_iter_remove (CC_ArraySizedZipIter *iter, uint8_t *out1, uint8_t *out2);
enum cc_stat  cc_array_sized_zip_iter_replace(CC_ArraySizedZipIter *iter, uint8_t *e1, uint8_t *e2, uint8_t *out1, uint8_t *out2);
size_t        cc_array_sized_zip_iter_index  (CC_ArraySizedZipIter *iter);

const uint8_t* const* cc_array_sized_get_buffer(CC_ArraySized *ar);


#define CC_ARRAY_SIZED_FOREACH(val, array, body)                      \
    {                                                           \
        CC_ArraySizedIter cc_array_sized_iter_53d46d2a04458e7b;            \
        cc_array_sized_iter_init(&cc_array_sized_iter_53d46d2a04458e7b, array);     \
        uint8_t *val;                                                      \
        while (cc_array_sized_iter_next(&cc_array_sized_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_ARRAY_SIZED_FOREACH_ZIP(val1, val2, array1, array2, body)         \
    {                                                                  \
        CC_ArraySizedZipIter cc_array_sized_zip_iter_ea08d3e52f25883b3;            \
        cc_array_sized_zip_iter_init(&cc_array_sized_zip_iter_ea08d3e52f25883b3, array1, array2); \
        uint8_t *val1;                                                     \
        uint8_t *val2;                                                     \
        while (cc_array_sized_zip_iter_next(&cc_array_sized_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#ifdef __cplusplus
}
#endif

#endif /* COLLECTIONS_C_ARRAY_SIZED_H */
