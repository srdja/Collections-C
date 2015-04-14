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
static INLINE void *tree_min       (TreeTable *table, RBNode *n);
static INLINE void *tree_max       (TreeTable *table, RBNode *n);

static RBNode *get_tree_node_by_key(TreeTable *table, void *key);
static RBNode *get_successor_node  (TreeTable *table, RBNode *n);

void treetable_conf_init(TreeTableConf *conf)
{
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}

TreeTable *treetable_new(int (*cmp) (void *, void *))
{
    TreeTableConf conf;
    treetable_conf_init(&conf);
    conf.cmp = cmp;
    return treetable_new_conf(&conf);
}

TreeTable *treetable_new_conf(TreeTableConf *conf)
{
    TreeTable *table  = conf->mem_calloc(1, sizeof(TreeTable));
    RBNode *sentinel  = conf->mem_calloc(1, sizeof(RBNode));

    if (!table || !sentinel)
        return NULL;

    sentinel->color   = RB_BLACK;

    table->size       = 0;
    table->cmp        = conf->cmp;
    table->mem_alloc  = conf->mem_alloc;
    table->mem_calloc = conf->mem_calloc;
    table->mem_free   = conf->mem_free;
    table->root       = sentinel;
    table->sentinel   = sentinel;

    return table;
}

static void tree_destroy(TreeTable *table, RBNode *n)
{
    if (n == table->sentinel)
        return;

    tree_destroy(table, n->left);
    tree_destroy(table, n->right);

    table->mem_free(n);
}

void treetable_destroy(TreeTable *table)
{
    tree_destroy(table, table->root);

    table->mem_free(table->sentinel);
    table->mem_free(table);
}

void *treetable_get(TreeTable *table, void *key)
{
    RBNode *node = get_tree_node_by_key(table, key);

    if (!node)
        return NULL;

    return node->value;
}

void *treetable_get_first(TreeTable *table)
{
    RBNode *node = tree_min(table, table->root);

    if (node != table->sentinel)
        return node->value;

    return NULL;
}

void *treetable_get_last(TreeTable *table)
{
    RBNode *node = tree_max(table, table->root);

    if (node != table->sentinel)
        return node->value;

    return NULL;
}

void *treetable_get_greater_than(TreeTable *table, void *key)
{
    RBNode *n = get_tree_node_by_key(table, key);
    RBNode *s = get_successor_node(table, n);

    if (n && s)
        return s->key;

    return NULL;
}

void *treetable_get_lesser_than(TreeTable *table, void *key)
{
    RBNode *n = get_tree_node_by_key(table, key);
    RBNode *s = get_predecessor_node(table, n);

    if (n && s)
        return s->key;

    return NULL;
}

size_t treetable_size(TreeTable *table)
{
    return table->size;
}

bool treetable_contains_key(TreeTable *table, void *key)
{
    RBNode *node = get_tree_node_by_key(table, key); // keys can't be null

    if (node)
        return true;
    return false;
}

bool treetable_add(TreeTable *table, void *key, void *val)
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
            return true;
        }
    }
    RBNode *n = table->mem_alloc(sizeof(RBNode));

    if (!n)
        return false;

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
    return true;
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

static INLINE void *tree_min(TreeTable *table, RBNode *n)
{
    RBNode *s = table->sentinel;

    while (n->left != s)
        n = n->left;
    return n;
}

static INLINE void *tree_max(TreeTable *table, RBNode *n)
{
    RBNode *s = table->sentinel;

    while (n->right != s)
        n = n->right;
    return n;
}

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

void *treetable_remove(TreeTable *table, void *key)
{
    RBNode *node = get_tree_node_by_key(table, key);

    if (!node)
        return NULL;

    void *val = node->value;
    remove_node(table, node);

    return val;
}

void *treetable_remove_first(TreeTable *table)
{
    RBNode *node = tree_min(table, table->root);
    void   *val  = node->value;

    remove_node(table, node);
    return val;
}

void *treetable_remove_last(TreeTable *table)
{
    RBNode *node = tree_max(table, table->root);
    void   *val  = node->value;

    remove_node(table, node);
    return val;
}

void treetable_remove_all(TreeTable *table)
{
    tree_destroy(table, table->root);
    table->size = 0;
    table->root = table->sentinel;
}

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

static RBNode *get_successor_node(TreeTable *table, RBNode *x)
{
    if (x == NULL)
        return NULL;

    if (x->right != table->sentinel)
        return treetable_min(table, x->right);

    RBNode *y = x->parent;

    while (y != table->sentinel && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

static RBNode *get_predecessor_node(TreeTable *table, RBNode *x)
{
    if (x == NULL)
        return NULL;

    if (x->left != table->sentinel)
        return treetable_max(table, x->left);

    RBNode *y = x->parent;

    while (y != table->sentinel && x == y->left) {
        x = y;
        y = y->parent;
    }
    return y;
}

void treetable_foreach_key(TreeTable *table, void (*op) (void *k))
{
    RBNode *n = treetable_min(table->root);

    while (n != table->sentinel) {
        op(n->key);
        n = get_successor_node(table, n);
    }
}

void treetable_foreach_value(TreeTable *table, void (*op) (void *k))
{
    RBNode *n = treetable_min(table->root);

    while (n != table->sentinel) {
        op(n->value);
        n = get_successor_node(table, n);
    }
}

void treetable_iter_init(TreeTableIter *iter, TreeTable *table)
{
    iter->table   = table;
    iter->current = table->sentinel;
    iter->next    = treetable_min(table->root);
}

bool treetable_iter_has_next(TreeTableIter *iter)
{
    return iter->next != iter->table->sentinel ? true : false;
}

void treetable_iter_next(TreeTableIter *iter, TreeTableEntry *entry)
{
    entry->value  = iter->next->value;
    entry->key    = iter->next->key;
    iter->current = iter->next;
    iter->next    = get_successor_node(iter->table, iter->current);
}

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
