//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_EVENT_H
#define SOCKET_EVENT_H

#include "global.h"

typedef struct event_s event_t;

struct event_s{
    void* data;
    void* handler;
    unsigned write:1;
    unsigned active:1;
    unsigned ready:1;
    unsigned timed_out:1;
    unsigned timer_set:1;
    //timer
    rbtree_node_t timer;
};

#endif //SOCKET_EVENT_H
