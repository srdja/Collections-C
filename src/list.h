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

#ifndef LIST_H_
#define LIST_H_

#include "common.h"

typedef struct dlist_s            List;
typedef struct dlist_iter_s       ListIter;
typedef struct dlist_iter_desc_s  ListDIter;

List*      list_new             ();
bool       list_destroy         (List *list);
bool       list_destroy_free    (List *list);

bool       list_splice          (List *list1, List *list2);
bool       list_splice_at       (List *list, List *list2, size_t index);

bool       list_add             (List *list, void *element);
bool       list_add_at          (List *list, void *element, size_t index);
bool       list_add_all         (List *list1, List *list2);
bool       list_add_all_at      (List *list, List *list2, size_t index);
bool       list_add_first       (List *list, void *element);
bool       list_add_last        (List *list, void *element);

void*      list_remove          (List *list, void *element);
void*      list_remove_first    (List *list);
void*      list_remove_last     (List *list);
void*      list_remove_at       (List *list, size_t index);

bool       list_remove_all      (List *list);
bool       list_remove_all_free (List *list);

void*      list_get             (List *list, size_t index);
void*      list_get_first       (List *list);
void*      list_get_last        (List *list);

List*      list_sublist         (List *list, size_t from, size_t to);
List*      list_copy_shallow    (List *list);
List*      list_copy_deep       (List *list, void *(*cp) (void*));
 
void*      list_replace_at      (List *list, void *element, size_t index);

size_t     list_contains        (List *list, void *element);
size_t     list_index_of        (List *list, void *element);
void**     list_to_array        (List *list);

void       list_reverse         (List *list);
void       list_sort            (List *list, int (*cmp) (void*, void*));
size_t     list_size            (List *list);

void       list_foreach         (List *list, void (*op) (void *));

ListIter*  list_iter_new        (List *list);
void       list_iter_destroy    (ListIter *iter);
void*      list_iter_remove     (ListIter *iter);
bool       list_iter_add        (ListIter *iter,  void *element);
void*      list_iter_replace    (ListIter *iter, void *element);
bool       list_iter_has_next   (ListIter *iter);
size_t     list_iter_index      (ListIter *iter);
void*      list_iter_next       (ListIter *iter);

ListDIter* list_diter_new       (List *list);
void       list_diter_destroy   (ListDIter *iter);
void*      list_diter_remove    (ListDIter *iter);
bool       list_diter_add       (ListDIter *iter, void *element);
void*      list_diter_replace   (ListDIter *iter, void *element);
bool       list_diter_has_next  (ListDIter *iter);
size_t     list_iter_index      (ListIter *iter);
void*      list_diter_next      (ListDIter *iter);

#endif /* LIST_H_ */
