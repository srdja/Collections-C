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

#ifndef COLLECTIONS_C_HASHSET_H
#define COLLECTIONS_C_HASHSET_H

#include "common.h"
#include "hashtable.h"

/**
 * An unordered set. The lookup, deletion, and insertion are
 * performed in amortized constant time and in the worst case
 * in amortized linear time.
 */
typedef struct hashset_s HashSet;

/**
 * HashSet configuration object.
 */
typedef HashTableConf HashSetConf;

/**
 * HashSet iterator structure. Used to iterate over the elements
 * of the HashSet. The iterator also supports operations for safely
 * removing elements during iteration.
 */
typedef struct hashset_iter_s {
    HashTableIter iter;
} HashSetIter;

void          hashset_conf_init     (HashSetConf *conf);

enum cc_stat  hashset_new           (HashSet **hs);
enum cc_stat  hashset_new_conf      (HashSetConf const * const conf, HashSet **hs);
void          hashset_destroy       (HashSet *set);

enum cc_stat  hashset_add           (HashSet *set, void *element);
enum cc_stat  hashset_remove        (HashSet *set, void *element, void **out);
void          hashset_remove_all    (HashSet *set);

bool          hashset_contains      (HashSet *set, void *element);
size_t        hashset_size          (HashSet *set);
size_t        hashset_capacity      (HashSet *set);

void          hashset_foreach       (HashSet *set, void (*op) (const void*));

void          hashset_iter_init     (HashSetIter *iter, HashSet *set);
enum cc_stat  hashset_iter_next     (HashSetIter *iter, void **out);
enum cc_stat  hashset_iter_remove   (HashSetIter *iter, void **out);

#endif /* COLLECTIONS_C_HASHSET_H */
