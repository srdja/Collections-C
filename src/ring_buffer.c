/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T10:32:56-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-07T11:28:59-06:00
 */

#include "include/ring_buffer.h"


enum cc_stat rbuf_new(Rbuf **rbuf)
{
  RbufConf conf;
  rbuf_conf_init(&conf);
  return rbuf_new_conf(&conf, rbuf);
}

enum cc_stat rbuf_conf_new(RbufConf *rconf, Rbuf **rbuf)
{
  Rbuf *ringbuf = rconf->mem_alloc(sizeof(Rbuf));
  if (!ringbuf)
     return CC_ERR_ALLOC;
  if (!(ringbuf->buf = rconf->mem_calloc(DEFAULT_RBUF_CAPACITY, sizeof(Rbuf)))) {
    rconf->mem_free(ringbuf);
    return CC_ERR_ALLOC;
  }

  for (int i = 0; i < DEFAULT_RBUF_CAPACITY; i++)
      ringbuf->buf[i] = 0x0;

  ringbuf->mem_alloc = rconf->mem_alloc;
  ringbuf->mem_calloc = rconf->mem_calloc;
  ringbuf->mem_free = rconf->mem_free;
  ringbuf->capacity = DEFAULT_RBUF_CAPACITY;
  ringbuf->head = 0, ringbuf->tail = 0;
  *rbuf = ringbuf;
  return CC_OK;
}

enum cc_stat rbuf_conf_init(RbufConf *rconf)
{
  rconf->capacity = DEFAULT_RBUF_CAPACITY;
  rconf->mem_alloc = malloc;
  rconf->mem_calloc = calloc;
  rconf->mem_free = free;
}

void rbuf_destroy(Rbuf *rbuf)
{
  rbuf->mem_free(rbuf->buf);
  rbuf->mem_free(rbuf);
}

int rbuf_is_empty(Rbuf *rbuf)
{
  return (rbuf->head == rbuf->tail);
}

enum cc_stat rbuf_enqueue(Rbuf *rbuf, uint64_t item)
{
  if (!rbuf)
     return CC_ERR_ALLOC;
  rbuf->buf[rbuf->head] = item;
  rbuf->head = (rbuf->head + 1) % rbuf->capacity;
  return CC_OK;
}

enum cc_stat rbuf_dequeue(Rbuf *rbuf, uint64_t *out)
{
  if (!rbuf)
     return CC_ERR_ALLOC;
  if (!rbuf_is_empty(rbuf))
     return CC_MAX_ELEMENTS;
  *out = rbuf->buf[rbuf->tail];
  rbuf->buf[rbuf->tail] = 0x0;
  rbuf->tail = (rbuf->tail + 1) % rbuf->capacity;
}
