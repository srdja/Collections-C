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
static enum cc_stat expand_capacity (Deque *deque);
static void   copy_buffer     (Deque *deque, void **buff, void *(*cp) (void*));

/**
 * Returns a new empty deque, or NULL if the allocation fails.
 *
 * @return a new deque if the allocation was successful, or NULL if it was not.
 */
enum cc_stat deque_new(Deque **deque)
{
    DequeConf conf;
    deque_conf_init(&conf);
    return deque_new_conf(&conf, deque);
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
enum cc_stat deque_new_conf(DequeConf const * const conf, Deque **d)
{
    Deque *deque = conf->mem_calloc(1, sizeof(Deque));

    if (!deque)
        return CC_ERR_ALLOC;

    if (!(deque->buffer = conf->mem_alloc(conf->capacity * sizeof(void*)))) {
        conf->mem_free(deque);
        return CC_ERR_ALLOC;
    }

    deque->mem_alloc  = conf->mem_alloc;
    deque->mem_calloc = conf->mem_calloc;
    deque->mem_free   = conf->mem_free;
    deque->capacity   = upper_pow_two(conf->capacity);
    deque->first      = 0;
    deque->last       = 0;
    deque->size       = 0;

    *d = deque;
    return CC_OK;
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
enum cc_stat deque_add(Deque *deque, void *element)
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
enum cc_stat deque_add_first(Deque *deque, void *element)
{
    if (deque->size >= deque->capacity && expand_capacity(deque) != CC_OK)
        return CC_ERR_ALLOC;

    deque->first = (deque->first - 1) & (deque->capacity - 1);
    deque->buffer[deque->first] = element;
    deque->size++;

    return CC_OK;
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
enum cc_stat deque_add_last(Deque *deque, void *element)
{
    if (deque->capacity == deque->size && expand_capacity(deque) != CC_OK)
        return CC_ERR_ALLOC;

    deque->buffer[deque->last] = element;
    deque->last = (deque->last + 1) & (deque->capacity - 1);
    deque->size++;

    return CC_OK;
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
enum cc_stat deque_add_at(Deque *deque, void *element, size_t index)
{
    if (index >= deque->size)
        return CC_ERR_OUT_OF_RANGE;

    if (deque->capacity == deque->size && expand_capacity(deque) != CC_OK)
        return CC_ERR_ALLOC;

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

    return CC_OK;
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
enum cc_stat deque_replace_at(Deque *deque, void *element, size_t index, void **out)
{
    if (index >= deque->size)
        return CC_ERR_OUT_OF_RANGE;

    size_t i = (deque->first + index) & (deque->capacity - 1);

    if (out)
        *out = deque->buffer[i];

    deque->buffer[i] = element;

    return CC_OK;
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
enum cc_stat deque_remove(Deque *deque, void *element, void **out)
{
    size_t index;
    enum cc_stat status = deque_index_of(deque, element, &index);

    if (status != CC_OK)
        return status;

    return deque_remove_at(deque, index, out);
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
enum cc_stat deque_remove_at(Deque *deque, size_t index, void **out)
{
    if (index >= deque->size || index == CC_ERR_OUT_OF_RANGE)
        return CC_ERR_OUT_OF_RANGE;

    const size_t c = deque->capacity - 1;
    const size_t l = deque->last & c;
    const size_t f = deque->first & c;
    const size_t p = (deque->first + index) & c;

    void *removed  = deque->buffer[index];

    if (index == 0)
        return deque_remove_first(deque, out);

    if (index == c)
        return deque_remove_last(deque, out);

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

    if (out)
        *out = removed;
    return CC_OK;
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
enum cc_stat deque_remove_first(Deque *deque, void **out)
{
    if (deque->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    void *element = deque->buffer[deque->first];
    deque->first = (deque->first + 1) & (deque->capacity - 1);
    deque->size--;

    if (out)
        *out = element;

    return CC_OK;
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
enum cc_stat deque_remove_last(Deque *deque, void **out)
{
    if (deque->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    size_t  last    = (deque->last - 1) & (deque->capacity - 1);
    void   *element = deque->buffer[last];
    deque->last = last;
    deque->size--;

    if (out)
        *out = element;

    return CC_OK;
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
enum cc_stat deque_get(Deque *deque, size_t index, void **out)
{
    if (index > deque->size)
        return CC_ERR_OUT_OF_RANGE;

    size_t i = (deque->first + index) & (deque->capacity - 1);
    *out = deque->buffer[i];
    return CC_OK;
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
enum cc_stat deque_get_first(Deque *deque, void **out)
{
    if (deque->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    *out = deque->buffer[deque->first];
    return CC_OK;
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
enum cc_stat deque_get_last(Deque *deque, void **out)
{
    if (deque->size == 0)
        return CC_ERR_OUT_OF_RANGE;

    size_t last = (deque->last - 1) & (deque->capacity - 1);
    *out = deque->buffer[last];
    return CC_OK;
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
enum cc_stat deque_copy_shallow(Deque *deque, Deque **out)
{
    Deque *copy = deque->mem_alloc(sizeof(Deque));

    if (!copy)
        return CC_ERR_ALLOC;

    if (!(copy->buffer = deque->mem_alloc(deque->capacity * sizeof(void*)))) {
        deque->mem_free(copy);
        return CC_ERR_ALLOC;
    }
    copy->size       = deque->size;
    copy->capacity   = deque->capacity;
    copy->mem_alloc  = deque->mem_alloc;
    copy->mem_calloc = deque->mem_calloc;
    copy->mem_free   = deque->mem_free;

    copy_buffer(deque, copy->buffer, NULL);

    copy->first = 0;
    copy->last  = copy->size;

    *out = copy;
    return CC_OK;
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
enum cc_stat deque_copy_deep(Deque *deque, void *(*cp) (void*), Deque **out)
{
    Deque *copy = deque->mem_alloc(sizeof(Deque));

    if (!copy)
        return CC_ERR_ALLOC;

    if (!(copy->buffer = deque->mem_alloc(deque->capacity * sizeof(void*)))) {
        deque->mem_free(copy);
        return CC_ERR_ALLOC;
    }

    copy->size       = deque->size;
    copy->capacity   = deque->capacity;
    copy->mem_alloc  = deque->mem_alloc;
    copy->mem_calloc = deque->mem_calloc;
    copy->mem_free   = deque->mem_free;

    copy_buffer(deque, copy->buffer, cp);

    copy->first = 0;
    copy->last  = copy->size;

    *out = copy;

    return CC_OK;
}

/**
 * Trims the capacity of the deque to a power of 2 that is the nearest to
 * the number of elements in the deque.
 *
 * @param[in] deque the deque on which this operation is being performed
 */
enum cc_stat deque_trim_capacity(Deque *deque)
{
    if (deque->capacity == deque->size)
        return CC_OK;

    size_t new_size = upper_pow_two(deque->size);

    if (new_size == deque->capacity)
        return CC_OK;

    void **new_buff = deque->mem_alloc(sizeof(void*) * new_size);

    if (!new_buff)
        return CC_ERR_ALLOC;

    copy_buffer(deque, new_buff, NULL);

    deque->buffer   = new_buff;
    deque->first    = 0;
    deque->last     = deque->size;
    deque->capacity = new_size;
    return CC_OK;
}

/**
 * Reverses the order of elements in the specified deque.
 *
 * @param[in] deque the deque that is being reversed
 */
void deque_reverse(Deque *deque)
{
    size_t i;
    size_t j;
    size_t s = deque->size;
    size_t c = deque->capacity - 1;

    size_t first = deque->first;

    for (i = 0, j = s - 1; i < (s - 1) / 2; i++, j--) {
        size_t f = (first + i) & c;
        size_t l = (first + j) & c;

        void *tmp = deque->buffer[f];
        deque->buffer[f] = deque->buffer[l];
        deque->buffer[l] = tmp;
    }
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
 * CC_ERR_OUT_OF_RANGE if the element could not be found.
 *
 * @param[in] deque deque being searched
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element, or NO_SUCH_INDEX if the element is
 *         not found
 */
enum cc_stat deque_index_of(Deque *deque, void *element, size_t *index)
{
    size_t i;
    size_t p;

    for (i = 0; i < deque->size; i++) {
        p = (deque->first + i) & (deque->capacity - 1);
        if (deque->buffer[p] == element) {
            *index = i;
            return CC_OK;
        }
    }
    return CC_ERR_OUT_OF_RANGE;
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
 * @param[in] deque the deque whose underlying buffer is being returned
 *
 * @return the buffer
 */
const void* const *deque_get_buffer(Deque *deque)
{
    return (const void* const*) deque->buffer;
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
static enum cc_stat expand_capacity(Deque *deque)
{
    if (deque->capacity == MAX_POW_TWO)
        return CC_ERR_MAX_CAPACITY;

    size_t new_capacity = deque->capacity << 1;
    void **new_buffer = deque->mem_calloc(new_capacity, sizeof(void*));

    if (!new_buffer)
        return CC_ERR_ALLOC;

    copy_buffer(deque, new_buffer, NULL);

    deque->first    = 0;
    deque->last     = deque->size;
    deque->capacity = new_capacity;
    deque->buffer   = new_buffer;

    return CC_OK;
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
 * Returns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
enum cc_stat deque_iter_next(DequeIter *iter, void **out)
{
    const size_t c     = (iter->deque->capacity - 1);
    const size_t last  = (iter->deque->last) & c;
    const size_t first = (iter->deque->first) & c;

    if (last == first || iter->index >= iter->deque->size)
        return CC_ITER_END;

    const size_t i = (iter->deque->first + iter->index) & c;

    iter->index++;
    if (out)
        *out = iter->deque->buffer[i];

    return CC_OK;
}

/**
 * Removes and returns the last returned element by <code>deque_iter_next()
 * </code> without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being perfrormed
 *
 * @return the removed element
 */
enum cc_stat deque_iter_remove(DequeIter *iter, void **out)
{
    void *rm;
    enum cc_stat status = deque_remove_at(iter->deque, iter->index, &rm);
    if (status == 0)
        iter->index--;

    if (out)
        *out = rm;

    return status;
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
enum cc_stat deque_iter_add(DequeIter *iter, void *element)
{
    enum cc_stat status = deque_add_at(iter->deque, element, iter->index - 1);
    if (status == 0)
        iter->index++;

    return status;
}

/**
 * Replaces the last returned element by <code>deque_iter_next()</code>
 * with the specified replacement element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] replacement the replacement element
 *
 * @return the old element that was replaced
 */
enum cc_stat deque_iter_replace(DequeIter *iter, void *replacement, void **out)
{
    return deque_replace_at(iter->deque, replacement, iter->index, out);
}

/**
 * Returns the index of the last returned element by <code>deque_iter_next()
 * </code>.
 *
 * @note
 * This function should not be called before a call to <code>deque_iter_next()
 * </code>
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index
 */
size_t deque_iter_index(DequeIter *iter)
{
    return iter->index - 1;
}
