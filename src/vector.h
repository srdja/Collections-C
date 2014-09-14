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

#ifndef VECTOR_H_
#define VECTOR_H_

#include "common.h"

typedef struct vector_s Vector;
typedef struct vector_iter_s VectorIter;

Vector *vector_new();
Vector *vector_new_capacity(size_t capacity);

void vector_destroy(Vector *vect);
void vector_destroy_free(Vector *vect);

bool vector_add(Vector *vect, void *element);
bool vector_add_at(Vector *vect, size_t index, void *element);
void *vector_replace_at(Vector *vect, size_t index, void *element);

void *vector_remove(Vector *vect, void *element);
void *vector_remove_at(Vector *vect, size_t index);
void *vector_remove_last(Vector *vect);
void vector_remove_all(Vector *vect);
void vector_remove_all_free(Vector *vect);

void *vector_get(Vector *vect, size_t index);
void *vector_get_last(Vector *vect);

Vector *vector_subvector(Vector *vect, size_t from, size_t to);
Vector *vector_copy_shallow(Vector *vect);
Vector *vector_copy_deep(Vector *vect, void *(*cp) (void*));

void vector_reverse(Vector *vect);
void vector_trim_capacity(Vector *vect);

int vector_cointains(Vector *vect, void *element);
size_t vector_size(Vector *vect);
size_t vector_capacity(Vector *vect);

int vector_index_of(Vector *vect, void *element);
void vector_sort(Vector *vect, int (*cmp) (const void*, const void*));

void vector_foreach(Vector *vect, void (*op) (void *));

VectorIter *vector_iter_new(Vector *vect);
void vector_iter_destroy(VectorIter *iter);
bool vector_iter_has_next(VectorIter *iter);
void *vector_iter_next(VectorIter *iter);
void *vector_iter_remove(VectorIter *iter);
void vector_iter_add(VectorIter *iter, void *element);
void *vector_iter_replace(VectorIter *iter, void *element);
size_t vector_iter_index(VectorIter *iter);
  
#endif /* VECTOR_H_ */
