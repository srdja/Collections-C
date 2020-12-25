#ifndef COLLECTIONS_C_CC_TSTTABLE_H
#define COLLECTIONS_C_CC_TSTTABLE_H

#include "cc_common.h"

/**
 * A Ternary Search Tree Table. Supports insertion, search, iteration, and deletion.
 */
typedef struct cc_tsttable_s CC_TSTTable;

/**
 * CC_TSTTable configuration structure. Used to initialize a new
 * CC_TSTTable with specific attributes.
 */
typedef struct cc_tsttable_conf_s {
    int    (*char_cmp)    (char c1, char c2);
    void  *(*mem_alloc)   (size_t size);
    void  *(*mem_calloc)  (size_t blocks, size_t size);
    void   (*mem_free)    (void *block);
} CC_TSTTableConf;


/**
 * A key-value pair that holds the data stored in the Ternary Search Tree.
 */
typedef struct cc_tsttable_data_s {
    char *key;
    void *value;
} CC_TSTTableEntry;

/**
 * CC_TSTTable iterator structure. Used to iterate over the entries
 * of the Ternary Search Tree. The iterator also supports operations for safely
 * removing elements during iteration.
 *
 * @note This structure should only be modified through the
 * iterator functions.
 */
typedef struct cc_tsttable_iter_s {
    CC_TSTTable        *table;
    void            *previous_node;
    void            *current_node;
    void            *next_node;
    bool            advanced_on_remove;
    enum cc_stat    next_stat;
} CC_TSTTableIter;


void          cc_tsttable_conf_init       (CC_TSTTableConf *conf);
enum cc_stat  cc_tsttable_new             (CC_TSTTable **out);
enum cc_stat  cc_tsttable_new_conf        (CC_TSTTableConf const * const conf, CC_TSTTable **out);

void          cc_tsttable_destroy         (CC_TSTTable *table);
enum cc_stat  cc_tsttable_add             (CC_TSTTable *table, char *key, void *val);
enum cc_stat  cc_tsttable_get             (CC_TSTTable *table, char *key, void **out);
enum cc_stat  cc_tsttable_remove          (CC_TSTTable *table, char *key, void **out);
void          cc_tsttable_remove_all      (CC_TSTTable *table);
bool          cc_tsttable_contains_key    (CC_TSTTable *table, char *key);

size_t        cc_tsttable_size            (CC_TSTTable *table);

void          cc_tsttable_foreach_key     (CC_TSTTable *table, void (*op) (const void *));
void          cc_tsttable_foreach_value   (CC_TSTTable *table, void (*op) (void *));

void          cc_tsttable_iter_init       (CC_TSTTableIter *iter, CC_TSTTable *table);
enum cc_stat  cc_tsttable_iter_next       (CC_TSTTableIter *iter, CC_TSTTableEntry **out);
enum cc_stat  cc_tsttable_iter_remove     (CC_TSTTableIter *iter, void **out);


#define CC_TSTTABLE_FOREACH(tsttable, key_53d46d2a04458e7b, value_53d46d2a04458e7b, body) \
    {                                                                               \
        CC_TSTTableIter cc_tsttable_iter_53d46d2a04458e7b;              \
        cc_tsttable_iter_init(&cc_tsttable_iter_53d46d2a04458e7b, tsttable); \
        CC_TSTTableEntry *entry_53d46d2a04458e7b;                       \
        while (cc_tsttable_iter_next(&cc_tsttable_iter_53d46d2a04458e7b, &entry_53d46d2a04458e7b) != CC_ITER_END) \
        {                                                                           \
            key_53d46d2a04458e7b = entry_53d46d2a04458e7b->key;                     \
            value_53d46d2a04458e7b = entry_53d46d2a04458e7b->value;                 \
            body                                                                    \
        }                                                                           \
    }

#endif /* COLLECTIONS_C_CC_TSTTABLE_H */
