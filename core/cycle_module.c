//
// Created by lzt11 on 2022/5/13.
//

#include "cycle_module.h"
#include "listening_module.h"

cycle_t* cycle_create(int connection_n){
    cycle_t *cycle;
    cycle = malloc(sizeof(cycle_t));
    if(cycle==NULL)return NULL;

    cycle->pool = memory_pool_create();
    if(cycle->pool==NULL)return NULL;

    cycle->connection_n = connection_n;

    cycle->connections = pmalloc(cycle->pool,connection_n * sizeof(connection_t));
    if(cycle->connections==NULL){
        return NULL;
    }
    cycle->read_events = pmalloc(cycle->pool, connection_n * sizeof(event_t));
    if(cycle->read_events==NULL){
        return NULL;
    }
    cycle->write_events = pmalloc(cycle->pool, connection_n * sizeof(event_t));
    if(cycle->write_events==NULL){
        return NULL;
    }

    int n;
    //init event
    connection_t *cs = cycle->connections;
    event_t *res = cycle->read_events;
    event_t *wes = cycle->write_events;
    n = 0;
    while(1){
        //double ptr
        cs[n].read = &res[n];
        res->data = &cs[n];
        cs[n].write = &wes[n];
        wes->data = &cs[n];

        if(n >= connection_n - 1){
            cs[n].data = NULL;
            break;
        }
        cs[n].data = &cs[n+1];

        n++;
    }
    cycle->free_connection = cycle->connections;

    cycle->config = config_create(cycle->config);
    pget(cycle->pool,cycle->config,config_delete_void);

    cycle->listenings = listenings_create(cycle->config);

    return cycle;
}

connection_t *get_free_connection(cycle_t *cycle){
    connection_t *conn = cycle->free_connection;
    // next
    cycle->free_connection = (connection_t*)cycle->free_connection->data;
    return conn;
}

int release_connection(cycle_t *cycle,connection_t *conn){
    conn->data = (void*)cycle->free_connection;
    //TODO 释放request
    cycle->free_connection = (connection_t*)conn;
    return OK;
}

void cycle_delete(cycle_t* cycle){
    memory_pool_delete(cycle->pool);
    free(cycle);
}