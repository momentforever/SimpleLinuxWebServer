//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_EVENT_MODULE_H
#define SOCKET_EVENT_MODULE_H

#include "lib.h"
#include "common.h"

typedef struct connection_s connection_t;

typedef struct event_s event_t;
typedef void (*event_handler_pt)(event_t *ev);

struct event_s{
    void* data;

    unsigned rw:1; //write/read

    unsigned active:1;
    unsigned ready:1;
    unsigned error:1;
    unsigned eof:1;

    event_handler_pt handler;
    //timer
    timer_node_t timer;
};

event_t* event_init(event_t *ev,unsigned rw,event_handler_pt handler,time_t ms);


#endif //SOCKET_EVENT_MODULE_H
