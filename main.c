// 1.实现一个简单的web服务器,通过epoll
// TODO 多线程 × 多进程 √
// 2.尝试实现一个非阻塞的web服务器
// 3.尝试实现一个异步的web服务器
// 3.1.定时器
// TODO 企图引入协程

#include "core.h"
#include "lib.h"
#include "common.h"
#include "auto.h"

#include "self_httpd.h"

int main(int argc,char *argv[]){

    module_init(modules);

    g_cycle = cycle_create(128);
    g_epoll_fd = epoll_fd_create(g_cycle);
    g_timer_tree = timer_tree_create();

    epoll_add_listening(&g_epoll_fd);

    epoll_cycle(&g_epoll_fd);

    return 0;

// --------------------------------------------------------------

//    struct epoll_event ev,events[MAX_EVENT_NUM];
//    //创建epoll实例
//    int epoll_fd = epoll_create(MAX_EVENT_NUM);
//
//    list_node_t *node;
//    for(node=config_list->begin;node!=NULL;node=node->next){
//        config_ptr = (config_t*)node->value;
//
//        struct sockaddr_in server_addr;
//        int serv_fd = 0;
//        serv_fd = socket(AF_INET,SOCK_STREAM,0);
//
//        config_ptr->fd = serv_fd;
//
//        //更改listen_fd属性
//        //设置为非阻塞
//        fcntl(serv_fd,F_SETFL,O_NONBLOCK);
//
//        // debug
//        //int on = 1;
//        //setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on));
//
//        bzero(&server_addr, sizeof(server_addr));
//        server_addr.sin_family = AF_INET;
//        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//        server_addr.sin_port = htons(config_ptr->port);
//        if(bind(serv_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))== -1){
//            perror("bind error, message: ");
//            exit(1);
//        }
//        if(listen(serv_fd,SOMAXCONN)==-1){
//            perror("listen error, message: ");
//            exit(1);
//        }
//        printf("listening %d ......\n",config_ptr->port);
//
//        ev.data.fd = serv_fd;
//        ev.events = EPOLLIN;
//        //将listen_df注册到刚刚创建的epoll中
//        if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,serv_fd,&ev)==-1) {
//            perror("epoll_ctl error, message: ");
//            exit(1);
//        }
//    }
//
//    //创建socket
//    int ready_fd_num;
//    struct sockaddr_in client_addr;
//    int client_addr_size = sizeof(client_addr);
//    char buf[128];
//
//    while(1){
//        ready_fd_num = epoll_wait(epoll_fd,events,MAX_EVENT_NUM,ETERNITY);
//
//        if(ready_fd_num == -1){
//            perror("epoll_wait error, message: ");
//            continue;
//        }
//        int i;
//        for(i = 0; i < ready_fd_num;i++){
//            int flag = FALSE;
//            for(node=config_list->begin;node!=NULL;node=node->next){
//                if(events[i].data.fd == ((config_t*)node->value)->listen_fd){
//                    flag = TRUE;
//                    break;
//                }
//            }
//            if(flag==TRUE){
//                int conn_fd = accept(events[i].data.fd,(struct sockaddr *)&client_addr, &client_addr_size);
//                if(conn_fd == -1){
//                    sprintf(buf,"[pid %d] accept wrong :",getpid());
//                    perror(buf);
//                    continue;
//                }
//                ev.data.fd = conn_fd;
//                ev.events = EPOLLIN | EPOLLET;
//                epoll_ctl(epoll_fd,EPOLL_CTL_ADD,conn_fd,&ev);
//                printf("connected client : %d \n",conn_fd);
//
//            }else if(events[i].events & EPOLLIN){
//                printf("[pid %d] receive %s:%d message\n",getpid(), inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
//
//                request_handler(&events[i].data.fd);
//
//            }else if(events[i].events & EPOLLERR){
//                fprintf(stderr,"epoll error\n");
//                close(events[i].data.fd);
//            }
//        }
//    }

}
