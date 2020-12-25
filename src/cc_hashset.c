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

#include "cc_hashset.h"

struct cc_hashset_s {
    CC_HashTable *table;
    int       *dummy;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

/**
 * Initializes the fields of the CC_HashSetConf struct to default values.
 *
 * @param[in, out] conf the configuration struct that is being initialized
 */
void cc_hashset_conf_init(CC_HashSetConf *conf)
{
    cc_hashtable_conf_init(conf);
}

/**
 * Creates a new CC_HashSet and returns a status code.
 *
 * @note The newly created CC_HashSet will be a set of strings.
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_HashSet failed.
 */
enum cc_stat cc_hashset_new(CC_HashSet **hs)
{
    CC_HashSetConf hsc;
    cc_hashset_conf_init(&hsc);
    return cc_hashset_new_conf(&hsc, hs);
}

/**
 * Creates a new empty CC_HashSet based on the specified CC_HashSetConf struct and
 * returns a status code.
 *
 * The CC_HashSet is allocated using the allocators specified in the CC_HashSetConf
 * object. The allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf The hashset configuration object. All fields must be initialized.
 * @param[out] out Pointer to where the newly created CC_HashSet is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_HashSet structure failed.
 */
enum cc_stat cc_hashset_new_conf(CC_HashSetConf const * const conf, CC_HashSet **hs)
{
    CC_HashSet *set = conf->mem_calloc(1, sizeof(CC_HashSet));

    if (!set)
        return CC_ERR_ALLOC;

    CC_HashTable *table;
    enum cc_stat stat = cc_hashtable_new_conf(conf, &table);

    if (stat != CC_OK) {
        conf->mem_free(set);
        return stat;
    }

    set->table      = table;
    set->mem_alloc  = conf->mem_alloc;
    set->mem_calloc = conf->mem_calloc;
    set->mem_free   = conf->mem_free;

    /* A dummy pointer that is never actually dereferenced
    *  that must not be null.*/
    set->dummy = (int*) 1;
    *hs = set;
    return CC_OK;
}

/**
 * Destroys the specified CC_HashSet structure without destroying the data
 * it holds.
 *
 * @param[in] table CC_HashSet to be destroyed.
 */
void cc_hashset_destroy(CC_HashSet *set)
{
    cc_hashtable_destroy(set->table);
    set->mem_free(set);
}

/**
 * Adds a new element to the CC_HashSet.
 *
 * @param[in] set the set to which the element is being added
 * @param[in] element the element being added
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC
 * if the memory allocation failed.
 */
enum cc_stat cc_hashset_add(CC_HashSet *set, void *element)
{
    return cc_hashtable_add(set->table, element, set->dummy);
}

/**
 * Removes the specified element from the CC_HashSet and sets the out
 * parameter to its value.
 *
 * @param[in] set the set from which the element is being removed
 * @param[in] element the element being removed
 * @param[out] out Pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or CC_ERR_VALUE_NOT_FOUND
 * if the value was not found.
 */
enum cc_stat cc_hashset_remove(CC_HashSet *set, void *element, void **out)
{
    return cc_hashtable_remove(set->table, element, out);
}

/**
 * Removes all elements from the specified set.
 *
 * @param set the set from which all elements are being removed
 */
void cc_hashset_remove_all(CC_HashSet *set)
{
    cc_hashtable_remove_all(set->table);
}

/**
 * Checks whether an element is a part of the specified set.
 *
 * @param[in] set the set being searched for the specified element
 * @param[in] element the element being searched for
 *
 * @return true if the specified element is an element of the set
 */
bool cc_hashset_contains(CC_HashSet *set, void *element)
{
    return cc_hashtable_contains_key(set->table, element);
}

/**
 * Returns the size of the specified set.
 *
 * @param[in] set the set whose size is being returned
 *
 * @return the size of the set
 */
size_t cc_hashset_size(CC_HashSet *set)
{
    return cc_hashtable_size(set->table);
}

/**
 * Returns the capacity of the specified set.
 *
 * @param[in] set the set whose capacity is being returned
 *
 * @return the capacity of the set
 */
size_t cc_hashset_capacity(CC_HashSet *set)
{
    return cc_hashtable_capacity(set->table);
}

/**
 * Applies the function fn to each element of the CC_HashSet.
 *
 * @param[in] set the set on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each element of the
 *               set
 */
void cc_hashset_foreach(CC_HashSet *set, void (*fn) (const void *e))
{
    cc_hashtable_foreach_key(set->table, fn);
}

/**
 * Initializes the set iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] set the set on which this iterator will operate
 */
void cc_hashset_iter_init(CC_HashSetIter *iter, CC_HashSet *set)
{
    cc_hashtable_iter_init(&(iter->iter), set->table);
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out Pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the CC_HashSet has been reached.
 */
enum cc_stat cc_hashset_iter_next(CC_HashSetIter *iter, void **out)
{
    TableEntry *entry;
    enum cc_stat status = cc_hashtable_iter_next(&(iter->iter), &entry);

    if (status != CC_OK)
        return status;

    if (out)
        *out = entry->key;

    return CC_OK;
}

/**
 * Removes the last returned entry by <code>cc_hashset_iter_next()</code>
 * function without invalidating the iterator and optionally sets the
 * out parameter to the value of the removed element.
 *
 * @note This Function should only ever be called after a call to <code>
 * cc_hashset_iter_next()</code>.
 *
 * @param[in] iter The iterator on which this operation is performed
 * @param[out] out Pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the entry was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND.
 */
enum cc_stat cc_hashset_iter_remove(CC_HashSetIter *iter, void **out)
{
    return cc_hashtable_iter_remove(&(iter->iter), out);
}
