//
// Created by lzt11 on 2022/7/18.
//

#ifndef SOCKET_EPOLL_CORE_H
#define SOCKET_EPOLL_CORE_H

#include "lib.h"
#include "cycle_module.h"

int epoll_fd_create(cycle_t *cycle);

void epoll_add_listening(int *epoll_fd);

#define epoll_add(epoll_fd,socket_fd,ev) \
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd,ev);

#define epoll_remove(epoll_fd,socket_fd,ev) \
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,socket_fd,ev);

void worker_cycle(int *epoll_fd);
void master_cycle();

#endif //SOCKET_EPOLL_CORE_H
