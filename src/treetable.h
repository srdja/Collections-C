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
 * along with Collections-C. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TREETABLE_H__
#define __TREETABLE_H__

#include "common.h"

typedef struct treetable_s TreeTable;

typedef struct rbnode_s {
    void *key;
    void *value;
    char  color;

    struct rbnode_s *parent;
    struct rbnode_s *left;
    struct rbnode_s *right;
} RBNode;

typedef struct tree_table_entry_s {
    void *key;
    void *value;
} TreeTableEntry;

typedef struct tree_table_iter_s {
    TreeTable *table;
    RBNode    *current;
    RBNode    *next;
} TreeTableIter;

typedef struct treetable_conf_s {
    int    (*cmp)         (void *k1, void *k2);
    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
} TreeTableConf;


void       treetable_conf_init        (TreeTableConf *conf);
TreeTable *treetable_new              (int (*cmp) (void *, void *));
TreeTable *treetable_new_conf         (TreeTableConf *conf);

void       treetable_destroy          (TreeTable *table);
bool       treetable_add              (TreeTable *table, void *key, void *val);

void      *treetable_remove           (TreeTable *table, void *key);
void       treetable_remove_all       (TreeTable *table);
void      *treetable_remove_first     (TreeTable *table);
void      *treetable_remove_last      (TreeTable *table);

void      *treetable_get              (TreeTable *table, void *key);
void      *treetable_get_first_value  (TreeTable *table);
void      *treetable_get_first_key    (TreeTable *table);
void      *treetable_get_last_value   (TreeTable *table);
void      *treetable_get_last_key     (TreeTable *table);
void      *treetable_get_greater_than (TreeTable *table, void *key);
void      *treetable_get_lesser_than  (TreeTable *table, void *key);

size_t     treetable_size             (TreeTable *table);
bool       treetable_contains_key     (TreeTable *table, void *key);
size_t     treetable_contains_value   (TreeTable *table, void *key);

void       treetable_foreach_key      (TreeTable *table, void (*op) (const void*));
void       treetable_foreach_value    (TreeTable *table, void (*op) (void*));

void       treetable_iter_init        (TreeTableIter *iter, TreeTable *table);
bool       treetable_iter_has_next    (TreeTableIter *iter);
void       treetable_iter_next        (TreeTableIter *iter, TreeTableEntry *entry);
void       treetable_iter_remove      (TreeTableIter *iter);

#ifdef DEBUG
#define RB_ERROR_CONSECUTIVE_RED 0
#define RB_ERROR_BLACK_HEIGHT    1
#define RB_ERROR_TREE_STRUCTURE  2
#define RB_ERROR_OK              4

int treetable_assert_rb_rules(TreeTable *table);
#endif

#endif
