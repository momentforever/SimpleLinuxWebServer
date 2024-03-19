//
// Created by lzt11 on 2022/7/21.
//
#include "lib.h"

const char g_version[] = "0.0.1";
char g_conf_filepath[PATH_MAX];
char g_work_dir[PATH_MAX];

int g_argc;
char **g_argv;

int g_epoll_fd = -1;

int g_process_type = MASTER;

timer_rbtree_t *g_event_timer_tree;
cycle_t *g_cycle;
