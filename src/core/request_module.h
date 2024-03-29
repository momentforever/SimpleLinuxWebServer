//
// Created by lzt11 on 2022/10/20.
//

#ifndef SOCKET_REQUEST_MODULE_H
#define SOCKET_REQUEST_MODULE_H

#include "common.h"

typedef struct request_s request_t;

struct request_s{
    config_t *conf;
    memory_pool_t *pool;
    char* header_in;

    char* uri;
    char* args;
    char* exten;
    char* unparsed_uri;

};

#endif //SOCKET_REQUEST_MODULE_H
