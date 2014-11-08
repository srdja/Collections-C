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

#include "slist.h"

typedef struct node_s {
    void *data;
    struct node_s *next;
} Node;

struct slist_s {
    int size;
    Node *head;
    Node *tail;
};

struct slist_iter_s {
    size_t index;
    SList *list;
    Node *next;
    Node *current;
    Node *prev;
};

static void* unlink              (SList *list, Node *node, Node *prev);
static bool  unlink_all          (SList *list, bool freed);
static void  splice_between      (SList *list1, SList *list2, Node *base, Node *end);
static bool  get_node_at         (SList *list, size_t index, Node **node, Node **prev);
static bool  get_node            (SList *list, void *element, Node **node, Node **prev);
static bool  link_all_externally (SList *list, Node **h, Node **t);

/**
 * Returns a new empty list, or NULL if the memory allocation fails.
 *
 * @return a new list, or NULL if the memory allocation fails
 */
SList *slist_new()
{
    return calloc(1, sizeof(SList));
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
    free(list);
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
    free(list);
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
    Node *node = calloc(1, sizeof(Node));

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
 * @return true if the element was successfuly added to the list
 */
bool slist_add_last(SList *list, void *element)
{
    Node *node = calloc(1, sizeof(Node));

    if (!node)
        return false;

    node->data = element;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return true;
}

/**
 * Adds a new element at the specified location in the list and shifts all
 * subsequent elements by one. This operation cannot be performed on an empty
 * list. The index at which the new element is being added must be within the 
 * bounds of the list. This function returns false if either the indes is out
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

    Node *new = calloc(1, sizeof(Node));

    if (!new)
        return false;

    new->data = element;

    if (!prev) {
        new->next = list->head;
        list->head = new;
    } else {
        Node *tmp = prev->next;
        prev->next = new;
        new->next = tmp;
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
 * @return true if the elemetns were successfully added
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
 * @return true if the elements were successfully addded
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
        tail->next = node;
        list1->head = head;
    } else {
        prev->next = head;
        tail->next = node;
    }

    list1->size += list2->size;

    return true;
}

/**
 *
 *
 */
static bool link_all_externally(SList *list, Node **h, Node **t)
{
    Node *ins = list->head;

    size_t i;
    for (i = 0; i < list->size; i++) {
        Node *new = calloc(1, sizeof(Node));

        if (!new) {
            while (*h) {
                Node *tmp = (*h)->next;
                free(*h);
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
 * Splices the two singly 
 *
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
 *
 *
 *
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
 *
 *
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
 *
 *
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
 *
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
 *
 *
 */
void *slist_remove_first(SList *list)
{
    if (list->size == 0)
        return NULL;
    
    return unlink(list, list->head, NULL);
}

/**
 *
 *
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
 *
 *
 */
bool slist_remove_all(SList *list)
{
    return unlink_all(list, false);
}

/**
 *
 *
 */
bool slist_remove_all_free(SList *list)
{
    return unlink_all(list, true);
}

/**
 *
 *
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
 *
 *
 */
void *slist_get_first(SList *list)
{
    if (list->size == 0)
        return NULL;
    
    return list->head->data;
}

/**
 *
 *
 */
void *slist_get_last(SList *list)
{
    if (list->size == 0)
        return NULL;

    return list->tail->data;
}

/**
 *
 *
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
 *
 *
 */
size_t slist_size(SList *list)
{
    return list->size;
}

/**
 *
 *
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
 *
 *
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
 *
 *
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
 *
 *
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
 *
 *
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
 *
 *
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
 *
 *
 */
void **slist_to_array(SList *list)
{
    void **array = malloc(list->size * sizeof(void*));
    Node  *node  = list->head;

    size_t i;
    for (i = 0; i < list->size; i++) {
        array[i] = node->data;
        node = node->next;
    }
    return array;
}


void slist_sort(SList *list, int (*cmp) (void *e1, void *e2))
{
    if (list->size == 1)
        return;

    void **elements = malloc(list->size * sizeof(void*));
    Node  *node     = list->head;
    
    size_t i;
    for (i = 0; i < list->size; i++) {
        elements[i] = node->data;
        node  = node->next;
    }
    
    qsort(elements, list->size, sizeof(void*), cmp);

    node = list->head;
    for (i = 0; i < list->size; i++) {
        node->data = elements[i];
        node  = node->next;
    }

    list->head = elements[0];
    list->tail = elements[list->size - 1];

    free(elements);
}

/**
 *
 *
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
 *
 *
 */
SListIter *slist_iter_new(SList *list)
{
    SListIter *iter = calloc(1, sizeof(SListIter));

    if (!iter)
        return NULL;

    iter->index = 0;
    iter->list = list;
    iter->current = NULL;
    iter->prev = NULL;
    iter->next = list->head;

    return iter;
}

/**
 *
 *
 */
void slist_iter_destroy(SListIter *iter)
{
    free(iter);
}

/**
 *
 *
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
 *
 *
 */
bool slist_iter_add(SListIter *iter, void *element)
{
    Node *new_node = calloc(1, sizeof(Node));

    if (!new_node)
        return false;

    new_node->data = element;
    new_node->next = iter->next;

    iter->prev = iter->current;
    iter->current->next = new_node;
    
    if (iter->index == iter->list->size - 1)
        iter->list->head = new_node;

    iter->index++;
    iter->list->size++;
    return true;
}

/**
 *
 *
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
 *
 *
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
 *
 *
 */
size_t slist_iter_index(SListIter *iter)
{
    return iter->index - 1;
}

/**
 *
 *
 */
bool slist_iter_has_next(SListIter *iter)
{
    return iter->next ? true : false;
}

/**
 *
 *
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

    free(node);
    list->size--;
}

/**
 *
 *
 */
static bool unlink_all(SList *list, bool freed)
{
    if (list->size == 0)
        return false;

    Node *n = list->head;
    
    while (n) {
        Node *tmp = n->next;
        
        if (freed)
            free(n->data);
        
        free(n);
        n = tmp;
    }
    return true;
}

/**
 *
 *
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
 *
 *
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
