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

int main(int argc,char *argv[]) {
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
        worker_cycle(&g_epoll_fd);
    }

    return 0;
}
