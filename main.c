// TODO 1.实现一个简单的web服务器,通过epoll finish
// TODO 2.尝试实现一个非阻塞的web服务器
// TODO 3.尝试实现一个异步的web服务器
// TODO 3.1.定时器

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#include <sys/epoll.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <httpd.h>
#include <self_httpd.h>

#define DEFAULT_PORT 10008
#define MAX_EVENT_NUM 1024
#define INFTIM (-1)

int main(int argc,char *argv[]){
    struct sockaddr_in server_addr;
    int listen_fd = 0;
    int cpu_core;
    int on = 1;
    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    //更改listen_fd属性
    //fcntl(listen_fd,F_SETFL,O_NONBLOCK);

    //debug
    //setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DEFAULT_PORT);
    if(bind(listen_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))== -1){
        perror("bind error, message: ");
        exit(1);
    }
    if(listen(listen_fd,SOMAXCONN)==-1){
        perror("listen error, message: ");
        exit(1);
    }
    printf("listening %d ......\n",DEFAULT_PORT);

    int conn_fd;
    int ready_fd_num;
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    char buf[128];

    struct epoll_event ev,events[MAX_EVENT_NUM];

    //创建epoll实例
    int epoll_fd = epoll_create(MAX_EVENT_NUM);
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN;

    //将listen_df注册到刚刚创建的epoll中
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&ev)==-1){
        perror("epoll_ctl error, message: ");
        exit(1);
    }
    while(1){
        ready_fd_num = epoll_wait(epoll_fd,events,MAX_EVENT_NUM,INFTIM);
        if(ready_fd_num == -1){
            perror("epoll_wait error, message: ");
            continue;
        }
        int i;
        for(i = 0; i < ready_fd_num;i++){
            if(events[i].data.fd == listen_fd){
                conn_fd = accept(listen_fd,(struct sockaddr *)&client_addr, &client_addr_size);
                if(conn_fd == -1){
                    sprintf(buf,"[pid %d] accept wrong :",getpid());
                    perror(buf);
                    continue;
                }
                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                epoll_ctl(epoll_fd,EPOLL_CTL_ADD,conn_fd,&ev);
                printf("connected client : %d \n",conn_fd);

            }else if(events[i].events & EPOLLIN){
                printf("[pid %d] receive %s:%d message\n",getpid(), inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
                conn_fd = events[i].data.fd;
                //accept_request(conn_fd, &client_addr);
                request_handler(&conn_fd);
                //close(conn_fd);
            }else if(events[i].events & EPOLLERR){
                fprintf(stderr,"epoll error\n");
                close(conn_fd);
            }
        }
    }
}
