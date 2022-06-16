// TODO 1.实现一个简单的web服务器,通过epoll finish
// TODO 多线程
// TODO 2.尝试实现一个非阻塞的web服务器 finish
// TODO 3.尝试实现一个异步的web服务器
// TODO 3.1.定时器

#include "lib.h"
#include "global.h"
#include <time.h>

typedef struct part_s part_t;

struct part_s{
    int key;
    int value;
    rbtree_node_t rb_node;
};

void rbtree_test(){
    rbtree_node_t sentinel;
    sentinel.left = NULL;
    sentinel.right =NULL;
    sentinel.parent = NULL;
    rbtree_t tree;
    rbtree_init(&tree,&sentinel,rbtree_insert_value);

    part_t ps[20];
    part_t *p = ps;
    srand((unsigned)time(NULL));
    //p = malloc(sizeof(part_t) * 20);
    for(int i = 0;i<20;i++){
        //p = malloc(sizeof(part_t));
        p->value = 0;
        p->key = rand() % 100;
        p->rb_node.key = (void*)&p->key;
        printf("node->%d\n",p->key);
        rbtree_node_insert(&tree,&p->rb_node);
        rbtree_middleorder(&tree,tree.root);
        printf("\n");
        p++;
    }
    printf("node->%d\n",ps[0].key);
    rbtree_node_delete(&tree,&ps[0].rb_node);
    rbtree_middleorder(&tree,tree.root);
    printf("\n");

    printf("node->%d\n",ps[15].key);
    rbtree_node_delete(&tree,&ps[15].rb_node);
    rbtree_middleorder(&tree,tree.root);
    printf("\n");

}


int main(int argc,char *argv[]){
    rbtree_test();
    return 0;
//    cycle_t *cycle;
//    cycle = cycle_create(1024);
//
//    list_t *config_list = cycle->config;
//    config_list = list_create(sizeof(config_t));
//    if(config_list == NULL){
//        error_handling("error");
//    }
//
//    config_t *config_ptr;
//    config_ptr = (config_t*)list_node_push(config_list);
//    if(config_ptr==NULL){
//        error_handling("error");
//    }
//    config_init(config_ptr);
//    config_ptr->port = 10080;
//
//    config_ptr = (config_t*)list_node_push(config_list);
//    if(config_ptr==NULL){
//        error_handling("error");
//    }
//    config_init(config_ptr);
//    config_ptr->port = 10081;
//
//
//    struct epoll_event ev,events[cycle->connection_n];
//    int epoll_fd = epoll_create((int)cycle->connection_n);
//
//    cycle->listening = list_create(sizeof(listening_t));
//
//    listening_t *listening;
//    list_node_t *node;
//    struct sockaddr_in *server_addr;
//    connection_t *conn;
//    for(node=cycle->config->begin;node!=NULL;node=node->next){
//        config_ptr = (config_t*)node->value;
//        listening = list_node_push(cycle->listening);
//
//        listening->type = SOCK_STREAM;
//        listening->serv_addr = malloc(sizeof(struct sockaddr));
//        listening->fd = socket(AF_INET,listening->type,0);
//
//        //更改listen_fd属性
//        //设置为非阻塞
//        fcntl(listening->fd,F_SETFL,O_NONBLOCK);
//
//        server_addr= malloc(sizeof(struct sockaddr));
//        bzero(server_addr, sizeof(struct sockaddr));
//        server_addr->sin_family = AF_INET;
//        server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
//        server_addr->sin_port = htons(config_ptr->port);
//
//        listening->serv_addr = (struct sockaddr *)server_addr;
//
//        if(bind(listening->fd,(struct sockaddr *)server_addr,sizeof(struct sockaddr))== -1){
//            perror("bind error, message: ");
//            exit(1);
//        }
//        if(listen(listening->fd,SOMAXCONN)==-1){
//            perror("listen error, message: ");
//            exit(1);
//        }
//        conn = get_free_connection(cycle);
//
//        conn->serv_addr = listening->serv_addr;
//        conn->listening = listening;
//        conn->type = listening->type;
//        conn->fd = listening->fd;
//        //TODO handler
//
//        ev.data.fd = conn->fd;
//        ev.events = EPOLLIN;
//        ev.data.ptr = (void*)conn;
//        //将listen_df注册到刚刚创建的epoll中
//        if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,ev.data.fd,&ev)==-1) {
//            perror("epoll_ctl error, message: ");
//            exit(1);
//        }
//    }
//
//
//    int ready_fd_num;
//    int i;
//    connection_t *ev_conn;
//    connection_t *new_conn;
//    while(1){
//        //TODO UNBLOCK Timer
//        ready_fd_num = epoll_wait(epoll_fd,events,(int)cycle->connection_n,ETERNITY);
//        for(i = 0; i < ready_fd_num;i++){
//            ev_conn = (connection_t*)events[i].data.ptr;
//            if(ev_conn->fd == ev_conn->listening->fd){
//
//                int conn_fd = accept(events[i].data.fd,(struct sockaddr *)&client_addr, &client_addr_size);
//                if(conn_fd == -1){
//                    perror("accept error.");
//                    continue;
//                }
//                //create connection
//                new_conn = get_free_connection(cycle);
//
//
//                ev.data.fd = conn_fd;
//                ev.events = EPOLLIN | EPOLLET;
//
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
//
//    return 0;

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
