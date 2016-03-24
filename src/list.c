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

#include "list.h"


struct list_s {
    size_t  size;
    Node   *head;
    Node   *tail;

    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
};

static void *unlink              (List *list, Node *node);
static bool  unlink_all          (List *list, bool free);
static void  link_behind         (Node *node, Node *inserted);
static void  link_after          (Node *base, Node *inserted);

static void  swap                (Node *n1, Node *n2);
static void  swap_adjacent       (Node *n1, Node *n2);

static void  splice_between      (List *list1, List *list2, Node *left, Node *right);

static enum cc_stat get_node_at  (List *list, size_t index, Node **out);

static Node *get_node            (List *list, void *element);

static enum cc_stat add_all_to_empty    (List *l1, List *l2);
static bool  link_all_externally (List *l, Node **h, Node **t);

/**
 * Initializes the fields of the ListConf struct to default values.
 *
 * @param[in] conf the configuration struct that is being initialized
 */
void list_conf_init(ListConf *conf)
{
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Returns a new empty list, or NULL if the memory allocation fails.
 *
 * @return a new list if the allocation was successful, or NULL if not.
 */
enum cc_stat list_new(List **out)
{
    ListConf lc;
    list_conf_init(&lc);
    return list_new_conf(&lc, out);
}

/**
 * Returns a new empty list based on the specified ListConf struct.
 *
 * The List is allocated using the allocators specified in the ListConf
 * struct. The allocation may fail if the undelying allocator fails.
 *
 * @param[in] conf the configuration
 *
 * @return a new list if the allocation was successful, or NULL if not.
 */
enum cc_stat list_new_conf(ListConf const * const conf, List **out)
{
    List *list = conf->mem_calloc(1, sizeof(List));

    if (!list)
        return CC_ERR_ALLOC;

    list->mem_alloc  = conf->mem_alloc;
    list->mem_calloc = conf->mem_calloc;
    list->mem_free   = conf->mem_free;

    *out = list;
    return CC_OK;
}

/**
 * Destroys the list structure, but leaves the data that is holds intact.
 *
 * @param[in] list a list to destroy
 *
 * @return true if the operation was successful
 */
bool list_destroy(List *list)
{
    bool success = true;

    if (list->size > 0)
        success = list_remove_all(list);

    list->mem_free(list);
    return success;
}

/**
 * Destroys the list structure along with all the data it holds. This function
 * returns true if the operation was successful, or false if the list was already
 * empty.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list - a list to destroy
 * @return true if the operation was successful
 */
bool list_destroy_free(List *list)
{
    bool success = list_remove_all_free(list);

    list->mem_free(list);
    return success;
}

/**
 * Adds a new element to the list. The element is appended to the list making it
 * the last element in the list. This function returns false if the memory
 * allocation for the new element fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being added
 *
 * @return true if the element was succesfuly added to the list.
 */
enum cc_stat list_add(List *list, void *element)
{
    return list_add_last(list, element);
}

/**
 * Prepends a new element to the list (adds a new "head") making it the first
 * element of the list. This function returns false if the memory allocation for
 * the new element fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being prepended
 *
 * @return true if the element was successfuly added to the list
 */
enum cc_stat list_add_first(List *list, void *element)
{
    Node *node = list->mem_calloc(1, sizeof(Node));

    if (node == NULL)
        return CC_ERR_ALLOC;

    node->data = element;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
    return CC_OK;
}

/**
 * Appends a new element to the list (adds an new "tail") making it the last
 * element of the list. This function returns false if the memory allocation for
 * the new element fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being appended
 *
 * @return true if the element was successfuly added to the list
 */
enum cc_stat list_add_last(List *list, void *element)
{
    Node *node = list->mem_calloc(1, sizeof(Node));

    if (node == NULL)
        return CC_ERR_ALLOC;

    node->data = element;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return CC_OK;
}

/**
 * Adds a new element at the specified location in the list and shifts all
 * subsequent elements by one. This operation cannot be performed on an empty
 * list. The index at which the new element is being added must be within the
 * bounds of the list. This function returns false if either the index is out
 * of bounds, or if the memory allocation for the new element fails.
 *
 * @param[in] list the list to which this element is being added.
 * @param[in] element element being added.
 * @param[in] index the position in the list at which the new element is being
 *                  added
 *
 * @return true if the element was successfully added to the list
 */
enum cc_stat list_add_at(List *list, void *element, size_t index)
{
    Node *base;
    enum cc_stat stat = get_node_at(list, index, &base);

    if (stat != CC_OK)
        return stat;

    Node *new = list->mem_calloc(1, sizeof(Node));

    if (!new)
        return CC_ERR_ALLOC;

    new->data = element;
    link_behind(base, new);

    if (index == 0)
        list->head = new;

    list->size++;

    return CC_OK;
}

/**
 * Adds all elements from the second list to the first. The elements from the
 * second list are added after the last element of the first list. This function
 * returns false if no elements were added to the first list. This could be the
 * case if either the second list is empty or if the memory allocation for the
 * elements that are being added fails.
 *
 * @param[in] list1 the list to which the elements are being added.
 * @param[in] list2 the list from which the elements are being taken.
 *
 * @return true if the operation was successful.
 */
enum cc_stat list_add_all(List *list1, List *list2)
{
    if (list1->size == 0)
        return add_all_to_empty(list1, list2);

    return list_add_all_at(list1, list2, list1->size);
}

/**
 * Adds all elements from the second list to the first by appending them to the
 * end of the first list. This function returns false if no elements were added
 * to the first list. This could be the case if either the second list is empty
 * or if the memory allocation for the elements being added fails.
 *
 * @param[in] list1 the list to which the elements are being added.
 * @param[in] list2 the list from which the elements are being taken.
 *
 * @return true if the operation was successful
 */
static enum cc_stat add_all_to_empty(List *list1, List *list2)
{
    if (list2->size == 0)
        return CC_OK;

    Node *head = NULL;
    Node *tail = NULL;

    if (!link_all_externally(list2, &head, &tail))
        return CC_ERR_ALLOC;

    list1->head = head;
    list1->tail = tail;
    list1->size = list2->size;
    return CC_OK;
}

/**
 * Adds all elements from the second list to the first at the specified position
 * by shifting all subsequent elements by the size of the second list. The index
 * range at which the elements can be added ranges from 0 to max_index + 1. This
 * function returns false if no elements were added to the first list. This
 * could be the case if either the second list is empty or if the memory
 * allocation for the elements being added fails.
 *
 * @param[in] list1 the list to which the elements are being added.
 * @param[in] list2 the list from which the elements are being taken.
 * @param[in] index position in the first list at which the element should be
 *                  added.
 * @return true if the operation was successful.
 */
enum cc_stat list_add_all_at(List *list1, List *list2, size_t index)
{
    if (list2->size == 0)
        return CC_OK;

    if (index > list1->size)
        return CC_ERR_OUT_OF_RANGE;

    /* Link the new nodes together outside of the list so
       that if anything goes wrong we don't have to leave
       garbage in the actual list. */
    Node *head = NULL;
    Node *tail = NULL;

    if (!link_all_externally(list2, &head, &tail))
        return CC_ERR_ALLOC;

    /* Now we can safely attach the new nodes. */
    Node *end = NULL;
    get_node_at(list1, index, &end);

    Node *base = NULL;
    if (end)
        base = end->prev;
    else
        get_node_at(list1, index - 1, &base);

    if (!end) {
        list1->tail->next = head;
        head->prev        = list1->tail;
        list1->tail       = tail;
    } else if (!base) {
        list1->head->prev = tail;
        tail->next        = list1->head;
        list1->head       = head;
    } else {
        head->prev        = base;
        base->next        = head;
        tail->next        = end;
        end->prev         = tail;
    }

    list1->size += list2->size;

    return CC_OK;
}

/**
 * Duplicates the structure of the list without directly attaching it to a
 * specific list. If the operation fails, the mess is cleaned up and false
 * is returned to indicate failure.
 *
 * @param[in] list the list whose structure is being duplicated
 * @param[in, out] h the pointer to which the new head will be attached
 * @param[in, out] t the pointer to which the new tail will be attached
 *
 * @return true if the operation was successful
 */
static bool link_all_externally(List *list, Node **h, Node **t)
{
    Node *insert = list->head;

    size_t i;
    for (i = 0; i < list->size; i++) {
        Node *new = list->mem_calloc(1, sizeof(Node));

        if (!new) {
            while (*h) {
                Node *tmp = (*h)->next;
                list->mem_free(*h);
                *h = tmp;
            }
            return false;
        }

        new->data = insert->data;

        if (!*h) {
            *h = new;
            *t = new;
        } else {
            (*t)->next = new;
            new->prev  = *t;
            *t         = new;
        }

        insert = insert->next;
    }
    return true;
}

/**
 * Splices the two doubly linked lists together by appending the second list to
 * the first. This function moves all the elements from the second list into
 * the first list, leaving the second list empty.
 *
 * @param[in] list1 The consumer list to which the elements are moved.
 * @param[in] list2 The producer list from which the elements are moved.
 *
 * @return true if the operation was successful
 */
enum cc_stat list_splice(List *list1, List *list2)
{
    return list_splice_at(list1, list2, list1->size);
}

/**
 * Splices the two lists together at the specified index of the first list. This
 * function moves all the element from the second list into the frist list at the
 * position specified by the <code>index</code> parameter. After this opertaion
 * the second list will be left empty. This function returns false if the second
 * list is already empty or if the specified index is out of bounds.
 *
 * @param[in] list1 the consumer list to which the elements are moved
 * @param[in] list2 the producer list from which the elements are moved
 * @param[in] index the index in the first list after which the elements from the
 *                  second list should be inserted
 *
 * @return true if at least one element was moved from the second list
 */
enum cc_stat list_splice_at(List *list1, List *list2, size_t index)
{
    if (list2->size == 0)
        return CC_OK;

    if (index > list1->size)
        return CC_ERR_OUT_OF_RANGE;

    if (list1->size == 0) {
        // TODO move to splice_between
        list1->head = list2->head;
        list1->tail = list2->tail;
        list1->size = list2->size;

        list2->head = NULL;
        list2->tail = NULL;
        list2->size = 0;
        return CC_OK;
    }

    Node *end = NULL;
    get_node_at(list1, index, &end);

    Node *base = NULL;
    if (end)
        base = end->prev;
    else
        get_node_at(list1, index - 1, &base);

    splice_between(list1, list2, base, end);

    return CC_OK;
}

/**
 * Inserts the second list between the two nodes of the first list. If the left
 * node is NULL the head of the second list will become the head of the first
 * list. Similarly if the right node is null the tail of the first list will
 * become the tail of the second list.
 *
 * @param[in, out] l1 the list to which the elements are being transferred
 * @param[in, out] l2 the list from which the elements are being transferred
 * @param[in] left the node after which the element are being added
 * @param[in] right the node behind which the elements are being added
 */
static void splice_between(List *l1, List *l2, Node *left, Node *right)
{
    if (!left) {
        l1->head->prev = l2->tail;
        l2->tail->next = l1->head;
        l1->head       = l2->head;
    } else if (!right) {
        l1->tail->next = l2->head;
        l2->head->prev = l1->tail;
        l1->tail       = l2->tail;
    } else {
        left->next     = l2->head;
        l2->head->prev = left;
        right->prev    = l2->tail;
        l2->tail->next = right;
    }
    l1->size += l2->size;

    l2->head = NULL;
    l2->tail = NULL;
    l2->size = 0;
}

/**
 * Removes and returns the first occurrence of the element from the specified
 * list. If the element is not a part of the list, NULL is returned. NULL may
 * also be returned if the removed element was NULL. Calling <code>
 * list_contains()</code> before this function can resolve the ambiguity.
 *
 * @param[in] list a list from which the element is being removed
 * @param[in] element element being removed
 *
 * @return the removed element
 */
enum cc_stat list_remove(List *list, void *element, void **out)
{
    Node *node = get_node(list, element);

    if (!node)
        return CC_ERR_VALUE_NOT_FOUND;

    if (out)
        *out = node->data;

    unlink(list, node);
    return CC_OK;
}

/**
 * Removes and returns the element at the specified index. The index must be
 * within the bounds of the list. In case the index is out of bounds this
 * function returns NULL. NULL may also be returned if the element at the
 * specified index is actually NULL. Calling <code>list_contains()</code>
 * before this function can resolve this ambiguity.
 *
 * @param[in] list the list from which the element is being removed.
 * @param[in] index Index of the element is being removed. Must be be within the
 *            index range of the list.
 *
 * @return the removed element, or NULL
 */
enum cc_stat list_remove_at(List *list, size_t index, void **out)
{
    Node *node;
    enum cc_stat status = get_node_at(list, index, &node);

    if (status != CC_OK)
        return status;

    if (out)
        *out = node->data;

    unlink(list, node);
    return CC_OK;
}

/**
 * Removes and returns the first (head) element of the list. If the list is
 * empty, NULL is returned.
 *
 * @param[in] list the list from which the first element is being removed
 *
 * @return the removed element
 */
enum cc_stat list_remove_first(List *list, void **out)
{
    if (!list->size)
        return CC_ERR_VALUE_NOT_FOUND;

    void *e = unlink(list, list->head);

    if (out)
        *out = e;

    return CC_OK;
}

/**
 * Removes and returns the first (tail) element of the list. If the list is
 * empty, NULL is returned.
 *
 * @param[in] list the list from which the last element is being removed
 *
 * @return true if the operation was successful
 */
enum cc_stat list_remove_last(List *list, void **out)
{
    if (!list->size)
        return CC_ERR_VALUE_NOT_FOUND;

    Node *node = unlink(list, list->tail);

    if (out)
        *out = node->data;

    return CC_OK;
}

/**
 * Removes all elements from the specified list. This function returns true if at
 * least one element was removed, or false if the list was already empty.
 *
 * @param[in] list the list from which all elements are being removed
 *
 * @return true if the operation was successful and at least one element was
 *         removed, or false if the list was already empty
 */
enum cc_stat list_remove_all(List *list)
{
    bool unlinked = unlink_all(list, false);

    if (unlinked) {
        list->head = NULL;
        list->tail = NULL;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}


/**
 * Removes and frees all the elements from the specified list. This function
 * returns true if at least one element was removed and freed, or false if
 * the list was already empty.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list the list from which all the elements are being removed and
 *            freed
 *
 * @return true if the operation was successful and at least one element was
 *         removed or false if the list is already empty
 */
enum cc_stat list_remove_all_free(List *list)
{
    bool unlinked = unlink_all(list, false);

    if (unlinked) {
        list->head = NULL;
        list->tail = NULL;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Replaces an element at the specified location and returns the old element.
 * The specified index must be within the bounds of the list. This function
 * returns false if the specified index is out of range.
 *
 * @param[in] list the list on which this operation is performed
 * @param[in] element the replacement element
 * @param[in] index index of the element being replaced
 *
 * @return the replaced element
 */
enum cc_stat list_replace_at(List *list, void *element, size_t index, void **out)
{
    Node *node;
    enum cc_stat status = get_node_at(list, index, &node);

    if (status == CC_OK) {
        void *old  = node->data;
        node->data = element;
        if (out)
            *out = old;
        return CC_OK;
    }
    return status;
}

/**
 * Returns the first element from the specified list, or NULL if the list is
 * empty.
 *
 * @param[in] list the list whose first element is being returned.
 *
 * @return the first element of the list, or NULL in case the list is empty.
 */
enum cc_stat list_get_first(List *list, void **out)
{
    if (list->size == 0)
        return CC_ERR_VALUE_NOT_FOUND;

    *out = list->head->data;
    return CC_OK;
}

/**
 * Returns the last element from the specified list. or NULL if the list is
 * empty.
 *
 * @param[in] list list whose last element is being returned
 *
 * @return the last element of the list, or NULL in case the list is empty.
 */
enum cc_stat list_get_last(List *list, void **out)
{
    if (list->size == 0)
        return CC_ERR_VALUE_NOT_FOUND;

    *out = list->tail->data;
    return CC_OK;
}

/**
 * Returns the list element from the specified index. In case the index is out
 * of bounds, this function returns NULL instead.
 *
 * @param[in] list  list from which the element is being returned.
 * @param[in] index The index of a list element being returned. The index must
 *                  be within the bound of the list.
 *
 * @return The list element at the specified index.
 */
enum cc_stat list_get_at(List *list, size_t index, void **out)
{
    Node *node;
    enum cc_stat status = get_node_at(list, index, &node);

    if (status == CC_OK)
        *out = node->data;

    return status;
}

/**
 * Reverses the order of element in the specified list.
 *
 * @param[in] list list that is being reversed.
 */
void list_reverse(List *list)
{
    if (list->size == 0 || list->size == 1)
        return;

    Node *head_old = list->head;
    Node *tail_old = list->tail;

    Node *left  = list->head;
    Node *right = list->tail;

    size_t i;
    for (i = 0; i < list->size / 2; i++) {
        Node *tmpl = left->next;
        Node *tmpr = right->prev;

        swap(left, right);

        left  = tmpl;
        right = tmpr;
    }

    list->head = tail_old;
    list->tail = head_old;
}

/**
 * Returns a sublist of the specified list. The returned sublist contains all
 * the elements from the list that are contained between the two indices
 * including the elements at the indices. For example if a list contains 5
 * elements [5, 6, 7, 8, 9], a sublist from index 1 to 3 will will be a new
 * list of length 3, containing [6, 7, 8]. The returned sublist is only a copy of
 * the original lists structure, meaning the data it points to is not copied.
 *
 * @param[in] list the list from which the sublist is taken
 * @param[in] b    The beginning index, ie., the first element to be included.
 *                 Must be a positive integer and may not exceed the list size
 *                 or the end index.
 * @param[in] e    The ending index, ie., the last element to be included. Must
 *                 be a positive integer no greater that the list size and no
 *                 smaller that the beginning index.
 *
 * @return a new sublist or NULL if any of the indices are out of list bounds
 */
enum cc_stat list_sublist(List *list, size_t b, size_t e, List **out)
{
    if (b > e || e >= list->size)
        return CC_ERR_INVALID_RANGE;

    ListConf conf;

    conf.mem_alloc  = list->mem_alloc;
    conf.mem_calloc = list->mem_calloc;
    conf.mem_free   = list->mem_free;

    List *sub;
    enum cc_stat status = list_new_conf(&conf, &sub);

    if (status != CC_OK)
        return status;

    Node *node;
    status = get_node_at(list, b, &node);

    if (status != CC_OK) {
        list->mem_free(sub);
        return status;
    }

    size_t i;
    for (i = b; i <= e; i++) {
        status = list_add(sub, node->data);
        if (status != CC_OK) {
            list_destroy(sub);
            return status;
        }
        node = node->next;
    }
    *out = sub;
    return CC_OK;
}

/**
 * Returns a shallow copy of the specified list. A shallow copy is a copy of the
 * list structure. This operation does not copy the actual data that this list
 * holds.
 *
 * @param[in] list list to be copied
 *
 * @return a shallow copy of the list
 */
enum cc_stat list_copy_shallow(List *list, List **out)
{
    ListConf conf;

    conf.mem_alloc  = list->mem_alloc;
    conf.mem_calloc = list->mem_calloc;
    conf.mem_free   = list->mem_free;

    List *copy;
    enum cc_stat status = list_new_conf(&conf, &copy);

    if (status != CC_OK)
        return status;

    Node *node = list->head;

    if (!node) {
        *out = copy;
        return CC_OK;
    }
    while (node) {
        status = list_add(copy, node->data);
        if (status != CC_OK) {
            list_destroy(copy);
            return status;
        }
        node = node->next;
    }
    *out = copy;
    return CC_OK;
}

/**
 * Returns a deep copy of the specified list. This functions copies the structure
 * of the list along with all the data it holds. The element copying is done
 * through the specified copy function that should return a pointer to the copy
 * of the element passed to it.
 *
 * @param[in] list list to be copied
 * @param[in] cp   the copy function that should return a pointer to the copy of
 *                 the data.
 *
 * @return a deep copy of the list
 */
enum cc_stat list_copy_deep(List *list, void *(*cp) (void *e1), List **out)
{
    ListConf conf;

    conf.mem_alloc  = list->mem_alloc;
    conf.mem_calloc = list->mem_calloc;
    conf.mem_free   = list->mem_free;

    List *copy;
    enum cc_stat status =list_new_conf(&conf, &copy);

    if (status != CC_OK)
        return status;

    Node *node = list->head;

    if (!node) {
        *out = copy;
        return CC_OK;
    }

    while (node) {
        status = list_add(copy, cp(node->data));
        if (status != CC_OK) {
            list_destroy(copy);
            return status;
        }
        node = node->next;
    }
    *out = copy;
    return CC_OK;
}

/**
 * Returns an array representation of the specified list. None of the elements
 * are copied into the array and thus any modification of the elements within
 * the array will affect the list elements as well. The size of the returned
 * array is the same as the size of the list from which this array was
 * constructed.
 *
 * @param[in] list the list on which this operation is being performed.
 *
 * @return an array representation of the specified list
 */
enum cc_stat list_to_array(List *list, void ***out)
{
    if (list->size == 0)
        return CC_ERR_INVALID_RANGE;

    void **array = list->mem_calloc(list->size, sizeof(void*));

    if (!array)
        return CC_ERR_ALLOC;

    Node *node = list->head;

    size_t   i;
    for (i = 0; i < list->size; i++) {
        array[i] = node->data;
        node     = node->next;
    }
    *out = array;
    return CC_OK;
}

/**
 * Returns an integer representing the number of occurrences of the specified
 * element within the list.
 *
 * @param[in] list list on which the search is performed
 * @param[in] element element being looked for
 *
 * @return number of found matches
 */
size_t list_contains(List *list, void *element)
{
    Node   *node    = list->head;
    size_t  e_count = 0;

    while (node) {
        if (node->data == element)
            e_count++;
        node = node->next;
    }
    return e_count;
}

/**
 * Returns the index of the specified element, or <code>CC_ERR_OUT_OF_RANGE</code> if
 * the element is not found. The returned index is the index of the first
 * occurrence of the element starting from the beginning of the list.
 *
 * @param[in] list    the list on which this operation is performed
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element or <code>CC_ERR_OUT_OF_RANGE</code> if
 *         the element is not found.
 */
enum cc_stat list_index_of(List *list, void *element, size_t *index)
{
    Node   *node = list->head;
    size_t  i    = 0;

    while (node) {
        if (node->data == element) {
            *index = i;
            return CC_OK;
        }
        i++;
        node = node->next;
    }
    return CC_ERR_OUT_OF_RANGE;
}

/**
 * Returns the number of elements in the specified list.
 *
 * @param[in] list whose size is being returned
 *
 * @return the number of the elements contained in the specified list
 */
size_t list_size(List *list)
{
    return list->size;
}

/**
 * Sorts the specified list. This function makes no guaranties that the
 * sort will be performed in place or in a stable way.
 *
 * @note
 * Pointers passed to the comparator function will be pointers to the list
 * elements that are of type (void*) ie. void**. So an extra step of
 * dereferencing will be required before the data can be used for comparison:
 * eg. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @param[in] list list to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                int cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first.
 */
enum cc_stat list_sort(List *list, int (*cmp) (void const *e1, void const *e2))
{
    void **elements;
    enum cc_stat status = list_to_array(list, &elements);

    if (status != CC_OK)
        return status;

    Node *node = list->head;

    qsort(elements, list->size, sizeof(void*), cmp);

    size_t i;
    for (i = 0; i < list->size; i++) {
        node->data = elements[i];
        node       = node->next;
    }
    list->mem_free(elements);
    return CC_OK;
}

static Node *split(List *, Node *b, size_t l, int (*cmp) (void const *e1, void const *e2));
static void  merge(Node**, Node**, size_t, size_t, int (*cmp) (void const *e1, void const *e2));

/**
 * Sorts the specified list in place in a stable way.
 *
 * @note
 * Pointers passed to the comparator function will be pointers to the list
 * elements that are of type (void*) ie. void**. So an extra step of
 * dereferencing will be required before the data can be used for comparison:
 * eg. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @param[in] list list to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                int cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first.
 */
void list_sort_in_place(List *list, int (*cmp) (void const *e1, void const *e2))
{
    split(list, list->head, list->size, cmp);
}

/**
 * Splits the list section into two partitions.
 *
 * @param[in] list the sublist
 * @param[in] b    the head node of the sublist
 * @param[in] size number of nodes in the sublist
 * @param[in] cmp  Comparator function.
 *
 * @return
 */
static Node *split(List *list, Node *b, size_t size, int (*cmp) (void const*, void const*))
{
    if (size < 2)
        return b;

    size_t l_size = size / 2;
    size_t r_size = size / 2 + (size % 2);

    Node *center = b;

    size_t i;
    for (i = 0; i < l_size; i++)
        center = center->next;

    Node *l_head = split(list, b, l_size, cmp);
    Node *r_head = split(list, center, r_size, cmp);

    merge(&l_head, &r_head, l_size, r_size, cmp);

    list->head = l_head;
    list->tail = r_head;

    return l_head;
}

/**
 * Merges the two partitions together in place in an stable way. The left
 * partition starts at the "left" node and ends "l_size" nodes to the right of it.
 * Similarly the right partition starts at the "right" and ends "r_size" nodes
 * to the right. Both "left" and "right" parameters are used for input and
 * output. As an output parameter the "left" parameter will be assigned as the
 * head node of the newly merged section, while the "right" parameter will be
 * assigned the tail after the merge. The result of this function is a sorted
 * section of the list in an ascending order.
 *
 *@param[in, out] left   the beginning of the left partition as input and the
 *                       the head node of the merged section as the output.
 *@param[in, out] right  the beginning of the right partition as input and the
 *                       the tail node of the merged section as the output.
 *@param[in]      l_size size of the the left partition
 *@param[in]      r_size size of the right partition
 *@param[in]      cmp    the comparator function
 */
static INLINE void merge(Node **left, Node **right, size_t l_size,
                        size_t r_size, int (*cmp) (void const*, void const*))
{
    size_t size = r_size + l_size;
    size_t l    = 0; /* Number of processed elements from the left partition */
    size_t r    = 0; /* NUmber of processed elements from the right partition. */

    Node *l_part = *left;
    Node *r_part = *right;

    size_t i;
    for (i = 0; i < size; i++) {
        int c = cmp(&(l_part->data), &(r_part->data));

        if ((c < 0 || c == 0)) {
            /* The two partitions are already sorted. */
            if (i == 0 && size == 2) {
                break;
            }
            /* If all the elements on the left side are in place,
               all the elements on the right must be as well. */
            if (l == l_size) {
                /* Quickly advance the right pointer to the end for
                 * the remainder of r and exit.*/
                for (;r < r_size - 1; r++)
                    r_part = r_part->next;
                *right = r_part;
                break;
            }
            l_part = l_part->next;
            l++;
        } else {
            Node *tmp = (r_part)->next;
            link_behind(l_part, r_part);
            /* The elements are swapped. Now set head and tail and
             * exit the loop. */
            if (i == 0 && size == 2) {
                *right = l_part;
                *left  = r_part;
                break;
            }
            r++;
            /* All right partition elements are now behind the left
             * partition elements.*/
            if (r == r_size) {
                /* Quickly advance the left partition pointer and set
                 * the rightmost element as the new head. */
                for (;l < l_size - 1; l++)
                    l_part = l_part->next;
                *right = l_part;
                break;
            }
            if (i == 0) {
                *left = r_part;
            }
            r_part = tmp;
        }
    }
}

/**
 * A 'foreach loop' function that invokes the specified function on each element
 * in the list.
 *
 * @param[in] list the list on which this operation is being performed
 * @param[in] op the operation function that is to be invoked on each list
 *               element
 */
void list_foreach(List *list, void (*op) (void *e))
{
    Node *n = list->head;

    while (n) {
        op(n->data);
        n = n->next;
    }
}

/**
 *
 *
 */
void list_iter_init(ListIter *iter, List *list)
{
    iter->index = 0;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->head;
}

/**
 * Removes the last returned list element by the specified iterator. Since this
 * function removes the last returned element, it should only be called after a
 * call to <code>list_iter_next()</code>. Only the first call to this function
 * removes the element. Any subsequent calls will have no effect until the
 * <code>list_iter_next()</code> is called again.
 *
 * @note This function should only ever be called after a call to <code>
 * list_iter_next()</code>
 *
 * @param[in] iter the iterator on which this operation is being performed.
 *
 * @return the removed element, or NULL
 */
enum cc_stat list_iter_remove(ListIter *iter, void **out)
{
    if (!iter->last)
        return CC_ERR_VALUE_NOT_FOUND;

    void *e = unlink(iter->list, iter->last);
    iter->last = NULL;

    if (out)
        *out = e;
    return CC_OK;
}

/**
 * Adds a new element to the list. The element is added before the element that
 * would be returned by the next call to <code>list_iter_next()</code> and
 * after the last returned element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added to the list
 *
 * @return true if the operation was successful
 */
enum cc_stat list_iter_add(ListIter *iter, void *element)
{
    Node *new_node = iter->list->mem_calloc(1, sizeof(Node));

    if (!new_node)
        return CC_ERR_ALLOC;

    new_node->data = element;

    link_behind(iter->last, new_node);

    if ((iter->index - 1) == 0)
        iter->list->head = new_node;

    iter->list->size++;
    iter->index++;

    return CC_OK;
}

/**
 * Replaces and returns the most recently returned element by this iterator
 * with the new element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 *
 * @return the replaced element
 */
enum cc_stat list_iter_replace(ListIter *iter, void *element, void **out)
{
    if (!iter->last)
        return CC_ERR_VALUE_NOT_FOUND;

    void *old = iter->last->data;
    iter->last->data = element;

    if (out)
        *out = old;
    return CC_OK;
}

/**
 * Returns the index of the previously returned element.
 *
 * @param[in] iter the iterator on which this operation is performed.
 *
 * @return index of the previously returned element.
 */
size_t list_iter_index(ListIter *iter)
{
    return iter->index - 1;
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @note Before this function is called, one should check whether the next
 * element in the sequence exists by calling <code>list_iter_has_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed.
 *
 * @return the next element in the sequence
 */
enum cc_stat list_iter_next(ListIter *iter, void **out)
{
    if (!iter->next)
        return CC_ITER_END;

    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->next;
    iter->index++;

    if (out)
        *out = data;

    return CC_OK;
}


/**
 * Returns a new descending iterator. A descending iterator or a reverse
 * iterator, is an iterator that traverses the list from tail to head. In
 * case the memory allocation for the new iterator fails, NULL is returned.
 *
 * @param[in] iter the iterator
 * @param[in] list the list on which this iterator will operate.
 *
 * @return a new descending iterator.
 */
void list_diter_init(ListIter *iter, List *list)
{
    iter->index = list->size;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->tail;
}

/**
 * Adds a new element to the list. The element is added before the element that
 * would be returned by the next call to <code>list_diter_next()</code> and after
 * the last returned element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added to the list
 *
 * @return true if the operation was successful
 */
enum cc_stat list_diter_add(ListIter *iter, void *element)
{
    Node *new_node = iter->list->mem_calloc(1, sizeof(Node));

    if (!new_node)
        return CC_ERR_ALLOC;

    new_node->data = element;

    if (iter->index == 0)
        iter->list->head = new_node;

    link_behind(iter->last, new_node);

    iter->list->size++;
    iter->last = new_node;
    return CC_OK;
}

/**
 * Removes the last returned list element by the specified iterator. Since this
 * function removes the last returned element, it should only be called after a
 * call to <code>list_diter_next()</code>. Only the first call to this function
 * removes the element. Any subsequent calls will have no effect until the
 * <code>list_diter_next()</code> is called again.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_diter_next()</code>
 *
 * @param[in] iter the iterator on which this operation is being performed.
 *
 * @return the removed element, or NULL
 */
enum cc_stat list_diter_remove(ListIter *iter, void **out)
{
    if (!iter->last)
        return CC_ERR_VALUE_NOT_FOUND;

    void *e = unlink(iter->list, iter->last);
    iter->last = NULL;

    if (out)
        *out = e;

    return CC_OK;
}

/**
 * Replaces and returns the most recently returned element by this iterator with
 * the new element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 *
 * @return the replaced element
 */
enum cc_stat list_diter_replace(ListIter *iter, void *element, void **out)
{
    void *old = iter->last->data;
    iter->last->data = element;

    if (out)
        *out = old;
    return CC_OK;
}

/**
 * Returns the index of the last returned element by <code>list_diter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index of the last returned element
 */
size_t list_diter_index(ListIter *iter)
{
    return iter->index - 1;
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @note Before this function is called, one should check whether the next
 * element in the sequence exists by calling <code>list_diter_has_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed.
 *
 * @return the next element in the sequence
 */
enum cc_stat list_diter_next(ListIter *iter, void **out)
{
    if (!iter->next)
        return CC_ITER_END;

    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->prev;
    iter->index--;

    if (out)
        *out = data;

    return CC_OK;
}

/**
 * Links the <code>ins</code> node behind the <code>base</code> node.
 *
 * @param[in] base the node behind which the <code>ins</code> is going to be
 *                 linked
 * @param[in] ins  the node that is being linked behind the <code>base</code>
 *                 node
 */
static void link_behind(Node *const base, Node *ins)
{
    /* link the gap */
    if (ins->next != NULL)
        ins->next->prev = ins->prev;

    if (ins->prev != NULL)
        ins->prev->next = ins->next;

    /* link behind */
    if (base->prev == NULL) {
        ins->prev       = NULL;
        ins->next       = base;
        base->prev      = ins;
    } else {
        ins->prev       = base->prev;
        ins->prev->next = ins;
        ins->next       = base;
        base->prev      = ins;
    }
}

/**
 * Links the <code>ins</code> in front of the <code>base</code> node.
 *
 * @param[in] base the node in front of which the <code>ins</code> not is going
 *                 to be linked
 * @param[in] ins  the node that is being linked in front the <code>base</code>
 *                 node
 */
static void link_after(Node *base, Node *ins)
{
    if (ins->next)
        ins->next->prev = ins->prev;

    if (ins->prev)
        ins->prev->next = ins->next;

    if (!base->next) {
        ins->prev       = base;
        base->next      = ins;
        ins->next       = NULL;
    } else {
        ins->next       = base->next;
        ins->next->prev = ins;
        ins->prev       = base;
        base->next      = ins;
    }
}

/**
 * Swaps the list positions of the specified nodes.
 *
 * @param[in] n1 the first node
 * @param[in] n2 the second node
 */
static void swap(Node *n1, Node *n2)
{
    /* A special case is when the nodes are next to each other */
    if (n1->next == n2 || n2->next == n1) {
        swap_adjacent(n1, n2);
        return;
    }

    Node *n1_left  = n1->prev;
    Node *n1_right = n1->next;
    Node *n2_left  = n2->prev;
    Node *n2_right = n2->next;

    if (n1_left)
        n1_left->next = n2;

    n2->prev = n1_left;

    if (n1_right)
        n1_right->prev = n2;

    n2->next = n1_right;

    if (n2_left)
        n2_left->next = n1;

    n1->prev = n2_left;

    if (n2_right)
        n2_right->prev = n1;

    n1->next = n2_right;
}

/**
 * Swaps two adjacent nodes. This handles the special case when the nodes
 * are adjacent to each other.
 *
 * @param[in] n1 the first node
 * @param[in] n2 the second node
 */
static void swap_adjacent(Node *n1, Node *n2)
{
    if (n1->next == n2) {
        if (n2->next)
            n2->next->prev = n1;

        n1->next = n2->next;

        if (n1->prev)
            n1->prev->next = n2;

        n2->prev = n1->prev;

        n1->prev = n2;
        n2->next = n1;
        return;
    }

    if (n2->next == n1) {
        if (n1->next)
            n1->next->prev = n2;

        n2->next = n1->next;

        if (n2->prev)
            n2->prev->next = n1;

        n1->prev = n2->prev;

        n2->prev = n1;
        n1->next = n2;
        return;
    }
}

/**
 * Unlinks a node from the list and returns the data that was associated with it.
 *
 * @param[in] list the list from which the node is being unlinked
 * @param[in] node the node being unlinked
 *
 * @return the data that was at this node
 */
static void *unlink(List *list, Node *node)
{
    void *data = node->data;

    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->prev == NULL)
        list->head = node->next;

    if (node->next == NULL)
        list->tail = node->prev;

    if (node->next != NULL)
        node->next->prev = node->prev;

    list->mem_free(node);
    list->size--;

    return data;
}

/**
 * Unlinks all nodes from the list and optionally frees the data at the nodes.
 *
 * @param[in] list the list from which all the nodes are being unlinked
 * @param[in] freed a bool that determines whether or not the data at the nodes
 *                  should also be freed.
 *
 * @return false if the list is already empty, otherwise returns true
 */
static bool unlink_all(List *list, bool freed)
{
    if (list->size == 0)
        return false;

    Node *node = list->head;

    while (node) {
        Node *tmp = node->next;

        if (freed)
            list->mem_free(node->data);

        unlink(list, node);
        node = tmp;
    }
    return true;
}

/**
 * Returns the node at the specified index. If the index is not in the bounds of
 * the list, NULL is returned.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] index the index of the node being returned
 *
 * @return the node at the specified index, or NULL if the index is out of bounds
 */
static enum cc_stat get_node_at(List *list, size_t index, Node **out)
{
    if (!list || index >= list->size)
        return CC_ERR_OUT_OF_RANGE;

    size_t i;
    Node *node = NULL;

    if (index < list->size / 2) {
        node = list->head;
        for (i = 0; i < index; i++)
            node = node->next;
    } else {
        node = list->tail;
        for (i = list->size - 1; i > index; i--)
            node = node->prev;
    }
    *out = node;
    return CC_OK;
}

/**
 * Returns the node first node from the beginning of the list that is associated
 * with the specified element. If no node is associated with the element, NULL is
 * returned instead.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] element the element whose list node is being returned
 *
 * @return the node associated with the specified element
 */
static Node *get_node(List *list, void *element)
{
    Node *node = list->head;
    while (node) {
        if (node->data == element)
            return node;
        node = node->next;
    }
    return NULL;
}
