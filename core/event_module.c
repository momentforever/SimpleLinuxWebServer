//
// Created by lzt11 on 2022/5/13.
//

#include "event_module.h"

event_t* event_add(unsigned rw,event_handler_pt handler,time_t ms){
    //ev->data = (void*)s;
    event_t *ev;
    ev = malloc(sizeof(event_t));
    if(ev == NULL){
        return NULL;
    }

    ev->rw = rw;
    ev->active = 0;
    ev->ready = 0;
    ev->error = 0;
    ev->eof = 0;
    ev->accept = 0;

    ev->handler = handler;

    return ev;
}

int event_delete(event_t *ev){

    free(ev);
    return OK;
}



