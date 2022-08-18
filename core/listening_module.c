//
// Created by lzt11 on 2022/5/17.
//

#include "lib.h"
#include "cycle_module.h"
#include "common.h"
#include "listening_module.h"

int listen_parse_json(cJSON* data,config_node_t* config);
int init_service_listening(cJSON* data, config_node_t *config);

static command_t listening_commands[] = {
        {
                "listen",
                SERVICE_MODULE,
                UNSET,
                TCP|UDP|HTTP|HTTPS|GRPC,
                SERVER,
                UNSET,
                listen_parse_json,
        },
        NULL
};

module_t listening_module = {
        MODULE_START,
        "listening_module",
        SERVICE_MODULE|MAIN_MODULE,
        UNSET,
        UNSET,
        UNSET,
        UNSET,
        init_service_listening,
        UNSET,
        listening_commands,
};

int listen_parse_json(cJSON* data, config_node_t* config){
    cJSON* listening = cJSON_GetObjectItem(data,"listen");
    if(listening == NULL){
        return ERROR;
    }
    listening_t *cl = config->data[listening_module.service_index];
    cl->port = listening->valueint;
    debug("port -> %d \n",cl->port);

    cl->ip = malloc(sizeof(listening->valuestring));
    strcpy(cl->ip,"0.0.0.0");

    struct sockaddr_in *serv_addr_in = malloc(sizeof(struct sockaddr));
    memset(serv_addr_in,0, sizeof(struct sockaddr));
    serv_addr_in->sin_family = AF_INET;
    serv_addr_in->sin_addr.s_addr = inet_addr(cl->ip);
    serv_addr_in->sin_port = htons(cl->port);

    cl->serv_addr = (struct sockaddr*)serv_addr_in;
    debug("OK\n");
    return OK;
}

int init_service_listening(cJSON* data, config_node_t *config){
    listening_t* cl;
    cl = malloc(sizeof(listening_t));
    if(cl == NULL){
        return ERROR;
    }
    memset(cl,0,sizeof(listening_t));

    cl->type = config->parent->type;
    if(cl->type == HTTP || cl->type == HTTPS || cl->type == TCP){
        cl->fd_type = SOCK_STREAM;
        debug("TCP\n");
    }else{
        cl->fd_type = SOCK_DGRAM;
        debug("UDP\n");
    }

    config->data[listening_module.service_index] = cl;

    return OK;
}

list_t* listenings_create(config_node_t *config){
    // traversal
    if(config->type != MAIN){
        return NULL;
    }

    list_t* ls = list_create(sizeof(listening_t));
    list_t *pcl = config->sons;
    list_node_t *pcln = pcl->begin;
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