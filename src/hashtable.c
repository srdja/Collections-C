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

#include "hashtable.h"
#include "dlist.h"

typedef struct table_entry_s {
    void *key;
    void *value;
    struct table_entry_s *next;
} TableEntry;

struct hashtable_s {
    int table_count;
    int capacity;
    int size;
    int theshold; // = capacity * load_factor
    int load_factor;
    TableEntry *table[];

    int (*hash) (void *key);
    bool (*key_compare) (void *k1, void *k2);
};

static void init();
static void hash(void*, int);

HashTable *hashtable_new()
{

}

void hashtable_destroy(HashTable *table)
{

}

void *hashtable_put(HashTable *table, void *key, void *val)
{

}

void *hashtable_get(HashTable *table, void *key)
{

}

void *hashtable_remove(HashTable *table, void *key)
{

}

static void init()
{

}

static void hash(void *key, int key_length)
{

}
