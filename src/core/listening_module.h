//
// Created by lzt11 on 2022/5/17.
//

#ifndef SOCKET_LISTENING_MODULE_H
#define SOCKET_LISTENING_MODULE_H

#include "lib.h"
#include "common.h"
#include "stl.h"


struct listening_s{
    int fd;
    int fd_type;
    unsigned int type;
    // format address
    // char* serv_addr_str;
    int port;
    char* ip;

    struct sockaddr* serv_addr;
    // int serv_addr_size;
    config_node_t* cn;
    connection_handler_pt handler;//do udp/tcp/http create connection

    unsigned listen:1;
};

list_t* listenings_create(config_t *c);

void accept_tcp(connection_t *c);
void accept_udp(connection_t *c);


#endif //SOCKET_LISTENING_MODULE_H
