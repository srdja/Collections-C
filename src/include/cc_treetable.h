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

#ifndef COLLECTIONS_C_CC_TREETABLE_H
#define COLLECTIONS_C_CC_TREETABLE_H

#include "cc_common.h"

/**
 * An ordered key-value map. CC_TreeTable supports logarithmic time
 * insertion, removal and lookup of values.
 */
typedef struct cc_treetable_s CC_TreeTable;

/**
 * Red-Black tree node.
 *
 * @note Modifying this structure may invalidate the table.
 */
typedef struct rbnode_s {
    /**
     * Key in the table. */
    void *key;

    /**
     * Value associated with the key */
    void *value;

    /**
     * The color of this node */
    char  color;

    /**
     * Parent of this node */
    struct rbnode_s *parent;

    /**
     * Left child node */
    struct rbnode_s *left;

    /**
     * Right child node */
    struct rbnode_s *right;
} RBNode;

/**
 * CC_TreeTable table entry.
 */
typedef struct tree_table_entry_s {
    void *key;
    void *value;
} CC_TreeTableEntry;

/**
 * CC_TreeTable iterator structure. Used to iterate over the entries
 * of the table. The iterator also supports operations for safely
 * removing elements during iteration.
 *
 * @note This structure should only be modified through the
 * iterator functions.
 */
typedef struct tree_table_iter_s {
    CC_TreeTable *table;
    RBNode    *current;
    RBNode    *next;
} CC_TreeTableIter;

/**
 * CC_TreeTable configuration structure. Used to initialize a new
 * CC_TreeTable with specific attributes.
 */
typedef struct cc_treetable_conf_s {
    int    (*cmp)         (const void *k1, const void *k2);
    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
} CC_TreeTableConf;


void          cc_treetable_conf_init        (CC_TreeTableConf *conf);
enum cc_stat  cc_treetable_new              (int (*cmp) (const void*, const void*), CC_TreeTable **tt);
enum cc_stat  cc_treetable_new_conf         (CC_TreeTableConf const * const conf, CC_TreeTable **tt);

void          cc_treetable_destroy          (CC_TreeTable *table);
enum cc_stat  cc_treetable_add              (CC_TreeTable *table, void *key, void *val);

enum cc_stat  cc_treetable_remove           (CC_TreeTable *table, void *key, void **out);
void          cc_treetable_remove_all       (CC_TreeTable *table);
enum cc_stat  cc_treetable_remove_first     (CC_TreeTable *table, void **out);
enum cc_stat  cc_treetable_remove_last      (CC_TreeTable *table, void **out);

enum cc_stat  cc_treetable_get              (CC_TreeTable const * const table, const void *key, void **out);
enum cc_stat  cc_treetable_get_first_value  (CC_TreeTable const * const table, void **out);
enum cc_stat  cc_treetable_get_first_key    (CC_TreeTable const * const table, void **out);
enum cc_stat  cc_treetable_get_last_value   (CC_TreeTable const * const table, void **out);
enum cc_stat  cc_treetable_get_last_key     (CC_TreeTable const * const table, void **out);
enum cc_stat  cc_treetable_get_greater_than (CC_TreeTable const * const table, const void *key, void **out);
enum cc_stat  cc_treetable_get_lesser_than  (CC_TreeTable const * const table, const void *key, void **out);

size_t        cc_treetable_size             (CC_TreeTable const * const table);
bool          cc_treetable_contains_key     (CC_TreeTable const * const table, const void *key);
size_t        cc_treetable_contains_value   (CC_TreeTable const * const table, const void *value);

void          cc_treetable_foreach_key      (CC_TreeTable *table, void (*op) (const void*));
void          cc_treetable_foreach_value    (CC_TreeTable *table, void (*op) (void*));

void          cc_treetable_iter_init        (CC_TreeTableIter *iter, CC_TreeTable *table);
enum cc_stat  cc_treetable_iter_next        (CC_TreeTableIter *iter, CC_TreeTableEntry *entry);
enum cc_stat  cc_treetable_iter_remove      (CC_TreeTableIter *iter, void **out);


#define CC_TREETABLE_FOREACH(entry, treetable, body)                    \
    {                                                                   \
        TreetableIter cc_treetable_iter_53d46d2a04458e7b;               \
        cc_treetable_iter_init(&cc_treetable_iter_53d46d2a04458e7b, treetable); \
        CC_TreeTableEntry *val;                                         \
        while (cc_treetable_iter_next(&cc_treetable_iter_53d46d2a04458e7b, &entry) != CC_ITER_END) \
            body                                                        \
                }


#ifdef DEBUG
#define RB_ERROR_CONSECUTIVE_RED 0
#define RB_ERROR_BLACK_HEIGHT    1
#define RB_ERROR_TREE_STRUCTURE  2
#define RB_ERROR_OK              4

int cc_treetable_assert_rb_rules(CC_TreeTable *table);
#endif /* DEBUG */

#endif /* COLLECTIONS_C_CC_TREETABLE_H */
