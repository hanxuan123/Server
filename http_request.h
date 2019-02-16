#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "list.h"

#define han_AGAIN EAGAIN

#define han_HTTP_PARSE_INVALID_METHOD        10
#define han_HTTP_PARSE_INVALID_REQUEST       11
#define han_HTTP_PARSE_INVALID_HEADER        12

#define han_HTTP_UNKNOWN                     0x0001
#define han_HTTP_GET                         0x0002
#define han_HTTP_HEAD                        0x0004
#define han_HTTP_POST                        0x0008

#define han_HTTP_OK                          200
#define han_HTTP_NOT_MODIFIED                304
#define han_HTTP_NOT_FOUND                   404
#define MAX_BUF 8124

typedef struct han_http_request{
    char* root;
    int fd;
    int epoll_fd;
    char buff[MAX_BUF];
    size_t pos;
    size_t last;
    int state;

    void* request_start;
    void* method_end;
    int method;
    void* uri_start;
    void* uri_end;
    void* path_start;
    void* path_end;
    void* query_start;
    void* query_end;
    int http_major;
    int http_minor;
    void* request_end;

    struct list_head list;    // 存储请求头，list.h中定义了此结构

    void* cur_header_key_start;
    void* cur_header_key_end;
    void* cur_header_value_start;
    void* cur_header_value_end;
    void* timer;
}han_http_request_t;

typedef struct han_http_out{
    int fd;
    int keep_alive;
    time_t mtime;
    int modified;
    int status;
}han_http_out_t;

typedef struct han_http_header{
    void* key_start;
    void* key_end;
    void* value_start;
    void* value_end;
    struct list_head list;
}han_http_header_t;

typedef int (*han_http_header_handler_pt)(han_http_request_t* request, han_http_out_t* out, char* data, int len);

typedef struct han_http_header_handle{
    char* name;
    han_http_header_handler_pt handler;    // 函数指针
}han_http_header_handle_t;

extern han_http_header_handle_t han_http_headers_in[];

void han_http_handle_header(han_http_request_t* request, han_http_out_t* out);
int han_http_close_conn(han_http_request_t* request);
int han_init_request_t(han_http_request_t* request, int fd, int epoll_fd, char* path);
int han_init_out_t(han_http_out_t* out, int fd);
const char* get_shortmsg_from_status_code(int status_code);

#endif