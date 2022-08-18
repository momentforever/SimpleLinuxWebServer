//
// Created by lzt11 on 2022/7/18.
//

#include "epoll_module.h"

#include "cycle_module.h"
#include "listening_module.h"
#include "connection_module.h"

int epoll_fd_create(cycle_t *cycle){
    return epoll_create(cycle->connection_n);
}

void epoll_add_listening(int *epoll_fd){
    struct epoll_event ev;

    listening_t *listening;
    list_node_t *node;

    connection_t *conn;

    //TODO throw listenings not init
    for(node=g_cycle->listenings->begin;node!=NULL;node=node->next){
        listening = (listening_t*)node->value;

        listening->fd = socket(AF_INET,listening->fd_type,0);

        //更改listen_fd属性
        //设置为非阻塞
        fcntl(listening->fd,F_SETFL,O_NONBLOCK);

        if(bind(listening->fd,listening->serv_addr,sizeof(struct sockaddr))== -1){
            perror("bind error, message: ");
            exit(1);
        }
        if(listen(listening->fd,SOMAXCONN)==-1){
            perror("listen error, message: ");
            exit(1);
        }

        conn = get_free_connection(g_cycle);

        conn->listening = listening;
        conn->fd = listening->fd;

        ev.data.ptr = conn;
        ev.data.fd = listening->fd;
        ev.events = EPOLLIN;

        //将listen_df注册到刚刚创建的epoll中
        if(epoll_ctl(g_epoll_fd,EPOLL_CTL_ADD,ev.data.fd,&ev)==-1) {
            perror("epoll_ctl error, message: ");
            exit(1);
        }
    }
}


_Noreturn void epoll_cycle(int *epoll_fd){

    struct epoll_event ev,events[g_cycle->connection_n];
    int ready_fd_num;
    int i;
    connection_t *ev_conn;
    connection_t *new_conn;
    timer_node_t *tn;
    int wait;
    debug("start socket!\n");

    for(;;){
FIND_TIMER:
        //TODO UNBLOCK Timer
        update_time();

        wait = ETERNITY;
        tn = timer_tree_get_min_time(g_timer_tree);

        if(tn != NULL){
            if(tn->timed_set == ON && tn->timed_out == ON){
                // TODO handler timeout

                timer_tree_timer_delete(g_timer_tree,tn);
                //find timer again
                goto FIND_TIMER;
            }else{
                wait = get_time_interval(tn);
            }
        }

        debug("waiting time -> %d\n",wait);
        ready_fd_num = epoll_wait(g_epoll_fd,events,(int)g_cycle->connection_n,wait);
        if(ready_fd_num == 0){
            //handler timeout
        }
        else{
            for(i = 0; i < ready_fd_num;i++){
                ev_conn = (connection_t*)events[i].data.ptr;

                if(ev_conn->fd == ev_conn->listening->fd){

                    int conn_fd = accept(events[i].data.fd,(struct sockaddr *)&ev_conn->clnt_addr, &ev_conn->clnt_addr_size);
                    if(conn_fd == -1){
                        perror("accept error.");
                        continue;
                    }
                    //create connection
                    new_conn = get_free_connection(g_cycle);

                    ev.data.fd = conn_fd;
                    // 边缘触发
                    ev.events = EPOLLIN | EPOLLET;

                    epoll_ctl(g_epoll_fd,EPOLL_CTL_ADD,conn_fd,&ev);
                    printf("connected client : %d \n",conn_fd);

                }else if(events[i].events & EPOLLIN){
                    //request_handler(&events[i].data.fd);
                    ev_conn->read->handler(ev_conn->read);
                }else if(events[i].events & EPOLLOUT){
                    ev_conn->write->handler(ev_conn->write);
                }
                else if(events[i].events & EPOLLERR){
                    fprintf(stderr,"epoll error\n");
                    close(events[i].data.fd);
                }
            }
        }
    }
}

