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
Vector *vector_new_capacity(unsigned int capacity);

bool vector_add(Vector *vect, void *element);
bool vector_add_at(Vector *vect, void *element, unsigned int index);
bool vector_replace_at(Vector *vect, void *element, unsigned int index);

void *vector_remove(Vector *vect, void *element);
void *vector_remove_at(Vector *vect, unsigned int index);
bool vector_remove_all(Vector *vect);

void *vector_get(Vector *vect, unsigned int index);

Vector *vector_subvector(Vector *vect, unsigned int from, unsigned int to);
Vector *vector_copy_shallow(Vector *vect);
Vector *vector_copy_deep(Vector *vect, void *(*cp) (void*));

void vector_reverse(Vector *vect);
void vector_trim_capacity(Vector *vect);

int vector_cointains(Vector *vect, void *element);
int vector_size(Vector *vect);
int vector_capacity(Vector *vect);

int vector_index_of(Vector *vect, void *element);

void vector_sort(Vector *vect, int (*cmp) (const void*, const void*));



#endif /* VECTOR_H_ */
