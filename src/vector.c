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

#include "vector.h"

#define DEFAULT_CAPACITY 8
#define DEFAULT_EXPANSION_FACTOR 2

struct vector_s {
    int size;
    int capacity;
    void **buffer;
};

struct vector_iter_s {
    Vector *vect;
    int index;
};

static void expand_capacity(Vector *vect);

/**
 *
 * @return
 */
Vector *vector_new()
{
    return vector_new_capacity(DEFAULT_CAPACITY);
}

/**
 *
 * @param[in] capacity
 * @return
 */
Vector *vector_new_capacity(size_t capacity)
{
    Vector *v = calloc(1, sizeof(Vector));

    v->capacity = capacity;
    v->buffer   = calloc(capacity, sizeof(void*));

    return v;
}

/**
 *
 * @param[in] vect
 */
void vector_destroy(Vector *vect)
{
    free(vect->buffer);
    free(vect);
}

/**
 *
 * @param[in] vect
 */
void vector_destroy_free(Vector *vect)
{
    int i;
    for (i = 0; i < vect->size; i++)
        free(vect->buffer[i]);
    
    vector_destroy(vect);
}

/**
 *
 * @param[in] vect
 * @param[in] element
 *
 * @return
 */
bool vector_add(Vector *vect, void *element)
{
    if (vect->size >= vect->capacity)
        expand_capacity(vect);

    vect->buffer[vect->size] = element;
    vect->size++;

    return true;
}

/**
 *
 * @param[in] vect
 * @param[in] element
 * @param[in] index
 *
 * @return
 */
bool vector_add_at(Vector *vect, size_t index, void *element)
{
    if (index > (vect->size - 1))
        return false;

    if (vect->size == vect->capacity)
        expand_capacity(vect);

    size_t shift = (vect->size - index) * sizeof(void*);
    memmove(&(vect->buffer[index + 1]), &(vect->buffer[index]), shift);

    vect->buffer[index] = element;
    vect->size++;

    return true;
}

/**
 *
 * @param[in] vect
 * @param[in] element
 * @param[in] index
 *
 * @return
 */
void *vector_replace_at(Vector *vect, size_t index, void *element)
{
    if (index >= vect->size)
        return NULL;

    void *old_e = vect->buffer[index];
    vect->buffer[index] = element;

    return old_e;
}

/**
 *
 * @param[in] vect
 * @param[in] element
 *
 * @return
 */
void *vector_remove(Vector *vect, void *element)
{
    int index = vector_index_of(vect, element);

    if (index == -1)
        return NULL;

    if (index != vect->size - 1) {
        size_t block_size = (vect->size - index) * sizeof(void*);
        memmove(&(vect->buffer[index]), &(vect->buffer[index + 1]), block_size);
    }
    vect->size--;

    return element;
}

/**
 *
 * @param[in] vect
 * @param[in] element
 *
 * @return
 */
void *vector_remove_at(Vector *vect, size_t index)
{
    if (index >= vect->size)
        return NULL;

    void *e = vect->buffer[index];

    if (index != vect->size - 1) {
        int block_size = (vect->size - index) * sizeof(void*);
        memmove(&(vect->buffer[index]), &(vect->buffer[index + 1]), block_size);
    }
    vect->size--;

    return e;
}

/**
 *
 * @param[in] vect
 */
void vector_remove_all(Vector *vect)
{
    free(vect->buffer);
    vect->buffer = calloc(DEFAULT_CAPACITY, sizeof(void*));
    vect->capacity = DEFAULT_CAPACITY;
    vect->size = 0;
}

/**
 *
 * @param[in] vect
 */
void vector_remove_all_free(Vector *vect)
{
    int i;
    for (i = 0; i < vect->size; i++)
        free(vect->buffer[i]);

    vector_remove_all(vect);
}

/**
 *
 * @param[in] vect
 * @param[in] index
 *
 * @return
 */
void *vector_get(Vector *vect, size_t index)
{
    if (index >= vect->size)
        return NULL;

    return vect->buffer[index];
}

/**
 *
 * @param[in] vect
 * @param[in] element
 *
 * @return
 */
int vector_index_of(Vector *vect, void *element)
{
    int i;
    for (i = 0; i < vect->size; i++) {
        if (vect->buffer[i] == element)
            return i;
    }
    return -1;
}

/**
 *
 * @param[in] vect
 * @param[in] from
 * @param[in] to
 *
 * @return
 */
Vector *vector_subvector(Vector *vect, size_t from, size_t to)
{
    if (from > to || from < 0 || to > vect->size)
        return NULL;

    Vector *v = vector_new();

    v->size = to - from + 1;
    v->capacity = v->size * 2;

    void **new_buff = calloc(v->capacity, sizeof(void*));
    memcpy(new_buff, &(vect->buffer[from]), v->size * sizeof(void*));

    v->buffer = new_buff;

    return v;
}

/**
 *
 * @param[in] vect
 *
 * @return
 */
Vector *vector_copy_shallow(Vector *vect)
{
    Vector *v = malloc(sizeof(Vector));
    v->size = vect->size;
    v->capacity = vect->capacity;
    v->buffer = calloc(v->capacity, sizeof(void*));

    memcpy(v->buffer, vect->buffer, v->size * sizeof(void*));

    return v;
}

/**
 *
 *
 * @param[in] vect
 * @param[in] cp
 *
 * @return
 */
Vector *vector_copy_deep(Vector *vect, void *(*cp) (void *))
{
    Vector *v = malloc(sizeof(Vector));
    v->size = vect->size;
    v->capacity = vect->capacity;
    v->buffer = calloc(v->capacity, sizeof(void*));

    int i;
    for (i = 0; i < v->size; i++)
        v->buffer[i] = cp(vect->buffer[i]);

    return v;
}

/**
 * Reverses the order of elements in the specified vector.
 *
 * @param[in] vect the vector that is being reversed.
 */
void vector_reverse(Vector *vect)
{
    int i;
    int j;
    for (i = 0, j = vect->size - 1; i < (vect->size - 1) / 2; i++, j--) {
        void *tmp = vect->buffer[i];
        vect->buffer[i] = vect->buffer[j];
        vect->buffer[j] = tmp;
    }
}

/**
 * Trims the vector's capacity, in other words, it shrinks the capacity to match
 * the number of elements in the specified vector.
 *
 * @param[in] vect the vector whose capacity is being trimmed.
 */
void vector_trim_capacity(Vector *vect)
{
    if (vect->size == vect->capacity)
        return;

    void **new_buff = calloc(vect->size, sizeof(void*));
    memcpy(new_buff, vect->buffer, vect->size * sizeof(void*));
    free(vect->buffer);
    vect->buffer = new_buff;
    vect->capacity= vect->size;
}

/**
 * Returns the number of occurrences of the element within the specified vector.
 *
 * @param[in] vect the vector that is being searched
 * @param[in] element the elment that is being searched for
 *
 * @return the number of occurrences of the element;
 */
int vector_contains(Vector *vect, void *element)
{
    int o = 0;
    int i;
    for (i = 0; i < vect->size; i++) {
        if (element == vect->buffer[i])
            o++;
    }
    return o;
}

/**
 * Returns the size of the specified vector. The size of the vector is the
 * number of elements within the vector
 *
 * @param[in] vect the vector whose size is being returned
 *
 * @return the size of the vector
 */
size_t vector_size(Vector *vect)
{
    return vect->size;
}

/**
 * Returns the capacity of the specified vector. The capacity of the vector is
 * the number of av
 * @param[in] vect
 *
 * @return
 */
size_t vector_capacity(Vector *vect)
{
    return vect->capacity;
}

/**
 *
 * @param[in] vect
 * @param[in] cmp
 */
void vector_sort(Vector *vect, int (*cmp) (const void*, const void*))
{
    qsort(vect->buffer, vect->size, sizeof(void*), cmp);
}

/**
 *
 * @param[in] vect
 */
static void expand_capacity(Vector *vect)
{
    size_t new_capacity = vect->capacity * DEFAULT_EXPANSION_FACTOR;
    void **new_buff  = calloc(new_capacity, sizeof(void*));

    memcpy(new_buff, vect->buffer, vect->size * sizeof(void*));

    free(vect->buffer);

    vect->capacity = new_capacity;
    vect->buffer = new_buff;
}

/**
 *
 *
 */
void vector_foreach(Vector *vect, void (*op) (void *e))
{
    int i;
    for (i = 0; i < vect->size; i++)
        op(vect->buffer[i]);
}

/**
 *
 *
 */
VectorIter *vector_iter_new(Vector *vect)
{
    VectorIter *iter = calloc(1, sizeof(VectorIter));

    iter->vect = vect;
    iter->index = 0;

    return iter;
}

/**
 *
 *
 */
void vector_iter_destroy(VectorIter *iter)
{
    free(iter);
}

/**
 *
 *
 */
bool vector_iter_has_next(VectorIter *iter)
{
    return iter->index < iter->vect->size;
}

/**
 *
 *
 */
void *vector_iter_next(VectorIter *iter)
{
    void *next = iter->vect->buffer[iter->index];
    iter->index++;
    return next;
}

/**
 *
 *
 */
void *vector_iter_remove(VectorIter *iter)
{
    void *rm = iter->vect->buffer[iter->index];
    vector_remove_at(iter->vect, iter->index);
    iter->index--;
    return rm;
}

/**
 *
 *
 */
void vector_iter_add(VectorIter *iter, void *element)
{
    vector_add_at(iter->vect, iter->index, element);
    iter->index++;
}

/**
 *
 *
 */
void *vector_iter_replace(VectorIter *iter, void *element)
{
    return vector_replace_at(iter->vect, iter->index, element);
}

/**
 *
 *
 */
size_t vector_iter_index(VectorIter *iter)
{
    return iter->index;
}
