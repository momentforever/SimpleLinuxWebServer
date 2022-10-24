// 1.实现一个简单的web服务器,通过epoll √
// TODO 多线程 × 多进程 √
// 2.尝试实现一个非阻塞的web服务器 √
// 3.尝试实现一个异步的web服务器
// 3.1.定时器 √
// 3.2 引入内存管理（内存池） √
// TODO 企图引入协程

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

    if(getpid() == 0){
        //master
        debug("master");
        while(1);
    }else{
        //worker
        debug("worker");
        epoll_cycle(&g_epoll_fd);
    }

    return 0;
}
