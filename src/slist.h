/*
 * slist.h
 *
 *  Created on: Jul 19, 2014
 *      Author: srdja
 */

#ifndef SLIST_H_
#define SLIST_H_

#include <stdbool.h>

typedef struct node_s {
    void *data;
    struct node_s *next;
} Node;

typedef struct slist_s {
    int size;
    Node *head;
} slist_t;

typedef struct siterator_s {
    int index;
    slist_t *list;
    Node *next;
} siter_t;

slist_t *slist_new();

bool slist_destroy(slist_t *list);
bool slist_destroy_free(slist_t *list);

slist_t *slist_splice(slist_t *list1, slist_t *list2);

bool slist_add(slist_t *list, void *element);
bool slist_add_at(slist_t *list, void *element, int index);
bool slist_add_all(slist_t *list1, slist_t *list2);
bool slist_add_all_at(slist_t *list1, slist_t *list2, int index);

bool slist_remove(slist_t *list, void *element);
bool slist_remove_head(slist_t *list);
bool slist_remove_tail(slist_t *list);
bool slist_remove_at(slist_t *list, int index);
bool slist_remove_all(slist_t *list);

bool slist_free(slist_t *list, void *element);
bool slist_free_head(slist_t *list);
bool slist_free_tail(slist_t *list);
bool slist_free_at(slist_t *list, int index);
bool slist_free_all(slist_t *list);

void *slist_get(slist_t *list, int index);
void *slist_get_head(slist_t *list);
void *slist_get_tail(slist_t *list);

slist_t *slist_sublist(slist_t *list, int from, int to);
slist_t *slist_copy_shallow(slist_t *list);
slist_t *slist_copy_deep(slist_t *list, void *(*cp) (void*));

bool slist_replace_at(slist_t *list, void *element, int index);

int slist_contains(slist_t *list, void *element);
int slist_index_of(slist_t *list, void *element);
void **slist_to_array(slist_t *list);

void slist_sort(slist_t *list, int (*cmp) (void*, void*));
int slist_size(slist_t *list);

#endif /* SLIST_H_ */
