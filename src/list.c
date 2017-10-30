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
static bool  unlink_all          (List *list, void (*cb) (void*));
static void  link_behind         (Node *node, Node *inserted);
static void  link_after          (Node *base, Node *inserted);
static void  swap                (Node *n1, Node *n2);
static void  swap_adjacent       (Node *n1, Node *n2);
static void  splice_between      (List *list1, List *list2, Node *left, Node *right);
static bool  link_all_externally (List *l, Node **h, Node **t);
static Node *get_node            (List *list, void *element);
static enum cc_stat get_node_at  (List *list, size_t index, Node **out);
static enum cc_stat add_all_to_empty    (List *l1, List *l2);


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
 * Creates a new empty list and returns a status code.
 *
 * @param[out] out pointer to where the newly created List is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if
 * the memory allocation for the new List failed.
 */
enum cc_stat list_new(List **out)
{
    ListConf lc;
    list_conf_init(&lc);
    return list_new_conf(&lc, out);
}

/**
 * Creates a new empty list based on the specified ListConf struct and
 * returns a status code.
 *
 * The List is allocated using the allocators specified in the ListConf
 * struct. The allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf List configuration struct. All fields must be
 *                 initialized to appropriate values.
 * @param[out] out Pointer to where the newly created List is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new List structure failed.
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
 * Destroys the list structure, but leaves the data that it holds intact.
 *
 * @param[in] list list that is to be destroyed
 */
void list_destroy(List *list)
{
    if (list->size > 0)
        list_remove_all(list);

    list->mem_free(list);
}

/**
 * Destroys the list structure along with all the data it holds.
 *
 * @note
 * This function should not be called on a list that has some of its elements
 * allocated on the stack.
 *
 * @param[in] list list that is to be destroyed
 */
void list_destroy_cb(List *list, void (*cb) (void*))
{
    list_remove_all_cb(list, cb);
    list->mem_free(list);
}

/**
 * Adds a new element to the list. The element is appended to the list making it
 * the last element in the list.
 *
 * @param[in] list list to which the element is being added
 * @param[in] element element being added
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC if the
 * memory allocation for the new element failed.
 */
enum cc_stat list_add(List *list, void *element)
{
    return list_add_last(list, element);
}

/**
 * Prepends a new element to the list (adds a new "head") making it the first
 * element of the list.
 *
 * @param[in] list list to which the element is being added
 * @param[in] element element being prepended
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC if the
 * memory allocation for the new element failed.
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
 * element of the list.
 *
 * @param[in] list list to which the element is being added
 * @param[in] element element being appended
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC if the
 * memory allocation for the new element failed.
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
 * Adds a new element at the specified location in the List and shifts all
 * subsequent elements by one. This operation cannot be performed on an empty
 * List. The index at which the new element is being added must be within the
 * bounds of the List.
 *
 * @param[in] list list to which this element is being added
 * @param[in] element element that is being added
 * @param[in] index the position in the list at which the new element is being
 *                  added
 *
 * @return CC_OK if the element was successfully added, CC_ERR_OUT_OF_RANGE if
 * the specified index was not in range, or CC_ERR_ALLOC if the memory
 * allocation for the new element failed.
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
 * second list are added after the last element of the first list.
 *
 * @param[in] list1 list to which the elements are being added
 * @param[in] list2 list from which the elements are being taken
 *
 * @return CC_OK if the elements where successfully added, or CC_ERR_ALLOC if
 * the memory allocation for the new elements failed.
 */
enum cc_stat list_add_all(List *list1, List *list2)
{
    if (list1->size == 0)
        return add_all_to_empty(list1, list2);

    return list_add_all_at(list1, list2, list1->size);
}

/**
 * Adds all elements from the second list to the first list which is empty.
 *
 * @param[in] list1 list to which the elements are being added
 * @param[in] list2 list from which the elements are being taken
 *
 * @return CC_OK if the element were successfully added, or CC_ERR_ALLOC if
 * the memory allocation for the new elements failed.
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
 * range at which the elements can be added ranges from 0 to max_index + 1.
 *
 * @param[in] list1 list to which the elements are being added
 * @param[in] list2 list from which the elements are being taken
 * @param[in] index position in the first list at which the element should be
 *                  added
 *
 * @return CC_OK if the elements were successfully added,
 * CC_ERR_INDEX_OUT_OF_BOUNDS if the index was out of range, or
 * CC_ERR_ALLOC if the memory allocation for the new elements failed.
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
 * @return true if the operation was successful, false otherwise.
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
 * the first. This function moves all elements from the second list into the
 * first list, leaving the second list empty.
 *
 * @param[in] list1 the consumer list to which the elements are moved
 * @param[in] list2 the producer list from which the elements are moved
 *
 * @return CC_OK if the elements were successfully moved, or CC_ERR_OUT_OF_RANGE
 * if the index was not in range.
 */
enum cc_stat list_splice(List *list1, List *list2)
{
    return list_splice_at(list1, list2, list1->size);
}

/**
 * Splices the two lists together at the specified index of the first list. This
 * function moves all elements from the second list into the first list at the
 * position specified by the <code>index</code> parameter. After this operation
 * the second list will be left empty.
 *
 * @param[in] list1 the consumer list to which the elements are moved
 * @param[in] list2 the producer list from which the elements are moved
 * @param[in] index the index in the first list after which the elements from the
 *                  second list should be inserted
 *
 * @return CC_OK if the elements were successfully moved, or CC_ERR_OUT_OF_RANGE
 * if the index was not in range.
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
 * node is NULL, the head of the second list will become the head of the first
 * list. Similarly, if the right node is null, the tail of the first list will
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
 * Removes the first occurrence of the element from the specified List
 * and optionally sets the out parameter to the value of the removed
 * element.
 *
 * @param[in] list list from which the element is being removed
 * @param[in] element element that is being removed
 * @param[out] out pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND if the element was not found.
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
 * Removes the element at the specified index and optionally sets the out
 * parameter to the value of the removed element. The index must be
 * within the bounds of the list.
 *
 * @param[in] list List from which the element is being removed.
 * @param[in] index Index of the element is being removed. Must be be within the
 *            index range of the list.
 * @param[out] out Pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored.
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_OUT_OF_RANGE if the index was out of range.
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
 * Removes and returns the first (head) element of the list and optionally sets the
 * out parameter to the value of the removed element.
 *
 * @param[in] list list from which the first element is being removed
 * @param[out] out pointer to where the removed value is stored, or NULL if it is
 *                 to be ignored
 *
 * @return CC_OK if the element was successfully removed, or CC_ERR_VALUE_NOT_FOUND
 * if the List is already empty.
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
 * Removes and returns the last (tail) element of the list and optionally sets the
 * out parameter to the value of the removed element.
 *
 * @param[in] list list from which the last element is being removed
 * @param[out] out pointer to where the removed value is stored, or NULL if it is
 *                 to be ignored
 *
 * @return CC_OK if the element was successfully removed, or CC_ERR_VALUE_NOT_FOUND
 * if the List is already empty.
 */
enum cc_stat list_remove_last(List *list, void **out)
{
    if (!list->size)
        return CC_ERR_VALUE_NOT_FOUND;

    void *e = unlink(list, list->tail);

    if (out)
        *out = e;

    return CC_OK;
}

/**
 * Removes all elements from the specified list.
 *
 * @param[in] list list from which all elements are being removed
 *
 * @return CC_OK if the elements were successfully removed, or CC_ERR_VALUE_NOT_FOUND
 *  if the list was already empty.
 */
enum cc_stat list_remove_all(List *list)
{
    bool unlinked = unlink_all(list, NULL);

    if (unlinked) {
        list->head = NULL;
        list->tail = NULL;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Removes and frees all elements from the specified list.
 *
 * @note
 * This function should not be called on a list that has some of its elements
 * allocated on the stack.
 *
 * @param[in] list list from which all the elements are being removed and
 *            freed
 *
 * @return CC_OK if the elements were successfully removed and freed, or
 * CC_ERR_VALUE_NOT_FOUND if the list was already empty.
 */
enum cc_stat list_remove_all_cb(List *list, void (*cb) (void*))
{
    bool unlinked = unlink_all(list, cb);

    if (unlinked) {
        list->head = NULL;
        list->tail = NULL;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Replaces an element at the specified location and optionally sets the
 * out parameter to the value of the replaced element. The specified index
 * must be within the bounds of the list.
 *
 * @param[in] list list on which this operation is performed
 * @param[in] element the replacement element
 * @param[in] index index of the element that is being replaced
 *
 * @return CC_OK if the element was successfully replaced, or CC_ERR_OUT_OF_RANGE
 * if the index was out of range.
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
    }

    return status;
}

/**
 * Gets the first element from the specified list and sets the out parameter to
 * its value.
 *
 * @param[in] list list whose first element is being returned
 * @param[in] out  pointer to where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat list_get_first(List *list, void **out)
{
    if (list->size == 0)
        return CC_ERR_VALUE_NOT_FOUND;

    *out = list->head->data;
    return CC_OK;
}

/**
 * Gets the last element from the specified list and sets the parameter to
 * its value.
 *
 * @param[in] list list whose last element is being returned
 * @param[in] out  pointer to where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_VALUE_NOT_FOUND if not.
 */
enum cc_stat list_get_last(List *list, void **out)
{
    if (list->size == 0)
        return CC_ERR_VALUE_NOT_FOUND;

    *out = list->tail->data;
    return CC_OK;
}

/**
 * Gets the list element from the specified index and sets the out parameter to
 * its value.
 *
 * @param[in] list  List from which the element is being returned.
 * @param[in] index The index of a list element being returned. The index must
 *                  be within the bound of the list.
 * @param[out] out  Pointer to where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_OUT_OF_RANGE if the index
 * was out of range.
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
 * Reverses the order of elements in the specified list.
 *
 * @param[in] list list that is being reversed
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
 * Creates a sublist of the specified List. The created sublist contains all
 * the elements from the List that are contained between the two indices
 * including the elements at the indices. For example, if a list contains 5
 * elements [5, 6, 7, 8, 9], a sublist from index 1 to 3 will will be a new
 * List of length 3, containing [6, 7, 8]. The returned sublist is only a copy of
 * the original lists structure, meaning the data it points to is not copied.
 *
 * @param[in] list List from which the sublist is taken.
 * @param[in] b    The beginning index, i.e., the first element to be included.
 *                 Must be a positive integer and may not exceed the list size
 *                 or the end index.
 * @param[in] e    The ending index, i.e., the last element to be included. Must
 *                 be a positive integer no greater than the list size and no
 *                 smaller that the beginning index.
 * @param[out] out Pointer to where the new sublist is stored.
 *
 * @return CC_OK if the sublist was successfully created, CC_ERR_INVALID_RANGE
 * if the specified index range is invalid, or CC_ERR_ALLOC if the memory allocation
 * for the new sublist failed.
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
 * Creates a shallow copy of the specified list. A shallow copy is a copy of the
 * list structure. This operation does not copy the actual data that this list
 * holds.
 *
 * @note The new list is allocated using the original lists allocators and also
 *       inherits the configuration of the original list.
 *
 * @param[in] list list to be copied
 * @param[out] out pointer to where the newly created copy is stored
 *
 * @return CC_OK if the copy was successfully created, or CC_ERR_ALLOC if the
 * memory allocation for the copy failed.
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
 * Creates a deep copy of the specified list. This functions copies the structure
 * of the list along with all the data it holds. The element copying is done
 * through the specified copy function that should return a pointer to the copy
 * of the element passed to it.
 *
 * @param[in] list list to be copied
 * @param[in] cp   the copy function that should return a pointer to the copy of
 *                 the data
 * @param[out] out pointer to where the newly created copy is stored
 *
 * @return  CC_OK if the copy was successfully created, or CC_ERR_ALLOC if the
 * memory allocation for the copy failed.
 */
enum cc_stat list_copy_deep(List *list, void *(*cp) (void *e1), List **out)
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
 * Creates an array representation of the specified list. None of the elements
 * are copied into the array and thus any modification of the elements within
 * the array will affect the list elements as well. The size of the created
 * array is the same as the size of the list from which this array was constructed.
 *
 * @param[in] list list on which this operation is being performed
 * @param[out] out pointer to where the newly created array is stored
 *
 * @return CC_OK if the array was successfully created, CC_ERR_INVALID_RANGE if the
 * list is emtpy, or CC_ERR_ALLOC if the memory allocation for the new array failed.
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
 * @param[in] element element being searched for
 *
 * @return number of matches found.
 */
size_t list_contains(List *list, void *element)
{
    return list_contains_value(list, element, cc_common_cmp_ptr);
}

/**
 * Returns the number occurrences of the value pointed to by <code>element</code>
 * within the List.
 *
 * @param[in] list list on which the search is performed
 * @param[in] element element being searched for
 * @param[in] cmp comparator function which returns 0 if the values passed to it
 *                are equal
 *
 * @return number of matches found.
 */
size_t list_contains_value(List *list, void *element, int (*cmp) (const void*, const void*))
{
    Node *node = list->head;
    size_t e_count = 0;

    while (node) {
        if (cmp(node->data, element) == 0)
            e_count++;
        node = node->next;
    }
    return e_count;
}

/**
 * Gets the index of the specified element. The returned index is the index
 * of the first occurrence of the element starting from the beginning of
 * the list.
 *
 * @param[in] list list on which this operation is performed
 * @param[in] element the element whose index is being looked up
 * @param[in] cmp comparator function which returns 0 if the values passed to it
 *                are equal
 * @param[out] index pointer to where the index is stored
 *
 * @return CC_OK if the index was found, or CC_OUT_OF_RANGE if not.
 */
enum cc_stat list_index_of(List *list, void *element, int (*cmp) (const void*, const void*), size_t *index)
{
    Node   *node = list->head;
    size_t  i    = 0;

    while (node) {
        if (cmp(node->data, element) == 0) {
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
 * @param[in] list list whose size is being returned
 *
 * @return the number of the elements contained in the specified list.
 */
size_t list_size(List *list)
{
    return list->size;
}

/**
 * Sorts the specified list. This function makes no guaranties that the
 * sort will be performed in place or in a stable way.
 *
 * @note Pointers passed to the comparator function will be pointers to
 *       the list elements that are of type (void*), i.e. void**. So an
 *       extra step of dereferencing will be required before the data can
 *       be used for comparison:
 *       e.g. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @param[in] list list to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                int cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first
 *
 * @return CC_OK if the sort was performed successfully, or CC_ERR_ALLOC
 * if it could not allocate enough memory to perform the sort.
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
 * @note Pointers passed to the comparator function will be pointers to the list
 *       elements that are of type (void*), i.e. void**. So an extra step of
 *       dereferencing will be required before the data can be used for comparison:
 *       e.g. <code>my_type e = *(*((my_type**) ptr));</code>.
 *
 * @param[in] list list to be sorted
 * @param[in] cmp the comparator function that must be of type <code>
 *                int cmp(const void e1*, const void e2*)</code> that
 *                returns < 0 if the first element goes before the second,
 *                0 if the elements are equal and > 0 if the second goes
 *                before the first
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
 * @param[in] cmp  comparator function
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
 * Merges the two partitions together in place in a stable way. The left
 * partition starts at the "left" node and ends "l_size" nodes to the right of it.
 * Similarly the right partition starts at the "right" and ends "r_size" nodes
 * to the right. Both "left" and "right" parameters are used for input and
 * output. As an output parameter the "left" parameter will be assigned as the
 * head node of the newly merged section, while the "right" parameter will be
 * assigned the tail after the merge. The result of this function is a sorted
 * section of the list in an ascending order.
 *
 *@param[in, out] left   the beginning of the left partition as input and the
 *                       the head node of the merged section as the output
 *@param[in, out] right  the beginning of the right partition as input and the
 *                       the tail node of the merged section as the output
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
 * @param[in] list list on which this operation is being performed
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
 * Filters the List by modifying it. It removes all elements that don't
 * return true on pred(element).
 *
 * @param[in] list list that is to be filtered
 * @param[in] pred predicate function which returns true if the element should
 *                 be kept in the List
 *
 * @return CC_OK if the list was filtered successfully, or CC_ERR_OUT_OF_RANGE
 * if the List is empty.
 */
enum cc_stat list_filter_mut(List *list, bool (*pred) (const void*))
{
    if (list_size(list) == 0)
        return CC_ERR_OUT_OF_RANGE;

    Node *curr = list->head;
    Node *next = NULL;

    while (curr) {
        next = curr->next;

        if (!pred(curr->data)) {
            unlink(list, curr);
        }
        curr = next;
    }

    return CC_OK;
}

/**
 * Filters the List by creating a new List that contains all elements from the
 * original List that return true on pred(element) without modifying the original
 * list.
 *
 * @param[in] list list that is to be filtered
 * @param[in] pred predicate function which returns true if the element should
 *                 be kept in the filtered list
 * @param[out] out pointer to where the new filtered list is to be stored
 *
 * @return CC_OK if the list was filtered successfully, CC_ERR_OUT_OF_RANGE
 * if the list is empty, or CC_ERR_ALLOC if the memory allocation for the
 * new list failed.
 */
enum cc_stat list_filter(List *list, bool (*pred) (const void*), List **out)
{
    if (list_size(list) == 0)
        return CC_ERR_OUT_OF_RANGE;

    List *filtered = NULL;
    list_new(&filtered);

    if (!filtered)
        return CC_ERR_ALLOC;

    Node *curr = list->head;

    while (curr) {
        if (pred(curr->data)) {
            list_add(filtered, curr->data);
        }
        curr = curr->next;
    }

    *out = filtered;

    return CC_OK;
}


/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] list list to iterate over
 */
void list_iter_init(ListIter *iter, List *list)
{
    iter->index = 0;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->head;
}

/**
 * Removes the last returned element by <code>list_iter_next()</code>
 * function without invalidating the iterator and optionally sets the out
 * parameter to the value of the removed element.
 *
 * @note This function should only ever be called after a call to <code>
 * list_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[out] out pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND.
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
 * Adds a new element to the list after the last returned element by
 * <code>list_iter_next()</code> function without invalidating the
 * iterator.
 *
 * @note This function should only ever be called after a call to <code>
 * list_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added to the list
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC
 * if the memory allocation for the new element failed.
 */
enum cc_stat list_iter_add(ListIter *iter, void *element)
{
    Node *new_node = iter->list->mem_calloc(1, sizeof(Node));

    if (!new_node)
        return CC_ERR_ALLOC;

    new_node->data = element;

    link_after(iter->last, new_node);

    if (iter->index == iter->list->size)
        iter->list->tail = new_node;

    iter->list->size++;
    iter->index++;

    return CC_OK;
}

/**
 * Replaces the last returned element by <code>list_iter_next()</code>
 * with the specified element and optionally sets the out parameter to
 * the value of the replaced element.
 *
 * @note This function should only ever be called after a call to <code>
 * list_iter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @param[out] out pointer to where the replaced element is stored, or NULL
 *                if it is to be ignored
 *
 * @return CC_OK if the element was replaced successfully, or
 * CC_ERR_VALUE_NOT_FOUND.
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
 * Returns the index of the last returned element by <code>list_iter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is performed
 *
 * @return the index.
 */
size_t list_iter_index(ListIter *iter)
{
    return iter->index - 1;
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element in the sequence.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[out] out pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the list has been reached.
 */
enum cc_stat list_iter_next(ListIter *iter, void **out)
{
    if (!iter->next)
        return CC_ITER_END;

    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->next;
    iter->index++;

    *out = data;

    return CC_OK;
}

/**
 * Returns a new descending iterator. A descending iterator or a reverse
 * iterator, is an iterator that traverses the list from tail to head.
 *
 * @param[in] iter the iterator
 * @param[in] list list on which this iterator will operate
 */
void list_diter_init(ListIter *iter, List *list)
{
    iter->index = list->size;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->tail;
}

/**
 * Adds a new element to the list after the last returned element by
 * <code>list_diter_next()</code> function (or before the element in
 * the list) without invalidating the  iterator.
 *
 * @note This function should only ever be called after a call to <code>
 * list_diter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added to the list
 *
 * @return CC_OK if the element was successfully added, or CC_ERR_ALLOC
 * if the memory allocation for the new element failed.
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
 * Removes the last returned element by <code>list_diter_next()</code>
 * function without invalidating the iterator and optionally sets the out
 * parameter to the value of the removed element.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_diter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[out] out pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the element was successfully removed, or
 * CC_ERR_VALUE_NOT_FOUND.
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
 * Replaces the last returned element by <code>list_diter_next()</code>
 * with the specified element and optionally sets the out parameter to
 * the value of the replaced element.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_diter_next()</code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @param[out] out pointer to where the replaced element is stored, or NULL
 *                if it is to be ignored
 *
 * @return CC_OK if the element was replaced successfully, or
 * CC_ERR_VALUE_NOT_FOUND.
 */
enum cc_stat list_diter_replace(ListIter *iter, void *element, void **out)
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
 * Returns the index of the last returned element by <code>list_diter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index.
 */
size_t list_diter_index(ListIter *iter)
{
    return iter->index - 1;
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next element in the sequence.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[out] out pointer to where the next element is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the list has been reached.
 */
enum cc_stat list_diter_next(ListIter *iter, void **out)
{
    if (!iter->next)
        return CC_ITER_END;

    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->prev;
    iter->index--;

    *out = data;

    return CC_OK;
}

/**
 * Initializes the zip iterator.
 *
 * @param[in] iter iterator that is being initialized
 * @param[in] l1   first list
 * @param[in] l2   second list
 */
void list_zip_iter_init(ListZipIter *iter, List *l1, List *l2)
{
    iter->index   = 0;
    iter->l1      = l1;
    iter->l2      = l2;
    iter->l1_last = NULL;
    iter->l2_last = NULL;
    iter->l1_next = l1->head;
    iter->l2_next = l2->head;
}

/**
 * Outputs the next element pair in the sequence and advances the iterator.
 *
 * @param[in]  iter iterator that is being advanced
 * @param[out] out1 output of the first list element
 * @param[out] out2 output of the second list element
 *
 * @return CC_OK if a next element pair is returned, or CC_ITER_END if the end of one
 * of the lists has been reached.
 */
enum cc_stat list_zip_iter_next(ListZipIter *iter, void **out1, void **out2)
{
    if (!iter->l1_next || !iter->l2_next)
        return CC_ITER_END;

    void *data1 = iter->l1_next->data;
    void *data2 = iter->l2_next->data;

    iter->l1_last = iter->l1_next;
    iter->l2_last = iter->l2_next;
    iter->l1_next = iter->l1_next->next;
    iter->l2_next = iter->l2_next->next;
    iter->index++;

    *out1 = data1;
    *out2 = data2;

    return CC_OK;
}

/**
 * Adds a new element pair to the lists after the last returned element pair by
 * <code>list_zip_iter_next()</code> and immediately before an element pair
 * that would be returned by a subsequent call to <code>list_zip_iter_next()</code>
 * without invalidating the iterator.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_zip_iter_next()</code>
 *
 * @param[in] iter Iterator on which this operation is being performed
 * @param[in] e1   element added to the first list
 * @param[in] e2   element added to the second list
 *
 * @return CC_OK if the element pair was successfully added to the lists, or
 * CC_ERR_ALLOC if the memory allocation for the new elements failed.
 */
enum cc_stat list_zip_iter_add(ListZipIter *iter, void *e1, void *e2)
{
    Node *new_node1 = iter->l1->mem_calloc(1, sizeof(Node));

    if (!new_node1)
        return CC_ERR_ALLOC;

    Node *new_node2 = iter->l2->mem_calloc(1, sizeof(Node));

    if (!new_node2) {
        iter->l1->mem_free(new_node1);
        return CC_ERR_ALLOC;
    }

    new_node1->data = e1;
    new_node2->data = e2;

    link_after(iter->l1_last, new_node1);
    link_after(iter->l2_last, new_node2);

    if (iter->index == iter->l1->size)
        iter->l1->tail = new_node1;

    if (iter->index == iter->l2->size)
        iter->l2->tail = new_node2;

    iter->l1->size++;
    iter->l2->size++;
    iter->index++;

    return CC_OK;
}

/**
 * Removes and outputs the last returned element pair by <code>list_zip_iter_next()
 * </code> without invalidating the iterator.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_zip_iter_next()</code>.
 *
 * @param[in]  iter iterator on which this operation is being performed
 * @param[out] out1 output of the removed element from the first List
 * @param[out] out2 output of the removed element from the second List
 *
 * @return CC_OK if the element was removed successfully, or CC_ERR_VALUE_NOT_FOUND.
 */
enum cc_stat list_zip_iter_remove(ListZipIter *iter, void **out1, void **out2)
{
    if (!iter->l1_last || !iter->l2_last)
        return CC_ERR_VALUE_NOT_FOUND;

    void *e1 = unlink(iter->l1, iter->l1_last);
    void *e2 = unlink(iter->l2, iter->l2_last);

    iter->l1_last = NULL;
    iter->l2_last = NULL;

    if (out1)
        *out1 = e1;

    if (out2)
        *out2 = e2;

    return CC_OK;
}

/**
 * Replaces the last returned element pair by <code>list_zip_iter_next()</code>
 * with the specified replacement element pair.
 *
 * @note This function should only ever be called after a call to
 *       <code>list_zip_iter_next()</code>.
 *
 * @param[in]  iter iterator on which this operation is being performed
 * @param[in]  e1   first list's replacement element
 * @param[in]  e2   second list's replacement element
 * @param[out] out1 output of the replaced element from the first list
 * @param[out] out2 output of the replaced element from the second list
 *
 * @return CC_OK if the element was replaced successfully, or CC_ERR_VALUE_NOT_FOUND.
 */
enum cc_stat list_zip_iter_replace(ListZipIter *iter, void *e1, void *e2, void **out1, void **out2)
{
    if (!iter->l1_last || !iter->l2_last)
        return CC_ERR_VALUE_NOT_FOUND;

    void *old1 = iter->l1_last->data;
    void *old2 = iter->l2_last->data;

    iter->l1_last->data = e1;
    iter->l2_last->data = e2;

    if (out1)
        *out1 = old1;

    if (out2)
        *out2 = old2;

    return CC_OK;
}

/**
 * Returns the index of the last returned element pair by <code>list_zip_iter_next()</code>.
 *
 * @param[in] iter iterator on which this operation is being performed
 *
 * @return current iterator index.
 */
size_t list_zip_iter_index(ListZipIter *iter)
{
    return iter->index - 1;
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
 * @return the data that was at this node.
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
 *                  should also be freed
 *
 * @return false if the list is already empty, otherwise returns true.
 */
static bool unlink_all(List *list, void (*cb) (void*))
{
    if (list->size == 0)
        return false;

    Node *node = list->head;

    while (node) {
        Node *tmp = node->next;

        if (cb)
            cb(node->data);

        unlink(list, node);
        node = tmp;
    }
    return true;
}

/**
 * Returns the node at the specified index.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] index the index of the node being returned
 *
 * @return CC_OK if the node was found, or CC_ERR_OUT_OF_RANGE if not.
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
 * Returns the first node from the beginning of the list that is associated
 * with the specified element. If no node is associated with the element, NULL is
 * returned instead.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] element the element whose list node is being returned
 *
 * @return the node associated with the specified element.
 */
static Node *get_node(List *list, void *element)
{
    Node *node = list->head;
    while (node) {
        if (cc_common_cmp_ptr(node->data, element) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}
