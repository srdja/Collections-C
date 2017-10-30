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

#ifndef COLLECTIONS_C_LIST_H
#define COLLECTIONS_C_LIST_H

#include "common.h"

/**
 * A doubly linked list. List is a sequential structure that
 * supports insertion, deletion and lookup from both ends in
 * constant time, while the worst case is O(n/2) at the middle
 * of the list.
 */
typedef struct list_s List;

/**
 * List node.
 *
 * @note Modifying the links may invalidate the list structure.
 */
typedef struct node_s {
    void          *data;
    struct node_s *next;
    struct node_s *prev;
} Node;

/**
 * List iterator structure. Used to iterate over the elements of the
 * list in an ascending or descending order. The iterator also supports
 * operations for safely adding and removing elements during iteration.
 */
typedef struct list_iter_s {
    /**
     * The current position of the iterator.*/
    size_t  index;

    /**
     * The list associated with this iterator */
    List   *list;

    /**
     * Last returned node */
    Node   *last;

    /**
     * Next node in the sequence. */
    Node   *next;
} ListIter;

/**
 * List zip iterator structure. Used to iterate over two Lists in
 * lockstep in an ascending order until one of the lists is exhausted.
 * The iterator also supports operations for safely adding and
 * removing elements during iteration.
 */
typedef struct list_zip_iter_s {
    List *l1;
    List *l2;
    Node *l1_last;
    Node *l2_last;
    Node *l1_next;
    Node *l2_next;
    size_t index;
} ListZipIter;


/**
 * List configuration structure. Used to initialize a new List with specific
 * values.
 */
typedef struct list_conf_s {
    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
} ListConf;


void          list_conf_init       (ListConf *conf);
enum cc_stat  list_new             (List **list);
enum cc_stat  list_new_conf        (ListConf const * const conf, List **list);
void          list_destroy         (List *list);
void          list_destroy_cb      (List *list, void (*cb) (void*));

enum cc_stat  list_splice          (List *list1, List *list2);
enum cc_stat  list_splice_at       (List *list, List *list2, size_t index);

enum cc_stat  list_add             (List *list, void *element);
enum cc_stat  list_add_at          (List *list, void *element, size_t index);
enum cc_stat  list_add_all         (List *list1, List *list2);
enum cc_stat  list_add_all_at      (List *list, List *list2, size_t index);
enum cc_stat  list_add_first       (List *list, void *element);
enum cc_stat  list_add_last        (List *list, void *element);

enum cc_stat  list_remove          (List *list, void *element, void **out);
enum cc_stat  list_remove_first    (List *list, void **out);
enum cc_stat  list_remove_last     (List *list, void **out);
enum cc_stat  list_remove_at       (List *list, size_t index, void **out);

enum cc_stat  list_remove_all      (List *list);
enum cc_stat  list_remove_all_cb   (List *list, void (*cb) (void*));

enum cc_stat  list_get_at          (List *list, size_t index, void **out);
enum cc_stat  list_get_first       (List *list, void **out);
enum cc_stat  list_get_last        (List *list, void **out);

enum cc_stat  list_sublist         (List *list, size_t from, size_t to, List **out);
enum cc_stat  list_copy_shallow    (List *list, List **out);
enum cc_stat  list_copy_deep       (List *list, void *(*cp) (void*), List **out);

enum cc_stat  list_replace_at      (List *list, void *element, size_t index, void **out);

size_t        list_contains        (List *list, void *element);
size_t        list_contains_value  (List *list, void *element, int (*cmp) (const void*, const void*));
enum cc_stat  list_index_of        (List *list, void *element, int (*cmp) (const void*, const void*), size_t *index);
enum cc_stat  list_to_array        (List *list, void ***out);

void          list_reverse         (List *list);
enum cc_stat  list_sort            (List *list, int (*cmp) (void const*, void const*));
void          list_sort_in_place   (List *list, int (*cmp) (void const*, void const*));
size_t        list_size            (List *list);

void          list_foreach         (List *list, void (*op) (void *));

enum cc_stat  list_filter_mut      (List *list, bool (*predicate) (const void*));
enum cc_stat  list_filter          (List *list, bool (*predicate) (const void*), List **out);

void          list_iter_init       (ListIter *iter, List *list);
enum cc_stat  list_iter_remove     (ListIter *iter, void **out);
enum cc_stat  list_iter_add        (ListIter *iter,  void *element);
enum cc_stat  list_iter_replace    (ListIter *iter, void *element, void **out);
size_t        list_iter_index      (ListIter *iter);
enum cc_stat  list_iter_next       (ListIter *iter, void **out);

void          list_diter_init      (ListIter *iter, List *list);
enum cc_stat  list_diter_remove    (ListIter *iter, void **out);
enum cc_stat  list_diter_add       (ListIter *iter, void *element);
enum cc_stat  list_diter_replace   (ListIter *iter, void *element, void **out);
size_t        list_diter_index     (ListIter *iter);
enum cc_stat  list_diter_next      (ListIter *iter, void **out);

void          list_zip_iter_init   (ListZipIter *iter, List *l1, List *l2);
enum cc_stat  list_zip_iter_next   (ListZipIter *iter, void **out1, void **out2);
enum cc_stat  list_zip_iter_add    (ListZipIter *iter, void *e1, void *e2);
enum cc_stat  list_zip_iter_remove (ListZipIter *iter, void **out1, void **out2);
enum cc_stat  list_zip_iter_replace(ListZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        list_zip_iter_index  (ListZipIter *iter);


#define LIST_FOREACH(val, list, body)                                   \
    {                                                                   \
        ListIter list_iter_53d46d2a04458e7b;                            \
        list_iter_init(&list_iter_53d46d2a04458e7b, list);              \
        void *val;                                                      \
        while (list_iter_next(&list_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define LIST_FOREACH_ZIP(val1, val2, list1, list2, body)                \
    {                                                                   \
        ListZipIter list_zip_iter_ea08d3e52f25883b3;                    \
        list_zip_iter_init(&list_zip_iter_ea08d3e52f25883b, list1, list2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (list_zip_iter_next(&list_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }


#endif /* COLLECTIONS_C_LIST_H */
