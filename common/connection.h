//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

#include "global.h"
#include "event.h"
#include "listening.h"

typedef struct connection_s connection_t;

struct connection_s{
    void* data;

    //char* serv_addr_str;
    //char* clnt_addr_str;

    struct sockaddr* serv_addr;
    struct sockaddr* clnt_addr;
    event_t *write;
    event_t *read;
    listening_t* listening;
    int fd;
    int type;
};



#endif //SOCKET_CONNECTION_H
