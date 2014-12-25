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
    int    size;
    int    capacity;
    void **buffer;
};

struct vector_iter_s {
    Vector *vect;
    int     index;
};

static bool expand_capacity(Vector *vect);

/**
 * Returns a new empty vector, or NULL if the allocation fails.
 *
 * @return a new vector if the allocation was successful, or NULL if it was not.
 */
Vector *vector_new()
{
    return vector_new_capacity(DEFAULT_CAPACITY);
}

/**
 * Returns a new empty vector with the specified initial capacity, or NULL if it
 * was not.
 * 
 * @param[in] capacity the initial capacity of the vector
 * 
 * @return a new vector if the allocation was successful, or NULL if it was not.
 */
Vector *vector_new_capacity(size_t capacity)
{
    Vector *v = calloc(1, sizeof(Vector));

    if (v == NULL)
        return NULL;

    size_t c = capacity == 0 ? 1 : capacity;

    v->capacity = c;
    v->buffer   = calloc(c, sizeof(void*));

    return v;
}

/**
 * Destroys the vector structure, but leaves the data it used to hold, intact.
 *
 * @param[in] vect the vector that is being destroyed.
 */
void vector_destroy(Vector *vect)
{
    free(vect->buffer);
    free(vect);
}

/**
 * Destroys the vector structure along with all the data it holds.
 *
 * @note
 * This functions should not be called on a vector that has some of it's elements
 * allocated on the stack.
 * 
 * @param[in] vect the vector that is being destroyed.
 */
void vector_destroy_free(Vector *vect)
{
    int i;
    for (i = 0; i < vect->size; i++)
        free(vect->buffer[i]);
    
    vector_destroy(vect);
}

/**
 * Adds a new element to the vector. The element is appended to the vector making
 * it the last element (the one with the highest index) of the vector. This
 * function returns true or false based on whether or not the space allocation 
 * for the new element was successful or not.
 *
 * @param[in] vect the vector to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return true if the operation was successful.
 */
bool vector_add(Vector *vect, void *element)
{
    bool alloc = true;

    if (vect->size >= vect->capacity)
        alloc = expand_capacity(vect);

    if (!alloc)
        return false;

    vect->buffer[vect->size] = element;
    vect->size++;

    return true;
}

/**
 * Adds a new element to the vector at a specified position by shifting all
 * subsequent elemnts by one. The specified index must be within the bounds
 * of the vector, otherwise this operation will fail and false will be
 * returned to indicate failure. This function may also fail if the space 
 * allocation for the new element was unsuccessful.
 *
 * @param[in] vect the vector to which the element is being added
 * @param[in] element the element that is being added
 * @param[in] index the position in the vector at which the element is being
 *            added that must be within bounds of the vector
 *
 * @return true if the operation was successful, false if not
 */
bool vector_add_at(Vector *vect, void *element, size_t index)
{
    bool alloc = true;

    if (index > (vect->size - 1))
        return false;

    if (vect->size == vect->capacity)
       alloc = expand_capacity(vect);

    if (!alloc)
        return false;
    
    size_t shift = (vect->size - index) * sizeof(void*);
    memmove(&(vect->buffer[index + 1]), &(vect->buffer[index]), shift);

    vect->buffer[index] = element;
    vect->size++;

    return true;
}

/**
 * Replaces a vector element at the specified index and returns the replaced
 * element. The specified index must be within the bounds of the vector,
 * otherwise NULL is returned. NULL can also be returned if the replaced element
 * was NULL. In this case calling <code>vector_contains()</code> before this
 * function can resolve this ambiguity.
 *
 * @param[in] vect the vector whose element is being replaced
 * @param[in] element the replacement element
 * @param[in] index the index at which the replacement element should be placed
 *
 * @return the replaced element or NULL if the index was out of bounds.
 */
void *vector_replace_at(Vector *vect, void *element, size_t index)
{
    if (index >= vect->size)
        return NULL;

    void *old_e = vect->buffer[index];
    vect->buffer[index] = element;

    return old_e;
}

/**
 * Removes and returns the specified element from the vector if such element
 * exists. In case the element does not exist NULL is returned. NULL can also be
 * returned if the specified element is NULL. In this case calling <code>
 * vector_contains()</code> before this function can resolve this ambiguity.
 *
 * @param[in] vect the vector from which the element is being removed
 * @param[in] element the element being removed
 *
 * @return the removed element, or NULL if the operation has failed
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
 * Removes and returns a vector element from the specified index. The index must
 * be within the bounds of the vector, otherwise NULL is returned. NULL may also
 * returned if the removed element was NULL. To resolve this ambiguity call
 * <code>vector_contains()</code> before this function.
 * 
 * @param[in] vect the vector from which the element is being removed
 * @param[in] index the index of the element being removed.
 *
 * @return the removed element, or NULL if the operation fails
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
 * Removes and returns a vector element from the end of the vector. Or NULL if
 * vector is empty. NULL may also be returned if the last element of the vector
 * is null. This ambiguity can be resolved by calling <code>vector_size()</code>
 * before this function.
 *
 * @param the vector whose last element is being returned
 *
 * @return the last element of the vector ie. the element at the highest index
 */
void *vector_remove_last(Vector *vect)
{
    return vector_remove_at(vect, vect->size - 1);
}

/**
 * Removes all elements from the specified vector. This function does not shrink
 * the vector capacity.
 *
 * @param[in] vect the vector from which all elements are to be removed
 */
void vector_remove_all(Vector *vect)
{
    vect->size = 0;
}

/**
 * Removes and frees all elements from the specified vector. This function does
 * not shrink the vector capacity.
 *
 * @param[in] vect the vector from which all elements are to be removed
 */
void vector_remove_all_free(Vector *vect)
{
    int i;
    for (i = 0; i < vect->size; i++)
        free(vect->buffer[i]);

    vector_remove_all(vect);
}

/**
 * Returns a vector element from the specified index. The specified index must be
 * within the bounds of the vector, otherwise NULL is returned. NULL can also be 
 * returned if the element at the specified index is NULL. This ambiguity can be
 * resolved by calling <code>vector_contains()</code> before this function.
 *
 * @param[in] vect the vector from which the element is being retrieved
 * @param[in] index the index of the vector element
 *
 * @return vector element at the specified index, or NULL if the operation has
 *         failed
 */
void *vector_get(Vector *vect, size_t index)
{
    if (index >= vect->size)
        return NULL;

    return vect->buffer[index];
}

/**
 * Returns the last element of the vector ie. the element at the highest index,
 * or NULL if the vector is empty. Null may also be returned if the vector is
 * empty. This ambiguity can be resolved by calling <code>vector_size()</code>
 * before this function.
 *
 * @param[in] the vector whose last element is being returned
 *
 * @return the last element of the specified vector
 */
void *vector_get_last(Vector *vect)
{
    return vector_get(vect, vect->size - 1);
}

/**
 * Returns the underlying vector buffer.
 *
 * @note Any direct modification of the buffer may invalidate the vector.
 *
 * @param[in] vect the vector whose underlying buffer is being returned
 *
 * @return the buffer
 */
const void **vector_get_buffer(Vector *vect)
{
    return vect->buffer;
}

/**
 * Returns the index of the first occurrence of the specified vector element, or
 * -1 if the element could not be found.
 *
 * @param[in] vect vector being searched
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element, or -1 if the element is not found
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
 * Returns a subvector of the specified vector, randing from <code>b</code>
 * index (inclusive) to <code>e</code> index (inclusive). The range indices
 * must be within the bounds of the vector, while the <code>e</code> index
 * must be greater or equal to the <code>b</code> index. If these conditions
 * not met, NULL is returned.
 *
 * @param[in] vect the vector from which the subvector is being returned
 * @param[in] b the beginning index (inclusive) of the subvector that must be
 *              within the bounds of the vector and must not exceed the 
 *              the end index
 * @param[in] e the end index (inclusive) of the subvector that must be within
 *              the bounds of the vector and must be greater or equal to the
 *              beginnig index
 *
 * @return a subvector of the specified vector, or NULL
 */
Vector *vector_subvector(Vector *vect, size_t b, size_t e)
{
    if (b > e || b < 0 || e > vect->size)
        return NULL;

    Vector *v   = vector_new();

    v->size     = e - b + 1;
    v->capacity = v->size * 2;

    void **new_buff = calloc(v->capacity, sizeof(void*));
    memcpy(new_buff, &(vect->buffer[b]), v->size * sizeof(void*));

    v->buffer   = new_buff;

    return v;
}

/**
 * Returns a shallow copy of the specified vector. A shallow copy is a copy of
 * the vector structure, but not the elements it holds.
 *
 * @param[in] vect the vector to be copied
 *
 * @return a shallow copy of the specified vector
 */
Vector *vector_copy_shallow(Vector *vect)
{
    Vector *v   = malloc(sizeof(Vector));
    
    v->size     = vect->size;
    v->capacity = vect->capacity;
    v->buffer   = calloc(v->capacity, sizeof(void*));

    memcpy(v->buffer, vect->buffer, v->size * sizeof(void*));

    return v;
}

/**
 * Returns a deep copy of the specified vector. A deep copy is a copy of
 * both the vector structure and the data it holds.
 *
 * @param[in] vect the vector to be copied
 * @param[in] cp the copy function that returns a copy of a vector element
 *
 * @return a deep copy of the specified vector
 */
Vector *vector_copy_deep(Vector *vect, void *(*cp) (void *))
{
    Vector *v   = malloc(sizeof(Vector));
    
    v->size     = vect->size;
    v->capacity = vect->capacity;
    v->buffer   = calloc(v->capacity, sizeof(void*));

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
 * the number of elements in the specified vector, however the capacity will
 * never shrink below 1.
 *
 * @param[in] vect the vector whose capacity is being trimmed.
 */
void vector_trim_capacity(Vector *vect)
{
    if (vect->size == vect->capacity)
        return;

    void **new_buff = calloc(vect->size, sizeof(void*));

    size_t size = vect->size < 1 ? 1 : vect->size;
    
    memcpy(new_buff, vect->buffer, size * sizeof(void*));
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
 * @return the number of occurrences of the element
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
 * number of elements contained within the vector.
 *
 * @param[in] vect the vector whose size is being returned
 *
 * @return the the number of element within the vector
 */
size_t vector_size(Vector *vect)
{
    return vect->size;
}

/**
 * Returns the capacity of the specified vector. The capacity of the vector is
 * the maximum number of elements a vector can hold before it has to be resized.
 *
 * @param[in] vect vector whose capacity is being returned
 *
 * @return the capacity of the vector
 */
size_t vector_capacity(Vector *vect)
{
    return vect->capacity;
}

/**
 * Sorts the specified vector.
 *
 * @note
 * Pointers passed to the comparator function will be pointers to the vector 
 * elements that are of type (void*) ie. void**. So an extra step of 
 * dereferencing will be required before the data can be used for comparison:
 * eg. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @param[in] vect vector to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                int cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first.
 */
void vector_sort(Vector *vect, int (*cmp) (const void*, const void*))
{
    qsort(vect->buffer, vect->size, sizeof(void*), cmp);
}

/**
 * Expands the vector capacity.
 *
 * @param[in] vect vector whose capacity is being expanded
 */
static bool expand_capacity(Vector *vect)
{
    // FIXME limit to max size to avoid integer overflow
    size_t new_capacity = vect->capacity * DEFAULT_EXPANSION_FACTOR;
    void **new_buff  = calloc(new_capacity, sizeof(void*));

    if (new_buff == NULL)
        return false;

    memcpy(new_buff, vect->buffer, vect->size * sizeof(void*));

    free(vect->buffer);

    vect->capacity = new_capacity;
    vect->buffer = new_buff;

    return true;
}

/**
 * A 'foreach loop' function that invokes the specified function on each element
 * in the vector.
 *
 * @param[in] vect the vector on which this operation is performed
 * @param[in] op the operation function that is to be invoked on each vector 
 *               element
 */
void vector_foreach(Vector *vect, void (*op) (void *e))
{
    int i;
    for (i = 0; i < vect->size; i++)
        op(vect->buffer[i]);
}

/**
 * Returns a new vector iterator.
 *
 * @param[in] vect the vector to iterate over
 *
 * @return a new vector iterator
 */
VectorIter *vector_iter_new(Vector *vect)
{
    VectorIter *iter = calloc(1, sizeof(VectorIter));

    iter->vect = vect;

    return iter;
}

/**
 * Destroys the specified iterator
 *
 * @param[in] iter iterator to be destroyed
 */
void vector_iter_destroy(VectorIter *iter)
{
    free(iter);
}

/**
 * Checks whether or not the iterator has reached the end of the vector
 *
 * @param[in] iter iterator whose position is being checked
 *
 * @return true if there are more elements to be iterated over, or false if not
 */
bool vector_iter_has_next(VectorIter *iter)
{
    return iter->index < iter->vect->size;
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
void *vector_iter_next(VectorIter *iter)
{
    return iter->vect->buffer[iter->index++];
}

/**
 * Removes and returns the last returned element by <code>vector_iter_next()
 * </code> without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the removed element
 */
void *vector_iter_remove(VectorIter *iter)
{ 
    return vector_remove_at(iter->vect, iter->index - 1);
}

/**
 * Adds a new element to the vector after the last retuned element by
 * <code>vector_iter_next()</code> without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @parma[in] element the element being added
 */
void vector_iter_add(VectorIter *iter, void *element)
{
    vector_add_at(iter->vect, element, iter->index++);
}

/**
 * Replaces the last returned element by <code>vector_iter_next()</code>
 * with the specified replacement element.
 * 
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 *
 * @return the old element that was replaced by the new one
 */
void *vector_iter_replace(VectorIter *iter, void *element)
{
    return vector_replace_at(iter->vect, element, iter->index);
}

/**
 * Returns the index of the last returned element by <code>vector_iter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index
 */
size_t vector_iter_index(VectorIter *iter)
{
    return iter->index;
}

