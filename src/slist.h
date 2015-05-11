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


#ifndef SLIST_H_
#define SLIST_H_

#include "common.h"

typedef struct slist_s SList;

typedef struct snode_s {
    void           *data;
    struct snode_s *next;
} SNode;

typedef struct slist_iter_s {
    size_t  index;
    SList  *list;
    SNode   *next;
    SNode   *current;
    SNode   *prev;
} SListIter;

typedef struct slist_conf_s {
    void  *(*mem_alloc)  (size_t size);
    void  *(*mem_calloc) (size_t blocks, size_t size);
    void   (*mem_free)   (void *block);
} SListConf;

void       slist_conf_init       (SListConf *conf);

SList*     slist_new             (void);
SList*     slist_new_conf        (SListConf *conf);
bool       slist_destroy         (SList *list);
bool       slist_destroy_free    (SList *list);

bool       slist_splice          (SList *list1, SList *list2);
bool       slist_splice_at       (SList *list1, SList *list2, size_t index);

bool       slist_add             (SList *list, void *element);
bool       slist_add_at          (SList *list, void *element, size_t index);
bool       slist_add_all         (SList *list1, SList *list2);
bool       slist_add_all_at      (SList *list1, SList *list2, size_t index);
bool       slist_add_first       (SList *list, void *element);
bool       slist_add_last        (SList *list, void *element);

void*      slist_remove          (SList *list, void *element);
void*      slist_remove_first    (SList *list);
void*      slist_remove_last     (SList *list);
void*      slist_remove_at       (SList *list, size_t index);

bool       slist_remove_all      (SList *list);
bool       slist_remove_all_free (SList *list);

void*      slist_get             (SList *list, size_t index);
void*      slist_get_first       (SList *list);
void*      slist_get_last        (SList *list);

SList*     slist_sublist         (SList *list, size_t from, size_t to);
SList*     slist_copy_shallow    (SList *list);
SList*     slist_copy_deep       (SList *list, void *(*cp) (void*));

void*      slist_replace_at      (SList *list, void *element, size_t index);

size_t     slist_contains        (SList *list, void *element);
size_t     slist_index_of        (SList *list, void *element);
void**     slist_to_array        (SList *list);

void       slist_reverse         (SList *list);
void       slist_sort            (SList *list, int (*cmp) (void const*, void const*));
size_t     slist_size            (SList *list);

void       slist_foreach         (SList *list, void (*op) (void *));

void       slist_iter_init       (SListIter *iter, SList *list);
void*      slist_iter_remove     (SListIter *iter);
bool       slist_iter_add        (SListIter *iter, void *element);
void*      slist_iter_replace    (SListIter *iter, void *element);
void*      slist_iter_next       (SListIter *iter);
size_t     slist_iter_index      (SListIter *iter);
bool       slist_iter_has_next   (SListIter *iter);

#endif /* SLIST_H_ */
