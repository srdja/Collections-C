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

#ifndef __TREESET_H_
#define __TREESET_H_

#include "common.h"
#include "treetable.h"

typedef struct treeset_s     TreeSet;
typedef        TreeTableConf TreeSetConf;

typedef struct treeset_iter_s {
    TreeTableIter i;
} TreeSetIter;

void      treeset_conf_init        (TreeSetConf *conf);
TreeSet  *treeset_new              (int (*cmp) (void*, void*));
TreeSet  *treeset_new_conf         (TreeSetConf *conf);
void      treeset_destroy          (TreeSet *set);

void      treeset_add              (TreeSet *set, void *element);
void     *treeset_remove           (TreeSet *set, void *element);
void      treeset_remove_all       (TreeSet *set);

void     *treeset_get_first        (TreeSet *set);
void     *treeset_get_last         (TreeSet *set);
void     *treeset_get_greater_than (TreeSet *set, void *element);
void     *treeset_get_lesser_than  (TreeSet *set, void *element);

bool      treeset_contains         (TreeSet *set, void *element);
size_t    treeset_size             (TreeSet *set);

void      treeset_foreach          (TreeSet *set, void (*op) (const void*));

void      treeset_iter_init        (TreeSetIter *iter, TreeSet *set);
bool      treeset_iter_has_next    (TreeSetIter *iter);
void      treeset_iter_next        (TreeSetIter *iter, void **element);
void      treeset_iter_remove      (TreeSetIter *iter);

#endif
