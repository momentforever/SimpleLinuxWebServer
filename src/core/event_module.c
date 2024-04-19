//
// Created by lzt11 on 2022/5/13.
//

#include "event_module.h"


void event_init(event_t *ev){
    ev->write = 0;
    ev->read = 0;
    ev->active = 0;
    ev->ready = 0;
    ev->error = 0;
    ev->eof = 0;
    ev->accept = 0;
    ev->handler = NULL;
    ev->data = NULL;
}

