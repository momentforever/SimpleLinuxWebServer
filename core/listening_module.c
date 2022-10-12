//
// Created by lzt11 on 2022/5/17.
//

#include "lib.h"
#include "cycle_module.h"
#include "common.h"
#include "listening_module.h"

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

int listen_parse_json(cJSON* data, config_node_t *cn, config_t* c){
    cJSON* listening = cJSON_GetObjectItem(data,"listen");
    if(listening == NULL){
        return ERROR;
    }
    listening_t *cl = cn->data[listening_module.service_index];
    cl->port = listening->valueint;
    debug("port -> %d",cl->port);

    cl->ip = pmalloc(c->pool,sizeof(listening->valuestring));
    strcpy(cl->ip,"0.0.0.0");

    struct sockaddr_in *serv_addr_in = pmalloc(c->pool,sizeof(struct sockaddr));
    memset(serv_addr_in,0, sizeof(struct sockaddr));
    serv_addr_in->sin_family = AF_INET;
    serv_addr_in->sin_addr.s_addr = inet_addr(cl->ip);
    serv_addr_in->sin_port = htons(cl->port);

    cl->serv_addr = (struct sockaddr*)serv_addr_in;
    debug("OK");
    return OK;
}

int init_service_listening(cJSON* data, config_node_t *cn, config_t* c){
    listening_t* cl;
    cl = pmalloc(c->pool, sizeof(listening_t));
    if(cl == NULL){
        return ERROR;
    }
    memset(cl,0,sizeof(listening_t));

    cl->type = cn->parent->type;
    if(cl->type == HTTP || cl->type == HTTPS || cl->type == TCP){
        cl->fd_type = SOCK_STREAM;
    }else{
        cl->fd_type = SOCK_DGRAM;
    }

    cn->data[listening_module.service_index] = cl;

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
