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

#ifndef COLLECTIONS_C_CC_HASHTABLE_H
#define COLLECTIONS_C_CC_HASHTABLE_H

#include "cc_array.h"
#include "cc_common.h"

#define KEY_LENGTH_VARIABLE  -1
#define KEY_LENGTH_POINTER   sizeof(void*)

/**
 * An unordered key-value map. CC_HashTable supports best case amortized
 * constant time insertion, removal, and lookup of values. The worst
 * case complexity for these operations is amortized linear time.
 * The performance of the table depends greatly on the quality of the
 * hash function being used.
 */
typedef struct cc_hashtable_s CC_HashTable;

/**
 * A CC_HashTable table entry.
 *
 * @note modifying this structure may invalidate the table.
 */
typedef struct table_entry_s {
    /**
     * A key in the table */
    void     *key;

    /**
     * Value associated with the key */
    void     *value;

    /**
     * Hash of the key */
    size_t    hash;

    /**
     * Pointer the next entry in the list. */
    struct table_entry_s *next;
} TableEntry;

/**
 * CC_HashTable iterator object. Used to iterate over the entries of
 * the table in an undefined order. The iterator also supports operations
 * for safely removing elements during iteration.
 *
 * @note This structure should only be modified through the iterator functions.
 */
typedef struct cc_hashtable_iter {
    CC_HashTable  *table;
    size_t         bucket_index;
    TableEntry    *prev_entry;
    TableEntry    *next_entry;
} CC_HashTableIter;

/**
 * CC_HashTable configuration object. Used to initialize a new CC_HashTable
 * with specific values.
 */
typedef struct cc_hashtable_conf_s {
    /**
     * The load factor determines how the underlying
     * table array grows. For example if the load factor
     * is 0.5 and the arrays capacity is 100, the resize will
     * be triggered once the 50th entry is added. */
    float    load_factor;

    /**
     * The initial capacity of the table array. */
    size_t   initial_capacity;

    /**
     * Length of the key or -1 if the key length is
     * variable */
    int      key_length;

    /**
     * The hash seed passed to the hash function for
     * extra 'randomness'.*/
    uint32_t hash_seed;

    /**
     * Hash function used for hashing table keys */
    size_t (*hash)        (const void *key, int l, uint32_t seed);

    /**
     * The key comparator function */
    int    (*key_compare) (const void *key1, const void *key2);

    /**
     * Memory allocators used to allocate the CC_HashTable structure
     * and for all internal memory allocations. */
    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
} CC_HashTableConf;


void          cc_hashtable_conf_init       (CC_HashTableConf *conf);
enum cc_stat  cc_hashtable_new             (CC_HashTable **out);
enum cc_stat  cc_hashtable_new_conf        (CC_HashTableConf const * const conf, CC_HashTable **out);

void          cc_hashtable_destroy         (CC_HashTable *table);
enum cc_stat  cc_hashtable_add             (CC_HashTable *table, void *key, void *val);
enum cc_stat  cc_hashtable_get             (CC_HashTable *table, void *key, void **out);
enum cc_stat  cc_hashtable_remove          (CC_HashTable *table, void *key, void **out);
void          cc_hashtable_remove_all      (CC_HashTable *table);
bool          cc_hashtable_contains_key    (CC_HashTable *table, void *key);

size_t        cc_hashtable_size            (CC_HashTable *table);
size_t        cc_hashtable_capacity        (CC_HashTable *table);

enum cc_stat  cc_hashtable_get_keys        (CC_HashTable *table, CC_Array **out);
enum cc_stat  cc_hashtable_get_values      (CC_HashTable *table, CC_Array **out);

size_t        cc_hashtable_hash_string     (const void *key, int len, uint32_t seed);
size_t        cc_hashtable_hash            (const void *key, int len, uint32_t seed);
size_t        cc_hashtable_hash_ptr        (const void *key, int len, uint32_t seed);

void          cc_hashtable_foreach_key     (CC_HashTable *table, void (*op) (const void *));
void          cc_hashtable_foreach_value   (CC_HashTable *table, void (*op) (void *));

void          cc_hashtable_iter_init       (CC_HashTableIter *iter, CC_HashTable *table);
enum cc_stat  cc_hashtable_iter_next       (CC_HashTableIter *iter, TableEntry **out);
enum cc_stat  cc_hashtable_iter_remove     (CC_HashTableIter *iter, void **out);


#define CC_HASHTABLE_FOREACH(hashtable, key_53d46d2a04458e7b, value_53d46d2a04458e7b, body) \
    {                                                                   \
        CC_HashTableIter cc_hashtable_iter_53d46d2a04458e7b;            \
        cc_hashtable_iter_init(&cc_hashtable_iter_53d46d2a04458e7b, hashtable); \
        TableEntry *entry_53d46d2a04458e7b;                             \
        while (cc_hashtable_iter_next(&cc_hashtable_iter_53d46d2a04458e7b, &entry_53d46d2a04458e7b) != CC_ITER_END) \
        {                                                               \
            key_53d46d2a04458e7b = entry_53d46d2a04458e7b->key;         \
            value_53d46d2a04458e7b = entry_53d46d2a04458e7b->value;     \
            body                                                        \
                }                                                       \
    }


#define GENERAL_HASH cc_hashtable_hash
#define STRING_HASH  cc_hashtable_hash_string
#define POINTER_HASH cc_hashtable_hash_ptr


#endif /* COLLECTIONS_C_CC_HASHTABLE_H */
