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

#include "slist.h"


struct slist_s {
    size_t  size;
    Node   *head;
    Node   *tail;

    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
};

static void* unlink              (SList *list, Node *node, Node *prev);
static bool  unlink_all          (SList *list, bool freed);
static void  splice_between      (SList *list1, SList *list2, Node *base, Node *end);
static bool  get_node_at         (SList *list, size_t index, Node **node, Node **prev);
static bool  get_node            (SList *list, void *element, Node **node, Node **prev);
static bool  link_all_externally (SList *list, Node **h, Node **t);

/**
 * Initializes the fields SListConf struct to default values.
 *
 * @param[in] conf the SListConf struct that is being initialized.
 */
void slist_conf_init(SListConf *conf)
{
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Returns a new empty list, or NULL if the memory allocation fails.
 *
 * @return a new list, or NULL if the memory allocation fails
 */
SList *slist_new()
{
    SListConf conf;
    slist_conf_init(&conf);
    return slist_new_conf(&conf);
}

/**
 * Returns a new empty SList based on the specified SListConf struct.
 *
 * The SList is allocated using the allocators specified in the SListConf
 * struct. THe allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf SList configuration. All fields must be initialized
 *                 to appropriate values.
 *
 * @return a new SList if the allocation was successful, or NULL if not.
 */
SList *slist_new_conf(SListConf *conf)
{
    SList *list = conf->mem_calloc(1, sizeof(SList));

    list->mem_alloc  = conf->mem_alloc;
    list->mem_calloc = conf->mem_calloc;
    list->mem_free   = conf->mem_free;

    return list;
}

/**
 * Destroys the list structure, but leaves the data that is holds intact
 *
 * @param[in] list a list to destroy
  *
 * @return true if the operation was successful
 */
bool slist_destroy(SList *list)
{
    bool success = slist_remove_all(list);
    list->mem_free(list);
    return success;
}

/**
 * Destroys the list structure along with all the data it holds. This function
 * returns true if the operation was successful, or false if the list was already
 * empty.
 *
 * @node
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list the list to be destroyed
 * @return true if the operation was successful
 */
bool slist_destroy_free(SList *list)
{
    bool success = slist_remove_all_free(list);
    list->mem_free(list);
    return success;
}

/**
 * Adds a new element to the list. The element is appended to the list making it
 * the last element of the list. This function returns false if the memory
 * allocation for the new element has fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being added
 *
 * @return true if the element was successfully added to the list.
 */
bool slist_add(SList *list, void *element)
{
    return slist_add_last(list, element);
}

/**
 * Prepends a new element to the list (adds a new "head") making it the first
 * element of the list. This function returns false if the memory allocation for
 * the new element fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being added
 *
 * @return true if the element was successfully added to the list
 */
bool slist_add_first(SList *list, void *element)
{
    Node *node = list->mem_calloc(1, sizeof(Node));

    if (!node)
        return false;

    node->data = element;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head = node;
    }
    list->size++;
    return true;
}

/**
 * Appends a new element to the list (adds a new "tail") making it the last
 * element of the list. This function returns false if the memory allocation for
 * the new element fails.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being added
 *
 * @return true if the element was successfully added to the list
 */
bool slist_add_last(SList *list, void *element)
{
    Node *node = list->mem_calloc(1, sizeof(Node));

    if (!node)
        return false;

    node->data = element;

    if (list->size == 0) {
        list->head       = node;
        list->tail       = node;
    } else {
        list->tail->next = node;
        list->tail       = node;
    }
    list->size++;
    return true;
}

/**
 * Adds a new element at the specified location in the list and shifts all
 * subsequent elements by one. This operation cannot be performed on an empty
 * list. The index at which the new element is being added must be within the
 * bounds of the list. This function returns false if either the index is out
 * of bounds, or if the memory allocation for the new element fails.
 *
 * @param[in] list the list to which this element is being added
 * @param[in] element element being added
 * @param[in] index the position in the list at which the new element is being
 *                  added
 *
 * @return true if the element was successfully added to the list
 */
bool slist_add_at(SList *list, void *element, size_t index)
{
    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list, index, &node, &prev))
        return false;

    Node *new = list->mem_calloc(1, sizeof(Node));

    if (!new)
        return false;

    new->data = element;

    if (!prev) {
        new->next  = list->head;
        list->head = new;
    } else {
        Node *tmp  = prev->next;
        prev->next = new;
        new->next  = tmp;
    }

    list->size++;
    return true;
}

/**
 * Adds all elements from the second list to the first. The elements from the
 * second list are added after the last element of the first list. This function
 * returns false if no elements were added to the first list. This could be the
 * case if either the second list is empty or if the memory allocation for the
 * element that are being added fails.
 *
 * @param[in] list1 the list to which the elements are being added
 * @param[in] list2 the list from which the elements are being taken.
 *
 * @return true if the elements were successfully added
 */
bool slist_add_all(SList *list1, SList *list2)
{
    if (list2->size == 0)
        return false;

    Node *head = NULL;
    Node *tail = NULL;

    if (!link_all_externally(list2, &head, &tail))
        return false;

    if (list1->size == 0) {
        list1->head = head;
        list1->tail = tail;
    } else {
        list1->tail->next = head;
        list1->tail = tail;
    }
    list1->size += list2->size;

    return true;
}

/**
 * Adds all element from the second list to the first at the specified position
 * by shifting all subsequent elements by the size of the second list. The index
 * must be within the range of the list. This function returns false if no
 * elements were added to the first list. This could be the case if either the
 * second list is empty or if the memory allocation for the elements being added
 * fails.
 *
 * @param[in] list1 the list to which the elements are being added
 * @param[in] the list from which the elements are being taken
 * @param[in] index position in the first list at which the elements should be
 *                  added
 *
 * @return true if the elements were successfully added
 */
bool slist_add_all_at(SList *list1, SList *list2, size_t index)
{
    if (list2->size == 0)
        return false;

    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list1, index, &node, &prev))
        return false;

    Node *head = NULL;
    Node *tail = NULL;

    if (!link_all_externally(list2, &head, &tail))
        return false;

    if (!prev) {
        tail->next  = node;
        list1->head = head;
    } else {
        prev->next = head;
        tail->next = node;
    }

    list1->size += list2->size;

    return true;
}

/**
 * Duplicates the structure of the list without directly attaching it to a
 * specific list. If the operation fails, everything is cleaned up and false
 * is returned to indicate the failure.
 *
 * @param[in] list the list whose structure is being duplicated
 * @param[in, out] h the pointer to which the new head will be attached
 * @param[in, out] t the pointer to which the new tail will be attached
 *
 * @return true if the operation was successful
 */
static bool link_all_externally(SList *list, Node **h, Node **t)
{
    Node *ins = list->head;

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

        new->data = ins->data;

        if (!*h) {
            *h = new;
            *t = new;
        } else {
            (*t)->next = new;
            *t = new;
        }

        ins = ins->next;
    }
    return true;
}

/**
 * Splices the two singly linked lists together by appending the second list to
 * the first. This function moves all the elements from the second list into
 * the first list, leaving the second list empty.
 *
 * @param[in] list1 The consumer list to which the elements are moved.
 * @param[in] list2 The producer list from which the elements are moved.
 *
 * @return true if the operation was successful
 */
bool slist_splice(SList *list1, SList *list2)
{
    if (list2->size == 0)
        return false;

    if (list1->size == 0) {
        list1->head = list2->head;
        list1->tail = list2->tail;
    } else {
        list1->tail->next = list2->head;
        list1->tail = list2->tail;
    }
    list1->size += list2->size;

    list2->head = NULL;
    list2->tail = NULL;
    list2->size = 0;
}

/**
 * Splices the two singly lists together at the specified index of the first list.
 * this function moves all the elements from the second list into the first list
 * at the position specified by the <code>index</code> parameter. After this
 * operation the second list will be left empty. This function returns false if the
 * second list is already empty or if the specified index is out of bounds.
 *
 * @param[in] list1 the consumer list to which the elements are moved
 * @param[in] list2 the producer list from which the elements are moved
 * @param[in] index the index in the first list after which the elements from the
 *                  second list should be inserted
 *
 * @return true if at least one element was moved from the second list
 */
bool slist_splice_at(SList *list1, SList *list2, size_t index)
{
    if (list2->size == 0)
        return false;

    if (index >= list1->size)
        return false;

    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list1, index, &node, &prev))
        return false;

    splice_between(list1, list2, prev, node);

    return true;
}

/**
 * Inserts the second list between the two nodes of the first list. If the left
 * node is NULL the head of the second list will be become the head of the first
 * list. Similarly if the right node is null the tail of the first list will
 * become the tail of the second list.
 *
 * @param[in, out] l1 the consumer list
 * @param[in, out] l2 the producer list
 * @param[in] left the node after which the elements are being added
 * @param[in] right the node behind which the elements are being added
 */
static INLINE void splice_between(SList *l1, SList *l2, Node *base, Node *end)
{
    if (!base) {
        l2->tail->next = l1->head;
        l1->head = l2->head;
    } else if (!end) {
        l1->tail->next = l2->head;
        l1->tail = l2->tail;
    } else {
        base->next = l2->head;
        l2->tail->next = end;
    }
    l1->size += l2->size;

    l2->head = NULL;
    l2->tail = NULL;
    l2->size = 0;
}

/**
 * Removes and returns the first occurrence of the element from the specified
 * list. If the element if not a part of the list, NULL is returned instead.
 * NULL may also be returned if the removed element was NULL. Calling <code>
 * slist_contains()</code> before this function can resolve the ambiguity.
 *
 * @param[in] list a list from which the element is being removed
 * @param[in] element element being removed
 *
 * @return the removed element
 */
void *slist_remove(SList *list, void *element)
{
    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node(list, element, &node, &prev))
        return NULL;

    return unlink(list, node, prev);
}

/**
 * Removes and returns the element at the specified index. The index must be
 * within the bounds of the list. In case the index is out of bounds this
 * function returns NULL. NULL may also be returned if the element at the
 * specified index is actually NULL. Calling <code>slist_contains()</code>
 * before this function can resolve this ambiguity.
 *
 * @param[in] list the list from which the element is being removed.
 * @param[in] index Index of the element is being removed. Must be be within the
 *            index range of the list.
 *
 * @return the removed element, or NULL
 *
 */
void *slist_remove_at(SList *list, size_t index)
{
    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list, index, &node, &prev))
        return NULL;

    return unlink(list, node, prev);
}

/**
 * Removes and returns the first (head) element of the list. If the list is
 * empty, NULL is returned instead.
 *
 * @param[in] list the list from which the first element is being removed
 *
 * @return the removed element, or NULL
 */
void *slist_remove_first(SList *list)
{
    if (list->size == 0)
        return NULL;

    return unlink(list, list->head, NULL);
}

/**
 * Removes and returns the last (tail) element of the list. If the list is
 * empty, NULL is returned instead.
 *
 * @param[in] list the list from which the last element is being removed
 *
 * @return the removed element, or NULL
 */
void *slist_remove_last(SList *list)
{
    if (list->size == 0)
        return NULL;

    Node *prev = NULL;
    Node *node = NULL;

    get_node_at(list, list->size - 1, &node, &prev);

    return unlink(list, node, prev);
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
bool slist_remove_all(SList *list)
{
    return unlink_all(list, false);
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
bool slist_remove_all_free(SList *list)
{
    return unlink_all(list, true);
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
void *slist_replace_at(SList *list, void *element, size_t index)
{
    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list, index, &node, &prev))
        return NULL;

    void *old = node->data;
    node->data = element;
    return old;
}

/**
 * Returns the first element from the specified list, or NULL if the list is
 * empty.
 *
 * @param[in] list the list whose first element is being returned.
 *
 * @return the first element of the list, or NULL in case the list is empty.
 */
void *slist_get_first(SList *list)
{
    if (list->size == 0)
        return NULL;

    return list->head->data;
}

/**
 * Returns the last element from the specified list. or NULL if the list is
 * empty.
 *
 * @param[in] list list whose last element is being returned
 *
 * @return the last element of the list, or NULL in case the list is empty.
 */
void *slist_get_last(SList *list)
{
    if (list->size == 0)
        return NULL;

    return list->tail->data;
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
void *slist_get(SList *list, size_t index)
{
    Node *prev = NULL;
    Node *node = NULL;

    if (!get_node_at(list, index, &node, &prev))
        return NULL;

    return node->data;
}

/**
 * Returns the number of elements in the specified list.
 *
 * @param[in] list whose size is being returned
 *
 * @return the number of the elements contained in the specified list
 */
size_t slist_size(SList *list)
{
    return list->size;
}

/**
 * Reverses the order of elements in the specified list.
 *
 * @param[in] list the list that is being reversed
 */
void slist_reverse(SList *list)
{
    if (list->size == 0 || list->size == 1)
        return;

    Node *prev = NULL;
    Node *flip = list->head;
    Node *next = flip->next;

    list->tail = list->head;

    size_t i;
    for (i = 0; i < list->size; i++) {
        flip->next = prev;

        prev = flip;
        flip = next;
        next = next ? next->next : NULL;
    }

    list->head = prev;
}

/**
 * Returns a sublist of the specified list. The returned sublist contains all
 * the elements from the list that are contained between the two indices
 * including the elements at the indices. For example if a list contains 5
 * elements [5, 6, 7, 8, 9], a sublist from index 1 to 3 will will be a new
 * list of length 3, containing [6, 7, 8]. The returned sublist is only a copy of
 * the original lists structure, meaning the data it points to is not copied.
 *
 * @note The sublist is allocated using the original lists allocators and also
 *       inherits the configuration of the original list.
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
SList *slist_sublist(SList *list, size_t from, size_t to)
{
    if (from > to || to >= list->size)
        return NULL;

    Node *base = NULL;
    Node *node = NULL;

    SList *sub = slist_new();

    get_node_at(list, from, &node, &base);

    size_t i;
    for (i = from; i <= to; i++) {
        slist_add(sub, node->data);
        node = node->next;
    }
    return sub;
}

/**
 * Returns a shallow copy of the specified list. A shallow copy is a copy of the
 * list structure. This operation does not copy the actual data that this list
 * holds.
 *
 * @note The new list is allocated using the original lists allocators and also
 *       inherits the configuration of the original list.
 *
 * @param[in] list list to be copied
 *
 * @return a shallow copy of the list
 */
SList *slist_copy_shallow(SList *list)
{
    SList *copy = slist_new();
    Node *node = list->head;

    while (node) {
        slist_add(copy, node->data);
        node = node->next;
    }
    return copy;
}

/**
 * Returns a deep copy of the specified list. This function copies the structure
 * of the list along with all the data it holds. The element copying is done
 * through the specified copy function that should return a pointer to the copy
 * of the element passed to it.
 *
 * @note The new list is allocated using the original lists allocators and also
 *       inherits the configuration of the original list.
 *
 * @param[in] list list to be copied
 * @parama[in] cp  the copy function that should return a pointer to the copy of
 *                 the data.
 *
 * @return a deep copy of the list
 */
SList *slist_copy_deep(SList *list, void *(*cp) (void*))
{
    SList *copy = slist_new();
    Node *node = list->head;

    while (node) {
        slist_add(copy, cp(node->data));
        node = node->next;
    }
    return copy;
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
size_t slist_contains(SList *list, void *element)
{
    Node *node = list->head;

    size_t e_count = 0;

    while (node) {
        if (node->data == element)
            e_count++;
        node = node->next;
    }
    return e_count;
}

/**
 * Returns the index of the specified element, or <code>NO_SUCH_INDEX</code> if
 * the element is not found. The returned index is the index of the first
 * occurrence of the element starting from the beginning of the list.
 *
 * @param[in] list    the list on which this operation is performed
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element or <code>NO_SUCH_INDEX</code> if
 *         the element is not found.
 */
size_t slist_index_of(SList *list, void *element)
{
    Node *node = list->head;

    size_t i = 0;
    while (node) {
        if (node->data == element)
            return i;
        i++;
        node = node->next;
    }
    return NO_SUCH_INDEX;
}

/**
 * Returns an array representation of the specified list. None of the elements
 * are copied into the array and thus any modification of the elements within
 * the array will affect the list elements as well. The size of the returned
 * array is the same as the size of the list from which the array was constructed.
 *
 * @param[in] list the list on which this operation is being performed
 *
 * @return an array representation of the specified list
 */
void **slist_to_array(SList *list)
{
    void **array = list->mem_alloc(list->size * sizeof(void*));
    Node  *node  = list->head;

    size_t i;
    for (i = 0; i < list->size; i++) {
        array[i] = node->data;
        node = node->next;
    }
    return array;
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
void slist_sort(SList *list, int (*cmp) (void const *e1, void const *e2))
{
    if (list->size == 1)
        return;

    void **elements = slist_to_array(list);
    Node  *node     = list->head;

    qsort(elements, list->size, sizeof(void*), cmp);

    size_t i;
    for (i = 0; i < list->size; i++) {
        node->data = elements[i];
        node       = node->next;
    }
    list->mem_free(elements);
}

/**
 * A 'foreach loop' function that invokes the specified function on each element
 * in the list.
 *
 * @param[in] list the list on which this operation is being performed
 * @param[in] op the operation function that is to be invoked on each list
 *               element
 */
void slist_foreach(SList *list, void (*op) (void *))
{
    Node *n = list->head;

    while (n) {
        op(n->data);
        n = n->next;
    }
}

/**
 * Initializes the iterator.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] list the slist to iterate over
 */
void slist_iter_init(SListIter *iter, SList *list)
{
    iter->index   = 0;
    iter->list    = list;
    iter->current = NULL;
    iter->prev    = NULL;
    iter->next    = list->head;
}

/**
 * Removes and returns the last returned element by <code>slist_iter_next()
 * </code> function without invalidating the iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the removed element
 */
void *slist_iter_remove(SListIter *iter)
{
    if (!iter->current)
        return NULL;

    void *e = unlink(iter->list, iter->current, iter->prev);
    iter->current = NULL;
    iter->index--;

    return e;
}

/**
 * Adds a new element to the list after the last returned element by
 * <code>slist_iter_next()</code> function without invalidating the
 * iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added
 */
bool slist_iter_add(SListIter *iter, void *element)
{
    Node *new_node = iter->list->mem_calloc(1, sizeof(Node));

    if (!new_node)
        return false;

    new_node->data      = element;
    new_node->next      = iter->next;
    iter->prev          = iter->current;
    iter->current->next = new_node;

    if (iter->index == iter->list->size - 1)
        iter->list->head = new_node;

    iter->index++;
    iter->list->size++;
    return true;
}

/**
 * Replaces the last returned element by <code>slist_iter_next()</code>
 * with the specified element.
 *
 * @param[in] iter the iterator on which this operation is being pefromed
 * @param[in] element the replacement element
 *
 * @return the replaced element
 */
void *slist_iter_replace(SListIter *iter, void *element)
{
    if (!iter->current)
        return NULL;

    void *old = iter->current->data;
    iter->current->data = element;
    return old;
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
void *slist_iter_next(SListIter *iter)
{
    void *data = iter->next->data;

    if (iter->current)
        iter->prev = iter->current;

    iter->current = iter->next;
    iter->next = iter->next->next;
    iter->index++;

    return data;
}

/**
 * Return s the index of the last returned element by <code>slist_iter_next()
 * </code>.
 *
 * @param[in] iter the iterator on which this operation is being performed
 *
 * @return the index
 */
size_t slist_iter_index(SListIter *iter)
{
    return iter->index - 1;
}

/**
 * Checks whether or not the iterator has reached the end of the list
 *
 * @param[in] iter iterator whose position is being checked
 *
 * @return true if there are more element to be iterated over, or false if not
 */
bool slist_iter_has_next(SListIter *iter)
{
    return iter->next ? true : false;
}

/**
 * Unlinks the node from the list and returns the data tat was associated with it.
 *
 * @param[in] list the list from which the node is being unlinked
 * @parma[in] node the node being unlinked
 * @param[in] prev the node that immediately precedes the node that is being unlinked
 *
 * @return the data that was at this node
 */
static void *unlink(SList *list, Node *node, Node *prev)
{
    void *data = node->data;

    if (prev)
        prev->next = node->next;

    if (!prev)
        list->head = node->next;

    if (!node->next)
        list->tail = prev;

    list->mem_free(node);
    list->size--;
}

/**
 * Unlinks all nodes from the list and optionally frees the data at the nodes.
 *
 * @param[in] list the list from which all nodes are being unlinked
 * @param[in] freed specified whether or not the data at the nodes should also
 *                  be deallocated.
 *
 * @return false if the list is already y empty, otherwise returns true
 */
static bool unlink_all(SList *list, bool freed)
{
    if (list->size == 0)
        return false;

    Node *n = list->head;

    while (n) {
        Node *tmp = n->next;

        if (freed)
            list->mem_free(n->data);

        list->mem_free(n);
        n = tmp;
    }
    return true;
}

/**
 * Finds the node at the specified index. If the index is not in the bounds
 * of the list, NULL is returned instead.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] index the index of the node
 * @param[out] node the node at the specified index
 * @param[out] prev the node that immediately precedes the node at the
 *                  specified index
 *
 * @return true if the operation was successful
 */
static bool
get_node_at(SList *list, size_t index, Node **node, Node **prev)
{
    if (index >= list->size)
        return false;

    *node = list->head;
    *prev = NULL;

    size_t i;
    for (i = 0; i < index; i++) {
        *prev = *node;
        *node = (*node)->next;
    }
    return true;
}

/**
 * Finds the first node from the beginning of the list that is associated
 * with the specified element. If no node is associated with the element,
 * NULL is returned instead.
 *
 * @param[in] list the list from which the node is being returned
 * @param[in] element the element whose list node is being returned
 * @param[out] node the node associated with the data
 * @param[out] prev the node that immediately precedes the node at the
 *                  specified index
 *
 * @return true if a node containing the specified data was found
 */
static bool
get_node(SList *list, void *element, Node **node, Node **prev)
{
   *node = list->head;
   *prev = NULL;

    while (*node) {
        if ((*node)->data == element)
            return true;

        *prev = *node;
        *node = (*node)->next;
    }
    return false;
}
