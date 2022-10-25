//
// Created by lzt11 on 2022/5/17.
//

#include "lib.h"
#include "cycle_module.h"
#include "common.h"
#include "listening_module.h"
#include "self_httpd.h"

int listen_parse_json(cJSON* data,config_node_t *cn, config_t* c);
int init_service_listening(cJSON* data, config_node_t *cn, config_t* c);

static command_t listening_commands[] = {
        {
                "listen",
                SERVICE_MODULE,
                ALL_MAIN,
                ALL_PROTOCOL,
                SERVER,
                UNSET,
                listen_parse_json,
        },
        NULL
};

module_t listening_module = {
        MODULE_START,
        "listening_module",
        SERVICE_MODULE,
        UNSET,
        UNSET,
        UNSET,
        UNSET,
        init_service_listening,
        UNSET,
        listening_commands,
};

int init_service_listening(cJSON* data, config_node_t *cn, config_t* c){
    listening_t* cl;
    cl = pmalloc(c->pool, sizeof(listening_t));
    if(cl == NULL){
        return ERROR;
    }

    cl->type = cn->parent->type;
    if(cl->type == HTTP || cl->type == HTTPS || cl->type == TCP){
        cl->fd_type = SOCK_STREAM;
        cl->handler = accept_tcp;
    }else{
        cl->fd_type = SOCK_DGRAM;
        cl->handler = accept_udp;
    }

    cn->data[listening_module.service_index] = cl;
    return OK;
}

int listen_parse_json(cJSON* data, config_node_t *cn, config_t* c){
    cJSON* listening = cJSON_GetObjectItem(data,"listen");
    if(listening == NULL){
        return ERROR;
    }
    listening_t *cl = cn->data[listening_module.service_index];
    cl->port = listening->valueint;
    debugln("port -> %d", cl->port);

    cl->ip = pmalloc(c->pool,sizeof(listening->valuestring));
    strcpy(cl->ip,"0.0.0.0");

    struct sockaddr_in *serv_addr_in = pmalloc(c->pool,sizeof(struct sockaddr));
    memset(serv_addr_in,0, sizeof(struct sockaddr));
    serv_addr_in->sin_family = AF_INET;
    serv_addr_in->sin_addr.s_addr = inet_addr(cl->ip);
    serv_addr_in->sin_port = htons(cl->port);

    cl->serv_addr = (struct sockaddr*)serv_addr_in;
    return OK;
}


list_t* listenings_create(config_t *c){
    // traversal
    config_node_t* config = c->root;

    list_t* ls = list_create(sizeof(listening_t));
    pget(c->pool,ls,list_delete_void);

    list_t *pcl = config->sons;
    list_node_t *pcln = pcl->begin;
    // TODO 去重
    for(int pi = 0; pi < pcl->size; pi++){
        config_node_t *pc = pcln->value;
        list_t *scl = pc->sons;
        list_node_t *scln = scl->begin;
        for(int si = 0; si < scl->size ; si++){
            config_node_t *sc = scln->value;
            listening_t *cl = sc->data[listening_module.service_index];
            if(cl!=NULL){
                list_node_t* ln = list_node_push(ls);
                listening_t* l = ln->value;
                memcpy(l,cl,sizeof(listening_t));
            }
            scln = scln->next;
        }
        pcln = pcln->next;
    }
    return ls;
}

void write_handler(event_t *ev){
    debugln("write_handler");
    connection_t *con = ev->data;
    request_handler((void*)&con->fd);
}

void accept_tcp(connection_t *c){
    struct epoll_event ev;
    connection_t *new_conn;
    struct sockaddr clnt_addr;
    unsigned int clnt_addr_size;

    int conn_fd = accept(c->fd, &clnt_addr, &clnt_addr_size);
    if(conn_fd == -1){
        perror("accept error.");
        return;
    }

    new_conn = get_free_connection(g_cycle);
    if(new_conn == NULL){
        debugln("no free connection!");
        return;
    }
    memcpy(&new_conn->clnt_addr,&clnt_addr, sizeof(struct sockaddr));
    new_conn->clnt_addr_size = clnt_addr_size;
    new_conn->listening = c->listening;

    new_conn->fd = conn_fd;

    // 根据协议分配不同handler
    new_conn->read->handler = write_handler;
    //new_conn->write->handler = NULL;

    ev.data.ptr = new_conn;
    // 边缘触发
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(g_epoll_fd,EPOLL_CTL_ADD,conn_fd,&ev);
}

void accept_udp(connection_t *c) {

}