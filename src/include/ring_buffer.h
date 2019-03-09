/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T10:32:52-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-09T15:14:36-06:00
 */

#ifndef COLLECTIONS_C_RBUF_H
#define COLLECTIONS_C_RBUF_H

#include "common.h"
#include <stdio.h>

#define DEFAULT_RBUF_CAPACITY 10

typedef struct ring_buffer_conf RbufConf;
typedef struct ring_buffer Rbuf;

enum cc_stat  rbuf_new           (Rbuf **rbuf);
void          rbuf_conf_init     (RbufConf *rconf);
enum cc_stat  rbuf_conf_new      (RbufConf *rconf, Rbuf **rbuf);
void          rbuf_enqueue       (Rbuf *rbuf, uint64_t item);
enum cc_stat  rbuf_dequeue       (Rbuf *rbuf, uint64_t *out);
bool          rbuf_is_empty      (Rbuf *rbuf);
size_t        rbuf_size          (Rbuf *rbuf);
void          rbuf_destroy       (Rbuf *rbuf);
uint64_t      rbuf_peek          (Rbuf *rbuf, int index);


#endif
