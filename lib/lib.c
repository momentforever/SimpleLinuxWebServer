//
// Created by lzt11 on 2022/7/21.
//
#include "lib.h"

int g_epoll_fd = -1;

int g_process_type = MASTER;

timer_rbtree_t *g_event_timer_tree;
cycle_t *g_cycle;
