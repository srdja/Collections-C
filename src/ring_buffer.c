/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T10:32:56-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-09T15:23:25-06:00
 */

#include "include/ring_buffer.h"

struct ring_buffer_conf {

   size_t capacity;
   void *(*mem_alloc)  (size_t size);
   void *(*mem_calloc) (size_t blocks, size_t size);
   void  (*mem_free)   (void *block);

};

struct ring_buffer {

    size_t size, capacity;
    size_t head, tail;
    uint64_t *buf;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);

};

enum cc_stat rbuf_new(Rbuf **rbuf)
{
    RbufConf conf;
    rbuf_conf_init(&conf);
    return rbuf_conf_new(&conf, rbuf);
}

enum cc_stat rbuf_conf_new(RbufConf *rconf, Rbuf **rbuf)
{
    Rbuf *ringbuf = rconf->mem_calloc(1, sizeof(Rbuf));
    if (!ringbuf)
       return CC_ERR_ALLOC;
    if (!(ringbuf->buf = rconf->mem_calloc(rconf->capacity, sizeof(uint64_t)))) {
      rconf->mem_free(ringbuf);
      return CC_ERR_ALLOC;
    }

    ringbuf->mem_alloc = rconf->mem_alloc;
    ringbuf->mem_calloc = rconf->mem_calloc;
    ringbuf->mem_free = rconf->mem_free;
    ringbuf->capacity = rconf->capacity;
    ringbuf->size = 0;
    ringbuf->head = 0, ringbuf->tail = 0;
    *rbuf = ringbuf;
    return CC_OK;
}

void rbuf_conf_init(RbufConf *rconf)
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

bool rbuf_is_empty(Rbuf *rbuf)
{
    return (rbuf->size == 0);
}

size_t rbuf_size(Rbuf *rbuf)
{
  return rbuf->size;
}

void rbuf_enqueue(Rbuf *rbuf, uint64_t item)
{
    if (rbuf->head == rbuf->tail)
       rbuf->tail = (rbuf->tail + 1) % rbuf->capacity;
    rbuf->buf[rbuf->head] = item;
    rbuf->head = (rbuf->head + 1) % rbuf->capacity;
    if (rbuf->size < rbuf->capacity)
       ++rbuf->size;
}

enum cc_stat rbuf_dequeue(Rbuf *rbuf, uint64_t *out)
{
    if (rbuf_is_empty(rbuf))
       return CC_ERR_OUT_OF_RANGE;
    *out = rbuf->buf[rbuf->tail];
    rbuf->tail = (rbuf->tail + 1) % rbuf->capacity;
    --rbuf->size;
    return CC_OK;
}

uint64_t rbuf_peek(Rbuf *rbuf, int index)
{
    return rbuf->buf[index];
}
