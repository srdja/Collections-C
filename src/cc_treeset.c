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

#include "cc_treeset.h"

struct cc_treeset_s {
    CC_TreeTable *t;
    int       *dummy;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

/**
 * Initializes the fields of the CC_TreeSetConf struct to default values.
 *
 * @param[in, out] conf the configuration struct that is being initialized
 */
void cc_treeset_conf_init(CC_TreeSetConf *conf)
{
    cc_treetable_conf_init(conf);
}

/**
 * Creates a new CC_TreeSet and returns a status code.
 *
 * @param[in] cmp the comparator function used to order elements
 * @param[out] out pointer to where the newly created CC_TreeSet is to be stored
 *
 * @return  CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_TreeSet failed.
 */
enum cc_stat cc_treeset_new(int (*cmp) (const void*, const void*), CC_TreeSet **set)
{
    CC_TreeSetConf conf;
    cc_treeset_conf_init(&conf);
    conf.cmp = cmp;
    return cc_treeset_new_conf(&conf, set);
}

/**
 * Creates a new CC_TreeSet based on the specified CC_TreeSetConf struct and returns
 * a status code.
 *
 * The CC_TreeSet is allocated using the allocators specified in the CC_TreeSetConf
 * object. The allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf  CC_TreeSet configuration struct. All fields must be initialized.
 * @param[out] out Pointer to where the newly created CC_TreeSet is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_TreeSet structure failed.
 */
enum cc_stat cc_treeset_new_conf(CC_TreeSetConf const * const conf, CC_TreeSet **tset)
{
    CC_TreeSet *set = conf->mem_calloc(1, sizeof(CC_TreeSet));

    if (!set)
        return CC_ERR_ALLOC;

    CC_TreeTable *table;
    enum cc_stat s = cc_treetable_new_conf(conf, &table);

    if (s != CC_OK) {
        conf->mem_free(set);
        return s;
    }
    set->t          = table;
    set->dummy      = (int*) 1;
    set->mem_alloc  = conf->mem_alloc;
    set->mem_calloc = conf->mem_calloc;
    set->mem_free   = conf->mem_free;

    *tset = set;
    return CC_OK;
}

/**
 * Destroys the specified CC_TreeSet.
 *
 * @param[in] set the CC_TreeSet to be destroyed
 */
void cc_treeset_destroy(CC_TreeSet *set)
{
    cc_treetable_destroy(set->t);
    set->mem_free(set);
}

/**
 * Adds a new element to the set.
 *
 * @param[in] set the set to which the element is being added
 * @param[in] element the element being added
 *
 * @return CC_OK if the operation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new element failed.
 */
enum cc_stat cc_treeset_add(CC_TreeSet *set, void *element)
{
    return cc_treetable_add(set->t, element, set->dummy);
}

/**
 * Removes the specified element from the CC_TreeSet and sets the out
 * parameter to its value.
 *
 * @param[in] set the set from which the element is being removed
 * @param[in] element the element being removed
 * @param[out] out pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the mapping was successfully removed, or CC_ERR_VALUE_NOT_FOUND
 * if the value was not found.
 */
enum cc_stat cc_treeset_remove(CC_TreeSet *set, void *element, void **out)
{
    if (cc_treetable_remove(set->t, element, out) == CC_ERR_KEY_NOT_FOUND)
        return CC_ERR_VALUE_NOT_FOUND;

    return CC_OK;
}

/**
 * Removes all elements from the specified set.
 *
 * @param set the set from which all elements are being removed
 */
void cc_treeset_remove_all(CC_TreeSet *set)
{
    cc_treetable_remove_all(set->t);
}

/**
 * Returns the first element of the set.
 *
 * @param[in] set the set from which the first element is being returned
 * @param[out] out pointer to where the returned element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat cc_treeset_get_first(CC_TreeSet *set, void **out)
{
    if (cc_treetable_get_first_key(set->t, out) == CC_ERR_KEY_NOT_FOUND)
        return CC_ERR_VALUE_NOT_FOUND;

    return CC_OK;
}

/**
 * Returns the last (highest) element of the set.
 *
 * @param[in] set the set from which the last element is being returned
 * @param[out] out pointer to where the returned element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat cc_treeset_get_last(CC_TreeSet *set, void **out)
{
    if (cc_treetable_get_last_key(set->t, out) == CC_ERR_KEY_NOT_FOUND)
        return CC_ERR_VALUE_NOT_FOUND;

    return CC_OK;
}

/**
 * Returns the immediate successor of the specified element.
 *
 * @param[in] set the set on which this operation is performed
 * @param[in] element the element whose successor is being returned
 * @param[out] out pointer to where the returned element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat cc_treeset_get_greater_than(CC_TreeSet *set, void *element, void **out)
{
    if (cc_treetable_get_greater_than(set->t, element, out) == CC_ERR_KEY_NOT_FOUND)
        return CC_ERR_VALUE_NOT_FOUND;

    return CC_OK;
}

/**
 * Returns the immediate predecessor of the specified element.
 *
 * @param[in] set the set on which this operation is performed
 * @param[in] element the element whose predecessor is being returned
 * @param[out] out pointer to where the returned element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat cc_treeset_get_lesser_than(CC_TreeSet *set, void *element, void **out)
{
    if (cc_treetable_get_lesser_than(set->t, element, out) == CC_ERR_KEY_NOT_FOUND)
        return CC_ERR_VALUE_NOT_FOUND;

    return CC_OK;
}

/**
 * Checks whether an element is a part of the specified set.
 *
 * @param[in] set the set being searched for the specified element
 * @param[in] element the element being searched for
 *
 * @return true if the specified element is an element of the set.
 */
bool cc_treeset_contains(CC_TreeSet *set, void *element)
{
    return cc_treetable_contains_key(set->t, element);
}

/**
 * Returns the size of the specified set.
 *
 * @param[in] set the set whose size is being returned
 *
 * @return the size of the set.
 */
size_t cc_treeset_size(CC_TreeSet *set)
{
    return cc_treetable_size(set->t);
}

/**
 * Applies the function fn to each element of the CC_TreeSet.
 *
 * @param[in] set the set on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each element
 *               of the set
 */
void cc_treeset_foreach(CC_TreeSet *set, void (*fn) (const void*))
{
    cc_treetable_foreach_key(set->t, fn);
}

/**
 * Initializes the set iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] set the set on which this iterator will operate
 */
void cc_treeset_iter_init(CC_TreeSetIter *iter, CC_TreeSet *set)
{
    cc_treetable_iter_init(&(iter->i), set->t);
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] element pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the CC_TreeSet has been reached.
 */
enum cc_stat cc_treeset_iter_next(CC_TreeSetIter *iter, void **element)
{
    CC_TreeTableEntry entry;

    if (cc_treetable_iter_next(&(iter->i), &entry) != CC_OK)
        return CC_ITER_END;

    *element = entry.key;
    return CC_OK;
}

/**
 * Removes the last returned element by <code>cc_treeset_iter_next()</code>
 * function without invalidating the iterator and optionally sets the
 * out parameter to the value of the removed element.
 *
 * @note This Function should only ever be called after a call to <code>
 * cc_treeset_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is performed
 * @param[out] out pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_KEY_NOT_FOUND.
 */
enum cc_stat cc_treeset_iter_remove(CC_TreeSetIter *iter, void **out)
{
    return cc_treetable_iter_remove(&(iter->i), out);
}
