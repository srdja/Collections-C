#ifndef COLLECTIONS_C_TSTTABLE_H
#define COLLECTIONS_C_TSTTABLE_H

#include "common.h"

/**
 * A Ternary Search Tree Table. Supports insertion, search, iteration, and deletion.
 */
typedef struct tsttable_s TSTTable;

/**
 * TSTTable configuration structure. Used to initialize a new
 * TSTTable with specific attributes.
 */
typedef struct tsttable_conf_s {
    int    (*char_cmp)    (char c1, char c2);
    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
} TSTTableConf;


/**
 * A key-value pair that holds the data stored in the Ternary Search Tree.
 */
typedef struct tsttable_data_s {
    char *key;
    void *value;
} TSTTableEntry;

/**
 * TSTTable iterator structure. Used to iterate over the entries
 * of the Ternary Search Tree. The iterator also supports operations for safely
 * removing elements during iteration.
 * 
 * @note This structure should only be modified through the
 * iterator functions.
 */
typedef struct tsttable_iter_s {
    TSTTable        *table;
    void            *previous_node;
    void            *current_node;
    void            *next_node;
    bool            advanced_on_remove;
    enum cc_stat    next_stat;
} TSTTableIter;


void          tsttable_conf_init       (TSTTableConf *conf);
enum cc_stat  tsttable_new             (TSTTable **out);
enum cc_stat  tsttable_new_conf        (TSTTableConf const * const conf, TSTTable **out);

void          tsttable_destroy         (TSTTable *table);
enum cc_stat  tsttable_add             (TSTTable *table, char *key, void *val);
enum cc_stat  tsttable_get             (TSTTable *table, char *key, void **out);
enum cc_stat  tsttable_remove          (TSTTable *table, char *key, void **out);
void          tsttable_remove_all      (TSTTable *table);
bool          tsttable_contains_key    (TSTTable *table, char *key);

size_t        tsttable_size            (TSTTable *table);

void          tsttable_foreach_key     (TSTTable *table, void (*op) (const void *));
void          tsttable_foreach_value   (TSTTable *table, void (*op) (void *));

void          tsttable_iter_init       (TSTTableIter *iter, TSTTable *table);
enum cc_stat  tsttable_iter_next       (TSTTableIter *iter, TSTTableEntry **out);
enum cc_stat  tsttable_iter_remove     (TSTTableIter *iter, void **out);


#define TSTTABLE_FOREACH(tsttable, key_53d46d2a04458e7b, value_53d46d2a04458e7b, body)  \
    {                                                                               \
        TSTTableIter tsttable_iter_53d46d2a04458e7b;                                    \
        tsttable_iter_init(&tsttable_iter_53d46d2a04458e7b, tsttable);                    \
        TSTTableEntry *entry_53d46d2a04458e7b;                                         \
        while (tsttable_iter_next(&tsttable_iter_53d46d2a04458e7b, &entry_53d46d2a04458e7b) != CC_ITER_END) \
        {                                                                           \
            key_53d46d2a04458e7b = entry_53d46d2a04458e7b->key;                     \
            value_53d46d2a04458e7b = entry_53d46d2a04458e7b->value;                 \
            body                                                                    \
        }                                                                           \
    }

#endif /* COLLECTIONS_C_TSTTABLE_H */
