#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

#define han_PQ_DEFAULT_SIZE 10

typedef int (*han_pq_comparator_pt)(void *pi, void *pj);

typedef struct priority_queue{
    void **pq;
    size_t nalloc;
    size_t size;
    han_pq_comparator_pt comp;
}han_pq_t;

int han_pq_init(han_pq_t *han_pq, han_pq_comparator_pt comp, size_t size);
int han_pq_is_empty(han_pq_t *han_pq);
size_t han_pq_size(han_pq_t *han_pq);
void *han_pq_min(han_pq_t *han_pq);
int han_pq_delmin(han_pq_t *han_pq);
int han_pq_insert(han_pq_t *han_pq, void *item);
int han_pq_sink(han_pq_t *han_pq, size_t i);

#endif 
