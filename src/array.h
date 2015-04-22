/*
 * Collections-C
 * Copyright (C) 2013-2014 Srđan Panić <srdja.panic@gmail.com>
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

typedef struct array_s Array;
typedef struct array_iter_s ArrayIter;

typedef struct array_conf_s {
    /* The initial capacity of the array */
    size_t capacity;

    /* The rate at which the buffer expands (capacity * exp_factor). */
    float  exp_factor;

    /* Memory allocators used to allocate the Array structure and the
     * underlying data buffers. */
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
} ArrayConf;

struct array_iter_s {
    Array  *ar;
    size_t  index;
};

Array*       array_new             ();
Array*       array_new_conf        (ArrayConf *conf);
void         array_conf_init       (ArrayConf *conf);

void         array_destroy         (Array *ar);
void         array_destroy_free    (Array *ar);

bool         array_add             (Array *ar, void *element);
bool         array_add_at          (Array *ar, void *element, size_t index);
void*        array_replace_at      (Array *ar, void *element, size_t index);

void*        array_remove          (Array *ar, void *element);
void*        array_remove_at       (Array *ar, size_t index);
void*        array_remove_last     (Array *ar);
void         array_remove_all      (Array *ar);
void         array_remove_all_free (Array *ar);

void*        array_get             (Array *ar, size_t index);
void*        array_get_last        (Array *ar);
const void** array_get_buffer      (Array *ar);

Array*       array_subarray        (Array *ar, size_t from, size_t to);
Array*       array_copy_shallow    (Array *ar);
Array*       array_copy_deep       (Array *ar, void *(*cp) (void*));

void         array_reverse         (Array *ar);
void         array_trim_capacity   (Array *ar);

size_t       array_cointains       (Array *ar, void *element);
size_t       array_size            (Array *ar);
size_t       array_capacity        (Array *ar);

size_t       array_index_of        (Array *ar, void *element);
void         array_sort            (Array *ar, int (*cmp) (const void*, const void*));

void         array_foreach         (Array *ar, void (*op) (void *));

void         array_iter_init       (ArrayIter *iter, Array *vec);
bool         array_iter_has_next   (ArrayIter *iter);
void*        array_iter_next       (ArrayIter *iter);
void*        array_iter_remove     (ArrayIter *iter);
bool         array_iter_add        (ArrayIter *iter, void *element);
void*        array_iter_replace    (ArrayIter *iter, void *element);
size_t       array_iter_index      (ArrayIter *iter);

#endif /* ARRAY_H_ */
