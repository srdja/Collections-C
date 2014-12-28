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
    int    exp_factor;
    void **buffer;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

static bool expand_capacity(Vector *vec);


/**
 * Returns a new empty vector, or NULL if the allocation fails. 
 *
 * @return a new vector if the allocation was successful, or NULL if it was not.
 */
Vector *vector_new()
{
    VectorConf c;
    vector_conf_init(&c);
    return vector_new_conf(&c);
}

/**
 * Returns a new empty vector. The vector is allocated using the allocators
 * specified in the VectorConf struct. 
 *
 * @param[in] conf Vector configuration struct. All fields must be initialized to
 *                 appropriate values.
 *
 * @return a new vector if the allocation was successful, or NULL if not.
 */
Vector *vector_new_conf(VectorConf *conf)
{
    Vector *vec = conf->mem_calloc(1, sizeof(Vector));

    if (vec == NULL)
        return NULL;
    
    if (conf->exp_factor <= 1)
        vec->exp_factor = DEFAULT_EXPANSION_FACTOR;
    
    /* Needed to avoid an integer overflow on the first resize and
     * to easily check for any future oveflows. */
    else if (conf->exp_factor >= MAX_ELEMENTS / conf->capacity)
        return NULL;
    else
        vec->exp_factor = conf->exp_factor;
    
    vec->capacity   = conf->capacity;
    vec->mem_alloc  = conf->mem_alloc;
    vec->mem_calloc = conf->mem_calloc;
    vec->mem_free   = conf->mem_free;
    vec->buffer     = vec->mem_calloc(vec->capacity, sizeof(void*));

    return vec;
}

/**
 * Initializes the fields of the VectorConf struct to default values.
 *
 * @param[in, out] conf the struct that is being initialized
 */ 
void vector_conf_init(VectorConf *conf)
{
    conf->exp_factor = DEFAULT_EXPANSION_FACTOR;
    conf->capacity   = DEFAULT_CAPACITY;
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Destroys the vector structure, but leaves the data it used to hold, intact.
 *
 * @param[in] vect the vector that is being destroyed.
 */
void vector_destroy(Vector *vec)
{
    vec->mem_free(vec->buffer);
    vec->mem_free(vec);
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
void vector_destroy_free(Vector *vec)
{
    int i;
    for (i = 0; i < vec->size; i++)
        vec->mem_free(vec->buffer[i]);
    
    vector_destroy(vec);
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
bool vector_add(Vector *vec, void *element)
{
    bool alloc = true;

    if (vec->size >= vec->capacity)
        alloc = expand_capacity(vec);

    if (!alloc)
        return false;

    vec->buffer[vec->size] = element;
    vec->size++;

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
bool vector_add_at(Vector *vec, void *element, size_t index)
{
    bool alloc = true;

    if (index > (vec->size - 1))
        return false;

    if (vec->size == vec->capacity)
       alloc = expand_capacity(vec);

    if (!alloc)
        return false;
    
    size_t shift = (vec->size - index) * sizeof(void*);
    memmove(&(vec->buffer[index + 1]), &(vec->buffer[index]), shift);

    vec->buffer[index] = element;
    vec->size++;

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
void *vector_replace_at(Vector *vec, void *element, size_t index)
{
    if (index >= vec->size)
        return NULL;

    void *old_e = vec->buffer[index];
    vec->buffer[index] = element;

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
void *vector_remove(Vector *vec, void *element)
{
    int index = vector_index_of(vec, element);

    if (index == -1)
        return NULL;

    if (index != vec->size - 1) {
        size_t block_size = (vec->size - index) * sizeof(void*);
        memmove(&(vec->buffer[index]), &(vec->buffer[index + 1]), block_size);
    }
    vec->size--;

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
void *vector_remove_at(Vector *vec, size_t index)
{
    if (index >= vec->size)
        return NULL;

    void *e = vec->buffer[index];

    if (index != vec->size - 1) {
        int block_size = (vec->size - index) * sizeof(void*);
        memmove(&(vec->buffer[index]), &(vec->buffer[index + 1]), block_size);
    }
    vec->size--;

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
void *vector_remove_last(Vector *vec)
{
    return vector_remove_at(vec, vec->size - 1);
}

/**
 * Removes all elements from the specified vector. This function does not shrink
 * the vector capacity.
 *
 * @param[in] vect the vector from which all elements are to be removed
 */
void vector_remove_all(Vector *vec)
{
    vec->size = 0;
}

/**
 * Removes and frees all elements from the specified vector. This function does
 * not shrink the vector capacity.
 *
 * @param[in] vect the vector from which all elements are to be removed
 */
void vector_remove_all_free(Vector *vec)
{
    int i;
    for (i = 0; i < vec->size; i++)
        free(vec->buffer[i]);

    vector_remove_all(vec);
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
void *vector_get(Vector *vec, size_t index)
{
    if (index >= vec->size)
        return NULL;

    return vec->buffer[index];
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
void *vector_get_last(Vector *vec)
{
    return vector_get(vec, vec->size - 1);
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
const void **vector_get_buffer(Vector *vec)
{
    return (const void**) vec->buffer;
}



// XXX XXX 
// BROKEN

/**
 * Returns the index of the first occurrence of the specified vector element, or
 * -1 if the element could not be found.
 *
 * @param[in] vect vector being searched
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element, or -1 if the element is not found
 */
int vector_index_of(Vector *vec, void *element)
{
    int i;
    for (i = 0; i < vec->size; i++) {
        if (vec->buffer[i] == element)
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
 * @note The new vector is allocated using the original vectors allocators.
 *
 * @param[in] vec the vector from which the subvector is being returned
 * @param[in] b the beginning index (inclusive) of the subvector that must be
 *              within the bounds of the vector and must not exceed the 
 *              the end index
 * @param[in] e the end index (inclusive) of the subvector that must be within
 *              the bounds of the vector and must be greater or equal to the
 *              beginnig index
 *
 * @return a subvector of the specified vector, or NULL
 */
Vector *vector_subvector(Vector *vec, size_t b, size_t e)
{
    if (b > e || b < 0 || e > vec->size)
        return NULL;

    Vector *sub_vec     = vec->mem_calloc(1, sizeof(Vector));
    sub_vec->mem_alloc  = vec->mem_alloc;
    sub_vec->mem_calloc = vec->mem_calloc;
    sub_vec->mem_free   = vec->mem_free;
    sub_vec->size       = e - b + 1;
    sub_vec->capacity   = sub_vec->size;
    sub_vec->buffer     = vec->mem_alloc(sub_vec->capacity * sizeof(void*));
    
    memcpy(sub_vec->buffer, &(vec->buffer[b]), sub_vec->size * sizeof(void*));

    return sub_vec;
}

/**
 * Returns a shallow copy of the specified vector. A shallow copy is a copy of
 * the vector structure, but not the elements it holds.
 *
 * @param[in] vect the vector to be copied
 *
 * @return a shallow copy of the specified vector
 */
Vector *vector_copy_shallow(Vector *vec)
{
    Vector *copy = vec->mem_alloc(sizeof(Vector));

    copy->exp_factor = vec->exp_factor;
    copy->size       = vec->size;
    copy->capacity   = vec->capacity;
    copy->buffer     = vec->mem_calloc(copy->capacity, sizeof(void*));
    copy->mem_alloc  = vec->mem_alloc;
    copy->mem_calloc = vec->mem_calloc;
    copy->mem_free   = vec->mem_free;

    memcpy(copy->buffer, vec->buffer, copy->size * sizeof(void*));

    return copy;
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
Vector *vector_copy_deep(Vector *vec, void *(*cp) (void *))
{
    Vector *copy   = vec->mem_alloc(sizeof(Vector));

    copy->exp_factor = vec->exp_factor;
    copy->size       = vec->size;
    copy->capacity   = vec->capacity;
    copy->buffer     = vec->mem_calloc(copy->capacity, sizeof(void*));
    copy->mem_alloc  = vec->mem_alloc;
    copy->mem_calloc = vec->mem_calloc;
    copy->mem_free   = vec->mem_free;

    int i;
    for (i = 0; i < copy->size; i++)
        copy->buffer[i] = cp(vec->buffer[i]);

    return copy;
}

/**
 * Reverses the order of elements in the specified vector.
 *
 * @param[in] vect the vector that is being reversed.
 */
void vector_reverse(Vector *vec)
{
    int i;
    int j;
    for (i = 0, j = vec->size - 1; i < (vec->size - 1) / 2; i++, j--) {
        void *tmp = vec->buffer[i];
        vec->buffer[i] = vec->buffer[j];
        vec->buffer[j] = tmp;
    }
}

/**
 * Trims the vector's capacity, in other words, it shrinks the capacity to match
 * the number of elements in the specified vector, however the capacity will
 * never shrink below 1.
 *
 * @param[in] vect the vector whose capacity is being trimmed.
 */
void vector_trim_capacity(Vector *vec)
{
    if (vec->size == vec->capacity)
        return;

    void   **new_buff = vec->mem_calloc(vec->size, sizeof(void*));
    size_t   size     = vec->size < 1 ? 1 : vec->size;
    
    memcpy(new_buff, vec->buffer, size * sizeof(void*));    
    vec->mem_free(vec->buffer);
    
    vec->buffer   = new_buff;
    vec->capacity = vec->size;
}

/**
 * Returns the number of occurrences of the element within the specified vector.
 *
 * @param[in] vect the vector that is being searched
 * @param[in] element the elment that is being searched for
 *
 * @return the number of occurrences of the element
 */
int vector_contains(Vector *vec, void *element)
{
    int o = 0;
    int i;
    for (i = 0; i < vec->size; i++) {
        if (element == vec->buffer[i])
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
size_t vector_size(Vector *vec)
{
    return vec->size;
}

/**
 * Returns the capacity of the specified vector. The capacity of the vector is
 * the maximum number of elements a vector can hold before it has to be resized.
 *
 * @param[in] vect vector whose capacity is being returned
 *
 * @return the capacity of the vector
 */
size_t vector_capacity(Vector *vec)
{
    return vec->capacity;
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
void vector_sort(Vector *vec, int (*cmp) (const void*, const void*))
{
    qsort(vec->buffer, vec->size, sizeof(void*), cmp);
}

/**
 * Expands the vector capacity. This might fail if the the new buffer 
 * cannot be allocated. In case the expansion would overflow the index 
 * range, a maximum capacity buffer is allocated instead. If the capacity
 * is already at the maximum capacity, no new buffer is allocated and 
 * false is returned to indicate the failure.
 *
 * @param[in] vect vector whose capacity is being expanded
 *
 * @return 
 */
static bool expand_capacity(Vector *vec)
{
    if (vec->capacity == MAX_ELEMENTS)
        return false;
    
    size_t new_capacity = vec->capacity * vec->exp_factor;
    
    /* As long as the capacity is greater that the expansion factor
     * at the point of overflow, this is check is valid. */
    if (new_capacity <= vec->capacity)
        vec->capacity = MAX_ELEMENTS;
    else
        vec->capacity = new_capacity;
    
    void **new_buff = vec->mem_alloc(new_capacity * sizeof(void*));

    if (new_buff == NULL)
        return false;

    memcpy(new_buff, vec->buffer, vec->size * sizeof(void*));

    vec->mem_free(vec->buffer);
    vec->buffer = new_buff;

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
void vector_foreach(Vector *vec, void (*op) (void *e))
{
    int i;
    for (i = 0; i < vec->size; i++)
        op(vec->buffer[i]);
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] vec the vector to iterate over
 */
void *vector_iter_init(VectorIter *iter, Vector *vec)
{
    iter->vec   = vec;
    iter->index = 0;
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
    return iter->index < iter->vec->size;
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
    return iter->vec->buffer[iter->index++];
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
    return vector_remove_at(iter->vec, iter->index - 1);
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
    vector_add_at(iter->vec, element, iter->index++);
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
    return vector_replace_at(iter->vec, element, iter->index);
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

