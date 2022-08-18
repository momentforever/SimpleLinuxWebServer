//
// Created by lzt11 on 2022/5/13.
//

#include "event_module.h"

timer_rbtree_t *event_timer_tree;

event_t* event_init(event_t *ev,unsigned rw,event_handler_pt handler,time_t ms){
    //ev->data = (void*)s;
    ev->rw = rw;
    ev->active = 0;
    ev->ready = 0;
    ev->error = 0;
    ev->eof = 0;

    ev->handler = handler;

    timer_tree_timer_add(event_timer_tree,&ev->timer,ms);

    return ev;
}

void update_event_timer(event_t *ev,time_t ms){
    timer_tree_timer_add(event_timer_tree,&ev->timer,ms);
}


