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

#include "deque.h"

#define DEFAULT_CAPACITY 8
#define DEFAULT_EXPANSION_FACTOR 2

struct deque_s {
    size_t   size;
    size_t   capacity;
    size_t   first;
    size_t   last;
    void   **buffer;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

static size_t upper_pow_two   (size_t);
static bool   expand_capacity (Deque *deque);
static void   copy_buffer     (Deque *deque, void **buff, void *(*cp) (void*));

/**
 * Returns a new empty deque, or NULL if the allocation fails.
 *
 * @return a new deque if the allocation was successful, or NULL if it was not.
 */
Deque *deque_new()
{
    DequeConf conf;
    deque_conf_init(&conf);
    return deque_new_conf(&conf);
}

/**
 * Returns a new empty deque based on the specified DequeConf object.
 *
 * The deque is allocated using the allocators specified the DequeConf object
 * The allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf Deque configuration object. All fields must be initialized
 *                 appropriate values.
 *
 * @return a new deque if the allocation was successful, or NULL if not.
 */
Deque *deque_new_conf(DequeConf *conf)
{
    Deque *deque = conf->mem_calloc(1, sizeof(Deque));

    if (deque == NULL)
        return NULL;

    deque->mem_alloc  = conf->mem_alloc;
    deque->mem_calloc = conf->mem_calloc;
    deque->mem_free   = conf->mem_free;
    deque->capacity   = upper_pow_two(conf->capacity);
    deque->first      = 0;
    deque->last       = 0;
    deque->size       = 0;
    deque->buffer     = deque->mem_alloc(deque->capacity * sizeof(void*));

    return deque;
}

/**
 * Initializes the fields of the DequeConf struct to default values.
 *
 * @param[in, out] conf the configuration object that is being initialized
 */
void deque_conf_init(DequeConf *conf)
{
    conf->capacity   = DEFAULT_CAPACITY;
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Destroys the deque structure, but leaves the data it used to hold, intact.
 *
 * @param[in] deque the deque that is to be destroyed.
 */
void deque_destroy(Deque *deque)
{
    deque->mem_free(deque->buffer);
    deque->mem_free(deque);
}

/**
 * Destroys the deque structure along with all the data it holds.
 *
 * @note
 * This function should not be called on a deque that has some of its elements
 * allocated on the stack.
 *
 * @param[in] deque the deque that is being destroyed
 */
void deque_destroy_free(Deque *deque)
{
    deque_remove_all_free(deque);
    deque_destroy(deque);
}

/**
 * Adds a new element to the deque. The element is appended to the tail of the
 * deque. This function returns a <code>bool</code> based on whether or not the
 * space allocation for the new element was successful or not.
 *
 * @param[in] deque the deque to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return true if the operation was successful
 */
bool deque_add(Deque *deque, void *element)
{
    return deque_add_last(deque, element);
}

/**
 * Adds a new element to the front of the deque. This function returns a
 * <code>bool</code> based on whether the memory allocation for the new element
 * was successful or not.
 *
 * @param[in] deque the deque to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return true if the operation was successful
 */
bool deque_add_first(Deque *deque, void *element)
{
    if (deque->size >= deque->capacity && !expand_capacity(deque))
        return false;

    deque->first = (deque->first - 1) & (deque->capacity - 1);
    deque->buffer[deque->first] = element;
    deque->size++;

    return true;
}

/**
 * Adds a new element to the back of the deque. This function returns a
 * <code>bool</code> based on whether the memory allocation for the new element
 * was successful or not.
 *
 * @param[in] deque the deque to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return true if the operation was successful
 */
bool deque_add_last(Deque *deque, void *element)
{
    if (deque->capacity == deque->size && !expand_capacity(deque))
        return false;

    deque->buffer[deque->last] = element;
    deque->last = (deque->last + 1) & (deque->capacity - 1);
    deque->size++;

    return true;
}

/**
 * Inserts a new element at the specified index within the deque. The index
 * must be within the range of the deque. This function may also fail if the
 * memory allocation for the new element was not successful.
 *
 * @param[in] deque the deque to which this new element is being added
 * @param[in] element the element that is being added
 * @param[in] index the position within the dequeu at which this new element is
 *                  is being added
 *
 * @return true if the the operation was successful
 */
bool deque_add_at(Deque *deque, void *element, size_t index)
{
    if (index >= deque->size)
        return false;

    if (deque->capacity == deque->size && !expand_capacity(deque))
        return false;

    const size_t c = deque->capacity - 1;
    const size_t l = deque->last & c;
    const size_t f = deque->first & c;
    const size_t p = (deque->first + index) & c;

    if (index == 0)
        return deque_add_first(deque, element);

    if (index == c)
        return deque_add_last(deque, element);

    if (index <= (deque->size / 2) - 1) {
        if (p < f || f == 0) {
            /* _________________________________
             * | 1 | 2 | 3 | 4 | 5 | . | . | 6 |
             * ---------------------------------
             *  (p) <--          L           F
             *
             * Left circular shift from (p)
             */
            const size_t r_move = (f != 0) ? c - f + 1 : 0;
            const size_t l_move = p;

            void *e_first = deque->buffer[0];

            if (f != 0) {
                memmove(&(deque->buffer[f - 1]),
                        &(deque->buffer[f]),
                        r_move * sizeof(void*));
            }
            if (p != 0) {
                memmove(&(deque->buffer[0]),
                        &(deque->buffer[1]),
                        l_move * sizeof(void*));
            }
            deque->buffer[c] = e_first;
        } else {
            memmove(&(deque->buffer[f - 1]),
                    &(deque->buffer[f]),
                    index * sizeof(void*));
        }
        deque->first = (deque->first - 1) & c;
    } else {
        if (p > l || l == c) {
            /* _________________________________
             * | 1 | . | . | 6 | 5 | 4 | 3 | 2 |
             * ---------------------------------
             *   L           F          (p) -->
             *
             * Circular right shift from (p)
             */
            void* e_last = deque->buffer[c];

            if (p != c) {
                memmove(&(deque->buffer[p + 1]),
                        &(deque->buffer[p]),
                        (c - p) * sizeof(void*));
            }
            if (l != c) {
                memmove(&(deque->buffer[1]),
                        &(deque->buffer[0]),
                        (l + 1) * sizeof(void*));
            }
            deque->buffer[0] = e_last;
        } else {
            memmove(&(deque->buffer[p + 1]),
                    &(deque->buffer[p]),
                    (deque->size - index) * sizeof(void*));
        }
        deque->last = (deque->last + 1) & c;
    }
    deque->buffer[p] = element;
    deque->size++;

    return true;
}

/**
 * Replaces a deque element at the specified index and returns the replaced
 * element. The specified index must be within the bounds of the deque,
 * otherwise NULL is returned. NULL may also be returned if the replaced element
 * was NULL. In this case calling <code>deque_contains()</code> before this
 * function can resolve this ambiguity.
 *
 * @param[in] deque the deque whose element is being replaced
 * @param[in] element the replacement element
 * @param[in] index the index at which the replacement element should be inserted
 *
 * @return replaced element, or NULL if the index was out of bounds.
 */
void *deque_replace_at(Deque *deque, void *element, size_t index)
{
    if (index >= deque->size)
        return NULL;

    size_t  i   = (deque->first + index) & (deque->capacity - 1);
    void   *old = deque->buffer[i];

    deque->buffer[i] = element;

    return old;
}

/**
 * Removes and returns the specified element from the deque if such element
 * exists. In case the element doesn't exist NULL is retruned. NULL may also
 * be returned if the specified element is NULL. In this case calling <code>
 * deque_contains()</code> before this function can resolve the ambiguity.
 *
 * @param[in] deque the deque from which the element is being removed
 * @param[in] element the element being removed
 *
 * @return removed element, or NULL if the operation fails
 */
void *deque_remove(Deque *deque, void *element)
{
    size_t index = deque_index_of(deque, element);

    if (index == NO_SUCH_INDEX)
        return NULL;

    return deque_remove_at(deque, index);
}

/**
 * Removes and returns a deque element from the specified index. The index must
 * be within the bounds of the deque, otherwise NULL is returned. NULL may also
 * be returned if the removed element was NULL. To resolve this ambiguity call
 * <code>deque_contains()</code> before this function.
 *
 * @param[in] deque the deque from which the element is being removed
 * @param[in] index the index of the element being removed
 *
 * @return the removed element, or NULL if the operation fails
 */
void *deque_remove_at(Deque *deque, size_t index)
{
    if (index >= deque->size || index == NO_SUCH_INDEX)
        return NULL;

    const size_t c = deque->capacity - 1;
    const size_t l = deque->last & c;
    const size_t f = deque->first & c;
    const size_t p = (deque->first + index) & c;

    void *removed  = deque->buffer[index];

    if (index == 0)
        return deque_remove_first(deque);

    if (index == c)
        return deque_remove_last(deque);

    if (index <= (deque->size / 2) - 1) {
        if (p < f) {
            void *e = deque->buffer[c];

            if (f != c) {
                memmove(&(deque->buffer[f + 1]),
                        &(deque->buffer[f]),
                        (c - f) * sizeof(void*));
            }
            if (p != 0) {
                memmove(&(deque->buffer[1]),
                        &(deque->buffer[0]),
                        p * sizeof(void*));
            }
            deque->buffer[0] = e;
        } else {
            memmove(&(deque->buffer[f + 1]),
                    &(deque->buffer[f]),
                    index * sizeof(void*));
        }
        deque->first = (deque->first + 1) & c;
    } else {
        if (p > l) {
            void *e = deque->buffer[0];

            if (p != c) {
                memmove(&(deque->buffer[p]),
                        &(deque->buffer[p + 1]),
                        (c - p) * sizeof(void*));
            }
            if (p != 0) {
                memmove(&(deque->buffer[1]),
                        &(deque->buffer[0]),
                        l * sizeof(void*));
            }
            deque->buffer[c] = e;
        } else {
            memmove(&(deque->buffer[p]),
                    &(deque->buffer[p + 1]),
                    (l - p) * sizeof(void*));
        }
        deque->last = (deque->last- 1) & c;
    }
    deque->size--;

    return removed;
}

/**
 * Removes and returns the element from the front of the deque, or NULL if
 * the deque is empty. NULL may also be returned if the first element in the
 * deque is of NULL value. This ambiguity can be resolved by calling
 * <code>deque_size()</code> before this function.
 *
 * @param[in] deque the deque whose first element (or head) is being removed
 *
 * @return the removed element
 */
void* deque_remove_first(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    void *element = deque->buffer[deque->first];
    deque->first = (deque->first + 1) & (deque->capacity - 1);
    deque->size--;

    return element;
}

/**
 * Removes and returns the element from the back of the deque, or NULL if
 * the deque is empty. NULL may also be returned if the last element in the
 * deque is of NULL value. This ambiguity can be resolved by calling <code>
 * deque_size()</code> before this function.
 *
 * @param[in] deque the deque whose last element (or tail) is being removed
 *
 * @return the removed element
 */
void* deque_remove_last(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    size_t  last    = (deque->last - 1) & (deque->capacity - 1);
    void   *element = deque->buffer[last];
    deque->last = last;
    deque->size--;

    return element;
}

/**
 * Removes all elements from the specified deque.
 *
 * @note This function does not shrink the deque capacity.

 * @param[in] deque the deque from which all element are being removed
 */
void deque_remove_all(Deque *deque)
{
    deque->first = 0;
    deque->last  = 0;
    deque->size  = 0;
}

/**
 * Removes and frees all element from the specified deque.
 *
 * @note This function does not shrink the deque capacity.
 * @note This function should not be called on deques that have some
 *       of their element allocated on the stack.
 *
 * @param[in] deque the deque from which all elements are being removed
 */
void deque_remove_all_free(Deque *deque)
{
    deque_foreach(deque, deque->mem_free);
    deque_remove_all(deque);
}

/**
 * Returns a deque element from the specified index. The specified index must
 * be withing the bounds of the deque, otherwise NULL is returned. NULL can also
 * be returned if the element at the specified index is NULL. This ambiguity can
 * be resolved by calling <code>deque_contains()</code> before this function.
 *
 * @param[in] deque the deque from which the element is being returned
 * @param[in] index the index of the deque element
 *
 * @return element at the specified index, or NULL if the operation has
 *         failed.
 */
void* deque_get(Deque *deque, size_t index)
{
    if (index > deque->size)
        return NULL;

    size_t i = (deque->first + index) & (deque->capacity - 1);
    return deque->buffer[i];
}

/**
 * Returns the first (head) element of the deque or NULL if the deque is empty.
 * NULL may also be returned if the first element of the deque is of NULL value.
 * This ambiguity can be resolved by calling <code>deque_size()</code> before this
 * function.
 *
 * @param[in] deque the deque whose first element is being returned
 *
 * @return the first element of the specified deque
 */
void* deque_get_first(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    return deque->buffer[deque->first];
}

/**
 * Returns the last (tail) element of the deque or NULL if the deque is empty.
 * NULL may also be returned if the first element of the deque is of NULL value.
 * This ambiguity can be resolved by calling <code>deque_size()</code> before this
 * function.
 *
 * @param[in] deque the deque whose last element is being returned
 *
 * @return the last element of the specified deque
 */
void* deque_get_last(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    size_t last = (deque->last - 1) & (deque->capacity - 1);
    return deque->buffer[last];
}

/**
 * Returns a shallow copy of the specified deque. A shallow copy is a copy of
 * the deque structure, but not the elements it holds.
 *
 * @note The new deque is allocated using the original deque's allocators
 *       and also inherits the configuration of the original deque.
 *
 * @param[in] deque the deque to be copied
 *
 * @return a shallow copy of the specified deque
 */
Deque* deque_copy_shallow(Deque *deque)
{
    Deque *copy = deque->mem_alloc(sizeof(Deque));

    copy->size       = deque->size;
    copy->capacity   = deque->capacity;
    copy->mem_alloc  = deque->mem_alloc;
    copy->mem_calloc = deque->mem_calloc;
    copy->mem_free   = deque->mem_free;
    copy->buffer     = copy->mem_alloc(copy->capacity * sizeof(void*));

    copy_buffer(deque, copy->buffer, NULL);

    copy->first = 0;
    copy->last  = copy->size;

    return copy;
}

/**
 * Returns a deep copy of the specified deque. A deep copy is a copy of
 * both the deque structure and the data it holds.
 *
 * @note The new deque is allocated using the original deque's allocators
 *       and also inherits the configuration of the original deque.
 *
 * @param[in] deque the deque to be copied
 * @param[in] cp the copy function that returns a copy of a deque element
 *
 * @return a deep copy of the specified deque
 */
Deque* deque_copy_deep(Deque *deque, void *(*cp) (void*))
{
    Deque *copy = deque->mem_alloc(sizeof(Deque));

    copy->size       = deque->size;
    copy->capacity   = deque->capacity;
    copy->mem_alloc  = deque->mem_alloc;
    copy->mem_calloc = deque->mem_calloc;
    copy->mem_free   = deque->mem_free;
    copy->buffer     = copy->mem_alloc(copy->capacity * sizeof(void*));

    copy_buffer(deque, copy->buffer, cp);

    copy->first = 0;
    copy->last  = copy->size; // XXX

    return copy;
}

/**
 * Returns the number of occurrences of the element within the specified deque.
 *
 * @param[in] deque the deque that is being searched
 * @param[in] element the element that is being searched for
 *
 * @return the number of occurrences of the element
 */
size_t deque_contains(Deque *deque, void *element)
{
    size_t i;
    size_t p;
    size_t o = 0;

    for (i = 0; i < deque->size; i++) {
        p = (deque->first + i) & (deque->capacity - 1);
        if (deque->buffer[p] == element)
            o++;
    }
    return o;
}

/**
 * Returns the index of the first occurence of the specified deque element, or
 * NO_SUCH_INDEX if the element could not be found.
 *
 * @param[in] deque deque being searched
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element, or NO_SUCH_INDEX if the element is
 *         not found
 */
size_t deque_index_of(Deque *deque, void *element)
{
    size_t i;
    size_t p;

    for (i = 0; i < deque->size; i++) {
        p = (deque->first + i) & (deque->capacity - 1);
        if (deque->buffer[p] == element)
            return i;
    }
    return NO_SUCH_INDEX;
}

/**
 * Returns the size of the specified deque. The size of the deque is the
 * number of element contained within the deque.
 *
 * @param[in] deque the deque whose size is being returned
 *
 * @return the number of element within the specified deque
 */
size_t deque_size(Deque *deque)
{
    return deque->size;
}

/**
 * Retruns the capacity of the specified deque. The capacity of the deque is
 * the maximum number of elements a deque can hold before it's underlying buffer
 * needs to be resized.
 *
 * @param[in] deque deque whose capacity is being returned
 *
 * @return the capacity of the specified deque
 */
size_t deque_capacity(Deque *deque)
{
    return deque->capacity;
}

/**
 * Return the underlying deque buffer.
 *
 * @note Any direct modification of the buffer may invalidate the deque.
 *
 * @param[in] deque the deque whose underlying buffer is being returned
 *
 * @return the buffer
 */
void **deque_get_buffer(Deque *deque)
{
    return deque->buffer;
}

/**
 * A 'foreach loop' function that invokes the specified function on each element
 * of the deque.
 *
 * @param[in] deque the deque on which this operation is performed
 * @param[in] op    the operation function that is to be invoked on each deque
 *                  element
 */
void deque_foreach(Deque *deque, void (*op) (void *))
{
    size_t i;
    size_t p = 0;

    for (i = 0; i < deque->size; i++) {
        p = (deque->first + i) & (deque->capacity - 1);
        op(deque->buffer[p]);
    }
}

/**
 * Only copy the elements instead of the whole buffer and realign the buffer.
 *
 *
 * @param[in] deque The deque whose buffer is being copied to the new buffer
 * @param[in] buff The destination buffer. This buffer is expected to be at least the
 *            capacity of the original deques buffer or greater.
 * @param[in] cp An optional copy function that returns a copy of the element passed to it.
 *            If NULL is passed, then only a shallow copy will be performed.
 */
static void copy_buffer(Deque *deque, void **buff, void *(*cp) (void *))
{
    if (cp == NULL) {
        if (deque->last > deque->first) {
            memcpy(buff,
                   &(deque->buffer[deque->first]),
                   deque->size * sizeof(void*));
        } else {
            size_t l = deque->last;
            size_t e = deque->capacity - deque->first;

            memcpy(buff,
                   &(deque->buffer[deque->first]),
                   e * sizeof(void*));

            memcpy(&(buff[e]),
                   deque->buffer,
                   l * sizeof(void*));
        }
    } else {
        size_t i;
        for (i = 0; i < deque->size; i++) {
            size_t p = (deque->first + i) & (deque->capacity - 1);
            buff[i]  = cp(deque->buffer[p]);
        }
    }
}

/**
 * Expands the deque capacity. This might operation might fail if the new
 * buffer cannot be allocated. If the capacity is already the maximum capacity
 * no new buffer is allocated and false is returned to indicated failure.
 *
 * @param[in] deque the deque whose capacity is being expanded
 *
 * @return true if the operation was successful
 */
static bool expand_capacity(Deque *deque)
{
    if (deque->capacity == MAX_POW_TWO)
        return false;

    size_t   new_capacity = deque->capacity << 1;
    void   **new_buffer   = deque->mem_calloc(new_capacity, sizeof(void*));

    copy_buffer(deque, new_buffer, NULL);

    deque->first    = 0;
    deque->last     = deque->size;
    deque->capacity = new_capacity;
    deque->buffer   = new_buffer;

    return true;
}

/**
 * Rounds the integer to the nearest upper power of two.
 *
 * @param[in] the unsigned integer that is being rounded
 *
 * @return the nearest upper power of two
 */
static INLINE size_t upper_pow_two(size_t n)
{
    if (n >= MAX_POW_TWO)
        return MAX_POW_TWO;

    if (n == 0)
        return 2;

    /**
     * taken from:
     * http://graphics.stanford.edu/~seander/
     * bithacks.html#RoundUpPowerOf2Float
     */
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;

    return n;
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] deque the vector to iterate over
 */
void deque_iter_init(DequeIter *iter, Deque *deque)
{
    iter->deque = deque;
    iter->index = 0;
}

/**
 * Checks wheteher or not the iterator has reached the end of the deque
 *
 * @param[in] iter iterator whose position is being checked
 *
 * @return true if there are more elemnets to be iterated over
 */
bool deque_iter_has_next(DequeIter *iter)
{
    const size_t last = (iter->deque->last) &
                        (iter->deque->capacity - 1);

    if (iter->index < last) {
        return true;
    }
    return false;
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
void *deque_iter_next(DequeIter *iter)
{
    const size_t i = iter->index & (iter->deque->capacity - 1);
    iter->index++;
    return iter->deque->buffer[i];
}

/**
 * Removes and returns the last returned element by <code>deque_iter_next()
 * </code> without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being perfrormed
 *
 * @return the removed element
 */
void *deque_iter_remove(DequeIter *iter)
{
    return deque_remove_at(iter->deque, iter->index);
}

/**
 * Adds a new element tothe deque after the last returned element by
 * <code>deque_iter_next()</code> without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added
 *
 * @return true if the elmenet was successfully added or false if the allocation
 *         for the new element failed.
 */
bool deque_iter_add(DequeIter *iter, void *element)
{
    return deque_add_at(iter->deque, element, iter->index++);
}

/**
 * Replaces the last returned element by <code>deque_iter_next()</code>
 * with the specified replacement element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] replacemenet the replacement element
 *
 * @return the old element that was replaced
 */
void *deque_iter_replace(DequeIter *iter, void *replacement)
{
    return deque_replace_at(iter->deque, replacement, iter->index);
}
