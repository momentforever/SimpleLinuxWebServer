//
// Created by lzt11 on 2022/5/10.
//

#ifndef SOCKET_CONFIG_H
#define SOCKET_CONFIG_H

#include "lib.h"
#include "stl.h"

typedef struct config_node_s config_node_t;
struct config_node_s{
    // 0 - 3 MAIN - ROUTE
    int part;
    //
    int type;
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
};

config_t* config_create();

#endif //SOCKET_CONFIG_H
