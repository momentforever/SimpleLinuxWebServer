//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_CYCLE_MODULE_H
#define SOCKET_CYCLE_MODULE_H

#include "lib.h"
#include "common.h"
#include "event_module.h"

struct connection_s{
    void* data;
    int fd;

    // format address
    //char* clnt_addr_str;
    struct sockaddr clnt_addr;
    unsigned int clnt_addr_size;

    listening_t* listening;

    event_t *write;
    event_t *read;

    memory_pool_t *pool;
};

struct cycle_s{
    config_t *config;

    list_t *listenings;

    memory_pool_t *pool;
    //list data->data
    connection_t* free_connection;
    //array [1024]
    int connection_n;
    int connection_cur;
    connection_t* connections;
    event_t* read_events;
    event_t* write_events;
};

cycle_t* cycle_create(int connection_n);
connection_t *get_free_connection(cycle_t *cycle);
int release_connection(cycle_t *cycle,connection_t *conn);

void connection_init(connection_t* c);

#endif //SOCKET_CYCLE_MODULE_H
