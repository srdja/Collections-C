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

#include "treeset.h"

struct treeset_s {
    TreeTable *t;
    int       *dummy;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

void treeset_conf_init(TreeSetConf *conf)
{
    treetable_conf_init(conf);
}

TreeSet *treeset_new(int (*cmp) (void*, void*))
{
    TreeSetConf conf;
    treeset_conf_init(&conf);
    conf.cmp = cmp;
    return treeset_new_conf(&conf);
}

TreeSet *treeset_new_conf(TreeSetConf *conf)
{
    TreeSet *set = conf->mem_calloc(1, sizeof(TreeSet));

    set->t          = treetable_new_conf(conf);
    set->dummy      = (int*) 1;
    set->mem_alloc  = conf->mem_alloc;
    set->mem_calloc = conf->mem_calloc;
    set->mem_free   = conf->mem_free;

    return set;
}

void treeset_destroy(TreeSet *set)
{
    treetable_destroy(set->t);
    set->mem_free(set);
}

void treeset_add(TreeSet *set, void *element)
{
    treetable_add(set->t, element, set->dummy);
}

void *treeset_remove(TreeSet *set, void *element)
{
    return treetable_remove(set->t, element);
}

void treeset_remove_all(TreeSet *set)
{
    treetable_remove_all(set->t);
}

void *treeset_get_first(TreeSet *set)
{
    return treetable_get_first_key(set->t);
}

void *treeset_get_last(TreeSet *set)
{
    return treetable_get_last_key(set->t);
}

void *treeset_get_greater_than(TreeSet *set, void *element)
{
    return treetable_get_greater_than(set->t, element);
}

void *treeset_get_lesser_than(TreeSet *set, void *element)
{
    return treetable_get_lesser_than(set->t, element);
}

bool treeset_contains(TreeSet *set, void *element)
{
    return treetable_contains_key(set->t, element);
}

size_t treeset_size(TreeSet *set)
{
    return treetable_size(set->t);
}

void treeset_foreach(TreeSet *set, void (*op) (const void*))
{
    treetable_foreach_key(set->t, op);
}

void treeset_iter_init(TreeSetIter *iter, TreeSet *set)
{
    treetable_iter_init(&(iter->i), set->t);
}

bool treeset_iter_has_next(TreeSetIter *iter)
{
    return treetable_iter_has_next(&(iter->i));
}

void treeset_iter_next(TreeSetIter *iter, void **element)
{
    TreeTableEntry entry;
    treetable_iter_next(&(iter->i), &entry);
    *element = entry.key;
}
void treeset_iter_remove(TreeSetIter *iter)
{
    treetable_iter_remove(&(iter->i));
}
