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

#ifndef HASHSET_H_
#define HASHSET_H_

#include "common.h"
#include "hashtable.h"

typedef struct hashset_s      HashSet;
typedef struct hashset_iter_s HashSetIter;
typedef        HashTableConf  HashSetConf;

struct hashset_iter_s {
    HashTableIter *iter;
};

void        hashset_conf_init     (HashSetConf *conf);

HashSet    *hashset_new           (void);
HashSet    *hashset_new_conf      (HashSetConf *properties);
void        hashset_destroy       (HashSet *set);

void        hashset_add           (HashSet *set, void *element);
void       *hashset_remove        (HashSet *set, void *element);
void        hashset_remove_all    (HashSet *set);

bool        hashset_contains      (HashSet *set, void *element);
size_t      hashset_size          (HashSet *set);

void        hashset_foreach       (HashSet *set, void (*op) (const void*));

void        hashset_iter_init     (HashSetIter *iter, HashSet *set);
bool        hashset_iter_has_next (HashSetIter *iter);
const void *hashset_iter_next     (HashSetIter *iter);
void       *hashset_iter_remove   (HashSetIter *iter);

#endif /* HASHSET_H_ */
