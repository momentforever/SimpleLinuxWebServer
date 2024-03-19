// 一个简单的web服务器,通过epoll
// 多线程 × 多进程 √
// 一个非阻塞的web服务器
// 一个异步的web服务器
// 添加定时器
// 引入内存管理（内存池）
// TODO 企图引入协程
// TODO 日志
// TODO 信号

#include "core.h"
#include "lib.h"
#include "common.h"
#include "auto.h"
#include <bits/getopt_core.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arg_parser(int argc,char *argv[]){
    // 初始化
    memset(g_work_dir, 0, sizeof(g_work_dir));
    if (getcwd(g_work_dir, sizeof(g_work_dir)) != NULL) {
        printf("Current working directory: %s\n", g_work_dir);
    } else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    memset(g_conf_filepath, 0, sizeof(g_conf_filepath));
    memcpy(g_conf_filepath, "./config.json", strlen("./config.json") + 1);

    // 解析命令行选项和参数
    int opt;
    while ((opt = getopt(argc, argv, "vhc:d:")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: %s [-v] [-h] [-c config_file] [-d work_dir]\n", argv[0]);
                exit(EXIT_SUCCESS);
            case 'v':
                printf("Version is %s\n", g_version);
                exit(EXIT_SUCCESS);
            case 'c':
                memset(g_conf_filepath, 0, sizeof(g_conf_filepath));
                memcpy(g_conf_filepath, optarg, strlen(optarg) + 1);
                debugln("config path: %s", g_conf_filepath);
                break;
            case 'd':
                memset(g_work_dir, 0, sizeof(g_work_dir));
                memcpy(g_work_dir, optarg, strlen(optarg) + 1);
                debugln("work dir: %s, %ld", g_work_dir, sizeof(g_work_dir));
                break;
            case '?': // 用于未识别的选项
                printf("Usage: %s [-v] [-h] [-c config_file] [-d work_dir]\n", argv[0]);
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    // 任何非选项的参数都可以在这里处理
    for (int index = optind; index < argc; index++) {
        printf("Non-option argument %s\n", argv[index]);
    }
    
    return 0;
}

int main(int argc,char *argv[]) {
    
    arg_parser(argc,argv);

    //TODO env func
    g_argc = argc;
    g_argv = argv;

    module_init(modules);
    config_t *c = config_create();
    g_cycle = cycle_create(c);

    g_epoll_fd = epoll_fd_create(g_cycle);
    g_event_timer_tree = timer_tree_create();

    epoll_add_listening(&g_epoll_fd);

    //process fork
    cycle_process_fork(g_cycle);

    if(g_process_type == MASTER){
        //master
        master_cycle();
    }

    if(g_process_type == WORKER){
        worker_cycle();
    }

    return 0;
}
