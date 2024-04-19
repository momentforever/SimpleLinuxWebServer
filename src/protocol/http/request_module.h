//
// Created by lzt11 on 2022/10/20.
//

#ifndef SOCKET_REQUEST_MODULE_H
#define SOCKET_REQUEST_MODULE_H

#include "common.h"
#include "lib.h"
#include "memory.h"

typedef struct request_conf_s request_conf_t;
typedef struct http_request_s http_request_t;

struct http_request_conf_s{

};

struct http_request_s{
    config_node_t *conf;
    memory_pool_t *pool;
    connection_t *conn;
    // FILE *clnt_read;
    // FILE *clnt_write;
    char* header_in;
    char* uri;
    char* args;
    char* exten;
    char* unparsed_uri;
};

#endif //SOCKET_REQUEST_MODULE_H
