//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_CONNECTION_MODULE_H
#define SOCKET_CONNECTION_MODULE_H

#include "lib.h"
#include "listening_module.h"


typedef struct event_s event_t;
typedef struct connection_s connection_t;

struct connection_s{
    void* data;
    int fd;

    // format address
    //char* clnt_addr_str;
    struct sockaddr clnt_addr;
    unsigned int clnt_addr_size;

    listening_t* listening;

    event_t *write;
    event_t *read;
};


#endif //SOCKET_CONNECTION_MODULE_H
