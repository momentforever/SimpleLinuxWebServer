//
// Created by lzt11 on 2022/6/16.
//

#ifndef SOCKET_TIMER_H
#define SOCKET_TIMER_H

#include "lib.h"
#include "stl.h"
// ms
#define TIMER_LAZY_DELAY 300

struct timer_rbtree_s{
    rbtree_t rbtree;
    rbtree_node_t sentinel;
};

typedef void (*timer_handler_pt)(void* data);

struct timer_node_s{
    void *data;
    timer_handler_pt handler;

    unsigned timed_out:1;
    time_t start_time;
    time_t timeout_time;
    rbtree_node_t rbtree_node;
};

typedef struct timer_node_s timer_node_t;

timer_rbtree_t* timer_tree_create();
int timer_tree_init(timer_rbtree_t *timer_tree);

timer_node_t *timer_tree_timer_create(timer_rbtree_t *timer_tree,timer_handler_pt handler, time_t ms);

int timer_tree_timer_update(timer_rbtree_t *timer_tree,timer_node_t *timer,time_t ms);

void timer_tree_timer_remove(timer_rbtree_t *timer_tree,timer_node_t *timer);
void timer_tree_timer_delete(timer_rbtree_t *timer_tree,timer_node_t *timer);

//void rec_timer_set_timed_out(rbtree_node_t *node,rbtree_node_t *sentinel);

int timer_tree_update(timer_rbtree_t *timer_tree);

//timer_node_t *timer_tree_get_least_time(timer_rbtree_t *timer_tree);
timer_node_t *timer_tree_get_min_timer(timer_rbtree_t *timer_tree);

void timer_tree_delete(timer_rbtree_t *timer_tree);

#define get_time_interval(tn) (int)((tn)->timeout_time - current_monotonic_timestamp)

#endif //SOCKET_TIMER_H
