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

#ifndef COLLECTIONS_C_CC_LIST_H
#define COLLECTIONS_C_CC_LIST_H

#include "cc_common.h"

/**
 * A doubly linked list. CC_List is a sequential structure that
 * supports insertion, deletion and lookup from both ends in
 * constant time, while the worst case is O(n/2) at the middle
 * of the list.
 */
typedef struct cc_list_s CC_List;

/**
 * CC_List node.
 *
 * @note Modifying the links may invalidate the list structure.
 */
typedef struct node_s {
    void          *data;
    struct node_s *next;
    struct node_s *prev;
} Node;

/**
 * CC_List iterator structure. Used to iterate over the elements of the
 * list in an ascending or descending order. The iterator also supports
 * operations for safely adding and removing elements during iteration.
 */
typedef struct cc_list_iter_s {
    /**
     * The current position of the iterator.*/
    size_t  index;

    /**
     * The list associated with this iterator */
    CC_List   *list;

    /**
     * Last returned node */
    Node   *last;

    /**
     * Next node in the sequence. */
    Node   *next;
} CC_ListIter;

/**
 * CC_List zip iterator structure. Used to iterate over two CC_Lists in
 * lockstep in an ascending order until one of the lists is exhausted.
 * The iterator also supports operations for safely adding and
 * removing elements during iteration.
 */
typedef struct cc_list_zip_iter_s {
    CC_List *l1;
    CC_List *l2;
    Node *l1_last;
    Node *l2_last;
    Node *l1_next;
    Node *l2_next;
    size_t index;
} CC_ListZipIter;


/**
 * CC_List configuration structure. Used to initialize a new CC_List with specific
 * values.
 */
typedef struct cc_list_conf_s {
    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
} CC_ListConf;


void          cc_list_conf_init       (CC_ListConf *conf);
enum cc_stat  cc_list_new             (CC_List **list);
enum cc_stat  cc_list_new_conf        (CC_ListConf const * const conf, CC_List **list);
void          cc_list_destroy         (CC_List *list);
void          cc_list_destroy_cb      (CC_List *list, void (*cb) (void*));

enum cc_stat  cc_list_splice          (CC_List *list1, CC_List *list2);
enum cc_stat  cc_list_splice_at       (CC_List *list, CC_List *list2, size_t index);

enum cc_stat  cc_list_add             (CC_List *list, void *element);
enum cc_stat  cc_list_add_at          (CC_List *list, void *element, size_t index);
enum cc_stat  cc_list_add_all         (CC_List *list1, CC_List *list2);
enum cc_stat  cc_list_add_all_at      (CC_List *list, CC_List *list2, size_t index);
enum cc_stat  cc_list_add_first       (CC_List *list, void *element);
enum cc_stat  cc_list_add_last        (CC_List *list, void *element);

enum cc_stat  cc_list_remove          (CC_List *list, void *element, void **out);
enum cc_stat  cc_list_remove_first    (CC_List *list, void **out);
enum cc_stat  cc_list_remove_last     (CC_List *list, void **out);
enum cc_stat  cc_list_remove_at       (CC_List *list, size_t index, void **out);

enum cc_stat  cc_list_remove_all      (CC_List *list);
enum cc_stat  cc_list_remove_all_cb   (CC_List *list, void (*cb) (void*));

enum cc_stat  cc_list_get_at          (CC_List *list, size_t index, void **out);
enum cc_stat  cc_list_get_first       (CC_List *list, void **out);
enum cc_stat  cc_list_get_last        (CC_List *list, void **out);

enum cc_stat  cc_list_sublist         (CC_List *list, size_t from, size_t to, CC_List **out);
enum cc_stat  cc_list_copy_shallow    (CC_List *list, CC_List **out);
enum cc_stat  cc_list_copy_deep       (CC_List *list, void *(*cp) (void*), CC_List **out);

enum cc_stat  cc_list_replace_at      (CC_List *list, void *element, size_t index, void **out);

size_t        cc_list_contains        (CC_List *list, void *element);
size_t        cc_list_contains_value  (CC_List *list, void *element, int (*cmp) (const void*, const void*));
enum cc_stat  cc_list_index_of        (CC_List *list, void *element, int (*cmp) (const void*, const void*), size_t *index);
enum cc_stat  cc_list_to_array        (CC_List *list, void ***out);

void          cc_list_reverse         (CC_List *list);
enum cc_stat  cc_list_sort            (CC_List *list, int (*cmp) (void const*, void const*));
void          cc_list_sort_in_place   (CC_List *list, int (*cmp) (void const*, void const*));
size_t        cc_list_size            (CC_List *list);

void          cc_list_foreach         (CC_List *list, void (*op) (void *));

enum cc_stat cc_list_reduce(CC_List *list, void (*fn)(void *, void *, void *), void *result);

enum cc_stat  cc_list_filter_mut      (CC_List *list, bool (*predicate) (const void*));
enum cc_stat  cc_list_filter          (CC_List *list, bool (*predicate) (const void*), CC_List **out);

void          cc_list_iter_init       (CC_ListIter *iter, CC_List *list);
enum cc_stat  cc_list_iter_remove     (CC_ListIter *iter, void **out);
enum cc_stat  cc_list_iter_add        (CC_ListIter *iter,  void *element);
enum cc_stat  cc_list_iter_replace    (CC_ListIter *iter, void *element, void **out);
size_t        cc_list_iter_index      (CC_ListIter *iter);
enum cc_stat  cc_list_iter_next       (CC_ListIter *iter, void **out);

void          cc_list_diter_init      (CC_ListIter *iter, CC_List *list);
enum cc_stat  cc_list_diter_remove    (CC_ListIter *iter, void **out);
enum cc_stat  cc_list_diter_add       (CC_ListIter *iter, void *element);
enum cc_stat  cc_list_diter_replace   (CC_ListIter *iter, void *element, void **out);
size_t        cc_list_diter_index     (CC_ListIter *iter);
enum cc_stat  cc_list_diter_next      (CC_ListIter *iter, void **out);

void          cc_list_zip_iter_init   (CC_ListZipIter *iter, CC_List *l1, CC_List *l2);
enum cc_stat  cc_list_zip_iter_next   (CC_ListZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_list_zip_iter_add    (CC_ListZipIter *iter, void *e1, void *e2);
enum cc_stat  cc_list_zip_iter_remove (CC_ListZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_list_zip_iter_replace(CC_ListZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        cc_list_zip_iter_index  (CC_ListZipIter *iter);


#define CC_LIST_FOREACH(val, list, body)                                \
    {                                                                   \
        CC_ListIter cc_list_iter_53d46d2a04458e7b;                      \
        cc_list_iter_init(&cc_list_iter_53d46d2a04458e7b, list);        \
        void *val;                                                      \
        while (cc_list_iter_next(&cc_list_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_LIST_FOREACH_ZIP(val1, val2, list1, list2, body)                \
    {                                                                   \
        CC_ListZipIter cc_list_zip_iter_ea08d3e52f25883b3;              \
        cc_list_zip_iter_init(&cc_list_zip_iter_ea08d3e52f25883b, list1, list2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_list_zip_iter_next(&cc_list_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }


#endif /* COLLECTIONS_C_CC_LIST_H */
