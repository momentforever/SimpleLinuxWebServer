//
// Created by lzt11 on 2022/5/17.
//

#ifndef SOCKET_LISTENING_MODULE_H
#define SOCKET_LISTENING_MODULE_H

#include "lib.h"
#include "common.h"
#include "stl.h"

typedef struct listening_s listening_t;

struct listening_s{
    int fd;
    int fd_type;
    int type;
    // format address
    // char* serv_addr_str;
    int port;
    char* ip;

    struct sockaddr* serv_addr;
    // int serv_addr_size;

    void* handler;//do udp/tcp/http create connection
    unsigned listen:1;
};

list_t* listenings_create(config_node_t *config);

#endif //SOCKET_LISTENING_MODULE_H
