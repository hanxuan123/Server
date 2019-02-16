#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"

#define MAXEVENTS 1024

int han_epoll_create(int flags);
int han_epoll_add(int epoll_fd, int fd, han_http_request_t* request, int events);
int han_epoll_mod(int epoll_fd, int fd, han_http_request_t* request, int events);
int han_epoll_del(int epoll_fd, int fd, han_http_request_t* request, int events);
int han_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
void han_handle_events(int epoll_fd, int listen_fd, struct epoll_event* events,
                      int events_num, char* path, han_threadpool_t* tp);

#endif
