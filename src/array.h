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

#ifndef ARRAY_H_
#define ARRAY_H_

#include "common.h"

/**
 * A dynamic array that expands automatically as elements are added. The array supports
 * amortized constant time insertion and removal of elements at the end of the array, as
 * well as constant time access.
 */
typedef struct array_s Array;

/**
 * Array configuration object. Used to initalize a new Array with specific
 * values.
 *
 * @code
 * ArrayConf c;
 * array_conf_init(&c);
 *
 * c.capcity = 32;
 *
 * Array *a = array_new_conf(&c);
 * @endcode
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
 * Array iterator object. Used to iterate over the elements of the array
 * in an ascending order. The iterator also supports operations for safely
 * adding and removing elements during iteration.
 *
 * @code
 * ArrayIter i;
 * array_iter_init(&i);
 *
 * while (array_iter_has_next(&i)) {
 *     MyType *e = array_iter_next(&i);
 * }
 * @endcode
 */
typedef struct array_iter_s {
    /**
     * The array associated with this iterator */
    Array  *ar;

    /**
     * The current position of the iterator.*/
    size_t  index;
} ArrayIter;

enum cc_stat  array_new             (Array **out);
enum cc_stat  array_new_conf        (ArrayConf const * const conf, Array **out);
void          array_conf_init       (ArrayConf *conf);

void          array_destroy         (Array *ar);
void          array_destroy_free    (Array *ar);

enum cc_stat  array_add             (Array *ar, void *element);
enum cc_stat  array_add_at          (Array *ar, void *element, size_t index);
enum cc_stat  array_replace_at      (Array *ar, void *element, size_t index, void **out);

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
size_t        array_size            (Array *ar);
size_t        array_capacity        (Array *ar);

enum cc_stat  array_index_of        (Array *ar, void *element, size_t *index);
void          array_sort            (Array *ar, int (*cmp) (const void*, const void*));

void          array_map             (Array *ar, void (*fn) (void *));

void          array_iter_init       (ArrayIter *iter, Array *ar);
enum cc_stat  array_iter_next       (ArrayIter *iter, void **out);
enum cc_stat  array_iter_remove     (ArrayIter *iter, void **out);
enum cc_stat  array_iter_add        (ArrayIter *iter, void *element);
enum cc_stat  array_iter_replace    (ArrayIter *iter, void *element, void **out);
size_t        array_iter_index      (ArrayIter *iter);

const void* const* array_get_buffer(Array *ar);

#endif /* ARRAY_H_ */
