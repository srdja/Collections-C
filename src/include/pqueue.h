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

#ifndef COLLECTIONS_C_PQUEUE_H
#define COLLECTIONS_C_PQUEUE_H

#include "common.h"
#include "array.h"

/**
 * A Priority Queue data structure. Stores the elements and according to
 * some property associated with the element stored in the pqueue, the elements
 * can be retrieved in a specific order
 **/

typedef struct pqueue_s PQueue;

/**
 * The pqueue initialization configuration structure. Used to initialize the
 * PQueue with the specified attributes
 **/

typedef struct pqueue_conf_s {

    /**
     * cfg it is the basic configuration of the PQueue
     * it includes info to create the memory for PQueue
     */

    ArrayConf cfg;



    /**
     * comparator, used to hold the address of the function which will 
     * be used to compare the elements of the PQueue
     */

    int (*comparator)(const void *a, const void *b);
} PQueueConf;

void          pqueue_conf_init       (PQueueConf *conf);
enum cc_stat  pqueue_new             (PQueue **out);
enum cc_stat  pqueue_new_conf        (PQueueConf const * const conf, PQueue **out);
void          pqueue_destroy         (PQueue *pqueue);
void          pqueue_destroy_free    (PQueue *pqueue);

enum cc_stat  pqueue_push            (PQueue *pqueue, void *element);
enum cc_stat  pqueue_top             (PQueue *pqueue, void **out);
enum cc_stat  pqueue_pop             (PQueue *pqueue, void **out);


#endif
