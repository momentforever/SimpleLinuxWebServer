//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_EVENT_MODULE_H
#define SOCKET_EVENT_MODULE_H

#include "lib.h"
#include "common.h"

//typedef struct event_s event_t;
typedef void (*event_handler_pt)(event_t *ev);

struct event_s{
    void* data;

    unsigned write:1; //write/read
    unsigned read:1;
    unsigned active:1;
    unsigned ready:1;
    unsigned error:1;
    unsigned eof:1;
    unsigned accept:1;

//    unsigned timer_set:1;
//    unsigned timer_out:1;

    event_handler_pt handler;
};

void event_init(event_t *ev);

#endif //SOCKET_EVENT_MODULE_H
