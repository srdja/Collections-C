#include "tsttable.h"

/**
 * A Ternary Search Tree Table Node.
 */
typedef struct tsttable_node_s {
    char c;
    union {
        const void * eow;
        TSTTableEntry * data;
    };
    struct tsttable_node_s * parent;
    struct tsttable_node_s * left;
    struct tsttable_node_s * mid;
    struct tsttable_node_s * right;
} TSTTableNode;

/**
 * A Ternary Search Tree Table. Supports insertion, search, and deletion.
 */
struct tsttable_s {
    size_t      size;
    TSTTableNode  *root;
    int     (*char_cmp)    (char c1, char c2);
    void    *(*mem_alloc)  (size_t size);
    void    *(*mem_calloc) (size_t blocks, size_t size);
    void    (*mem_free)    (void *block);
};

/**
 * comparing charcters in natural order
 */
static int char_cmp(char c1, char c2) {
    return c1 - c2;
}

/**
 * Initializes the TSTTableConf structs fields to default values.
 *
 * @param[in] conf the struct that is being initialized
 */
void tsttable_conf_init (TSTTableConf *conf) {
    conf->char_cmp         = char_cmp;
    conf->mem_alloc        = malloc;
    conf->mem_calloc       = calloc;
    conf->mem_free         = free;
}

/**
 * Creates a new TSTTable and returns a status code.
 *
 * @note The newly created TSTTable will work with string keys.
 *
 * @param[out] out Pointer to where the newly created TSTTable is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new TSTTable failed.
 */
enum cc_stat tsttable_new (TSTTable **out) {
    TSTTableConf conf;
    tsttable_conf_init(&conf);
    return tsttable_new_conf(&conf, out);
}

/**
 * Creates a new TSTTable based on the specified TSTTableConf struct and returns
 * a status code.
 *
 * The table is allocated using the memory allocators specified in the TSTTableConf
 * struct.
 *
 * @param[in] conf the TSTTable conf structure
 * @param[out] out Pointer to where the newly created TSTTable is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new TSTTable structure failed.
 */
enum cc_stat tsttable_new_conf (TSTTableConf const * const conf, TSTTable **out) {
    TSTTable * table = conf->mem_alloc(sizeof(TSTTable));
    if(!table)
        return CC_ERR_ALLOC;

    table->size = 0;
    table->root = NULL;
    table->char_cmp = conf->char_cmp;
    table->mem_alloc = conf->mem_alloc;
    table->mem_calloc = conf->mem_calloc;
    table->mem_free = conf->mem_free;

    *out = table;
    return CC_OK;
}

/**
 * Destroys the specified TSTTable structure without destroying the data
 * contained within it. In other words, the keys and the values are not freed,
 * but only the table structure.
 *
 * @param[in] table TSTTable to be destroyed
 */
void tsttable_destroy (TSTTable *table) {
    tsttable_remove_all(table);
    table->mem_free(table);
}

/**
 * sotre the address of the last matched node of the given key in last_node.
 * if the key has unmatched chars, then the the location of where to insert 
 * the next char is stored in last_node.
 */
static void get_last_node(TSTTable * table, TSTTableNode ***last_node, TSTTableNode ** last_parent, int *last_index, char *key, size_t key_len) {
    *last_node = &table->root;
    *last_index = -1;
    *last_parent = NULL;

    while (**last_node && (*last_index + 1) < key_len) {
        char c = key[*last_index + 1];
        int cmp = table->char_cmp(c, (**last_node)->c);
        if(cmp < 0) {
            *last_parent = **last_node;
            *last_node = &((**last_node)->left);
        }
        else if(cmp > 0) {
            *last_parent = **last_node;
            *last_node = &((**last_node)->right);
        }
        else if(cmp == 0) {
            *last_index += 1;
            if(*last_index + 1 == key_len)
                break;
            else {
                *last_parent = **last_node;
                *last_node = &((**last_node)->mid);
            }
        }
    }
}

/**
 * make a list of nodes connected by their 'mid' pointer. This list matches the provided key
 * and starts at **begin and ends at **end.
 * */
static enum cc_stat make_mid_subtree(TSTTable *table, TSTTableNode **begin, TSTTableNode **end, char *key, size_t key_len) {
    *begin = table->mem_calloc(1, sizeof(TSTTableNode));
    if(!*begin)
        return CC_ERR_ALLOC;

    (*begin)->c = key[0];
    TSTTableNode * node = *begin;

    for(size_t i = 1; i < key_len; i++) {
        node->mid = table->mem_calloc(1, sizeof(TSTTableNode));
        if(!node->mid)
            return CC_ERR_ALLOC;

        node->mid->parent = node;
        node->mid->c = key[i];
        node = node->mid;
    }

    *end = node;
    return CC_OK;
}

/**
 * Creates a new key-value mapping in the specified TSTTable. If the unique key
 * is already mapped to a value in this table, that value is replaced with the
 * new value. This operation may fail if the space allocation for the new entry
 * fails.
 *
 * @param[in] table the table to which this new key-value mapping is being added
 * @param[in] key a key used to access the specified value
 * @param[in] val a value that is being stored in the table
 *
 * @return CC_OK if the mapping was successfully added, or CC_ERR_ALLOC if the
 * memory allocation failed.
 */
enum cc_stat tsttable_add(TSTTable *table, char *key, void *val) {
    size_t key_len = strlen(key);

    TSTTableNode ** last_node;
    TSTTableNode * last_parent;
    int last_index;

    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    size_t postfix_len = key_len - (last_index + 1);
    char *postfix = key + (last_index + 1);

    if(*last_node) {
        if(!(*last_node)->eow) {
            (*last_node)->data = table->mem_alloc(sizeof(TSTTableEntry));
            if(!(*last_node)->data)
                return CC_ERR_ALLOC;
            table->size += 1;
        }
        (*last_node)->data->key = key;
        (*last_node)->data->value = val;
        return CC_OK;
    }

    TSTTableNode *begin, *end;
    enum cc_stat status = make_mid_subtree(table, &begin, &end, postfix, postfix_len);
    if(status != CC_OK)
        return CC_ERR_ALLOC;
    begin->parent = last_parent;
    end->data = table->mem_alloc(sizeof(TSTTableEntry));
    if(!end->data)
        return CC_ERR_ALLOC;
    table->size += 1;
    end->data->key = key;
    end->data->value = val;

    *last_node = begin;
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
enum cc_stat tsttable_get (TSTTable *table, char *key, void **out) {
    size_t key_len = strlen(key);

    TSTTableNode * last_parent;
    TSTTableNode ** last_node;
    int last_index;
    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    if(*last_node && (*last_node)->eow && last_index + 1 == key_len) {
        *out = (*last_node)->data->value;
        return CC_OK;
    }
    else
        return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Removes all key-value mappings from the specified table.
 *
 * @param[in] table the table from which all mappings are being removed
 */
void tsttable_remove_all (TSTTable *table) {
    TSTTableNode * node = table->root;
    while (node) {
        TSTTableNode * parent = node->parent;
        if(!node->left && !node->right && !node->mid) {
            if(parent) {
                if(parent->left == node)
                    parent->left = NULL;
                else if(parent->mid == node)
                    parent->mid = NULL;
                else if(parent->right == node)
                    parent->right = NULL;
                
                if(node->eow) 
                    table->size -= 1;

                table->mem_free(node);
                node = parent;
            }
            else {
                if(node->eow)
                    table->size -= 1;
                
                table->mem_free(node);
                node = NULL;
            }
        }
        else if(node->left)
            node = node->left;
        else if(node->mid)
            node = node->mid;
        else if(node->right)
            node = node->right;
    }

    table->root = NULL;
}

/**
 * if the node is the end of a word, remove it and all the related 
 * nodes containing the key without invalidating other keys.
 */
static void remove_eow_node(TSTTable *table, TSTTableNode *node) {
    if(!node->eow)
        return;

    node->data = NULL;
    TSTTableNode * parent = node->parent;
    while (node) {
        if(!node->left && !node->mid && !node->right && !node->eow) {
            if(parent) {
                if(parent->left == node)
                    parent->left = NULL;
                else if(parent->right == node)
                    parent->right = NULL;
                else if(parent->mid == node)
                    parent->mid = NULL;
                table->mem_free(node);
                node = parent;
                parent = node->parent;
            }
            else {
                table->mem_free(node);
                node = NULL;
                table->root = NULL;
            }
        }
        else
            break;
    }
}

/**
 * Removes a key-value mapping from the specified TSTTable and sets the out
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
enum cc_stat tsttable_remove (TSTTable *table, char *key, void **out) {
    size_t key_len = strlen(key);

    TSTTableNode ** last_node;
    TSTTableNode * last_parent;
    int last_index;
    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    if(*last_node && (*last_node)->eow && last_index + 1 == key_len) {
        if(out)
            *out = (*last_node)->data->value;
        remove_eow_node(table, *last_node);
        table->size = (table->size > 0) ? (table->size - 1) : 0;
        return CC_OK;
    }
    else
        return CC_ERR_KEY_NOT_FOUND;
}

/**
 * Checks whether or not the TSTTable contains the specified key.
 *
 * @param[in] table the table on which the search is being performed
 * @param[in] key the key that is being searched for
 *
 * @return true if the table contains the key.
 */
bool tsttable_contains_key (TSTTable *table, char *key) {
    void *out;
    enum cc_stat res = tsttable_get(table, key, &out);
    return (res == CC_OK);
}

/**
 * Returns the size of the specified TSTTable. Size of a TSTTable represents
 * the number of key-value mappings within the table.
 *
 * @param[in] table the table whose size is being returned
 *
 * @return the size of the table.
 */
size_t tsttable_size (TSTTable *table) {
    return table->size;
}

/**
 * Applies the function fn to each key of the TSTTable.
 *
 * @note The operation function should not modify the key. Any modification
 * of the key will invalidate the TSTTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each key of the table
 */
void tsttable_foreach_key (TSTTable *table, void (*fn) (const void *)) {
    TSTTableIter iter;
    tsttable_iter_init(&iter, table);
    TSTTableEntry * data;
    while (tsttable_iter_next(&iter, &data) != CC_ITER_END)
       fn(data->key);
}

/**
 * Applies the function fn to each value of the TSTTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each value of the table
 */
void tsttable_foreach_value (TSTTable *table, void (*fn) (void *)) {
    TSTTableIter iter;
    tsttable_iter_init(&iter, table);
    TSTTableEntry * data;
    while (tsttable_iter_next(&iter, &data) != CC_ITER_END)
       fn(data->value);
}

/**
 * Initializes the TSTTableIter structure.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] table the table over whose entries the iterator is going to iterate
 */
void tsttable_iter_init (TSTTableIter *iter, TSTTable *table) {
    iter->table = table;
    iter->current_node = NULL;
    iter->next_node = table->root;
    iter->previous_node = NULL;
    iter->advanced_on_remove = 0;
}

/**
 * Advances the iterator and sets the out parameter to the value of the
 * next TSTTableEntry.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out Pointer to where the next entry is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the TSTTable has been reached.
 */
enum cc_stat tsttable_iter_next (TSTTableIter *iter, TSTTableEntry **out) {
    TSTTableNode * node = iter->next_node;
    TSTTableNode * previous_node = iter->current_node;
    TSTTableNode * next_node = NULL;

    if(iter->advanced_on_remove) {
        iter->advanced_on_remove = 0;
        return iter->next_stat;
    }

    int error = 0;
    while(node) {
        if(previous_node == node->parent) {
            if(node->left)
                next_node = node->left;
            else if(node->mid) 
                next_node = node->mid;
            else if(node->right)
                next_node = node->right;
            else if(node->parent)
                next_node = node->parent;
            else error = 1;
        }
        else if(previous_node == node->left) {
            if(node->mid)
                next_node = node->mid;
            else if(node->right)
                next_node = node->right;
            else if(node->parent)
                next_node = node->parent;
            else error = 1;
        }
        else if(previous_node == node->mid) {
            if(node->right)
                next_node = node->right;
            else if(node->parent)
                next_node = node->parent;
            else error = 1;
        }
        else if(previous_node == node->right) {
            if(node->parent)
                next_node = node->parent;
            else error = 1;
        }
        else error = 1;

        if(error) {
            iter->next_node = NULL;
            iter->current_node = NULL;
            iter->previous_node = NULL;
            return CC_ITER_END;
        }
        else if(node->eow) {
            *out = node->data;
            iter->current_node = node;
            iter->next_node = next_node;
            iter->previous_node = previous_node;
            return CC_OK;
        }

        previous_node = node;
        node = next_node;
        next_node = NULL;
    }

    iter->next_node = NULL;
    iter->current_node = NULL;
    iter->previous_node = NULL;
    return CC_ITER_END;
}

/**
 * Removes the last returned entry by <code>tsttable_iter_next()</code>
 * function without invalidating the iterator and optionally sets the
 * out parameter to the value of the removed entry.
 *
 * @note This Function should only ever be called after a call to <code>
 * tsttable_iter_next()</code>
 *
 * @param[in] iter The iterator on which this operation is performed
 * @param[out] out Pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the entry was successfully removed, or
 * CC_ERR_KEY_NOT_FOUND if the entry was already removed.
 */
enum cc_stat tsttable_iter_remove (TSTTableIter *iter, void **out) {
    if (!iter->current_node)
        return CC_ERR_KEY_NOT_FOUND;
    
    if (out) {
        TSTTableNode * node = iter->current_node;
        *out = node->data->value;
    }

    TSTTableNode * to_remove_eow = iter->current_node;
    
    TSTTableEntry *entry;
    enum cc_stat next_stat = tsttable_iter_next(iter, &entry);
    iter->advanced_on_remove = 1;
    iter->next_stat = next_stat;

    remove_eow_node(iter->table, to_remove_eow);
    iter->table->size -= 1;

    return CC_OK;
}
 
