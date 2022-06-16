//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_CYCLE_H
#define SOCKET_CYCLE_H

#include "global.h"
#include "lib.h"

#include "connection.h"
#include "event.h"

typedef struct cycle_s cycle_t;

struct cycle_s{
    rbtree_t timer_rbtree;
    rbtree_node_t timer_sentinel;

    list_t *config;

    list_t *listening;
    //list data->data
    connection_t* free_connection;
    //array [1024]
    uint connection_n;
    connection_t* connections;
    event_t* read_events;
    event_t* write_events;
};

cycle_t *cycle_create(uint connection_n);
connection_t *get_free_connection(cycle_t *cycle);
int release_connection(cycle_t *cycle,connection_t *conn);

#endif //SOCKET_CYCLE_H
