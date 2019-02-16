#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#define CR '\r'
#define LF '\n'

// http请求行解析
int han_http_parse_request_line(han_http_request_t *request);
// http请求体解析
int han_http_parse_request_body(han_http_request_t *request);

#endif
