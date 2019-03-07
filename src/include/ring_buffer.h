/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T10:32:52-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-07T11:24:59-06:00
 */

#ifndef COLLECTIONS_C_RBUF_H
#define COLLECTIONS_C_RBUF_H

#include "common.h"

#define DEFAULT_RBUF_CAPACITY 10

typedef struct ring_buffer_conf {

   size_t capacity;
   void *(*mem_alloc)  (size_t size);
   void *(*mem_calloc) (size_t blocks, size_t size);
   void  (*mem_free)   (void *block);

} RbufConf;

typedef struct ring_buffer {

    size_t capacity;
    int head, tail;
    uint64_t *buf;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);

} RBuf;

enum cc_stat  rbuf_new           (Rbuf **rbuf);
enum cc_stat  rbuf_conf_init     (RbufConf *rconf, Rbuf **rbuf);
enum cc_stat  rbuf_conf_new      (RbufConf *rconf);
enum cc_stat  rbuf_enqueue       (Rbuf *rbuf, uint64_t item);
enum cc_stat  rbuf_dequeue       (Rbuf *rbuf, uint64_t *out);
int           rbuf_is_empty      (Rbuf *rbuf);
void          rbuf_destroy       (Rbuf *rbuf);

#endif
