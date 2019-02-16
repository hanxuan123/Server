//
// Latest edit by TeeKee on 2017/7/23.
//


#include <sys/time.h>
#include "timer.h"

han_pq_t han_timer;
size_t han_current_msec;

int timer_comp(void* ti, void* tj){
    han_timer_t* timeri = (han_timer_t*)ti;
    han_timer_t* timerj = (han_timer_t*)tj;
    return (timeri->key < timerj->key) ? 1 : 0;
}

void han_time_update(){
    // 获取当前时间
    struct timeval tv;
    int rc = gettimeofday(&tv, NULL);
    han_current_msec = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int han_timer_init(){
    // 建立连接后立即初始化
    // 初始优先队列大小han_PQ_DEFAULT_SIZE = 10
    int rc = han_pq_init(&han_timer, timer_comp, han_PQ_DEFAULT_SIZE);

    // 更新当前时间
    han_time_update();
    return 0;
}

int han_find_timer(){
    int time;
    // 返回队列中最早时间和当前时间之差
    while(!han_pq_is_empty(&han_timer)){
        // 更新当前时间
        han_time_update();
        // timer_node指向最小的时间
        han_timer_t* timer_node = (han_timer_t*)han_pq_min(&han_timer);
        // 如果已删则释放此节点（han_del_timer只置位不删除）
        if(timer_node->deleted){
            int rc = han_pq_delmin(&han_timer);
            free(timer_node);
            continue;
        }
        // 此时timer_node为时间最小节点，time为优先队列里最小时间减去当前时间
        time = (int)(timer_node->key - han_current_msec);
        time = (time > 0) ? time : 0;
        break;
    }
    return time;
}

void han_handle_expire_timers(){
    while(!han_pq_is_empty(&han_timer)){
        // 更新当前时间
        han_time_update();
        han_timer_t* timer_node = (han_timer_t*)han_pq_min(&han_timer);
        // 如果已删则释放此节点
        if(timer_node->deleted){
            int rc = han_pq_delmin(&han_timer); 
            free(timer_node);
            continue;
        }
        // 最早入队列节点超时时间大于当前时间（未超时）
        // 结束超时检查，顺带删了下标记为删除的节点
        if(timer_node->key > han_current_msec){
            return;
        }
        // 出现了没被删但是超时的情况，调用handler处理
        if(timer_node->handler){
            timer_node->handler(timer_node->request);
        }
        int rc = han_pq_delmin(&han_timer); 
        free(timer_node);
    }
}

void han_add_timer(han_http_request_t* request, size_t timeout, timer_handler_pt handler){
    han_time_update();
    // 申请新的han_timer_t节点，并加入到han_http_request_t的timer下
    han_timer_t* timer_node = (han_timer_t*)malloc(sizeof(han_timer_t));
    request->timer = timer_node;
    // 加入时设置超时阈值，删除信息等
    timer_node->key = han_current_msec + timeout;
    timer_node->deleted = 0;
    timer_node->handler = handler;
    // 注意需要在han_timer_t节点中反向设置指向对应resquest的指针
    timer_node->request = request;
    // 将新节点插入优先队列
    int rc = han_pq_insert(&han_timer, timer_node);
}

void han_del_timer(han_http_request_t* request) {
    han_time_update();
    han_timer_t* timer_node = request->timer;
    // 惰性删除
    // 标记为已删，在find_timer和handle_expire_timers检查队列时会删除
    timer_node->deleted = 1;
}