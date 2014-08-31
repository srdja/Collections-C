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
#include <string.h>

#define DEFAULT_CAPACITY 16
#define DEFAULT_LOAD_FACTOR 0.75f

#define MAX_BUCKETS 0xFFFFFFFF
#define MAX_POW_TWO 0x80000000

typedef struct table_entry_s {
    void *key;
    void *value;
    uint32_t hash;
    struct table_entry_s *next;
} TableEntry;

struct hashtable_s {
    uint32_t capacity;
    uint32_t size;
    uint32_t inflation_threshold;
    uint32_t hash_seed;
    int      key_len;
    float    load_factor;

    TableEntry **buckets;

    uint32_t (*hash) (const void *key, int l, uint32_t seed);
    bool (*key_cmp) (void *k1, void *k2);
};

struct hashtable_key_iter {
    HashTable *table;
    int bucket_index;
    TableEntry *prev_entry;
    TableEntry *next_entry;
};

static uint32_t get_table_index(HashTable *table, void *key);
static void resize(HashTable *t, uint32_t new_capacity);
static uint32_t round_pow_two(uint32_t n);

static void move_entries(TableEntry **src_bucket, TableEntry **dest_bucket,
        uint32_t src_size, uint32_t dest_size);

/**
 * Returns a new HashTableProperties object.
 *
 * @return a new HashTableProperties object
 */
HashTableProperties *hashtable_properties_new()
{
    HashTableProperties *properties = calloc(1, sizeof(HashTableProperties));

    properties->hash             = hashtable_hash_string;
    properties->key_compare      = hashtable_string_key_cmp;
    properties->initial_capacity = DEFAULT_CAPACITY;
    properties->load_factor      = DEFAULT_LOAD_FACTOR;
    properties->key_length       = KEY_LENGTH_VARIABLE;
    properties->hash_seed        = 0;

    return properties;
}

/**
 * Destroys a HashTableProperties object.
 *
 * @param[in] properties HashTableProperties object to be destroyed.
 */
void hashtable_properties_destroy(HashTableProperties *properties)
{
    free(properties);
}

/**
 * Creates a new HashTable based on the provided HashTable properties.
 *
 * @param properties a the HashTableProperties object used to configure this new
 *                     HashTable
 * @return a new HashTable instance
 */
HashTable *hashtable_new(HashTableProperties *properties)
{
    HashTable *table = calloc(1, sizeof(HashTable));

    table->hash        = properties->hash;
    table->key_cmp     = properties->key_compare;
    table->load_factor = properties->load_factor;
    table->capacity    = round_pow_two(properties->initial_capacity);
    table->hash_seed   = properties->hash_seed;
    table->key_len     = properties->key_length;
    table->size        = 0;

    table->buckets = calloc(table->capacity, sizeof(TableEntry));
    table->inflation_threshold = table->capacity * table->load_factor;

    return table;
}

/**
 * Destroys the specified HashTable structure without destroying the the data
 * contained within it.
 *
 * @param[in] table HashTable to be destroyed.
 */
void hashtable_destroy(HashTable *table)
{
    int i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *next = table->buckets[i];

        while (next) {
            TableEntry *tmp = next->next;
            free(next);
            next = tmp;
        }
    }
    free(table->buckets);
    free(table);
}

/**
 * Creates a new key-value mapping in the specified HashTable. Each key in the
 * table is unique and can only map to single value, therefore if the specified
 * key already exists in the table, the old value that the key maps to will be
 * replaced with the new value.
 *
 * @param[in] table the table to which this new key-value mapping is being
 *                  added to
 * @param[in] key a hash table key used to access the specified value
 * @param[in] val a value that is being stored in the table
 */
void hashtable_put(HashTable *table, void *key, void *val)
{
    if (table->size >= table->inflation_threshold)
        resize(table, table->capacity << 1);

    uint32_t hash = table->hash(key, table->key_len, table->hash_seed);
    uint32_t i    = hash & (table->capacity - 1);

    TableEntry *bucket = table->buckets[i];

    table->size++;
    while (bucket) {
        if (table->key_cmp(bucket->key, key)) {
            bucket->value = val;
            return;
        }
        bucket = bucket->next;
    }

    TableEntry *new_entry = calloc(1, sizeof(TableEntry));
    new_entry->key = key;
    new_entry->value = val;
    new_entry->hash = hash;
    new_entry->next = table->buckets[i];
    table->buckets[i] = new_entry;
}

/**
 * Returns a value associated with the specified key. If there is no value 
 * associated with this key NULL is returned. This function might also 
 * return null if the value mapped to this key is null.
 *
 * @param[in] table the table from which the value mapped to the specified key
 *                  is being returned
 * @param[in] key   the key into the
 *
 * @return The value mapped to the specified key or null if the mapping doesn't
 *         exit
 */
void *hashtable_get(HashTable *table, void *key)
{
    uint32_t    index  = get_table_index(table, key);
    TableEntry *bucket = table->buckets[index];

    while (bucket) {
        if (table->key_cmp(bucket->key, key))
            return bucket->value;
        bucket = bucket->next;
    }
    return NULL;
}

/**
 * Removes a key-value mapping from the specified hash table and returns the
 * value that was mapped to the specified key. In case the key doesn't exit
 * NULL is returned. NULL might also be returned if the key maps to a null value.
 * Calling hashtable_contains_key(), to check whether the key exists, before
 * this function can resolve the ambiguity.
 *
 * @param[in] table the table from which the key-value pair is being removed
 * @param[in] key the key of the value being returned
 *
 * @return the value associated with the removed key
 */
void *hashtable_remove(HashTable *t, void *k)
{
    uint32_t index = get_table_index(t, k);

    TableEntry *entry = t->buckets[index];
    TableEntry *prev  = NULL;
    TableEntry *next;

    while (entry) {
        next = entry->next;

        if (t->key_cmp(k, entry->key)) {
            void *value = entry->value;

            if (!prev)
                t->buckets[index] = next;
            else
                prev->next = next;

            free(entry);
            t->size--;
            return value;
        }
        prev  = entry;
        entry = next;
    }
    return NULL;
}

/**
 * Removes all key-value mappings from the specified table.
 *
 * @param[in] table the table from which all mappings are being removed
 */
void hashtable_remove_all(HashTable *table)
{
    int i;
    for (i = 0; i < table->capacity; i++) {
        TableEntry *entry = table[i];
        while (entry) {
            TableEntry *next = entry->next;
            free(entry);
            table->size--;
            entry = next;
        }
        table[i] = NULL;
    }
}

/**
 * Resizes the table to match the provided capacity. The new capacity must be a
 * power of two ranging from 2^0 to 2^31.
 *
 * @param[in] table the table that is being resized.
 * @param[in] new_capacity the new capacity to which the table should be resized
 */
static void resize(HashTable *t, uint32_t new_capacity)
{
    if (t->capacity == MAX_BUCKETS)
        return;

    if (t->capacity >= MAX_POW_TWO)
        new_capacity = MAX_BUCKETS;

    TableEntry **new_buckets = calloc(new_capacity, sizeof(TableEntry));
    TableEntry **old_buckets = t->buckets;

    move_entries(old_buckets, new_buckets, t->capacity, new_capacity);

    t->buckets = new_buckets;
    t->capacity = new_capacity;
    t->inflation_threshold = t->load_factor * new_capacity;

    free(old_buckets);
}

/**
 * Rounds the integer to the nearest upper power of two.
 *
 * @param[in] the unsigned integer that is being rounded
 * @return the nearest upper power of two
 */
static uint32_t round_pow_two(uint32_t n)
{
    if (n >= MAX_POW_TWO)
        return MAX_BUCKETS;

    /**
     * taken from:
     * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2Float
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
static void move_entries(TableEntry **src_bucket, TableEntry **dest_bucket,
        uint32_t src_size, uint32_t dest_size)
{
    int i;
    for (i = 0; i < src_size; i++) {
        TableEntry *entry = src_bucket[i];

        while (entry) {
            TableEntry *next  = entry->next;
            uint32_t    index = entry->hash & (dest_size - 1);

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
 * @return the size of the table
 */
int hashtable_size(HashTable *table)
{
    return table->size;
}

/**
 * Returns the current capacity of the table. The capacity is is the number of
 * buckets or the number of random access for table entries.
 *
 * @param[in] table the table whos current capacity is being returned
 *
 * @return the current capacity of the specified table
 */
int hashtable_capacity(HashTable *table)
{
    return table->capacity;
}

/**
 * Checks whether or not the HashTable contains the specified key.
 *
 * @param[in] table the table on which the search is being performed
 * @param[in] key the key that is being searched for
 * @return true if the table contains the key.
 */
bool hastable_contains_key(HashTable *table, void *key)
{
    TableEntry *entry = table->buckets[get_table_index(table, key)];

    while (entry) {
        if (table->key_cmp(key, entry->key))
            return true;

        entry = entry->next;
    }
    return false;
}

/**
 * Returns a list of hashtable values.
 *
 * @param[in] table the table whos values are being returned
 *
 * @return a list of values
 */
List *hashtable_get_values(HashTable *table)
{
    List *values = list_new();

    int i;
    for (i = 0; i < table->capacity; i++) {
        if (!table->buckets[i])
            continue;

        TableEntry entry = table->buckets[i];

        while (entry) {
            list_add(values, entry->value);
            entry = entry->next;
        }
    }
    return values;
}

/**
 * Returns a list of hashtable keys.
 *
 * @param[in] table the table whos keys are being returned
 *
 * @return a list of keys
 */
List *hashtable_get_keys(HashTable *table)
{
    List *keys = list_new();

    int i;
    for (i = 0; i < table->capacity; i++) {
        if (!table->buckets[i])
            continue;

        TableEntry entry = table->buckets[i];

        while (entry) {
            list_add(keys, entry->key);
            entry = entry->next;
        }
    }
    return keys;
}

/**
 * Returns the bucket index that maps to the specified key.
 */
static uint32_t get_table_index(HashTable *table, void *key)
{
    uint32_t hash  = table->hash(key, table->key_len, table->hash_seed);
    return hash & (table->capacity - 1);
}

/**
 * String key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical and false if otherwise
 */
bool hashtable_string_key_cmp(void *key1, void *key2)
{
    return strcmp((char *) key1, (char *) key2) == 0;
}

/**
 * Double key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_double_key_cmp(void *key1, void *key2)
{
    return *(double*) key1 == *(double*) key2;
}

/**
 * Double key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_float_key_cmp(void *key1, void *key2)
{
    return *(float*) key1 == *(float*) key2;
}

/**
 * Char key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_char_key_cmp(void *key1, void *key2)
{
    return *(char*) key1 == *(char*) key2;
}

/**
 * Short key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_short_key_cmp(void *key1, void *key2)
{
    return *(short*) key1 == *(short*) key2;
}

/**
 * Int key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_int_key_cmp(void *key1, void *key2)
{
    return *(int*) key1 == *(int*) key2;
}

/**
 * Int key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_long_key_cmp(void *key1, void *key2)
{
    return *(long*) key1 == *(long*) key2;
}

/**
 * Pointer key comparator function.
 *
 * @param[in] key1 first key
 * @param[in] key2 second key
 *
 * @return true if the keys are identical
 */
bool hashtable_pointer_key_compare(void *key1, void *key2)
{
    return key1 == key2;
}

/**
 * Creats a new HashTable iterator that iterates over hashtable entries.
 *
 * @note The order at which the entries are returned is unspecified.
 *
 * @return a new HashTableIter object
 */
HashTableIter *hastable_iter_new(HashTable *table)
{
    HashTableIter *iter = calloc(1, sizeof(HashTableIter));
    iter->table = table;

    int i;
    for (i = 0; i < table->buckets; i++) {
        TableEntry *e = table->buckets[i];
        if (e) {
            iter->bucket_index = i;
            iter->next_entry = e;
            break;
        }
    }
    return iter;
}

/**
 * Checks whether or not the iterator has a next entry iterate over.
 *
 * @return true if the next entry exists or false if the iterator has reached
 * the end of the table.
 */
bool hashtable_key_iter_has_next(HashTableIter *iter)
{
    return iter->next_entry ? true : false;
}

/**
 * Advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 */
void hashtable_key_iter_next(HashTableIter *iter)
{
    TableEntry *next = iter->next_entry->next;

    if (next)
        iter->next_entry = next;

    int i;
    for (i = iter->bucket_index; i < iter->table->capacity; i++) {
        next = iter->table->buckets[i];
        if (next) {
            iter->bucket_index = i;
            iter->next_entry = next;
            break;
        }
    }
}

/**
 * Removes the last returned table entry
 */
void hashtable_iter_remove(HashTableIter *iter)
{
    hashtable_remove(iter->table, iter->prev_entry->key);
}

/**
 * Returns the key associated with the last returned table entry by the
 * specified iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed on
 */
void const *hashtable_iter_get_key(HashTableIter *iter)
{
    return iter->prev_entry->key;
}

/**
 * Returns the value associated with the last returned table entry by the
 * specified iterator.
 *
 * @param[in] iter the iterator on which this operation is being performed on
 */
void *hashtable_iter_get_value(HashTableIter *iter)
{
    return iter->prev_entry->value;
}

/**
 * Slightly modified djb2 string hashing function
 */
uint32_t hashtable_hash_string(const void *key, int len, uint32_t seed)
{
    char     *str = (char*) key;
    uint32_t hash = 5381;
    int      c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

/*******************************************************************************
 *
 * MurmurHash3 by Austin Appleby, adapted for hashtable use.
 *
 ******************************************************************************/

#if defined(_MSC_VER)

#define FORCE_INLINE    __forceinline

#define ROTL32(x,y) _rotl(x,y)
#define ROTL64(x,y) _rotl64(x,y)

#define BIG_CONSTANT(x) (x)

#else   // defined(_MSC_VER)

#define FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32 (uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

#define ROTL32(x,y) rotl32(x,y)
#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE uint32_t fmix32 (uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

uint32_t hashtable_murmur_hash3(const void * key, int len, uint32_t seed)
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

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

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 = fmix32(h1);

    return h1;
}
