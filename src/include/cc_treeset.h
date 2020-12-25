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

#ifndef COLLECTIONS_C_CC_TREESET_H
#define COLLECTIONS_C_CC_TREESET_H

#include "cc_common.h"
#include "cc_treetable.h"

/**
 * An ordered set. The lookup, deletion, and insertion are
 * performed in logarithmic time.
 */
typedef struct cc_treeset_s CC_TreeSet;

/**
 * CC_TreeSet configuration structure.
 */
typedef CC_TreeTableConf CC_TreeSetConf;

/**
 * CC_TreeSet iterator structure. Used to iterate over the elements of the set.
 * The iterator also supports operations for safely removing elements
 * during iteration.
 */
typedef struct cc_treeset_iter_s {
    CC_TreeTableIter i;
} CC_TreeSetIter;


void          cc_treeset_conf_init        (CC_TreeSetConf *conf);
enum cc_stat  cc_treeset_new              (int (*cmp) (const void*, const void*), CC_TreeSet **set);
enum cc_stat  cc_treeset_new_conf         (CC_TreeSetConf const * const conf, CC_TreeSet **set);

void          cc_treeset_destroy          (CC_TreeSet *set);

enum cc_stat  cc_treeset_add              (CC_TreeSet *set, void *element);
enum cc_stat  cc_treeset_remove           (CC_TreeSet *set, void *element, void **out);
void          cc_treeset_remove_all       (CC_TreeSet *set);

enum cc_stat  cc_treeset_get_first        (CC_TreeSet *set, void **out);
enum cc_stat  cc_treeset_get_last         (CC_TreeSet *set, void **out);
enum cc_stat  cc_treeset_get_greater_than (CC_TreeSet *set, void *element, void **out);
enum cc_stat  cc_treeset_get_lesser_than  (CC_TreeSet *set, void *element, void **out);

bool          cc_treeset_contains         (CC_TreeSet *set, void *element);
size_t        cc_treeset_size             (CC_TreeSet *set);

void          cc_treeset_foreach          (CC_TreeSet *set, void (*op) (const void*));

void          cc_treeset_iter_init        (CC_TreeSetIter *iter, CC_TreeSet *set);
enum cc_stat  cc_treeset_iter_next        (CC_TreeSetIter *iter, void **element);
enum cc_stat  cc_treeset_iter_remove      (CC_TreeSetIter *iter, void **out);


#define CC_TREESET_FOREACH(val, treeset, body)                          \
    {                                                                   \
        CC_TreeSetIter cc_treeset_iter_53d46d2a04458e7b;                \
        cc_treeset_iter_init(&cc_treeset_iter_53d46d2a04458e7b, treeset); \
        void *val;                                                      \
        while (cc_treeset_iter_next(&cc_treeset_iter_53d46d2a04458e7b, &val) != CC_ITER_END) \
            body                                                        \
                }

#endif /* COLLECTIONS_C_CC_TREESET_H */
