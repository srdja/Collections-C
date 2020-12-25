#include "cc_tsttable.h"

/**
 * A Ternary Search Tree Table Node.
 */
typedef struct cc_tsttable_node_s {
    char c;
    union {
        const void * eow;
        CC_TSTTableEntry * data;
    };
    struct cc_tsttable_node_s *parent;
    struct cc_tsttable_node_s *left;
    struct cc_tsttable_node_s *mid;
    struct cc_tsttable_node_s *right;
} CC_TSTTableNode;


/**
 * A Ternary Search Tree Table. Supports insertion, search, and deletion.
 */
struct cc_tsttable_s {
    size_t         size;
    CC_TSTTableNode  *root;

    int   (*char_cmp)   (char c1, char c2);
    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};


/**
 * comparing charcters in natural order
 */
static int char_cmp(char c1, char c2)
{
    return c1 - c2;
}


/**
 * Initializes the CC_TSTTableConf structs fields to default values.
 *
 * @param[in] conf the struct that is being initialized
 */
void cc_tsttable_conf_init (CC_TSTTableConf *conf)
{
    conf->char_cmp   = char_cmp;
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
}


/**
 * Creates a new CC_TSTTable and returns a status code.
 *
 * @note The newly created CC_TSTTable will work with string keys.
 *
 * @param[out] out Pointer to where the newly created CC_TSTTable is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_TSTTable failed.
 */
enum cc_stat cc_tsttable_new (CC_TSTTable **out)
{
    CC_TSTTableConf conf;
    cc_tsttable_conf_init(&conf);
    return cc_tsttable_new_conf(&conf, out);
}


/**
 * Creates a new CC_TSTTable based on the specified CC_TSTTableConf struct and returns
 * a status code.
 *
 * The table is allocated using the memory allocators specified in the CC_TSTTableConf
 * struct.
 *
 * @param[in] conf the CC_TSTTable conf structure
 * @param[out] out Pointer to where the newly created CC_TSTTable is stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the memory
 * allocation for the new CC_TSTTable structure failed.
 */
enum cc_stat cc_tsttable_new_conf (CC_TSTTableConf const * const conf, CC_TSTTable **out)
{
    CC_TSTTable * table = conf->mem_alloc(sizeof(CC_TSTTable));

    if (!table)
        return CC_ERR_ALLOC;

    table->size       = 0;
    table->root       = NULL;
    table->char_cmp   = conf->char_cmp;
    table->mem_alloc  = conf->mem_alloc;
    table->mem_calloc = conf->mem_calloc;
    table->mem_free   = conf->mem_free;

    *out = table;
    return CC_OK;
}


/**
 * Destroys the specified CC_TSTTable structure without destroying the data
 * contained within it. In other words, the keys and the values are not freed,
 * but only the table structure.
 *
 * @param[in] table CC_TSTTable to be destroyed
 */
void cc_tsttable_destroy (CC_TSTTable *table)
{
    cc_tsttable_remove_all(table);
    table->mem_free(table);
}


/**
 * sotre the address of the last matched node of the given key in last_node.
 * if the key has unmatched chars, then the the location of where to insert
 * the next char is stored in last_node.
 */
static void get_last_node(
    CC_TSTTable       *table,
    CC_TSTTableNode ***last_node,
    CC_TSTTableNode ** last_parent,
    int            *last_index,
    char           *key,
    size_t          key_len)
{
    *last_node   = &table->root;
    *last_index  = -1;
    *last_parent = NULL;

    while (**last_node && (*last_index + 1) < key_len) {
        char c   = key[*last_index + 1];
        int  cmp = table->char_cmp(c, (**last_node)->c);

        if (cmp < 0) {
            *last_parent = **last_node;
            *last_node = &((**last_node)->left);
        }
        else if (cmp > 0) {
            *last_parent = **last_node;
            *last_node = &((**last_node)->right);
        }
        else if (cmp == 0) {
            *last_index += 1;
            if (*last_index + 1 == key_len)
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
static enum cc_stat make_mid_subtree(
    CC_TSTTable      *table,
    CC_TSTTableNode **begin,
    CC_TSTTableNode **end,
    char          *key,
    size_t         key_len)
{
    *begin = table->mem_calloc(1, sizeof(CC_TSTTableNode));

    if (!*begin)
        return CC_ERR_ALLOC;

    (*begin)->c = key[0];
    CC_TSTTableNode *node = *begin;

    for (size_t i = 1; i < key_len; i++) {
        node->mid = table->mem_calloc(1, sizeof(CC_TSTTableNode));

        if (!node->mid)
            return CC_ERR_ALLOC;

        node->mid->parent = node;
        node->mid->c      = key[i];
        node              = node->mid;
    }

    *end = node;
    return CC_OK;
}


/**
 * Creates a new key-value mapping in the specified CC_TSTTable. If the unique key
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
enum cc_stat cc_tsttable_add(CC_TSTTable *table, char *key, void *val)
{
    size_t key_len = strlen(key);

    CC_TSTTableNode **last_node;
    CC_TSTTableNode  *last_parent;
    int            last_index;

    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    size_t postfix_len = key_len - (last_index + 1);
    char  *postfix     = key + (last_index + 1);

    if (*last_node) {
        if (!(*last_node)->eow) {
            (*last_node)->data = table->mem_alloc(sizeof(CC_TSTTableEntry));

            if (!(*last_node)->data)
                return CC_ERR_ALLOC;

            table->size += 1;
        }
        (*last_node)->data->key   = key;
        (*last_node)->data->value = val;
        return CC_OK;
    }

    CC_TSTTableNode *begin;
    CC_TSTTableNode *end;

    enum cc_stat status = make_mid_subtree(table, &begin, &end, postfix, postfix_len);

    if (status != CC_OK)
        return CC_ERR_ALLOC;

    begin->parent = last_parent;
    end->data     = table->mem_alloc(sizeof(CC_TSTTableEntry));

    if (!end->data)
        return CC_ERR_ALLOC;

    table->size      += 1;
    end->data->key    = key;
    end->data->value  = val;

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
enum cc_stat cc_tsttable_get (CC_TSTTable *table, char *key, void **out)
{
    size_t key_len = strlen(key);

    CC_TSTTableNode  *last_parent;
    CC_TSTTableNode **last_node;

    int last_index;

    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    if (*last_node && (*last_node)->eow && last_index + 1 == key_len) {
        *out = (*last_node)->data->value;
        return CC_OK;
    } else {
        return CC_ERR_KEY_NOT_FOUND;
    }
}

/**
 * Removes all key-value mappings from the specified table.
 *
 * @param[in] table the table from which all mappings are being removed
 */
void cc_tsttable_remove_all (CC_TSTTable *table)
{
    CC_TSTTableNode * node = table->root;

    while (node) {
        CC_TSTTableNode * parent = node->parent;
        if (!node->left && !node->right && !node->mid) {
            if (parent) {
                if (parent->left == node)
                    parent->left = NULL;
                else if (parent->mid == node)
                    parent->mid = NULL;
                else if (parent->right == node)
                    parent->right = NULL;

                if (node->eow)
                    table->size -= 1;

                table->mem_free(node);
                node = parent;
            } else {
                if (node->eow)
                    table->size -= 1;

                table->mem_free(node);
                node = NULL;
            }
        } else if (node->left) {
            node = node->left;
        } else if (node->mid) {
            node = node->mid;
        } else if (node->right) {
            node = node->right;
        }
    }

    table->root = NULL;
}


/**
 * if the node is the end of a word, remove it and all the related
 * nodes containing the key without invalidating other keys.
 */
static void remove_eow_node(CC_TSTTable *table, CC_TSTTableNode *node)
{
    if (!node->eow)
        return;

    node->data            = NULL;
    CC_TSTTableNode * parent = node->parent;

    while (node) {
        if (!node->left && !node->mid && !node->right && !node->eow) {
            if (parent) {
                if (parent->left == node)
                    parent->left = NULL;
                else if (parent->right == node)
                    parent->right = NULL;
                else if (parent->mid == node)
                    parent->mid = NULL;

                table->mem_free(node);

                node   = parent;
                parent = node->parent;
            } else {
                table->mem_free(node);
                node        = NULL;
                table->root = NULL;
            }
        } else {
            break;
        }
    }
}


/**
 * Removes a key-value mapping from the specified CC_TSTTable and sets the out
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
enum cc_stat cc_tsttable_remove (CC_TSTTable *table, char *key, void **out)
{
    size_t key_len = strlen(key);

    CC_TSTTableNode **last_node;
    CC_TSTTableNode  *last_parent;

    int last_index;
    get_last_node(table, &last_node, &last_parent, &last_index, key, key_len);

    if (*last_node && (*last_node)->eow && last_index + 1 == key_len) {
        if (out)
            *out = (*last_node)->data->value;

        remove_eow_node(table, *last_node);
        table->size = (table->size > 0) ? (table->size - 1) : 0;
        return CC_OK;
    } else {
        return CC_ERR_KEY_NOT_FOUND;
    }
}


/**
 * Checks whether or not the CC_TSTTable contains the specified key.
 *
 * @param[in] table the table on which the search is being performed
 * @param[in] key the key that is being searched for
 *
 * @return true if the table contains the key.
 */
bool cc_tsttable_contains_key (CC_TSTTable *table, char *key)
{
    void *out;
    enum cc_stat res = cc_tsttable_get(table, key, &out);
    return (res == CC_OK);
}


/**
 * Returns the size of the specified CC_TSTTable. Size of a CC_TSTTable represents
 * the number of key-value mappings within the table.
 *
 * @param[in] table the table whose size is being returned
 *
 * @return the size of the table.
 */
size_t cc_tsttable_size (CC_TSTTable *table)
{
    return table->size;
}


/**
 * Applies the function fn to each key of the CC_TSTTable.
 *
 * @note The operation function should not modify the key. Any modification
 * of the key will invalidate the CC_TSTTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each key of the table
 */
void cc_tsttable_foreach_key (CC_TSTTable *table, void (*fn) (const void *))
{
    CC_TSTTableIter iter;
    cc_tsttable_iter_init(&iter, table);
    CC_TSTTableEntry * data;

    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END)
        fn(data->key);
}


/**
 * Applies the function fn to each value of the CC_TSTTable.
 *
 * @param[in] table the table on which this operation is being performed
 * @param[in] fn the operation function that is invoked on each value of the table
 */
void cc_tsttable_foreach_value (CC_TSTTable *table, void (*fn) (void*))
{
    CC_TSTTableIter iter;
    cc_tsttable_iter_init(&iter, table);
    CC_TSTTableEntry *data;

    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END)
        fn(data->value);
}


/**
 * Initializes the CC_TSTTableIter structure.
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] table the table over whose entries the iterator is going to iterate
 */
void cc_tsttable_iter_init (CC_TSTTableIter *iter, CC_TSTTable *table)
{
    iter->table              = table;
    iter->current_node       = NULL;
    iter->next_node          = table->root;
    iter->previous_node      = NULL;
    iter->advanced_on_remove = 0;
}


/**
 * Advances the iterator and sets the out parameter to the value of the
 * next CC_TSTTableEntry.
 *
 * @param[in] iter the iterator that is being advanced
 * @param[out] out Pointer to where the next entry is set
 *
 * @return CC_OK if the iterator was advanced, or CC_ITER_END if the
 * end of the CC_TSTTable has been reached.
 */
enum cc_stat cc_tsttable_iter_next (CC_TSTTableIter *iter, CC_TSTTableEntry **out)
{
    CC_TSTTableNode *node          = iter->next_node;
    CC_TSTTableNode *previous_node = iter->current_node;
    CC_TSTTableNode *next_node     = NULL;

    if (iter->advanced_on_remove) {
        iter->advanced_on_remove = 0;
        return iter->next_stat;
    }

    int error = 0;
    while (node) {
        if (previous_node == node->parent) {
            if (node->left)
                next_node = node->left;
            else if (node->mid)
                next_node = node->mid;
            else if (node->right)
                next_node = node->right;
            else if (node->parent)
                next_node = node->parent;
            else
                error = 1;

        }  else if (previous_node == node->left) {
            if (node->mid)
                next_node = node->mid;
            else if (node->right)
                next_node = node->right;
            else if (node->parent)
                next_node = node->parent;
            else
                error = 1;

        } else if (previous_node == node->mid) {
            if (node->right)
                next_node = node->right;
            else if (node->parent)
                next_node = node->parent;
            else
                error = 1;

        } else if (previous_node == node->right) {
            if (node->parent)
                next_node = node->parent;
            else
                error = 1;

        } else {
            error = 1;
        }

        if (error) {
            iter->next_node     = NULL;
            iter->current_node  = NULL;
            iter->previous_node = NULL;
            return CC_ITER_END;

        } else if (node->eow) {
            *out                = node->data;
            iter->current_node  = node;
            iter->next_node     = next_node;
            iter->previous_node = previous_node;
            return CC_OK;
        }

        previous_node = node;
        node          = next_node;
        next_node     = NULL;
    }

    iter->next_node     = NULL;
    iter->current_node  = NULL;
    iter->previous_node = NULL;

    return CC_ITER_END;
}


/**
 * Removes the last returned entry by <code>cc_tsttable_iter_next()</code>
 * function without invalidating the iterator and optionally sets the
 * out parameter to the value of the removed entry.
 *
 * @note This Function should only ever be called after a call to <code>
 * cc_tsttable_iter_next()</code>
 *
 * @param[in] iter The iterator on which this operation is performed
 * @param[out] out Pointer to where the removed element is stored, or NULL
 *                 if it is to be ignored
 *
 * @return CC_OK if the entry was successfully removed, or
 * CC_ERR_KEY_NOT_FOUND if the entry was already removed.
 */
enum cc_stat cc_tsttable_iter_remove (CC_TSTTableIter *iter, void **out)
{
    if (!iter->current_node)
        return CC_ERR_KEY_NOT_FOUND;

    if (out) {
        CC_TSTTableNode *node = iter->current_node;
        *out = node->data->value;
    }

    CC_TSTTableNode *to_remove_eow = iter->current_node;

    CC_TSTTableEntry *entry;
    enum cc_stat next_stat = cc_tsttable_iter_next(iter, &entry);

    iter->advanced_on_remove = 1;
    iter->next_stat          = next_stat;

    remove_eow_node(iter->table, to_remove_eow);
    iter->table->size -= 1;

    return CC_OK;
}
