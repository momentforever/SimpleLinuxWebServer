//
// Created by lzt11 on 2022/7/21.
//
#include "lib.h"

int g_epoll_fd = -1;

timer_rbtree_t *g_timer_tree;
cycle_t *g_cycle;
