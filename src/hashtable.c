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
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hashtable.h"

#define DEFAULT_CAPACITY 16
#define DEFAULT_LOAD_FACTOR 0.75f

struct hashtable_s {
    size_t       capacity;
    size_t       size;
    size_t       threshold;
    uint32_t     hash_seed;
    int          key_len;
    float        load_factor;
    TableEntry **buckets;

    size_t  (*hash)       (const void *key, int l, uint32_t seed);
    int     (*key_cmp)    (const void *k1, const void *k2);
    void   *(*mem_alloc)  (size_t size);
    void   *(*mem_calloc) (size_t blocks, size_t size);
    void    (*mem_free)   (void *block);
};

static enum cc_stat resize          (HashTable *t, size_t new_capacity);
static enum cc_stat get_null_key    (HashTable *table, void **out);
static enum cc_stat add_null_key    (HashTable *table, void *val);
static enum cc_stat remove_null_key (HashTable *table, void **out);

static size_t get_table_index  (HashTable *table, void *key);
static size_t round_pow_two    (size_t n);
static void   move_entries     (TableEntry **src_bucket, TableEntry **dest_bucket,
                                 size_t src_size, size_t dest_size);

/**
 * Creates a new HashTable and returns a status code.
 *
 * @note The newly created HashTable will work with string keys.
 *
 * @param[out] out Pointer to where the newly created HashTable is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new HashTable failed.
 */
enum cc_stat hashtable_new(HashTable **out)
{
    HashTableConf htc;
    hashtable_conf_init(&htc);
    return hashtable_new_conf(&htc, out);
}

/**
 * Creates a new HashTable based on the specified HashTableConf struct and returns
 * a status code.
 *
 * The table is allocated using the memory allocators specified in the HashTableConf
 * struct.
 *
 * @param[in] conf the HashTable conf structure
 * @param[out] out Pointer to where the newly created HashTable is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new HashTable structure failed.
 */
enum cc_stat hashtable_new_conf(HashTableConf const * const conf, HashTable **out)
{
    HashTable *table = conf->mem_calloc(1, sizeof(HashTable));

    if (!table)
        return CC_ERR_ALLOC;

    table->capacity = round_pow_two(conf->initial_capacity);
    table->buckets  = conf->mem_calloc(table->capacity, sizeof(TableEntry *));

    if (!table->buckets) {
        conf->mem_free(table);
        return CC_ERR_ALLOC;
    }

    table->hash        = conf->hash;
    table->key_cmp     = conf->key_compare;
    table->load_factor = conf->load_factor;
    table->hash_seed   = conf->hash_seed;
    table->key_len     = conf->key_length;
    table->size        = 0;
    table->mem_alloc   = conf->mem_alloc;
    table->mem_calloc  = conf->mem_calloc;
    table->mem_free    = conf->mem_free;
    table->threshold   = table->capacity * table->load_factor;

    *out = table;
    return CC_OK;
}

/**
 * Initializes the HashTableConf structs fields to default values.
 *
 * @param[in] conf the struct that is being initialized
 */
void hashtable_conf_init(HashTableConf *conf)
{
    conf->hash             = STRING_HASH;
    conf->key_compare      = cc_common_cmp_str;
    conf->initial_capacity = DEFAULT_CAPACITY;
    conf->load_factor      = DEFAULT_LOAD_FACTOR;
    conf->key_length       = KEY_LENGTH_VARIABLE;
    conf->hash_seed        = 0;
    conf->mem_alloc        = malloc;
    conf->mem_calloc       = calloc;
    conf->mem_free         = free;
}

/**
 * Destroys the specified HashTable structure without destroying the data
 * contained within it. In other words, the keys and the values are not freed,
 * but only the table structure.
 *
 * @param[in] table HashTable to be destroyed
 */
void hashtable_destroy(HashTable *table)
{
    size_t i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *next = table->buckets[i];

        while (next) {
            TableEntry *tmp = next->next;
            table->mem_free(next);
            next = tmp;
        }
    }
    table->mem_free(table->buckets);
    table->mem_free(table);
}

/**
 * Creates a new key-value mapping in the specified HashTable. If the unique key
 * is already mapped to a value in this table, that value is replaced with the
 * new value. This operation may fail if the space allocation for the new entry
 * fails.
 *
 * @param[in] table the table to which this new key-value mapping is being added
 * @param[in] key a hash table key used to access the specified value
 * @param[in] val a value that is being stored in the table
 *
 * @return CC_OK if the mapping was successfully added, or CC_ERR_ALLOC if the
 * memory allocation failed.
 */
enum cc_stat hashtable_add(HashTable *table, void *key, void *val)
{
    enum cc_stat stat;
    if (table->size >= table->threshold) {
        if ((stat = resize(table, table->capacity << 1)) != CC_OK)
            return stat;
    }

    if (!key)
        return add_null_key(table, val);

    const size_t hash = table->hash(key, table->key_len, table->hash_seed);
    const size_t i    = hash & (table->capacity - 1);

    TableEntry *replace = table->buckets[i];

    while (replace) {
        void *rk = replace->key;
        if (rk && table->key_cmp(rk, key) == 0) {
            replace->value = val;
            return CC_OK;
        }
        replace = replace->next;
    }

    TableEntry *new_entry = table->mem_alloc(sizeof(TableEntry));

    if (!new_entry)
        return CC_ERR_ALLOC;

    new_entry->key   = key;
    new_entry->value = val;
    new_entry->hash  = hash;
    new_entry->next  = table->buckets[i];

    table->buckets[i] = new_entry;
    table->size++;

    return CC_OK;
}

/**
 * Creates a new key-value mapping for the NULL key. This operation may fail if
 * the space allocation for the new entry fails.
 *
 * @param[in] table the table into which this key value-mapping is being added
 * @param[in] val the value that is being mapped to the NULL key
 *
 * @return CC_OK if the mapping was successfully added, or CC_ERR_ALLOC if the
 * memory allocation failed.
 */
static enum cc_stat add_null_key(HashTable *table, void *val)
{
    TableEntry *replace = table->buckets[0];

    while (replace) {
        if (!replace->key) {
            replace->value = val;
            return CC_OK;
        }
        replace = replace->next;
    }

    TableEntry *new_entry = table->mem_alloc(sizeof(TableEntry));

    if (!new_entry)
        return CC_ERR_ALLOC;

    new_entry->key   = NULL;
    new_entry->value = val;
    new_entry->hash  = 0;
    new_entry->next  = table->buckets[0];

    table->buckets[0] = new_entry;
    table->size++;

    return CC_OK;
}

/**
 * Gets a value associated with the specified key and sets the out
 * parameter to it.
 *
 * @param[in] table the table from which the mapping is being returned
 * @param[in] key   the key that is being looked up
 * @param[out] out  pointer to where the value is stored
 *
 * @return CC_OK if the key was found, or CC_ERR_KEY_NOT_FOUND if not.
 */
enum cc_stat hashtable_get(HashTable *table, void *key, void **out)
{
    if (!key)
        return get_null_key(table, out);

    size_t      index  = get_table_index(table, key);
    TableEntry *bucket = table->buckets[index];

    while (bucket) {
        if (bucket->key && table->key_cmp(bucket->key, key) == 0) {
            *out = bucket->value;
            return CC_OK;
        }
        bucket = bucket->next;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Returns a value associated with the NULL key and sets the out parameter
 * to it.
 *
 * @param[in] table the table from which the value mapped to this key is being
 *                  returned
 * @param[out] pointer to where the value is stored
 *
 * @return CC_OK if a NULL key was found, or CC_ERR_KEY_NOT_FOUND if not.
 */
static enum cc_stat get_null_key(HashTable *table, void **out)
{
    TableEntry *bucket = table->buckets[0];

    while (bucket) {
        if (bucket->key == NULL) {
            *out = bucket->value;
            return CC_OK;
        }
        bucket = bucket->next;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Removes a key-value mapping from the specified hash table and sets the out
 * parameter to value.
 *
 * @param[in] table the table from which the key-value pair is being removed
 * @param[in] key the key of the value being returned
 * @param[out] out pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the mapping was successfully removed, or CC_ERR_KEY_NOT_FOUND
 * if the key was not found.
 */
enum cc_stat hashtable_remove(HashTable *table, void *key, void **out)
{
    if (!key)
        return remove_null_key(table, out);

    const size_t i = get_table_index(table, key);

    TableEntry *e    = table->buckets[i];
    TableEntry *prev = NULL;
    TableEntry *next = NULL;

    while (e) {
        next = e->next;

        if (e->key && table->key_cmp(key, e->key) == 0) {
            void *value = e->value;

            if (!prev)
                table->buckets[i] = next;
            else
                prev->next = next;

            table->mem_free(e);
            table->size--;
            if (out)
                *out = value;
            return CC_OK;
        }
        prev = e;
        e = next;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Removes a NULL key mapping from the specified hash table and sets the out
 * parameter to value.
 *
 * @param[in] table the table from which the NULL key mapping is being removed
 * @param[out] out pointer to where the removed value is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the mapping was successfully removed, or CC_ERR_KEY_NOT_FOUND
 * if the key was not found.
 */
enum cc_stat remove_null_key(HashTable *table, void **out)
{
    TableEntry *e = table->buckets[0];

    TableEntry *prev = NULL;
    TableEntry *next = NULL;

    while (e) {
        next = e->next;

        if (e->key == NULL) {
            void *value = e->value;

            if (!prev)
                table->buckets[0] = next;
            else
                prev->next = next;

            table->mem_free(e);
            table->size--;
            if (out)
                *out = value;
            return CC_OK;
        }
        prev = e;
        e = next;
    }
    return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Removes all key-value mappings from the specified table.
 *
 * @param[in] table the table from which all mappings are being removed
 */
void hashtable_remove_all(HashTable *table)
{
    size_t i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *entry = table->buckets[i];
        while (entry) {
            TableEntry *next = entry->next;
            table->mem_free(entry);
            table->size--;
            entry = next;
        }
        table->buckets[i] = NULL;
    }
}

/**
 * Resizes the table to match the provided capacity. The new capacity must be a
 * power of two.
 *
 * @param[in] table the table that is being resized
 * @param[in] new_capacity the new capacity to which the table should be resized
 *
 * @return CC_OK if the resize was successful, or CC_ERR_MAX_CAPACITY if maximum
 * capacity has been reached, or CC_ERR_ALLOC if the memory allocation for the
 * new buffer failed.
 */
static enum cc_stat resize(HashTable *t, size_t new_capacity)
{
    if (t->capacity == MAX_POW_TWO)
        return CC_ERR_MAX_CAPACITY;

    TableEntry **new_buckets = t->mem_calloc(new_capacity, sizeof(TableEntry *));

    if (!new_buckets)
        return CC_ERR_ALLOC;

    TableEntry **old_buckets = t->buckets;

    move_entries(old_buckets, new_buckets, t->capacity, new_capacity);

    t->buckets   = new_buckets;
    t->capacity  = new_capacity;
    t->threshold = t->load_factor * new_capacity;

    t->mem_free(old_buckets);

    return CC_OK;
}

/**
 * Rounds the integer to the nearest upper power of two.
 *
 * @param[in] the unsigned integer that is being rounded
 *
 * @return the nearest upper power of two.
 */
static INLINE size_t round_pow_two(size_t n)
{
    if (n >= MAX_POW_TWO)
        return MAX_POW_TWO;

    if (n == 0)
        return 2;
    /**
     * taken from:
     * http://graphics.stanford.edu/~seander/
     * bithacks.html#RoundUpPowerOf2Float
     */
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;

    return n;
}

/**
 * Moves all entries from one bucket array to another.
 *
 * @param[in] src_bucket  the source bucket from which the entries are moved
 * @param[in] dest_bucket the destination bucket to which the entries are being
 *                        moved
 * @param[in] src_size    size of the source bucket
 * @param[in] dest_size   size of the destination bucket
 */
static INLINE void
move_entries(TableEntry **src_bucket, TableEntry **dest_bucket,
             size_t       src_size,   size_t       dest_size)
{
    size_t i;
    for (i = 0; i < src_size; i++) {
        TableEntry *entry = src_bucket[i];

        while (entry) {
            TableEntry *next  = entry->next;
            size_t      index = entry->hash & (dest_size - 1);

            entry->next = dest_bucket[index];
            dest_bucket[index] = entry;

            entry = next;
        }
    }
}

/**
 * Returns the size of the specified HashTable. Size of a HashTable represents
 * the number of key-value mappings within the table.
 *
 * @param[in] table the table whose size is being returned
 *
 * @return the size of the table.
 */
size_t hashtable_size(HashTable *table)
{
    return table->size;
}

/**
 * Returns the current capacity of the table. The capacity is the number of
 * buckets or the number of random access for table entries.
 *
 * @param[in] table the table whose current capacity is being returned
 *
 * @return the current capacity of the specified table.
 */
size_t hashtable_capacity(HashTable *table)
{
    return table->capacity;
}

/**
 * Checks whether or not the HashTable contains the specified key.
 *
 * @param[in] table the table on which the search is being performed
 * @param[in] key the key that is being searched for
 *
 * @return true if the table contains the key.
 */
bool hashtable_contains_key(HashTable *table, void *key)
{
    TableEntry *entry = table->buckets[get_table_index(table, key)];

    while (entry) {
        if (table->key_cmp(key, entry->key) == 0)
            return true;

        entry = entry->next;
    }
    return false;
}

/**
 * Returns an Array of hashtable values. The returned Array is allocated
 * using the same memory allocators used by the HashTable.
 *
 * @param[in] table the table whose values are being returned
 * @param[out] out pointer to where the array is stored
 *
 * @return CC_OK if the Array was successfully created, or CC_ERR_ALLOC
 * if the memory allocation for the Array failed.
 */
enum cc_stat hashtable_get_values(HashTable *table, Array **out)
{
    ArrayConf ac;
    array_conf_init(&ac);

    ac.capacity   = table->size;
    ac.mem_alloc  = table->mem_alloc;
    ac.mem_calloc = table->mem_calloc;
    ac.mem_free   = table->mem_free;

    Array *values;
    enum cc_stat stat = array_new_conf(&ac, &values);
    if (stat != CC_OK)
        return stat;

    size_t i;
    for (i = 0; i <table->capacity; i++) {
        TableEntry *entry = table->buckets[i];

        while (entry) {
            if ((stat = array_add(values, entry->value)) == CC_OK) {
                entry = entry->next;
            } else {
                array_destroy(values);
                return stat;
            }
        }
    }
    *out = values;
    return CC_OK;
}

/**
 * Returns an Array of hashtable keys. The returned Array is allocated
 * using the same memory allocators used by the HashTable.
 *
 * @param[in] table the table whose keys are being returned
 * @param[out] out pointer to where the array is stored
 *
 * @return CC_OK if the Array was successfully created, or CC_ERR_ALLOC
 * if the memory allocation for the Array failed.
 */
enum cc_stat hashtable_get_keys(HashTable *table, Array **out)
{
    ArrayConf vc;
    array_conf_init(&vc);

    vc.capacity   = table->size;
    vc.mem_alloc  = table->mem_alloc;
    vc.mem_calloc = table->mem_calloc;
    vc.mem_free   = table->mem_free;

    Array *keys;
    enum cc_stat stat = array_new_conf(&vc, &keys);
    if (stat != CC_OK)
        return stat;

    size_t i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *entry = table->buckets[i];

        while (entry) {
            if ((stat = array_add(keys, entry->key)) == CC_OK) {
                entry = entry->next;
            } else {
                array_destroy(keys);
                return stat;
            }
        }
    }
    *out = keys;
    return CC_OK;
}

/**
 * Returns the bucket index that maps to the specified key.
 */
static INLINE size_t get_table_index(HashTable *table, void *key)
{
    size_t hash = table->hash(key, table->key_len, table->hash_seed);
    return hash & (table->capacity - 1);
}

/**
 * Applies the function fn to each key of the HashTable.
 *
 * @note The operation function should not modify the key. Any modification
 * of the key will invalidate the HashTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each key of the table
 */
void hashtable_foreach_key(HashTable *table, void (*fn) (const void *key))
{
    size_t i;
    for (i = 0; i <table->capacity; i++) {
        TableEntry *entry = table->buckets[i];

        while (entry) {
            fn(entry->key);
            entry = entry->next;
        }
    }
}

/**
 * Applies the function fn to each value of the HashTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each value of the
 *               table
 */
void hashtable_foreach_value(HashTable *table, void (*fn) (void *val))
{
    size_t i;
    for (i = 0; i <table->capacity; i++) {
        TableEntry *entry = table->buckets[i];

        while (entry) {
            fn(entry->value);
            entry = entry->next;
        }
    }
}

/**
 * Initializes the HashTableIter structure.
 *
 * @note The order at which the entries are returned is unspecified.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] table the table over whose entries the iterator is going to iterate
 */
void hashtable_iter_init(HashTableIter *iter, HashTable *table)
{
    iter->table = table;

    size_t i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *e = table->buckets[i];
        if (e) {
            iter->bucket_index = i;
            iter->next_entry   = e;
            iter->prev_entry   = NULL;
            break;
        }
    }
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next TableEntry.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out pointer to where the next entry is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the HashTable has been reached.
 */
enum cc_stat hashtable_iter_next(HashTableIter *iter, TableEntry **te)
{
    if (!iter->next_entry)
        return CC_ITER_END;

    iter->prev_entry = iter->next_entry;
    iter->next_entry = iter->next_entry->next;

    /* Iterate through the list */
    if (iter->next_entry) {
        *te = iter->prev_entry;
        return CC_OK;
    }

    /* Find the next list and return the first element*/
    size_t i;
    for (i = iter->bucket_index + 1; i < iter->table->capacity; i++) {
        iter->next_entry = iter->table->buckets[i];

        if (iter->next_entry) {
            iter->bucket_index = i;
            break;
        }
    }
    *te = iter->prev_entry;

    return CC_OK;
}

/**
 * Removes the last returned entry by <code>hashtable_iter_next()</code>
 * function without invalidating the iterator and optionally sets the
 * out parameter to the value of the removed entry.
 *
 * @note This Function should only ever be called after a call to <code>
 * hashtable_iter_next()</code>.
 *
 * @param[in] iter The iterator on which this operation is performed
 * @param[out] out Pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the entry was successfully removed, or
 * CC_ERR_KEY_NOT_FOUND.
 */
enum cc_stat hashtable_iter_remove(HashTableIter *iter, void **out)
{
    return hashtable_remove(iter->table, iter->prev_entry->key, out);
}


/*******************************************************************************
 *
 *
 *  djb2 string hash
 *
 *
 ******************************************************************************/

size_t hashtable_hash_string(const void *key, int len, uint32_t seed)
{
    const    char   *str  = key;
    register size_t  hash = seed + 5381 + len + 1; /* Suppress the unused param warning */

    while (*str++)
        hash = ((hash << 5) + hash) ^ *str;

    return hash;
}

/*******************************************************************************
 *
 *
 *  MurmurHash3 by Austin Appleby, adapted for hashtable use.
 *
 *
 ******************************************************************************/

#ifdef _MSC_VER

#define ROTL32(x,y) _rotl(x,y)
#define ROTL64(x,y) _rotl64(x,y)
#define BIG_CONSTANT(x) (x)

#else

FORCE_INLINE uint32_t rotl32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

FORCE_INLINE uint64_t rotl64(uint64_t x, int8_t r)
{
    return (x << r) | (x >> (64 - r));
}

#define ROTL32(x,y) rotl32(x,y)
#define ROTL64(x,y) rotl64(x,y)
#define BIG_CONSTANT(x) (x##LLU)

#endif


/*****************************************************************************
 *
 *                            -- 64bit --
 *
 ****************************************************************************/
#ifdef ARCH_64


FORCE_INLINE uint64_t fmix64(uint64_t k)
{
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

uint64_t hashtable_hash(const void *key, int len, uint32_t seed)
{
    const uint8_t  *data    = (const uint8_t*) key;
    const int       nblocks = len / 16;

    uint64_t        h1      = seed;
    uint64_t        h2      = seed;

    const uint64_t  c1      = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t  c2      = BIG_CONSTANT(0x4cf5ad432745937f);

    const uint64_t *blocks  = (const uint64_t*)(data);

    int i;
    for(i = 0; i < nblocks; i++) {
        uint64_t k1 = blocks[i*2+0];
        uint64_t k2 = blocks[i*2+1];

        k1 *= c1;
        k1  = ROTL64(k1,31);
        k1 *= c2;
        h1 ^= k1;
        h1  = ROTL64(h1,27);
        h1 += h2;
        h1  = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2  = ROTL64(k2,33);
        k2 *= c1;
        h2 ^= k2;
        h2  = ROTL64(h2,31);
        h2 += h1;
        h2  = h2 * 5 + 0x38495ab5;
    }

    const uint8_t *tail = (const uint8_t*)(data + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch(len & 15) {
    case 15: k2 ^= ((uint64_t)tail[14]) << 48;
    case 14: k2 ^= ((uint64_t)tail[13]) << 40;
    case 13: k2 ^= ((uint64_t)tail[12]) << 32;
    case 12: k2 ^= ((uint64_t)tail[11]) << 24;
    case 11: k2 ^= ((uint64_t)tail[10]) << 16;
    case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
    case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
             k2 *= c2;
             k2  = ROTL64(k2,33);
             k2 *= c1;
             h2 ^= k2;

    case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
    case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
    case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
    case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
    case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
    case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
    case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
    case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
             k1 *= c1;
             k1  = ROTL64(k1,31);
             k1 *= c2;
             h1 ^= k1;
    };

    h1 ^= len; h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    return h1;
}

/*
 * MurmurHash3 the 64bit variant that hashes the pointer itself
 */
uint64_t hashtable_hash_ptr(const void *key, int len, uint32_t seed)
{
    const int nblocks = len / 4;

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    int i;
    for (i = 0; i < nblocks; i++) {
        uint64_t k1 = ((uintptr_t) key >> (2 * i)) & 0xff;
        uint64_t k2 = ROTL64(k1, 13);

        k1 *= c1;
        k1  = ROTL64(k1,31);
        k1 *= c2;
        h1 ^= k1;
        h1  = ROTL64(h1,27);
        h1 += h2;
        h1  = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2  = ROTL64(k2,33);
        k2 *= c1;
        h2 ^= k2;
        h2  = ROTL64(h2,31);
        h2 += h1;
        h2  = h2 * 5 + 0x38495ab5;
    }

    /* Since the pointers are power of two length
     * we don't need a tail mix */

    h1 ^= len; h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    return h1;
}


/*****************************************************************************
 *
 *                            -- 32bit --
 *
 ****************************************************************************/
#else

FORCE_INLINE uint32_t fmix32(uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

/**
 * MurmurHash3 the 32bit variant.
 */
size_t hashtable_hash(const void *key, int len, uint32_t seed)
{
    const uint8_t *data    = (const uint8_t*)key;
    const int      nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const uint32_t *blocks = (const uint32_t *)(data + nblocks*4);

    int i;
    for (i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1;
            k1  = ROTL32(k1,15);
            k1 *= c2;
            h1 ^= k1;
    };

    h1 ^= len;
    h1  = fmix32(h1);

    return (size_t) h1;
}

/*
 * MurmurHash3 the 32bit variant that hashes the pointer itself
 */
size_t hashtable_hash_ptr(const void *key, int len, uint32_t seed)
{
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    int i;
    for (i = 0; i < nblocks; i++) {
        uint32_t k1 = ((uintptr_t) key >> (2*i)) & 0xff;

        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    /* Since the pointers are power of two length
     * we don't need a tail mix */

    h1 ^= len;
    h1  = fmix32(h1);

    return (size_t) h1;
}

#endif /* ARCH_64 */
