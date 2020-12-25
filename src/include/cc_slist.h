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

#ifndef COLLECTIONS_C_CC_SLIST_H
#define COLLECTIONS_C_CC_SLIST_H

#include "cc_common.h"

/**
 * A singly linked list. List is a sequential structure that
 * supports constant time insertion, deletion  and lookup at
 * the beginning of the list, while the worst case for these
 * operations is linear time.
 */
typedef struct cc_slist_s CC_SList;

/**
 * CC_SList node.
 *
 * @note Modifying the links may invalidate the list structure.
 */
typedef struct snode_s {
    void           *data;
    struct snode_s *next;
} SNode;

/**
 * CC_SList iterator structure. Used to iterate over the elements
 * of the list in an ascending order. The iterator also supports
 * operations for safely adding and removing elements during iteration.
 */
typedef struct cc_slist_iter_s {
    size_t  index;
    CC_SList  *list;
    SNode   *next;
    SNode   *current;
    SNode   *prev;
} CC_SListIter;

/**
 * CC_SList zip iterator structure. Used to iterate over two CC_SLists in
 * lockstep in an ascending order until one of the lists is exhausted.
 * The iterator also supports operations for safely adding and
 * removing elements during iteration.
 */
typedef struct cc_slist_zip_iter_s {
    size_t index;
    CC_SList *l1;
    CC_SList *l2;
    SNode *l1_next;
    SNode *l2_next;
    SNode *l1_current;
    SNode *l2_current;
    SNode *l1_prev;
    SNode *l2_prev;
} CC_SListZipIter;


/**
 * CC_SList configuration structure. Used to initialize a new CC_SList with
 * specific values.
 */
typedef struct cc_slist_conf_s {
    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
} CC_SListConf;


void          cc_slist_conf_init       (CC_SListConf *conf);
enum cc_stat  cc_slist_new             (CC_SList **list);
enum cc_stat  cc_slist_new_conf        (CC_SListConf const * const conf, CC_SList **list);
void          cc_slist_destroy         (CC_SList *list);
void          cc_slist_destroy_cb      (CC_SList *list, void (*cb) (void*));

enum cc_stat  cc_slist_splice          (CC_SList *list1, CC_SList *list2);
enum cc_stat  cc_slist_splice_at       (CC_SList *list1, CC_SList *list2, size_t index);

enum cc_stat  cc_slist_add             (CC_SList *list, void *element);
enum cc_stat  cc_slist_add_at          (CC_SList *list, void *element, size_t index);
enum cc_stat  cc_slist_add_all         (CC_SList *list1, CC_SList *list2);
enum cc_stat  cc_slist_add_all_at      (CC_SList *list1, CC_SList *list2, size_t index);
enum cc_stat  cc_slist_add_first       (CC_SList *list, void *element);
enum cc_stat  cc_slist_add_last        (CC_SList *list, void *element);

enum cc_stat  cc_slist_remove          (CC_SList *list, void *element, void **out);
enum cc_stat  cc_slist_remove_first    (CC_SList *list, void **out);
enum cc_stat  cc_slist_remove_last     (CC_SList *list, void **out);
enum cc_stat  cc_slist_remove_at       (CC_SList *list, size_t index, void **out);

enum cc_stat  cc_slist_remove_all      (CC_SList *list);
enum cc_stat  cc_slist_remove_all_cb   (CC_SList *list, void (*cb) (void*));

enum cc_stat  cc_slist_get_at          (CC_SList *list, size_t index, void **out);
enum cc_stat  cc_slist_get_first       (CC_SList *list, void **out);
enum cc_stat  cc_slist_get_last        (CC_SList *list, void **out);

enum cc_stat  cc_slist_sublist         (CC_SList *list, size_t from, size_t to, CC_SList **out);
enum cc_stat  cc_slist_copy_shallow    (CC_SList *list, CC_SList **out);
enum cc_stat  cc_slist_copy_deep       (CC_SList *list, void *(*cp) (void*), CC_SList **out);

enum cc_stat  cc_slist_replace_at      (CC_SList *list, void *element, size_t index, void **out);

size_t        cc_slist_contains        (CC_SList *list, void *element);
size_t        cc_slist_contains_value  (CC_SList *list, void *element, int (*cmp) (const void*, const void*));
enum cc_stat  cc_slist_index_of        (CC_SList *list, void *element, size_t *index);
enum cc_stat  cc_slist_to_array        (CC_SList *list, void ***out);

void          cc_slist_reverse         (CC_SList *list);
enum cc_stat  cc_slist_sort            (CC_SList *list, int (*cmp) (void const*, void const*));
size_t        cc_slist_size            (CC_SList *list);

void          cc_slist_foreach         (CC_SList *list, void (*op) (void *));

enum cc_stat  cc_slist_filter          (CC_SList *list, bool (*predicate) (const void*), CC_SList **out);
enum cc_stat  cc_slist_filter_mut      (CC_SList *list, bool (*predicate) (const void*));

void          cc_slist_iter_init       (CC_SListIter *iter, CC_SList *list);
enum cc_stat  cc_slist_iter_remove     (CC_SListIter *iter, void **out);
enum cc_stat  cc_slist_iter_add        (CC_SListIter *iter, void *element);
enum cc_stat  cc_slist_iter_replace    (CC_SListIter *iter, void *element, void **out);
enum cc_stat  cc_slist_iter_next       (CC_SListIter *iter, void **out);
size_t        cc_slist_iter_index      (CC_SListIter *iter);

void          cc_slist_zip_iter_init   (CC_SListZipIter *iter, CC_SList *l1, CC_SList *l2);
enum cc_stat  cc_slist_zip_iter_next   (CC_SListZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_slist_zip_iter_add    (CC_SListZipIter *iter, void *e1, void *e2);
enum cc_stat  cc_slist_zip_iter_remove (CC_SListZipIter *iter, void **out1, void **out2);
enum cc_stat  cc_slist_zip_iter_replace(CC_SListZipIter *iter, void *e1, void *e2, void **out1, void **out2);
size_t        cc_slist_zip_iter_index  (CC_SListZipIter *iter);


#define CC_SLIST_FOREACH(val, slist, body)                              \
    {                                                                   \
        CC_SListIter cc_slist_iter_53d46d2a04458e7b;                    \
        cc_slist_iter_init(&cc_slist_iter_53d46d2a04458e7b, slist);     \
        void *val;                                                      \
        while (cc_slist_iter_next(&cc_slist_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }


#define CC_SLIST_FOREACH_ZIP(val1, val2, slist1, slist2, body)          \
    {                                                                   \
        CC_SListZipIter cc_slist_zip_iter_ea08d3e52f25883b3;            \
        cc_slist_zip_iter_init(&cc_slist_zip_iter_ea08d3e52f25883b, slist1, slist2); \
        void *val1;                                                     \
        void *val2;                                                     \
        while (cc_slist_zip_iter_next(&cc_slist_zip_iter_ea08d3e52f25883b3, &val1, &val2) != CC_ITER_END) \
            body                                                        \
                }

#endif /* COLLECTIONS_C_CC_SLIST_H */
