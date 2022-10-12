//
// Created by lzt11 on 2022/5/10.
//

#ifndef SOCKET_CONFIG_H
#define SOCKET_CONFIG_H

#include "lib.h"
#include "stl.h"
#include "memory.h"

typedef struct config_node_s config_node_t;
struct config_node_s{
    // 0 - 3 MAIN - ROUTE
    unsigned int part;
    // MAIN - ... PROTOCOL - ... SERVICE - ... ROUTE - ...
    unsigned int type;
    config_node_t* parent;
    list_t *sons;
    void** data;
};


typedef struct config_s config_t;
struct config_s{
    char* str;
    void* fmt; //cJSON*`
    //char type[8];

    //main
    config_node_t *root;

    memory_pool_t *pool;
};

config_t* config_create();

void config_delete(config_t* config);

void config_delete_void(void* config);

#endif //SOCKET_CONFIG_H
