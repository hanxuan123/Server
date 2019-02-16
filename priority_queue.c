#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

void exch(han_pq_t *han_pq, size_t i, size_t j){
    void *tmp = han_pq->pq[i];
    han_pq->pq[i] = han_pq->pq[j];
    han_pq->pq[j] = tmp;
}

void swim(han_pq_t *han_pq, size_t k){
    while (k > 1 && han_pq->comp(han_pq->pq[k], han_pq->pq[k/2])){
        exch(han_pq, k, k/2);
        k /= 2;
    }
}

int sink(han_pq_t *han_pq, size_t k){
    size_t j;
    size_t nalloc = han_pq->nalloc;
    while((k << 1) <= nalloc){
        j = k << 1;
        if((j < nalloc) && (han_pq->comp(han_pq->pq[j+1], han_pq->pq[j])))
            j++;

        if(!han_pq->comp(han_pq->pq[j], han_pq->pq[k]))
            break;

        exch(han_pq, j, k);
        k = j;
    }
    return k;
}

int han_pq_sink(han_pq_t *han_pq, size_t i){
    return sink(han_pq, i);
}

int han_pq_init(han_pq_t *han_pq, han_pq_comparator_pt comp, size_t size){
    // 为han_pq_t节点的pq分配(void *)指针
    han_pq->pq = (void **)malloc(sizeof(void *) * (size + 1));
    if (!han_pq->pq)
        return -1;

    han_pq->nalloc = 0;
    han_pq->size = size + 1;
    han_pq->comp = comp;
    return 0;
}

int han_pq_is_empty(han_pq_t *han_pq){
    // 通过nalloc值款快速判断是否为空
    return (han_pq->nalloc == 0) ? 1 : 0;
}

size_t han_pq_size(han_pq_t *han_pq){
    // 获取优先队列大小
    return han_pq->nalloc;
}

void *han_pq_min(han_pq_t *han_pq){
    // 优先队列最小值直接返回第一个元素（指针）
    if (han_pq_is_empty(han_pq))
        return (void *)(-1);

    return han_pq->pq[1];
}


int resize(han_pq_t *han_pq, size_t new_size){
    if(new_size <= han_pq->nalloc)
        return -1;

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if(!new_ptr)
        return -1;
    // 将原本nalloc + 1个元素值拷贝到new_ptr指向的位置
    memcpy(new_ptr, han_pq->pq, sizeof(void *) * (han_pq->nalloc + 1));
    // 释放旧元素
    free(han_pq->pq);
    // 重新改写优先队列元素pq指针为new_ptr
    han_pq->pq = new_ptr;
    han_pq->size = new_size;
    return 0;
}

int han_pq_delmin(han_pq_t *han_pq){
    if(han_pq_is_empty(han_pq))
        return 0;

    exch(han_pq, 1, han_pq->nalloc);
    --han_pq->nalloc;
    sink(han_pq, 1);
    if((han_pq->nalloc > 0) && (han_pq->nalloc <= (han_pq->size - 1)/4)){
        if(resize(han_pq, han_pq->size / 2) < 0)
            return -1;
    }
    return 0;
}

int han_pq_insert(han_pq_t *han_pq, void *item){
    if(han_pq->nalloc + 1 == han_pq->size){
        if(resize(han_pq, han_pq->size * 2) < 0){
            return -1;
        }
    }
    han_pq->pq[++han_pq->nalloc] = item;
    swim(han_pq, han_pq->nalloc);
    return 0;
}



