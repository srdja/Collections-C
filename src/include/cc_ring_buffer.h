/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T10:32:52-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-09T15:14:36-06:00
 */

#ifndef COLLECTIONS_C_CC_RBUF_H
#define COLLECTIONS_C_CC_RBUF_H

#include "cc_common.h"

#define DEFAULT_CC_RBUF_CAPACITY 10

typedef struct ring_buffer_conf CC_RbufConf;
typedef struct ring_buffer CC_Rbuf;

enum cc_stat  cc_rbuf_new           (CC_Rbuf **rbuf);
void          cc_rbuf_conf_init     (CC_RbufConf *rconf);
enum cc_stat  cc_rbuf_conf_new      (CC_RbufConf *rconf, CC_Rbuf **rbuf);
void          cc_rbuf_enqueue       (CC_Rbuf *rbuf, uint64_t item);
enum cc_stat  cc_rbuf_dequeue       (CC_Rbuf *rbuf, uint64_t *out);
bool          cc_rbuf_is_empty      (CC_Rbuf *rbuf);
size_t        cc_rbuf_size          (CC_Rbuf *rbuf);
void          cc_rbuf_destroy       (CC_Rbuf *rbuf);
uint64_t      cc_rbuf_peek          (CC_Rbuf *rbuf, int index);


#endif
