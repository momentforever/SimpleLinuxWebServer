//
// Created by lzt11 on 2022/5/17.
//

#ifndef SOCKET_LISTENING_H
#define SOCKET_LISTENING_H

#include "global.h"

typedef struct listening_s listening_t;

struct listening_s{
    int fd;
    int type;
    void* handler;
    unsigned listen:1;
    struct sockaddr *serv_addr;
};



#endif //SOCKET_LISTENING_H
