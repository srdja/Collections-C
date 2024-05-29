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


#include "sized/cc_array_sized.h"

#define DEFAULT_CAPACITY 8
#define DEFAULT_EXPANSION_FACTOR 2

#define INDEX(a, i) a->data_length * i
#define BUF_ADDR(a, i) &a->buffer[a->data_length * i]


struct cc_array_sized_s {
    size_t   data_length;
    size_t   size;
    size_t   capacity;
    float    exp_factor;
    uint8_t *buffer;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

static enum cc_stat expand_capacity(CC_ArraySized *ar);


/**
 * Creates a new empty array and returns a status code.
 *
 * @param[in] Size of the array element in bytes
 * @param[out] out pointer to where the newly created CC_ArraySized is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new CC_ArraySized structure failed.
 */
enum cc_stat cc_array_sized_new(size_t element_size, CC_ArraySized **out)
{
    CC_ArraySizedConf c;
    cc_array_sized_conf_init(&c);
    return cc_array_sized_new_conf(element_size, &c, out);
}

/**
 * Creates a new empty CC_ArraySized based on the specified CC_ArraySizedConf struct 
 * and returns a status code.
 *
 * The CC_ArraySized is allocated using the allocators specified in the CC_ArraySizedConf
 * struct. The allocation may fail if underlying allocator fails. It may also
 * fail if the values of exp_factor and capacity in the CC_ArraySizedConf do not meet
 * the following condition: <code>exp_factor < (CC_MAX_ELEMENTS / capacity)</code>.
 *
 * @param[in] element_size the size of the data being stored in bytes
 * @param[in] conf array configuration structure
 * @param[out] out pointer to where the newly created CC_ArraySized is to be stored
 *
 * @return CC_OK if the creation was successful, CC_ERR_INVALID_CAPACITY if
 * the above mentioned condition is not met, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_ArraySized structure failed.
 */
enum cc_stat cc_array_sized_new_conf(
    size_t element_size, 
    CC_ArraySizedConf const * const conf, 
    CC_ArraySized **out)
{
    float ex;

    /* The expansion factor must be greater than one for the
     * array to grow */
    if (conf->exp_factor <= 1)
        ex = DEFAULT_EXPANSION_FACTOR;
    else
        ex = conf->exp_factor;

    /* Needed to avoid an integer overflow on the first resize and
     * to easily check for any future overflows. */
    if (!conf->capacity || ex >= CC_MAX_ELEMENTS / conf->capacity)
        return CC_ERR_INVALID_CAPACITY;

    CC_ArraySized *ar = conf->mem_calloc(1, sizeof(CC_ArraySized));

    if (!ar)
        return CC_ERR_ALLOC;

    uint8_t *buff = conf->mem_alloc(conf->capacity * element_size);

    if (!buff) {
        conf->mem_free(ar);
        return CC_ERR_ALLOC;
    }

    ar->data_length = element_size;
    ar->buffer      = buff;
    ar->exp_factor  = ex;
    ar->capacity    = conf->capacity;
    ar->mem_alloc   = conf->mem_alloc;
    ar->mem_calloc  = conf->mem_calloc;
    ar->mem_free    = conf->mem_free;

    *out = ar;
    return CC_OK;
}

/**
 * Initializes the fields of the CC_ArraySizedConf struct to default values.
 *
 * @param[in, out] conf CC_ArraySizedConf structure that is being initialized
 */
void cc_array_sized_conf_init(CC_ArraySizedConf *conf)
{
    conf->exp_factor = DEFAULT_EXPANSION_FACTOR;
    conf->capacity   = DEFAULT_CAPACITY;
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Destroys the CC_ArraySized structure, but leaves the data it used to hold intact.
 *
 * @param[in] ar the array that is to be destroyed
 */
void cc_array_sized_destroy(CC_ArraySized *ar)
{
    ar->mem_free(ar->buffer);
    ar->mem_free(ar);
}


/**
 * Adds a new element to the CC_ArraySized. The element is appended to the array making
 * it the last element (the one with the highest index) of the CC_ArraySized.
 *
 * @param[in] ar the array to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return CC_OK if the element was successfully added, CC_ERR_ALLOC if the
 * memory allocation for the new element failed, or CC_ERR_MAX_CAPACITY if the
 * array is already at maximum capacity.
 */
enum cc_stat cc_array_sized_add(CC_ArraySized *ar, uint8_t *element)
{
    if (ar->size >= ar->capacity) {
        enum cc_stat status = expand_capacity(ar);
        if (status != CC_OK)
            return status;
    }

    memcpy(BUF_ADDR(ar, ar->size), element, ar->data_length);
    ar->size++;

    return CC_OK;
}

/**
 * Adds a new element to the array at a specified position by shifting all
 * subsequent elements by one. The specified index must be within the bounds
 * of the array. This function may also fail if the memory allocation for
 * the new element was unsuccessful.
 *
 * @param[in] ar the array to which the element is being added
 * @param[in] element the element that is being added
 * @param[in] index the position in the array at which the element is being
 *            added
 *
 * @return CC_OK if the element was successfully added, CC_ERR_OUT_OF_RANGE if
 * the specified index was not in range, CC_ERR_ALLOC if the memory
 * allocation for the new element failed, or CC_ERR_MAX_CAPACITY if the
 * array is already at maximum capacity.
 */
enum cc_stat cc_array_sized_add_at(CC_ArraySized *ar, uint8_t *element, size_t index)
{
    if (index == ar->size)
        return cc_array_sized_add(ar, element);

    if ((ar->size == 0 && index != 0) || index > (ar->size - 1))
        return CC_ERR_OUT_OF_RANGE;

    if (ar->size >= ar->capacity) {
        enum cc_stat status = expand_capacity(ar);
        if (status != CC_OK)
            return status;
    }

    size_t shift = (ar->size - index) * ar->data_length;

    memmove(BUF_ADDR(ar, (index + 1)),
            BUF_ADDR(ar, index),
            shift);

    memcpy(BUF_ADDR(ar, index), element, ar->data_length);
    ar->size++;

    return CC_OK;
}

/**
 * Replaces an array element at the specified index and optionally sets the out
 * parameter to the value of the replaced element. The specified index must be
 * within the bounds of the CC_ArraySized.
 *
 * @param[in]  ar      array whose element is being replaced
 * @param[in]  element replacement element
 * @param[in]  index   index at which the replacement element should be inserted
 * @param[out] out     pointer to where the replaced element is stored, or NULL if
 *                     it is to be ignored
 *
 * @return CC_OK if the element was successfully replaced, or CC_ERR_OUT_OF_RANGE
 *         if the index was out of range.
 */
enum cc_stat cc_array_sized_replace_at(CC_ArraySized *ar, uint8_t *element, size_t index, uint8_t *out)
{
    if (index >= ar->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    if (out) {
        memcpy(out, BUF_ADDR(ar, index), ar->data_length);
    }
    memcpy(BUF_ADDR(ar, index), element, ar->data_length);

    return CC_OK;
}

/**
* Swaps the values at the specified indices.
*
* @param[in] ar array whose elements are being swapped
* @param[in] index1 first position
* @param[in] index2 second position
* 
* @return CC_OK if the operation was successful, or CC_ERR_OUT_OF_RANGE if 
* one of the indices was out of range.
*/
enum cc_stat cc_array_sized_swap_at(CC_ArraySized *ar, size_t index1, size_t index2)
{
    if (index1 >= ar->size || index2 >= ar->size) {
        return CC_ERR_OUT_OF_RANGE;
    }

    for (size_t i = 0; i < ar->data_length; i++) {
        uint8_t tmp = ar->buffer[INDEX(ar, index1 + i)];
        ar->buffer[INDEX(ar, index1 + i)] = ar->buffer[INDEX(ar, index2 + i)];
        ar->buffer[INDEX(ar, index2 + i)] = tmp;
    }
    return CC_OK;
}

/**
 * Removes the specified element from the CC_ArraySized if such element exists and
 * optionally sets the out parameter to the value of the removed element.
 *
 * @param[in] ar array from which the element is being removed
 * @param[in] element element being removed
 * @param[out] out pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND if the element was not found.
 */
enum cc_stat cc_array_sized_remove(CC_ArraySized *ar, uint8_t *element)
{
    size_t index;
    enum cc_stat status = cc_array_sized_index_of(ar, element, &index);

    if (status == CC_ERR_OUT_OF_RANGE) {
        return CC_ERR_VALUE_NOT_FOUND;
    }

    if (index != ar->size - 1) {
        size_t block_size = (ar->size - 1 - index) * ar->data_length;

        memmove(BUF_ADDR(ar, index),
                BUF_ADDR(ar, (index + 1)),
                block_size);
    }
    ar->size--;
 
    return CC_OK;
}

/**
 * Removes an CC_ArraySized element from the specified index and optionally sets the
 * out parameter to the value of the removed element. The index must be within
 * the bounds of the array.
 *
 * @param[in] ar the array from which the element is being removed
 * @param[in] index the index of the element being removed.
 * @param[out] out  pointer to where the removed value is stored,
 *                  or NULL if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or CC_ERR_OUT_OF_RANGE
 * if the index was out of range.
 */
enum cc_stat cc_array_sized_remove_at(CC_ArraySized *ar, size_t index, uint8_t *out)
{
    if (index >= ar->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    if (out) {
        memcpy(out, BUF_ADDR(ar, index), ar->data_length);
    }

    if (index != ar->size - 1) {
        size_t block_size = (ar->size - 1 - index) * ar->data_length;

        memmove(BUF_ADDR(ar, index),
                BUF_ADDR(ar, (index + 1)),
                block_size);
    }
    ar->size--;

    return CC_OK;
}

/**
 * Removes an CC_ArraySized element from the end of the array and optionally sets the
 * out parameter to the value of the removed element.
 *
 * @param[in] ar the array whose last element is being removed
 * @param[out] out pointer to where the removed value is stored, or NULL if it is
 *                 to be ignored
 *
 * @return CC_OK if the element was successfully removed, or CC_ERR_OUT_OF_RANGE
 * if the CC_Array is already empty.
 */
enum cc_stat cc_array_sized_remove_last(CC_ArraySized *ar, uint8_t *out)
{
    return cc_array_sized_remove_at(ar, ar->size - 1, out);
}

/**
 * Removes all elements from the specified array. This function does not shrink
 * the array capacity.
 *
 * @param[in] ar array from which all elements are to be removed
 */
void cc_array_sized_remove_all(CC_ArraySized *ar)
{
    ar->size = 0;
}

/**
 * Copies an CC_ArraySized element from the specified index to the out pointer.
 * The specified index must be within the bounds of the array.
 *
 * @param[in] ar the array from which the element is being retrieved
 * @param[in] source index of the array element
 * @param[out] destination address
 *
 * @return CC_OK if the element was found, or CC_ERR_OUT_OF_RANGE if the index
 * was out of range.
 */
enum cc_stat cc_array_sized_get_at(CC_ArraySized *ar, size_t index, uint8_t *out)
{
    if (index >= ar->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    memcpy(out, BUF_ADDR(ar, index), ar->data_length);

    return CC_OK;
}


/**
 * Gets the last element of the array or the element at the highest index
 * and sets the out parameter to its value.
 *
 * @param[in] ar the array whose last element is being returned
 * @param[out] out pointer to where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if the
 * CC_Array is empty.
 */
enum cc_stat cc_array_sized_get_last(CC_ArraySized *ar, uint8_t *out)
{
    if (ar->size == 0) {
        return CC_ERR_VALUE_NOT_FOUND;
    }
    return cc_array_sized_get_at(ar, ar->size - 1, out);
}

/**
* Returns the pointer to the element at the specified index
*/
enum cc_stat cc_array_sized_peek(CC_ArraySized* ar, size_t index, uint8_t** out)
{
    if (index > ar->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    *out = BUF_ADDR(ar, index);
    return CC_OK;
}

/**
 * Returns the underlying array buffer.
 *
 * @note Any direct modification of the buffer may invalidate the CC_Array.
 *
 * @param[in] ar array whose underlying buffer is being returned
 *
 * @return array's internal buffer.
 */
const uint8_t * const* cc_array_sized_get_buffer(CC_ArraySized *ar)
{
    return (const uint8_t* const*) ar->buffer;
}

/**
 * Gets the index of the specified element. The returned index is the index
 * of the first occurrence of the element starting from the beginning of the
 * CC_ArraySized.
 *
 * @param[in] ar array being searched
 * @param[in] element the element whose index is being looked up
 * @param[out] index  pointer to where the index is stored
 *
 * @return CC_OK if the index was found, or CC_OUT_OF_RANGE if not.
 */
enum cc_stat cc_array_sized_index_of(CC_ArraySized *ar, uint8_t *element, size_t *index)
{
    for (size_t i = 0; i < ar->size; i++) {
        for (size_t j = 0; j < ar->data_length; j++) { 
            // Mismatching byte, skip to next block
            if (ar->buffer[INDEX(ar, i + j)] != *(element + j)) {
                break;
            }
            if (j == ar->data_length - 1) {
                *index = i;
                return CC_OK;
            }
        }
    }
    return CC_ERR_OUT_OF_RANGE;
}

/**
 * Creates a subarray of the specified CC_ArraySized, ranging from <code>b</code>
 * index (inclusive) to <code>e</code> index (inclusive). The range indices
 * must be within the bounds of the CC_ArraySized, while the <code>e</code> index
 * must be greater or equal to the <code>b</code> index.
 *
 * @note The new CC_ArraySized is allocated using the original CC_ArraySized's allocators
 *       and it also inherits the configuration of the original CC_ArraySized.
 *
 * @param[in] ar array from which the subarray is being created
 * @param[in] b the beginning index (inclusive) of the subarray that must be
 *              within the bounds of the array and must not exceed the
 *              the end index
 * @param[in] e the end index (inclusive) of the subarray that must be within
 *              the bounds of the array and must be greater or equal to the
 *              beginning index
 * @param[out] out pointer to where the new sublist is stored
 *
 * @return CC_OK if the subarray was successfully created, CC_ERR_INVALID_RANGE
 * if the specified index range is invalid, or CC_ERR_ALLOC if the memory allocation
 * for the new subarray failed.
 */
enum cc_stat cc_array_sized_subarray(CC_ArraySized *ar, size_t b, size_t e, CC_ArraySized **out)
{
    if (b > e || e >= ar->size)
        return CC_ERR_INVALID_RANGE;

    CC_ArraySized *sub_ar = ar->mem_calloc(1, sizeof(CC_ArraySized));

    if (!sub_ar)
        return CC_ERR_ALLOC;

    /* Try to allocate the buffer */
    if (!(sub_ar->buffer = ar->mem_alloc(ar->capacity * ar->data_length))) {
        ar->mem_free(sub_ar);
        return CC_ERR_ALLOC;
    }

    sub_ar->mem_alloc   = ar->mem_alloc;
    sub_ar->mem_calloc  = ar->mem_calloc;
    sub_ar->mem_free    = ar->mem_free;
    sub_ar->size        = e - b + 1;
    sub_ar->capacity    = sub_ar->size;
    sub_ar->data_length = ar->data_length;

    memcpy(sub_ar->buffer,
           BUF_ADDR(ar, b),
           sub_ar->size * ar->data_length);

    *out = sub_ar;
    return CC_OK;
}

/**
 * Creates a copy of the specified CC_ArraySized. 
 *
 * @note The new CC_ArraySized is allocated using the original CC_ArraySized's
 *       allocators and it also inherits the configuration of the original array.
 *
 * @param[in] ar the array to be copied
 * @param[out] out pointer to where the newly created copy is stored
 *
 * @return CC_OK if the copy was successfully created, or CC_ERR_ALLOC if the
 * memory allocation for the copy failed.
 */
enum cc_stat cc_array_sized_copy(CC_ArraySized *ar, CC_ArraySized **out)
{
    CC_ArraySized *copy = ar->mem_alloc(sizeof(CC_ArraySized));

    if (!copy)
        return CC_ERR_ALLOC;

    if (!(copy->buffer = ar->mem_calloc(ar->capacity, ar->data_length))) {
        ar->mem_free(copy);
        return CC_ERR_ALLOC;
    }
    copy->data_length = ar->data_length;
    copy->exp_factor  = ar->exp_factor;
    copy->size        = ar->size;
    copy->capacity    = ar->capacity;
    copy->mem_alloc   = ar->mem_alloc;
    copy->mem_calloc  = ar->mem_calloc;
    copy->mem_free    = ar->mem_free;

    memcpy(copy->buffer,
           ar->buffer,
           copy->size * ar->data_length);

    *out = copy;
    return CC_OK;
}

/**
 * Filters the CC_ArraySized by modifying it. It removes all elements that don't
 * return true on pred(element).
 *
 * @param[in] ar   array that is to be filtered
 * @param[in] pred predicate function which returns true if the element should
 *                 be kept in the CC_Array
 *
 * @return CC_OK if the CC_ArraySized was filtered successfully, or CC_ERR_OUT_OF_RANGE
 * if the CC_ArraySized is empty.
 */
enum cc_stat cc_array_sized_filter_mut(CC_ArraySized *ar, bool (*pred) (const uint8_t*))
{
    if (ar->size == 0) {
        return CC_ERR_OUT_OF_RANGE;
    }
    size_t rm   = 0;
    size_t keep = 0;

    /* Look for clusters of non matching elements before moving
     * in order to minimize the number of memmoves */
    for (size_t i = ar->size - 1; i != ((size_t) - 1); i--) {
        if (!pred(BUF_ADDR(ar, i))) {
            rm++;
            continue;
        }
        if (rm > 0) {
            if (keep > 0) {
                size_t block_size = keep * ar->data_length;
                memmove(BUF_ADDR(ar, (i + 1)),
                        BUF_ADDR(ar, (i + 1 + rm)),
                        block_size);
            }
            ar->size -= rm;
            rm = 0;
        }
        keep++;
    }
    /* Remove any remaining elements*/
    if (rm > 0) {
        size_t block_size = keep * ar->data_length;
        memmove(BUF_ADDR(ar, 0),
                BUF_ADDR(ar, rm),
                block_size);

        ar->size -= rm;
    }
    return CC_OK;
}

/**
 * Filters the CC_ArraySized by creating a new CC_ArraySized that contains all elements
 * from the original CC_ArraySized that return true on pred(element) without modifying 
 * the original CC_ArraySized.
 *
 * @param[in] ar   array that is to be filtered
 * @param[in] pred predicate function which returns true if the element should
 *                 be kept in the filtered array
 * @param[out] out pointer to where the new filtered CC_Array is to be stored
 *
 * @return CC_OK if the CC_Array was filtered successfully, CC_ERR_OUT_OF_RANGE
 * if the CC_Array is empty, or CC_ERR_ALLOC if the memory allocation for the
 * new CC_Array failed.
 */
enum cc_stat cc_array_sized_filter(CC_ArraySized *ar, bool (*pred) (const uint8_t*), CC_ArraySized **out)
{
    if (ar->size == 0) {
        return CC_ERR_OUT_OF_RANGE;
    }
    CC_ArraySized *filtered = ar->mem_alloc(sizeof(CC_ArraySized));

    if (!filtered) {
        return CC_ERR_ALLOC;
    }
    if (!(filtered->buffer = ar->mem_calloc(ar->capacity, ar->data_length))) {
        ar->mem_free(filtered);
        return CC_ERR_ALLOC;
    }

    filtered->data_length = ar->data_length;
    filtered->exp_factor  = ar->exp_factor;
    filtered->size        = 0;
    filtered->capacity    = ar->capacity;
    filtered->mem_alloc   = ar->mem_alloc;
    filtered->mem_calloc  = ar->mem_calloc;
    filtered->mem_free    = ar->mem_free;

    size_t f = 0;
    for (size_t i = 0; i < ar->size; i++) {
        if (pred(BUF_ADDR(ar, i))) {
            memcpy(BUF_ADDR(filtered, f++), 
                   BUF_ADDR(ar, i),
                   ar->data_length);
            filtered->size++;
        }
    }
    *out = filtered;

    return CC_OK;
}

/**
 * Reverses the order of elements in the specified array.
 *
 * @param[in] ar array that is being reversed
 */
void cc_array_sized_reverse(CC_ArraySized *ar, uint8_t *tmp)
{
    if (ar->size == 0) {
        return;
    }
    size_t i;
    size_t j;
    for (i = 0, j = ar->size - 1; i < ar->size / 2; i++, j--) {
        memcpy(tmp, BUF_ADDR(ar, i), ar->data_length);
        memcpy(BUF_ADDR(ar, i), BUF_ADDR(ar, j), ar->data_length);
        memcpy(BUF_ADDR(ar, j), tmp, ar->data_length);
    }
}

/**
 * Trims the array's capacity, in other words, it shrinks the capacity to match
 * the number of elements in the CC_ArraySized, however the capacity will never shrink
 * below 1.
 *
 * @param[in] ar array whose capacity is being trimmed
 *
 * @return CC_OK if the capacity was trimmed successfully, or CC_ERR_ALLOC if
 * the reallocation failed.
 */
enum cc_stat cc_array_sized_trim_capacity(CC_ArraySized *ar)
{
    if (ar->size == ar->capacity) {
        return CC_OK;
    }
    uint8_t *new_buff = ar->mem_calloc(ar->size, ar->data_length);

    if (!new_buff) {
        return CC_ERR_ALLOC;
    }
    size_t size = ar->size < 1 ? 1 : ar->size;

    memcpy(new_buff, ar->buffer, size * ar->data_length);
    ar->mem_free(ar->buffer);

    ar->buffer   = new_buff;
    ar->capacity = ar->size;

    return CC_OK;
}

/**
 * Returns the number of occurrences of the element within the specified CC_ArraySized.
 *
 * @param[in] ar array that is being searched
 * @param[in] element the element that is being searched for
 *
 * @return the number of occurrences of the element.
 */
size_t cc_array_sized_contains(CC_ArraySized *ar, uint8_t *element)
{
    size_t o = 0;
    for (size_t i = 0; i < ar->size; i++) {
        for (size_t j = 0; j < ar->data_length; j++) {
            // Mismatching byte, skip block
            if (ar->buffer[INDEX(ar, i + j)] != *(element + j)) {
                break;
            }
            // Last byte is equal, match found
            if (j == ar->data_length - 1) {
                o++;
            }
        }
    }
    return o;
}

/**
 * Returns the size of the specified CC_ArraySized. The size of the array is the
 * number of elements contained within the CC_ArraySized.
 *
 * @param[in] ar array whose size is being returned
 *
 * @return the the number of element within the CC_ArraySized.
 */
size_t cc_array_sized_size(CC_ArraySized *ar)
{
    return ar->size;
}

/**
 * Returns the capacity of the specified CC_ArraySized. The capacity of the CC_ArraySized is
 * the maximum number of elements an CC_ArraySized can hold before it has to be resized.
 *
 * @param[in] ar array whose capacity is being returned
 *
 * @return the capacity of the CC_ArraySized.
 */
size_t cc_array_sized_capacity(CC_ArraySized *ar)
{
    return ar->capacity;
}

/**
 * Sorts the specified array.
 *
 * @note
 * Pointers passed to the comparator function will be pointers to the array
 * elements that are of type (void*) ie. void**. So an extra step of
 * dereferencing will be required before the data can be used for comparison:
 * eg. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @code
 * enum cc_stat mycmp(const void *e1, const void *e2) {
 *     MyType el1 = *(*((enum cc_stat**) e1));
 *     MyType el2 = *(*((enum cc_stat**) e2));
 *
 *     if (el1 < el2) return -1;
 *     if (el1 > el2) return 1;
 *     return 0;
 * }
 *
 * ...
 *
 * cc_array_sort(array, mycmp);
 * @endcode
 *
 * @param[in] ar  array to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                enum cc_stat cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first
 */
void cc_array_sized_sort(CC_ArraySized *ar, int (*cmp) (const void*, const void*))
{
    qsort(ar->buffer, ar->size, ar->data_length, cmp);
}

/**
 * Expands the CC_ArraySized capacity. This might fail if the the new buffer
 * cannot be allocated. In case the expansion would overflow the index
 * range, a maximum capacity buffer is allocated instead. If the capacity
 * is already at the maximum capacity, no new buffer is allocated.
 *
 * @param[in] ar array whose capacity is being expanded
 *
 * @return CC_OK if the buffer was expanded successfully, CC_ERR_ALLOC if
 * the memory allocation for the new buffer failed, or CC_ERR_MAX_CAPACITY
 * if the array is already at maximum capacity.
 */
static enum cc_stat expand_capacity(CC_ArraySized *ar)
{
    if (ar->capacity == CC_MAX_ELEMENTS) {
        return CC_ERR_MAX_CAPACITY;
    }

    size_t new_capacity = (size_t) (ar->capacity * ar->exp_factor);

    /* As long as the capacity is greater that the expansion factor
     * at the point of overflow, this is check is valid. */
    if (new_capacity <= ar->capacity) {
        ar->capacity = CC_MAX_ELEMENTS;
    } else {
        ar->capacity = new_capacity;
    }
    uint8_t *new_buff = ar->mem_alloc(ar->capacity * ar->data_length);

    if (!new_buff) {
        return CC_ERR_ALLOC;
    }
    memcpy(new_buff, ar->buffer, ar->size * ar->data_length);

    ar->mem_free(ar->buffer);
    ar->buffer = new_buff;

    return CC_OK;
}

/**
 * Applies the function fn to each element of the CC_Array.
 *
 * @param[in] ar array on which this operation is performed
 * @param[in] fn operation function that is to be invoked on each CC_Array
 *               element
 */
void cc_array_sized_map(CC_ArraySized *ar, void (*fn) (uint8_t *e))
{
    for (size_t i = 0; i < ar->size; i++) {
        fn(BUF_ADDR(ar, i));
    }
}

/**
 * A fold/reduce function that collects all of the elements in the array
 * together. For example, if we have an array of [a,b,c...] the end result
 * will be (...((a+b)+c)+...).
 *
 * @param[in] ar the array on which this operation is performed
 * @param[in] fn the operation function that is to be invoked on each array
 *               element
 * @param[in] result the pointer which will collect the end result
 */
void cc_array_sized_reduce(CC_ArraySized *ar, void (*fn) (uint8_t*, uint8_t*, uint8_t*), uint8_t *result)
{
    if (ar->size == 1) {
        fn(BUF_ADDR(ar, 0), NULL, result);
        return;
    }
    if (ar->size > 1) {
        fn(BUF_ADDR(ar, 0), BUF_ADDR(ar, 1), result);
    }
    for (size_t i = 2; i < ar->size; i++) {
        fn(result, BUF_ADDR(ar, i), result);
    }
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] ar the array to iterate over
 */
void cc_array_sized_iter_init(CC_ArraySizedIter *iter, CC_ArraySized *ar)
{
    iter->ar    = ar;
    iter->index = 0;
    iter->last_removed = false;
}

/**
 * Advances the iterator and sets the out parameter to the reference of the
 * next element in the sequence.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the CC_Array has been reached.
 */
enum cc_stat cc_array_sized_iter_next(CC_ArraySizedIter *iter, uint8_t **out)
{
    if (iter->index >= iter->ar->size) {
        return CC_ITER_END;
    }
    *out = BUF_ADDR(iter->ar, iter->index);
    iter->index++;
    iter->last_removed = false;

    return CC_OK;
}

/**
 * Removes the last returned element by <code>cc_array_sized_iter_next()</code>
 * function without invalidating the iterator and optionally sets the out
 * parameter to the value of the removed element.
 *
 * @note This function should only ever be called after a call to <code>
 * cc_array_sized_iter_next()</code>.

 * @param[in] iter the iterator on which this operation is being performed
 * @param[out] out pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND.
 */
enum cc_stat cc_array_sized_iter_remove(CC_ArraySizedIter *iter, uint8_t *out)
{
    enum cc_stat status = CC_ERR_VALUE_NOT_FOUND;

    if (!iter->last_removed) {
        status = cc_array_sized_remove_at(iter->ar, iter->index - 1, out);
        if (status == CC_OK) {
            iter->last_removed = true;
        }
    }
    return status;
}

/**
 * Adds a new element to the CC_ArraySized after the last returned element by
 * <code>cc_array_sized_iter_next()</code> function without invalidating the
 * iterator.
 *
 * @note This function should only ever be called after a call to <code>
 * cc_array_sized_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added
 *
 * @return CC_OK if the element was successfully added, CC_ERR_ALLOC if the
 * memory allocation for the new element failed, or CC_ERR_MAX_CAPACITY if
 * the array is already at maximum capacity.
 */
enum cc_stat cc_array_sized_iter_add(CC_ArraySizedIter *iter, uint8_t *element)
{
    return cc_array_sized_add_at(iter->ar, element, iter->index++);
}

/**
 * Replaces the last returned element by <code>cc_array_sized_iter_next()</code>
 * with the specified element and optionally sets the out parameter to
 * the value of the replaced element.
 *
 * @note This function should only ever be called after a call to <code>
 * cc_array_sized_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @param[out] out pointer to where the replaced element is stored, or NULL
 *                if it is to be ignored
 *
 * @return CC_OK if the element was replaced successfully, or
 * CC_ERR_OUT_OF_RANGE.
 */
enum cc_stat cc_array_sized_iter_replace(CC_ArraySizedIter *iter, uint8_t *element, uint8_t *out)
{
    return cc_array_sized_replace_at(iter->ar, element, iter->index - 1, out);
}

/**
 * Returns the index of the last returned element by <code>cc_array_sized_iter_next()
 * </code>.
 *
 * @note
 * This function should not be called before a call to <code>cc_array_sized_iter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index.
 */
size_t cc_array_sized_iter_index(CC_ArraySizedIter *iter)
{
    return iter->index - 1;
}

/**
 * Initializes the zip iterator.
 *
 * @param[in] iter iterator that is being initialized
 * @param[in] ar1  first array
 * @param[in] ar2  second array
 */
void cc_array_sized_zip_iter_init(CC_ArraySizedZipIter *iter, CC_ArraySized *ar1, CC_ArraySized *ar2)
{
    iter->ar1 = ar1;
    iter->ar2 = ar2;
    iter->index = 0;
    iter->last_removed = false;
}

/**
 * Outputs the next element pair in the sequence and advances the iterator.
 *
 * @param[in]  iter iterator that is being advanced
 * @param[out] out1 output of the first array element
 * @param[out] out2 output of the second array element
 *
 * @return CC_OK if a next element pair is returned, or CC_ITER_END if the end of one
 * of the arrays has been reached.
 */
enum cc_stat cc_array_sized_zip_iter_next(CC_ArraySizedZipIter *iter, uint8_t **out1, uint8_t **out2)
{
    if (iter->index >= iter->ar1->size || iter->index >= iter->ar2->size) {
        return CC_ITER_END;
    }
    *out1 = BUF_ADDR(iter->ar1, iter->index);
    *out2 = BUF_ADDR(iter->ar2, iter->index);

    iter->index++;
    iter->last_removed = false;

    return CC_OK;
}

/**
 * Removes and outputs the last returned element pair by <code>cc_array_sized_zip_iter_next()
 * </code> without invalidating the iterator.
 *
 * @param[in]  iter iterator on which this operation is being performed
 * @param[out] out1 output of the removed element from the first array
 * @param[out] out2 output of the removed element from the second array
 *
 * @return CC_OK if the element was successfully removed, CC_ERR_OUT_OF_RANGE if the
 * state of the iterator is invalid, or CC_ERR_VALUE_NOT_FOUND if the element was
 * already removed.
 */
enum cc_stat cc_array_sized_zip_iter_remove(CC_ArraySizedZipIter *iter, uint8_t *out1, uint8_t *out2)
{
    if ((iter->index - 1) >= iter->ar1->size || (iter->index - 1) >= iter->ar2->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    if (!iter->last_removed) {
        cc_array_sized_remove_at(iter->ar1, iter->index - 1, out1);
        cc_array_sized_remove_at(iter->ar2, iter->index - 1, out2);
        iter->last_removed = true;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Adds a new element pair to the arrays after the last returned element pair by
 * <code>cc_array_sized_zip_iter_next()</code> and immediately before an element pair
 * that would be returned by a subsequent call to <code>cc_array_sized_zip_iter_next()</code>
 * without invalidating the iterator.
 *
 * @param[in] iter iterator on which this operation is being performed
 * @param[in] e1   element added to the first array
 * @param[in] e2   element added to the second array
 *
 * @return CC_OK if the element pair was successfully added to the arrays, or
 * CC_ERR_ALLOC if the memory allocation for the new elements failed.
 */
enum cc_stat cc_array_sized_zip_iter_add(CC_ArraySizedZipIter *iter, uint8_t *e1, uint8_t *e2)
{
    size_t index = iter->index++;
    CC_ArraySized  *ar1  = iter->ar1;
    CC_ArraySized  *ar2  = iter->ar2;

    /* Make sure both array buffers have room */
    if ((ar1->size == ar1->capacity && (expand_capacity(ar1) != CC_OK)) ||
        (ar2->size == ar2->capacity && (expand_capacity(ar2) != CC_OK))) {
        return CC_ERR_ALLOC;
    }
    cc_array_sized_add_at(ar1, e1, index);
    cc_array_sized_add_at(ar2, e2, index);

    return CC_OK;
}

/**
 * Replaces the last returned element pair by <code>cc_array_sized_zip_iter_next()</code>
 * with the specified replacement element pair.
 *
 * @param[in] iter  iterator on which this operation is being performed
 * @param[in]  e1   first array's replacement element
 * @param[in]  e2   second array's replacement element
 * @param[out] out1 output of the replaced element from the first array
 * @param[out] out2 output of the replaced element from the second array
 *
 * @return CC_OK if the element was successfully replaced, or CC_ERR_OUT_OF_RANGE.
 */
enum cc_stat cc_array_sized_zip_iter_replace(CC_ArraySizedZipIter *iter, uint8_t *e1, uint8_t *e2, uint8_t *out1, uint8_t *out2)
{
    if ((iter->index - 1) >= iter->ar1->size || (iter->index - 1) >= iter->ar2->size) {
        return CC_ERR_OUT_OF_RANGE;
    }
    cc_array_sized_replace_at(iter->ar1, e1, iter->index - 1, out1);
    cc_array_sized_replace_at(iter->ar2, e2, iter->index - 1, out2);

    return CC_OK;
}

/**
 * Returns the index of the last returned element pair by <code>cc_array_sized_zip_iter_next()</code>.
 *
 * @param[in] iter iterator on which this operation is being performed
 *
 * @return current iterator index.
 */
size_t cc_array_sized_zip_iter_index(CC_ArraySizedZipIter *iter)
{
    return iter->index - 1;
}


size_t cc_array_sized_struct_size()
{
    return sizeof(CC_ArraySized);
}