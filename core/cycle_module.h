//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_CYCLE_MODULE_H
#define SOCKET_CYCLE_MODULE_H

#include "lib.h"

#include "connection_module.h"
#include "event_module.h"



typedef struct cycle_s cycle_t;

struct cycle_s{
    config_t *config;

    list_t *listenings;
    //list data->data
    connection_t* free_connection;
    //array [1024]
    int connection_n;
    connection_t* connections;
    event_t* read_events;
    event_t* write_events;
};

cycle_t* cycle_create(int connection_n);
connection_t *get_free_connection(cycle_t *cycle);
int release_connection(cycle_t *cycle,connection_t *conn);


#endif //SOCKET_CYCLE_MODULE_H
