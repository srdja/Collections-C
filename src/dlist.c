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

#include <stdlib.h>
#include <assert.h>
#include "dlist.h"

typedef struct node_s {
    void *data;
    struct node_s *next;
    struct node_s *prev;
} Node;

struct dlist_s {
    int size;
    Node *head;
    Node *tail;
};

struct dlist_iter_s {
    int index;
    List *list;
    Node *last;
    Node *next;
};

struct dlist_iter_desc_s {
    int index;
    List *list;
    Node *last;
    Node *next;
};

static bool unlink(List *list, Node *node, bool freed);
static bool unlink_all(List *list, bool free);
static void link_behind(Node *node, Node *inserted);
static void link_after(Node *base, Node *inserted);
static void swap(Node *n1, Node *n2);
static bool splice_between(List *list1, List *list2, Node *left, Node *right);
static Node *get_node_at(List *list, int index);
static Node *get_node(List *list, void *element);

/**
 * Returns a new empty list, or NULL if the allocation fails.
 *
 * @return a new list if the allocation was successful, or NULL if not.
 */
List *list_new()
{
    return (List*) calloc(1, sizeof(List));
}

/**
 * Destroys the list structure, but leaves the data intact.
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

    free(list);
    return success;
}

/**
 * Destroys the list structure along with all the data it points to.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list - a list to destroy
 * \return true if the operation was successful
 */
bool list_destroy_free(List *list)
{
    bool success = true;

    if (list->size > 0)
        success = list_free_all(list);

    free(list);
    return success;
}

/**
 * Adds a new element to the list. The element is appended to the list making it
 * the last element in the list.
 *
 * @param[in] list the list to which the element is being added
 * @param[in] element element being added
 *
 * \return True if the operation was successful. The operation can fail if the
 *         memory allocation for the new element fails.
 */
bool list_add(List *list, void *element)
{
    return list_add_last(list, element);
}

/**
 * Adds a new element at the specified location in the list. This operation
 * cannot be performed on an empty list.
 *
 * @param[in] list    The list to which this element is being added.
 * @param[in] element Element being added.
 * @param[in] index   Position in the list. The index must be a positive
 *                    integer and may not exceed the list size
 *
 * @return True if the operation was successful. The operation can fail if the
 *         memory allocation for the new element fails, or if an attempt is made
 *         to insert the element into an empty list.
 */
bool list_add_at(List *list, void *element, int index)
{
    Node *node = get_node_at(list, index);
    Node *ins  = (Node*) calloc(1, sizeof(Node));

    if (!node || !ins)
        return false;

    ins->data = element;
    link_behind(node, ins);

    if (index == 0)
        list->head = ins;

    if (index == list->size - 1)
        list->tail = ins;

    list->size++;

    return true;
}

/**
 * Adds all elements from the second list to the first.
 *
 * @param[in] list1
 * @param[in] list2
 * @return returns true if the operation was successful.
 */
bool list_add_all(List *list1, List *list2)
{
    Node *node = list2->head;
    while (node) {
        list_add_last(list1, node->data);
        node = node->next;
    }
    return true;
}

/**
 *
 * @param[in] list1 consumer list
 * @param[in] list2 producer list
 * @param[in] index
 * @return
 */
bool list_add_all_at(List *list1, const List *list2, int index)
{
    Node *start = get_node_at(list1, index);

    if (!start)
        return false;

    Node *end = start->next;
    Node *ins = list2->head;

    int i;
    for (i = 0; i < list2->size; i++) {
        Node *new = (Node*) calloc(1, sizeof(Node));
        new->data = ins->data;
        start->next = new;
        start = new;
        ins = ins->next;
        list1->size++;
    }
    if (end)
        end->prev = start;

    return true;
}

/**
 * Prepends a new element to the list (adds a new "head").
 *
 * @param[in] list a list to which the element is prepended
 * @param[in] element element being prepended
 *
 * @return True if the operation was successful. The operation can fail if the
 *         provided list is null.
 */
bool list_add_first(List *list, void *element)
{
    Node *node = (Node*) calloc(1, sizeof(Node));

    if (node == NULL)
        return false;

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
    return true;
}

/**
 * Appends a new element to the list (adds an new "tail").
 *
 * @param[in] list a list to which the element is appended
 * @param[in] element element being appended
 *
 * @return True if the operation was successful. The operation can fail if the
 *         provided list is null.
 */
bool list_add_last(List *list, void *element)
{
    Node *node = (Node*) calloc(1, sizeof(Node));

    if (node == NULL)
        return false;

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
    return true;
}

/**
 * Splices the two doubly linked lists together by appending the second list to
 * the first. This function moves all the elements from the second list into
 * the first list, leaving second list empty.
 *
 * @param[in] list1 The consumer list to which the elements are moved to.
 * @param[in] list2 The produces list from which the elements are moved.
 *
 * @return returns true if the operation was successful
 */
bool list_splice(List *list1, List *list2)
{
    return splice_between(list1, list2, list1->tail, list1->tail->next);
}

/**
 * Splices the second list behind an element in the first list. This function
 * moves all the elements from the second list into the first list at the
 * specified position. After this operation the second list will be left empty.
 *
 * @param[in] list1 The consumer list to which the elements are moved to.
 * @param[in] list2 The produces list from which the elements are moved.
 * @param[in] index position of the element in the first list behind which the
 *            second list will be inserted. The index must be within the bounds
 *            of the fist list.
 * @return true if the operation was successful
 */
bool list_splice_before(List *list1, List *list2, int index)
{
    Node *new_tail = get_node_at(list1, index);

    if (!new_tail)
        return false;

    Node *new_head = new_tail->prev;

    return splice_between(list1, list2, new_head, new_tail);
}

/**
 * Splices the second list after an element in the first list. This function
 * moves all the elements from the second list into the first list at the
 * specified position. After this operation the second list will be left empty.
 *
 * @param[in] list1 The consumer list to which the elements are moved to.
 * @param[in] list2 The produces list from which the elements are moved.
 * @index[in] index Position of the element in the first list after which the
 *            second list will be inserted. The index must be within the bounds
 *            of the first list.
 * @return true if the operation was successful
 */
bool list_splice_after(List *list1, List *list2, int index)
{
    Node *new_head = get_node_at(list1, index);

    if (!new_head)
        return false;

    Node *new_tail = new_head->next;

    return splice_between(list1, list2, new_head, new_tail);
}

/**
 * Inserts the second list between the two nodes of the first list. If the left
 * node is NULL the head of the second list will become the head of the first
 * list. Similarly if the right node is null the tail of the first list will
 * become the tail of the second list.
 *
 * @param[in, out] l1
 * @param[in, out] l2
 * @param[in] left
 * @param[in] right
 */
static bool splice_between(List *l1, List *l2, Node *left, Node *right)
{
    if (l2->size == 0)
        return true;

    if (left)
        left->next = l2->head;
    else
        l1->head = l2->head;

    if (right)
        right->prev = l2->tail;
    else
        l1->tail = l2->tail;

    l1->size += l2->size;

    l2->head = NULL;
    l2->tail = NULL;
    l2->size = 0;

    return true;
}

/**
 * Removes the first occurrence of the element from the specified list. Removing
 * the element does not free the data.
 *
 * @param[in] list a list to which the element is being removed
 * @param[in] element element being removed
 *
 * @return true if the element was found and successfully removed or false if
 *         not.
 */
bool list_remove(List *list, void *element)
{
    Node *node = get_node(list, element);
    return unlink(list, node, false);
}

/**
 * Removes the first occurrence of the element from the specified list and frees
 * the data.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list a list to which the element is appended
 * @param[in] element element being removed.
 *
 * @return true if the element was found and successfully removed or false if
 *         not.
 */
bool list_free(List *list, void *element)
{
    Node *node = get_node(list, element);
    return unlink(list, node, true);
}

/**
 * Removes the head node_s from the specified list and leaves the data intact.
 *
 * @param[in] list the list from which the head node is being removed
 *
 * @return true if the operation was successful.
 */
bool list_remove_first(List *list)
{
    return unlink(list, list->head, false);
}

/**
 * Removes the head node_s from the specified list and frees the data.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in]list the list from which the head node is being removed
 *
 * @return true if the operation was successful.
 */
bool list_free_first(List *list)
{
    return unlink(list, list->head, true);
}

/**
 * Removes the tail node_s from the specified list and leaves the data intact.
 *
 * @param[in] list the list from which the tail node is being removed
 *
 * @return true if the operation was successful
 */
bool list_remove_last(List *list)
{
    return unlink(list, list->tail, false);
}

/**
 * Removes the tail node_s from the specified list and frees the data.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list the list from which the head node is being removed
 *
 * @return true if the operation was successful
 */
bool list_free_last(List *list)
{
    return unlink(list, list->tail, true);
}

/**
 * Removes all elements from the specified list.
 *
 * @param[in] list list that is being cleared
 *
 * @return true if the operation was successful and at least one element was
 *         removed, or false if the list was already empty
 */
bool list_remove_all(List *list)
{
    return unlink_all(list, false);
}

/**
 * Removes all elements from the specified list and frees the data.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list list that is being cleared
 *
 * @return true if the operation was successful and at least one element was
 *         removed or false if the list is already empty
 */
bool list_free_all(List *list)
{
    return unlink_all(list, true);
}

/*
 * Removes a list element from the specified index but keeps the data intact.
 *
 * @param[in] list list from which the element is being removed.
 * @param[in] index Index of the element being removed. Must be be within the
 *            index range of the list.
 *
 * @return returns true or false depending on whether or not the removal
 *         operation was successful. False is returned if the index was out of
 *         range.
 */
bool list_remove_at(List *list, int index)
{
    Node *node = get_node_at(list, index);
    return unlink(list, node, false);
}

/**
 * Removes a list element from the specified index and frees the data.
 *
 * @note
 * This function should not be called on a list that has some of it's elements
 * allocated on the stack.
 *
 * @param[in] list list from which the element is being removed.
 * @param[in] index Index of the element being removed. Must be be within the
 *            index range of the list.
 *
 * @return returns true or false depending on whether or not the removal
 *         operation was successful. False is returned if the index was out of
 *         range.
 */
bool list_free_at(List *list, int index)
{
    Node *node = get_node_at(list, index);
    return unlink(list, node, true);
}

/**
 * Replaces a list element at the specified location.
 *
 * @param[in] list the list on which this operation is performed
 * @param[in] element the replacement element
 * @param[in] index index of the element being replaced
 *
 * @return true if the operation was successful
 */
bool list_replace_at(List *list, void *element, int index)
{
    Node *node = get_node_at(list, index);
    if (node) {
        node->data = element;
        return true;
    }
    return false;
}

/**
 * Returns the head element from the specified list, or NULL if the list is
 * empty.
 *
 * @param[in] list the list whose head element is being returned.
 *
 * @return the head element of the list, or NULL in case the list is empty.
 */
void *list_get_first(List *list)
{
    if (list->size == 0)
        return NULL;
    return list->head->data;
}

/**
 * Returns the tail element from the specified list. or NULL if the list is
 * empty.
 *
 * @param[in] list list whose tail element is being returned
 *
 * @return the tail element of the list, or NULL in case the list is empty.
 */
void *list_get_last(List *list)
{
    if (list->size == 0)
        return NULL;
    return list->tail->data;
}

/**
 * Returns the list element from the specified index. In case the index is out
 * of bounds, this function returns NULL instead.
 *
 * @param[in] list  list from which the element is returned.
 * @param[in] index The index of a list element being returned. The index must
 *                  be a positive integer no greater that the list size.
 *
 * @return The list element at the specified index.
 */
void *list_get(List *list, int index)
{
    Node *node = get_node_at(list, index);

    if (node != NULL)
        return node->data;

    return NULL;
}

/**
 * Reverses the specified list.
 *
 * @param[in] list list that is being reversed.
 */
void list_reverse(List *list)
{
   Node *head = list->head;
   list->head = list->tail;
   list->tail = head;

   Node *left  = list->head;
   Node *right = list->tail;

   int i;
   for (i = 0; i < list->size / 2; i++) {
       Node *tmpl = left->next;
       Node *tmpr = right->prev;

       swap(left, right);

       left  = tmpl;
       right = tmpr;
   }
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
List *list_sublist(List *list, int b, int e)
{
    if (b > e || b < 0 || e >= list->size)
        return NULL;

    List *sub = (List*) list_new();
    Node *node = get_node_at(list, b);

    int i;
    for (i = b; i <= e; i++) {
        list_add(sub, node->data);
        node = node->next;
    }
    return sub;
}

/**
 * Returns a shallow copy of the specified list. A shallow copy is a copy of the
 * list structure. This operation does not copy the actual data that this list
 * points to.
 *
 * @param[in] list list to be copied
 *
 * @return copy of the list
 */
List *list_copy_shallow(List *list)
{
    List *copy = list_new();
    Node *node = list->head;

    while (node) {
        list_add(copy, node->data);
        node = node->next;
    }
    return copy;
}

/**
 * Returns a deep copy of the specified list. This functions copies the structure
 * and all the data it points to.
 *
 * @param[in] list list to be copied
 * @param[in] cp   the copy function that should return a pointer to the copy of
 *                 the data.
 *
 * @return a deep copy of the list
 */
List *list_copy_deep(List *list, void *(*cp) (void *e1))
{
    List *copy = list_new();
    Node *node = list->head;

    while (node) {
        list_add(copy, cp(node->data));
        node = node->next;
    }
    return copy;
}

/**
 * Returns an array off all the elements contained in the specified list. The
 * number of elements in the array is equal to the number of elements in the
 * list.
 *
 * @param[in] list the list on which this operation is being performed.
 *
 * @return an array containing all the elements from the specified list
 */
void **list_to_array(List *list)
{
    void **array = (void**) calloc(list->size, sizeof(void*));
    Node *node = list->head;
    int i;
    for (i = 0; i < list->size; i++) {
        array[i] = node->data;
        node = node->next;
    }
    return array;
}

/**
 * Returns an integer representing the number of occurrences of the specified
 * element within the list.
 *
 * @param[in] list    list on which the search is performed
 * @param[in] element element being looked for
 *
 * @return number of found matches
 */
int list_contains(List *list, void *element)
{
    Node *node   = list->head;
    int  e_count = 0;

    while (node) {
        if (node->data == element)
            e_count++;
        node = node->next;
    }
    return e_count;
}

/**
 * Returns the index of the specified element or -1 if the element is not found.
 * The returned index is the index of the first element found starting from the
 * head of the list.
 *
 * @param[in] list    the list on which this operation is performed
 * @param[in] element the element whose index is being looked up
 *
 * @return the index of the specified element or -1 if the element is not found.
 */
int list_index_of(List *list, void *element)
{
    Node *node = list->head;
    int i = 0;
    while (node) {
        if (node->data == element)
            return i;
        i++;
        node = node->next;
    }
    return -1;
}

/**
 * Returns the number of elements from the specified list.
 *
 * @param[in] list whose size is being returned
 *
 * @return the number of the elements contained in the specified list
 */
int list_size(List *list)
{
    return list->size;
}

static Node *split(List *, Node *b, int l, int (*cmp) (void *e1, void *e2));
static void merge(Node**, Node**, int, int, int (*cmp) (void *e1, void *e2));

/**
 * Sorts the given list in place.
 *
 * @param[in, out] list List to be sorted
 * @param[in]      cmp  The comparator function that returns -1 if the first
 *                      element is smaller than the second; 0 if both elements
 *                      are equal and 1 if the second is larger.
 */
void list_sort(List *list, int (*cmp) (void *e1, void *e2))
{
    split(list, list->head, list->size, cmp);
}

/**
 * Splits the list section into two partitions.
 *
 * @param[in] list the sublist
 * @param[in] b    first node in the sublist
 * @param[in] size size of the sublist
 * @param[in] cmp  Comparator function.
 *
 * @return
 */
static Node *split(List *list, Node *b, int size, int (*cmp) (void*, void*))
{
    if (size < 2)
        return b;

    /* If the split is uneven the larger partition is on the right. */
    int l_size = size / 2;
    int r_size = size / 2 + (size % 2);

    Node *center = b;

    int i;
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
 * Merges the two partitions together in an stable way. The left partition
 * starts at the "left" node and ends "l_size" nodes to the right of it.
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
static void merge(Node **left, Node **right, int l_size, int r_size,
        int (*cmp) (void*, void*))
{
    int size = r_size + l_size;
    int l    = 0; /* Number of processed elements from the left partition */
    int r    = 0; /* NUmber of processed elements from the right partition. */

    Node *l_part = *left;
    Node *r_part = *right;

    int i;
    for (i = 0; i < size; i++) {
        int c = cmp(l_part->data, r_part->data);

        if ((c == -1 || c == 0)) {
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
 * Returns a new ascending iterator. The ascending iterator or a forward
 * iterator, is an iterator that traverses the list from head to tail.
 *
 * @param[in] list the list on which this iterator will operate.
 * @return a new ascending iterator.
 */
ListIter *list_iter_new(List *list)
{
    ListIter *iter = (ListIter*) malloc(sizeof(ListIter));

    iter->index = 0;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->head;

    return iter;
}

/**
 * Destroys the specified iterator.
 *
 * @param[in] iter the iterator that is being destroyed.
 */
void list_iter_destroy(ListIter *iter)
{
    free(iter);
}

/**
 * Removes the last returned list element by the specified iterator. Since this
 * function removes the last returned element, it should only be called after a
 * call to <code>iter_next()</code>. Only the first call to this function
 * removes the element. Any subsequent calls will have no effect until the
 * <code>iter_next()</code> is called again.
 *
 * @note This function should only ever be called after a call to iter_next()
 *
 * @param[in] iter the iterator on which this operation is being performed.
 * @return true if the removal was successful.
 */
bool list_iter_remove(ListIter *iter)
{
    bool success = unlink(iter->list, iter->last, false);
    iter->last = NULL;
    return success;
}

/**
 * Removes and frees the last returned list element by the specified iterator.
 * Since this function removes the last returned element, it should only be
 * called after a call to <code>iter_next()</code>. Only the first call to this
 * function removes the element. Any subsequent calls will have no effect until
 * the <code>iter_next()</code> is called again.
 *
 * @note This function should only ever be called after a call to iter_next()
 *
 * @param[in] iter the iterator on which this operation is being performed.
 * @return true if the removal was successful.
 */
bool list_iter_free(ListIter *iter)
{
    bool success = unlink(iter->list, iter->last, true);
    iter->last = NULL;
    return success;
}

/**
 * Adds a new element to the list. The element is added before the element that
 * would be returned by the next call to <code>dlist_iter_next()</code> and
 * after the last returned element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the element being added to the list
 * @return true if the operation was successful
 */
bool list_iter_add(ListIter *iter, void *element)
{
    Node *new_node = (Node*) calloc(1, sizeof(Node));
    new_node->data = element;

    link_behind(iter->next, new_node);
    iter->list->size++;
    iter->index++;
    iter->last = new_node;
    return true;
}

/**
 * Replace the most recently returned element by this iterator with the new
 * element.
 *
 * @param[in] iter the iterator on which this operation is being performed
 * @param[in] element the replacement element
 * @return true if the operation was successful
 */
bool list_iter_replace(ListIter *iter, void *element)
{
    iter->last->data = element;
    return true;
}

/**
 * Checks whether or not there are more elements to be iterated over. Returns
 * true if the next element is available or false if the end of the list has
 * been reached.
 *
 * @param[in] iter the iterator on which this operation is performed
 * @return true if there is at least one more element in the sequence or false
 * if the end of the list has been reached.
 */
bool list_iter_has_next(ListIter *iter)
{
    return iter->next != NULL;
}

/**
 * Returns the index of the previously returned element.
 *
 * @param[in] iter the iterator on which this operation is performed.
 * @return index of the previously returned element.
 */
int list_iter_index(ListIter *iter)
{
    return iter->index - 1;
}

/**
 * Returns the next element in the sequence and advances the iterator forward.
 *
 * @note Before this function is called, one should check whether the next
 * element in the sequence exists by calling <code>dlist_iter_has_next()</code>.
 *
 * @param the iterator on which this operation is being performed.
 * @return the next element in the sequence
 */
void *list_iter_next(ListIter *iter)
{
    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->next;
    iter->index++;
    return data;
}


/**
 * Returns a new descending iterator. A descending iterator or a reverse
 * iterator, is an iterator that traverses the list from tail to head.
 *
 * @param[in] list the list on which this iterator will operate.
 *
 * @return a new descending iterator.
 */
ListDIter *list_diter_new(List *list)
{
    ListDIter *iter = (ListDIter*) malloc(sizeof(ListDIter));

    iter->index = list->size - 1;
    iter->list  = list;
    iter->last  = NULL;
    iter->next  = list->tail;

    return iter;
}

void list_diter_destroy(ListDIter *iter)
{
    free(iter);
}

/**
 *
 */
bool list_diter_add(ListDIter *iter, void *element)
{
    Node *new_node = (Node*) calloc(1, sizeof(Node));
    new_node->data = element;

    link_after(iter->next, new_node);
    iter->list->size++;
    iter->last = new_node;
    return true;
}

/**
 *
 */
bool list_diter_remove(ListDIter *iter)
{
    bool success = unlink(iter->list, iter->last, false);
    iter->last = NULL;
    return success;
}

/**
 *
 */
bool list_diter_replace(ListDIter *iter, void *element)
{
    iter->last->data = element;
    return true;
}

int dlist_diter_index(ListDIter *iter)
{
    return iter->index - 1;
}

/**
 *
 */
bool list_diter_has_next(ListDIter *iter)
{
    return iter->next != NULL;
}

/**
 *
 */
void *list_diter_next(ListDIter *iter)
{
    void *data = iter->next->data;
    iter->last = iter->next;
    iter->next = iter->next->prev;
    iter->index++;
    return data;
}

/**
 *
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
        ins->prev = NULL;
        ins->next = base;
        base->prev = ins;
    } else {
        ins->prev = base->prev;
        ins->prev->next = ins;
        ins->next = base;
        base->prev = ins;
    }
}

static void link_after(Node *base, Node *ins)
{
    if (ins->next)
        ins->next->prev = ins->prev;

    if (ins->prev)
        ins->prev->next = ins->next;

    if (!base->next) {
        ins->prev = base;
        base->next = ins;
        ins->next = NULL;
    } else {
        ins->next = base->next;
        ins->next->prev = ins;
        ins->prev = base;
        base->next = ins;
    }
}

/**
 *
 */
static void swap(Node *n1, Node *n2)
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
 *
 */
static bool unlink(List *list, Node *node, bool freed)
{
    if (node == NULL)
        return false;

    if (freed)
        free(node->data);

    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->prev == NULL)
        list->head = node->next;

    if (node->next == NULL)
        list->tail = node->prev;

    if (node->next != NULL)
        node->next->prev = node->prev;

    free(node);
    list->size--;

    return true;
}

static bool unlink_all(List *list, bool free)
{
    if (list->size <= 0)
        return false;

    Node *node = list->head;

    while (node) {
        Node *tmp = node->next;
        unlink(list, node, free);
        node = tmp;
    }
    return true;
}

static Node *get_node_at(List *list, int index)
{
    if (!list || index >= list->size || index < 0)
        return NULL;

    int i;
    Node *node = NULL;

    if (index < list->size) {
        node = list->head;
        for (i = 0; i < index; i++)
            node = node->next;
    } else {
        node = list->tail;
        for (i = list->size - 1; i > index; i--)
            node = node->prev;
    }
    return node;
}

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
