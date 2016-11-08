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

#ifndef COLLECTIONS_C_TREESET_H
#define COLLECTIONS_C_TREESET_H

#include "common.h"
#include "treetable.h"

/**
 * An ordered set. The lookup, deletion, and insertion are
 * performed in logarithmic time.
 */
typedef struct treeset_s TreeSet;

/**
 * TreeSet configuration structure.
 */
typedef TreeTableConf TreeSetConf;

/**
 * TreeSet iterator structure. Used to iterate over the elements of the set.
 * The iterator also supports operations for safely removing elements
 * during iteration.
 */
typedef struct treeset_iter_s {
    TreeTableIter i;
} TreeSetIter;


void          treeset_conf_init        (TreeSetConf *conf);
enum cc_stat  treeset_new              (int (*cmp) (const void*, const void*), TreeSet **set);
enum cc_stat  treeset_new_conf         (TreeSetConf const * const conf, TreeSet **set);

void          treeset_destroy          (TreeSet *set);

enum cc_stat  treeset_add              (TreeSet *set, void *element);
enum cc_stat  treeset_remove           (TreeSet *set, void *element, void **out);
void          treeset_remove_all       (TreeSet *set);

enum cc_stat  treeset_get_first        (TreeSet *set, void **out);
enum cc_stat  treeset_get_last         (TreeSet *set, void **out);
enum cc_stat  treeset_get_greater_than (TreeSet *set, void *element, void **out);
enum cc_stat  treeset_get_lesser_than  (TreeSet *set, void *element, void **out);

bool          treeset_contains         (TreeSet *set, void *element);
size_t        treeset_size             (TreeSet *set);

void          treeset_foreach          (TreeSet *set, void (*op) (const void*));

void          treeset_iter_init        (TreeSetIter *iter, TreeSet *set);
enum cc_stat  treeset_iter_next        (TreeSetIter *iter, void **element);
enum cc_stat  treeset_iter_remove      (TreeSetIter *iter, void **out);

#endif /* COLLECTIONS_C_TREESET_H */
