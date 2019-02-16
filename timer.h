//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef han_TIMER_H
#define han_TIMER_H

#include "priority_queue.h"
#include "http_request.h"

#define TIMEOUT_DEFAULT 500     /* ms */

// 函数指针，负责超时处理，han_add_timer时指定处理函数
typedef int (*timer_handler_pt)(han_http_request_t* request);

typedef struct han_timer{
    size_t key;    // 标记超时时间
    int deleted;    // 标记是否被删除
    timer_handler_pt handler;    // 超时处理，add时指定
    han_http_request_t* request;    // 指向对应的request请求
} han_timer_t;

// han_pq_t定义在"priority_queue.h"中，优先队列中节点
extern han_pq_t han_timer;
extern size_t han_current_msec;

int han_timer_init();
int han_find_timer();
void han_handle_expire_timers();
void han_add_timer(han_http_request_t* request, size_t timeout, timer_handler_pt handler);
void han_del_timer(han_http_request_t* request);
int timer_comp(void *ti, void *tj);

#endif