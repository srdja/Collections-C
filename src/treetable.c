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

/* Tree operations are based on CLRS RB Tree. */

#include "treetable.h"

#define RB_BLACK 1
#define RB_RED   0

struct treetable_s {
    RBNode *root;
    RBNode *sentinel;
    size_t  size;

    int    (*cmp)         (void *k1, void *k2);

    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
};

static void rotate_left            (TreeTable *table, RBNode *n);
static void rotate_right           (TreeTable *table, RBNode *n);
static void rebalance_after_insert (TreeTable *table, RBNode *n);
static void rebalance_after_delete (TreeTable *table, RBNode *n);
static void remove_node            (TreeTable *table, RBNode *z);
static void tree_destroy           (TreeTable *table, RBNode *s);

static INLINE void  transplant     (TreeTable *table, RBNode *u, RBNode *v);
static INLINE RBNode *tree_min     (TreeTable *table, RBNode *n);
static INLINE RBNode *tree_max     (TreeTable *table, RBNode *n);

static RBNode *get_tree_node_by_key(TreeTable *table, void *key);
static RBNode *get_successor_node  (TreeTable *table, RBNode *n);
static RBNode *get_predecessor_node(TreeTable *table, RBNode *n);

/**
 * Initializes the TreehTableConf structs fields to default values.
 *
 * @param[in] conf the struct that is being initialized
 */
void treetable_conf_init(TreeTableConf *conf)
{
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

/**
 * Allocates a new TreeTable object using the standard allocators.
 * NULL may be returned if the underlying memory allocators fail.
 *
 * @param[in] cmp the comparator used to order keys within the table
 *
 * @return a new TreeTable or NULL if the memory allocation fails.
 */
enum cc_stat treetable_new(int (*cmp) (void *, void *), TreeTable **tt)
{
    TreeTableConf conf;
    treetable_conf_init(&conf);
    conf.cmp = cmp;
    return treetable_new_conf(&conf, tt);
}

/**
 * Allocates a new configured TreeTable based on the provided TreeTableConf object.
 * The table is allocated using the memory allocators specified in the TreeTableConf
 * object. In case the allocation of the table structure fails, NULL is returned.
 * The TreeTableConf object is not modified by this function and can therefore
 * be used for other tables.
 *
 * @param conf the TreeTableConf object used to configure this new TreeTable
 *
 * @return a new TreeTable or NULL if the memory allocation fails.
 */
enum cc_stat treetable_new_conf(TreeTableConf const * const conf, TreeTable **tt)
{
    TreeTable *table = conf->mem_calloc(1, sizeof(TreeTable));

    if (!table)
        return CC_ERR_ALLOC;

    RBNode *sentinel = conf->mem_calloc(1, sizeof(RBNode));

    if (!sentinel) {
        conf->mem_free(table);
        return CC_ERR_ALLOC;
    }

    sentinel->color   = RB_BLACK;

    table->size       = 0;
    table->cmp        = conf->cmp;
    table->mem_alloc  = conf->mem_alloc;
    table->mem_calloc = conf->mem_calloc;
    table->mem_free   = conf->mem_free;
    table->root       = sentinel;
    table->sentinel   = sentinel;

    *tt = table;
    return CC_OK;
}

static void tree_destroy(TreeTable *table, RBNode *n)
{
    if (n == table->sentinel)
        return;

    tree_destroy(table, n->left);
    tree_destroy(table, n->right);

    table->mem_free(n);
}

/**
 * Destroys the specified TreeTable structure without destroying the the data
 * contained within it. In other words the keys and the values are not freed,
 * but only the table structure.
 *
 * @param[in] table TreeTable to be destroyed.
 */
void treetable_destroy(TreeTable *table)
{
    tree_destroy(table, table->root);

    table->mem_free(table->sentinel);
    table->mem_free(table);
}

/**
 * Returns a value associated with the specified key. If there is no value
 * associated with this key, NULL is returned. In the case where the provided
 * key explicitly maps to a NULL value, calling <code>treetable_contains_key()
 * </code> before this function can resolve the ambiguity.
 *
 * @param[in] table the table from which the mapping is being returned
 * @param[in] key   the key that is being looked up
 *
 * @return the value mapped to the specified key, or null if the mapping doesn't
 *         exit
 */
enum cc_stat treetable_get(TreeTable *table, void *key, void **out)
{
    RBNode *node = get_tree_node_by_key(table, key);

    if (!node)
        return CC_ERR_KEY_NOT_FOUND;

    *out = node->value;
    return CC_OK;
}

/**
 * Returns the value associated with the first (lowest) key in the table.
 *
 * @param[in] table the table in which the lookup is performed
 *
 * @return value associated with the first key, or NULL if the table is empty
 */
enum cc_stat treetable_get_first_value(TreeTable *table, void **out)
{
    RBNode *node = tree_min(table, table->root);

    if (node != table->sentinel) {
        *out = node->value;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Returns the value associated with the highest (last) key in the table.
 *
 * @param[in] table the table in which the lookup is performed
 *
 * @return value associated with the last key, or NULL if the table is empty
 */
enum cc_stat treetable_get_last_value(TreeTable *table, void **out)
{
    RBNode *node = tree_max(table, table->root);

    if (node != table->sentinel) {
        *out = node->value;
        return CC_OK;
    }
    return CC_ERR_VALUE_NOT_FOUND;
}

/**
 * Returns the first (lowest) key in the table.
 *
 * @param[in] table the table in which the lookup is performed
 *
 * @return the first key
 */
enum cc_stat treetable_get_first_key(TreeTable *table, void **out)
{
    RBNode *node = tree_min(table, table->root);

    if (node != table->sentinel) {
        *out = node->key;
        return CC_OK;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Returns the last (highest) key in the table.
 *
 * @param[in] table the table in which the lookup is performed
 *
 * @return the last key
 */
enum cc_stat treetable_get_last_key(TreeTable *table, void **out)
{
    RBNode *node = tree_max(table, table->root);

    if (node != table->sentinel) {
        *out = node->key;
        return CC_OK;
    }
    return CC_ERR_KEY_NOT_FOUND;
}


/**
 * Returns the immediate successor of the specified key.
 *
 * @param[in] table the table into which the lookup is performed
 * @param[in] key   the key whose successor is being returned
 *
 * @return successor of the key, or NULL if there is no successor key
 */
enum cc_stat treetable_get_greater_than(TreeTable *table, void *key, void **out)
{
    RBNode *n = get_tree_node_by_key(table, key);
    RBNode *s = get_successor_node(table, n);

    if (n && s) {
        *out = s->key;
        return CC_OK;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Returns the immediate predecessor of the specified key.
 *
 * @param[in] table the table into which the lookup is performed
 * @param[in] key   the key whose predecessor is being returned
 *
 * @return predecessor of the key, or NULL if there is no predecessor key
 */
enum cc_stat treetable_get_lesser_than(TreeTable *table, void *key, void **out)
{
    RBNode *n = get_tree_node_by_key(table, key);
    RBNode *s = get_predecessor_node(table, n);

    if (n && s) {
        *out = s->key;
        return CC_OK;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Returns the size of the specified TreeTable. Size of a TreeTable represents
 * the number of key-value mappings within the table.
 *
 * @param[in] table the table whose size is being returned
 *
 * @return the size of the table
 */
size_t treetable_size(TreeTable *table)
{
    return table->size;
}

/**
 * Checks whether or not the TreeTable contains the specified key.
 *
 * @param[in] table the table into which the lookup is performed
 * @param[in] key the key that is being looked up
 *
 * @return true if the table contains the key.
 */
bool treetable_contains_key(TreeTable *table, void *key)
{
    RBNode *node = get_tree_node_by_key(table, key); // keys can't be null

    if (node)
        return true;

    return false;
}

/**
 * Checks whether or not the TreeTable contains the specified value.
 *
 * @param[in] table the table into which the lookup is performed
 * @param[in] value the value that is being looked up
 *
 * @return number of occurrences of the specified value.
 */
size_t treetable_contains_value(TreeTable *table, void *value)
{
    RBNode *node = tree_min(table, table->root);

    size_t o = 0;
    while (node != table->sentinel) {
        if (node->value == value)
            o++;
        node = get_successor_node(table, node);
    }
    return o;
}

/**
 * Creates a new key-value mapping in the specified TreeTable. If the unique key
 * is already mapped to a value in this table, that value is replaced with the
 * new value. This operation may fail if the space allocation for the new entry
 * fails.
 *
 * @param[in] table the table to which this new key-value mapping is being added
 * @param[in] key a tree table key used to access the specified value
 * @param[in] val a value that is being stored in the table
 *
 * @return true if the operation was successful
 */
enum cc_stat treetable_add(TreeTable *table, void *key, void *val)
{
    RBNode *y = table->sentinel;
    RBNode *x = table->root;

    int cmp;
    while (x != table->sentinel) {
        cmp = table->cmp(key, x->key);
        y   = x;

        if (cmp < 0) {
            x = x->left;
        } else if (cmp > 0) {
            x = x->right;
        } else {
            x->value = val;
            return CC_OK;
        }
    }
    RBNode *n = table->mem_alloc(sizeof(RBNode));

    if (!n)
        return CC_ERR_ALLOC;

    n->value  = val;
    n->key    = key;
    n->parent = y;
    n->left   = table->sentinel;
    n->right  = table->sentinel;

    table->size++;

    if (y == table->sentinel) {
        table->root = n;
        n->color    = RB_BLACK;
    } else {
        n->color = RB_RED;
        if (table->cmp(key, y->key) < 0) {
            y->left = n;
        } else {
            y->right = n;
        }
        rebalance_after_insert(table, n);
    }
    return CC_OK;
}

static void rebalance_after_insert(TreeTable *table, RBNode *z)
{
    RBNode *y;

    while (z->parent->color == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RB_RED) {
                z->parent->color         = RB_BLACK;
                y->color                 = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(table, z);
                }
                z->parent->color         = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_right(table, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RB_RED) {
                z->parent->color         = RB_BLACK;
                y->color                 = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(table, z);
                }
                z->parent->color         = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_left(table, z->parent->parent);
            }
        }
    }
    table->root->color = RB_BLACK;
}

static void rebalance_after_delete(TreeTable *table, RBNode *x)
{
    RBNode *w;

    while (x != table->root && x->color == RB_BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_left(table, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->right->color == RB_BLACK) {
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_right(table, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                rotate_left(table, x->parent);
                x = table->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_right(table, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RB_BLACK && w->left->color == RB_BLACK) {
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->left->color == RB_BLACK) {
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_left(table, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                rotate_right(table, x->parent);
                x = table->root;
            }
        }
    }
    x->color = RB_BLACK;
}

static INLINE void transplant(TreeTable *table, RBNode *u, RBNode *v)
{
    if (u->parent == table->sentinel)
        table->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

static INLINE RBNode *tree_min(TreeTable *table, RBNode *n)
{
    RBNode *s = table->sentinel;

    while (n->left != s)
        n = n->left;
    return n;
}

static INLINE RBNode *tree_max(TreeTable *table, RBNode *n)
{
    RBNode *s = table->sentinel;

    while (n->right != s)
        n = n->right;
    return n;
}

/**
 * Removes a node from the RB tree.
 *
 * @param[in] table the table on which this operation is performed
 * @param[in] z the node that is being removed
 */
static void remove_node(TreeTable *table, RBNode *z)
{
    RBNode *x;
    RBNode *y = z;

    int y_color = y->color;

    if (z->left == table->sentinel) {
        x = z->right;
        transplant(table, z, z->right);
    } else if (z->right == table->sentinel) {
        x = z->left;
        transplant(table, z, z->left);
    } else {
        y = tree_min(table, z->right);
        y_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(table, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(table, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_color == RB_BLACK)
        rebalance_after_delete(table, x);

    table->mem_free(z);
    table->size--;
}

/**
 * Removes a key-value mapping from the specified TreeTable and returns the
 * value that was mapped to the specified key. In case the key doesn't exist
 * NULL is returned. NULL might also be returned if the key maps to a null value.
 * Calling <code>treetable_contains_key()</code> before this functin can resolve
 * the ambiguity.
 *
 * @param[in] table the table from which the key-value pair is being removed
 * @param[in] key the key of the value being returned
 *
 * @return the value associated with the removed key, or NULL if the key doesn't
 *         exist
 */
enum cc_stat treetable_remove(TreeTable *table, void *key, void **out)
{
    RBNode *node = get_tree_node_by_key(table, key);

    if (!node)
        return CC_ERR_KEY_NOT_FOUND;

    if (out)
        *out = node->value;

    remove_node(table, node);
    return CC_OK;
}

/**
 * Removes the frist (lowest) key from the specified table and returns the
 * value taht was associated to the removed key, or NULL if the table is empty.
 *
 * @param[in] table the table from which the first entry is being removed
 *
 * @return the value associated with the removed key.
 */
enum cc_stat treetable_remove_first(TreeTable *table, void **out)
{
    if (table->size == 0)
        return CC_ERR_KEY_NOT_FOUND;

    RBNode *node = tree_min(table, table->root);

    if (out)
        *out = node->value;

    remove_node(table, node);
    return CC_OK;
}

/**
 * Removes the last (highest) key from the specified table and returns the
 * value taht was associated to the removed key, or NULL if the table is empty.
 *
 * @param[in] table the table from which the last entry is being removed
 *
 * @return the value associated with the removed key.
 */
enum cc_stat treetable_remove_last(TreeTable *table, void **out)
{
    RBNode *node = tree_max(table, table->root);

    if (!node)
        return CC_ERR_KEY_NOT_FOUND;

    if (out)
        *out = node->value;

    remove_node(table, node);
    return CC_OK;
}

/**
 * Removes all entries from the table.
 *
 * @param[in] table the table from which all entries are to be removed
 */
void treetable_remove_all(TreeTable *table)
{
    tree_destroy(table, table->root);
    table->size = 0;
    table->root = table->sentinel;
}

/**
 * Performs a right rotation on the specified table's RB tree at root <code>
 * x</code>
 *
 * @param[in] table the table on which this operation is performed
 * @param[in] x the node around which this operation is performed
 */
static void rotate_right(TreeTable *table, RBNode *x)
{
    RBNode *y = x->left;

    x->left = y->right;

    if (y->right != table->sentinel)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == table->sentinel)
        table->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right  = x;
    x->parent = y;
}

/**
 * Performs a left rotation on the specified table's RB tree at root <code>
 * x</code>
 *
 * @param[in] table the table on which this operation is performed
 * @param[in] x the node around which this operation is performed
 */
static void rotate_left(TreeTable *table, RBNode *x)
{
    RBNode *y = x->right;

    x->right = y->left;

    if (y->left != table->sentinel)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == table->sentinel)
        table->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left   = x;
    x->parent = y;
}

/**
 * Returns a tree node associated with the specified key.
 *
 * @param[in] table the table on which this opertaion is performed
 * @param[in] key the key being looked up
 *
 * @return tree node associated with the key
 */
static RBNode *get_tree_node_by_key(TreeTable *table, void *key)
{
    if (table->size == 0)
        return NULL;

    RBNode *n = table->root;
    RBNode *s = table->sentinel;

    int cmp;
    do {
        cmp = table->cmp(key, n->key);

        if (cmp < 0)
            n = n->left;
        else if (cmp > 0)
            n = n->right;
        else
            return n;
    } while (n != s && cmp != 0);

    return NULL;
}

/**
 * Returns a successor node of the node <code>x</code>
 *
 * @param[in] table the table on which this operation is performed
 * @param[in] x the node whose successor is being returned
 *
 * @return successor node of x
 */
static RBNode *get_successor_node(TreeTable *table, RBNode *x)
{
    if (x == NULL)
        return NULL;

    if (x->right != table->sentinel)
        return tree_min(table, x->right);

    RBNode *y = x->parent;

    while (y != table->sentinel && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

/**
 * Returns a predecessor node of the node <code>x</code>
 *
 * @param[in] table the table on which this operation is performed
 * @param[in] x the node whose predecessor is being returned
 *
 * @return predecessor node of x
 */
static RBNode *get_predecessor_node(TreeTable *table, RBNode *x)
{
    if (x == NULL)
        return NULL;

    if (x->left != table->sentinel)
        return tree_max(table, x->left);

    RBNode *y = x->parent;

    while (y != table->sentinel && x == y->left) {
        x = y;
        y = y->parent;
    }
    return y;
}

/**
 * A 'foreach loop' function that invokes the specified function on every key in
 * the table. The operation function should not modify the key. Any modification
 * of the key will invalidate the TreeTable.
 *
 * @param[in] table the table on which this operation is being perfomed
 * @param[in] op the operation function that is invoked on each key of the table
 */
void treetable_foreach_key(TreeTable *table, void (*op) (const void *k))
{
    RBNode *n = tree_min(table, table->root);

    while (n != table->sentinel) {
        op(n->key);
        n = get_successor_node(table, n);
    }
}

/**
 * A 'foreach loop' function that invokes the specified function on every value
 * in the table.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] op the operation function that is invoked on each value of the
 *               table
 */
void treetable_foreach_value(TreeTable *table, void (*op) (void *k))
{
    RBNode *n = tree_min(table, table->root);

    while (n != table->sentinel) {
        op(n->value);
        n = get_successor_node(table, n);
    }
}

/**
 * Initializes the TreeTableIter structure.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] table the table over whose entries the iterator is going to iterate
 */
void treetable_iter_init(TreeTableIter *iter, TreeTable *table)
{
    iter->table   = table;
    iter->current = table->sentinel;
    iter->next    = tree_min(table, table->root);
}

/**
 * Checks whether or not the iterator has a next entry iterate over.
 *
 * @return true if the next entry exists or false if the iterator has reached
 * the end of the table.
 */
bool treetable_iter_has_next(TreeTableIter *iter)
{
    return iter->next != iter->table->sentinel ? true : false;
}

/**
 * Advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[in, out] entry the next entry
 */
void treetable_iter_next(TreeTableIter *iter, TreeTableEntry *entry)
{
    if (entry) {
        entry->value  = iter->next->value;
        entry->key    = iter->next->key;
    }
    iter->current = iter->next;
    iter->next    = get_successor_node(iter->table, iter->current);
}

/**
 * Removes the last returned table entry
 *
 * @param[in] iter The iterator on which this operation is performed
 */
void treetable_iter_remove(TreeTableIter *iter)
{
    remove_node(iter->table, iter->current);
}

#ifdef DEBUG
static int treetable_test(TreeTable *table, RBNode *node, int *nb)
{
    if (node == table->sentinel) {
        *nb = 1;
        return RB_ERROR_OK;
    }
    /* check tree oreder */
    if (node->left != table->sentinel) {
        int cmp = table->cmp(node->left->key, node->key);
        if (cmp >= 0)
            return RB_ERROR_TREE_STRUCTURE;
    }
    if (node->right != table->sentinel) {
        int cmp = table->cmp(node->right->key, node->key);
        if (cmp <= 0)
            return RB_ERROR_TREE_STRUCTURE;
    }

    /* check red rule */
    if (node->color == RB_RED && node->parent->color == RB_RED) {
        return RB_ERROR_CONSECUTIVE_RED;
    }

    int nb_left;
    int nb_right;

    int left_err = treetable_test(table, node->left, &nb_left);

    /* propagate the descendant errors all the way up */
    if (left_err != RB_ERROR_OK)
        return left_err;

    int right_err = treetable_test(table, node->right, &nb_right);

    if (right_err != RB_ERROR_OK)
        return right_err;

    /* check black rule */
    if (nb_left != nb_right)
        return RB_ERROR_BLACK_HEIGHT;

    if (node->color == RB_BLACK)
        *nb = nb_left + 1;
    else
        *nb = nb_left;

    return RB_ERROR_OK;
}

int treetable_assert_rb_rules(TreeTable *table)
{
    int x;
    int status = treetable_test(table, table->root, &x);
    return status;
}
#endif
