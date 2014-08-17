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

#include <stdlib.h>
#include <stdbool.h>
#include "slist.h"

static bool unlink(slist_t *list, Node *node, Node *prev, bool free);
static bool unlink_all(slist_t *list, bool free);

static Node *get_node_at(slist_t *list, int index);

slist_t *slist_new()
{
    return (slist_t*) calloc(1, sizeof(slist_t));
}

bool slist_destroy(slist_t *list)
{
    bool success = true;

    if (list->size > 0)
        success = slist_remove_all(list);

    free(list);
    return success;
}

bool slist_destroy_free(slist_t *list)
{
    bool success = true;

    if (list->size > 0)
        success = slist_free_all(list);

    free(list);
    return success;
}

bool slist_add(slist_t *list, void *element)
{

}

bool slist_add_at(slist_t *list, void *element, int index)
{

}

bool slist_add_all(slist_t *list1, slist_t *list2)
{

}

bool slist_add_all_at(slist_t *list1, slist_t *list2, int index)
{

}

slist_t *slist_splice(slist_t *list1, slist_t *list2)
{

}

bool slist_remove(slist_t *list, void *element)
{

}

bool slist_remove_head(slist_t *list)
{

}

bool slist_remove_tail(slist_t *list)
{

}

bool slist_remove_at(slist_t *list, int index)
{

}

bool slist_remove_all(slist_t *list)
{

}

bool slist_free(slist_t *list, void *element)
{

}

bool slist_free_head(slist_t *list)
{

}

bool slist_free_tail(slist_t *list)
{

}

bool slist_free_at(slist_t *list, int index)
{

}

bool slist_free_all(slist_t *list)
{

}

void *slist_get(slist_t *list, int index)
{

}

void *slist_get_head(slist_t *list)
{

}

void *slist_get_tail(slist_t *list)
{

}

slist_t *slist_sublist(slist_t *list, int from, int to)
{

}

slist_t *slist_copy_shallow(slist_t *list)
{

}

slist_t *slist_copy_deep(slist_t *list, void *(*cp) (void*))
{

}

bool slist_replace_at(slist_t *list, void *element, int index)
{

}

int slist_contains(slist_t *list, void *element)
{

}

int slist_index_of(slist_t *list, void *element)
{

}

int slist_size(slist_t *list)
{

}

void **slist_to_array(slist_t *list)
{

}

static bool unlink_all(slist_t *list, bool free)
{
    if (list->size <= 0)
        return false;

    Node *node = list->head;

    while (node) {
        Node *tmp = node->next;
        unlink(list, node, NULL, free);
        node = tmp;
    }
    return true;
}

static bool unlink(slist_t *list, Node *node, Node *prev, bool freed)
{
    if (node == NULL)
        return false;

    if (freed)
        free(node->data);

    if (prev)
        prev->next = node->next;

    free(node);
    list->size--;

    return true;
}

static Node *get_node_at(slist_t *list, int index)
{
    if (list == NULL || index >= list->size || index < 0)
        return NULL;

    int i;
    Node *node = list->head;

    for (i = 0; i < index; i++)
        node = node->next;

    return node;
}
