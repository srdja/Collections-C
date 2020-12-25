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

#ifndef COLLECTIONS_C_CC_HASHSET_H
#define COLLECTIONS_C_CC_HASHSET_H

#include "cc_common.h"
#include "cc_hashtable.h"

/**
 * An unordered set. The lookup, deletion, and insertion are
 * performed in amortized constant time and in the worst case
 * in amortized linear time.
 */
typedef struct cc_hashset_s CC_HashSet;

/**
 * CC_HashSet configuration object.
 */
typedef CC_HashTableConf CC_HashSetConf;

/**
 * CC_HashSet iterator structure. Used to iterate over the elements
 * of the CC_HashSet. The iterator also supports operations for safely
 * removing elements during iteration.
 */
typedef struct cc_hashset_iter_s {
    CC_HashTableIter iter;
} CC_HashSetIter;

void          cc_hashset_conf_init     (CC_HashSetConf *conf);

enum cc_stat  cc_hashset_new           (CC_HashSet **hs);
enum cc_stat  cc_hashset_new_conf      (CC_HashSetConf const * const conf, CC_HashSet **hs);
void          cc_hashset_destroy       (CC_HashSet *set);

enum cc_stat  cc_hashset_add           (CC_HashSet *set, void *element);
enum cc_stat  cc_hashset_remove        (CC_HashSet *set, void *element, void **out);
void          cc_hashset_remove_all    (CC_HashSet *set);

bool          cc_hashset_contains      (CC_HashSet *set, void *element);
size_t        cc_hashset_size          (CC_HashSet *set);
size_t        cc_hashset_capacity      (CC_HashSet *set);

void          cc_hashset_foreach       (CC_HashSet *set, void (*op) (const void*));

void          cc_hashset_iter_init     (CC_HashSetIter *iter, CC_HashSet *set);
enum cc_stat  cc_hashset_iter_next     (CC_HashSetIter *iter, void **out);
enum cc_stat  cc_hashset_iter_remove   (CC_HashSetIter *iter, void **out);


#define CC_HASHSET_FOREACH(val, hashset, body)                          \
    {                                                                   \
        CC_HashSetIter cc_hashset_iter_53d46d2a04458e7b;                \
        cc_hashset_iter_init(&cc_hashset_iter_53d46d2a04458e7b, hashset); \
        void *val;                                                      \
        while (cc_hashset_iter_next(&cc_hashset_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }

#endif /* COLLECTIONS_C_CC_HASHSET_H */
