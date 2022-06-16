//
// Created by lzt11 on 2022/5/13.
//

#include "cycle.h"

cycle_t *cycle_create(uint connection_n){
    cycle_t *cycle;
    cycle= malloc(sizeof(cycle_t));
    if(cycle==NULL){
        return NULL;
    }
    cycle->connection_n = connection_n;

    cycle->connections = malloc(connection_n * sizeof(connection_t));
    if(cycle->connections==NULL){
        return NULL;
    }
    cycle->read_events = malloc(connection_n * sizeof(event_t));
    if(cycle->read_events==NULL){
        return NULL;
    }
    cycle->write_events = malloc(connection_n * sizeof(event_t));
    if(cycle->write_events==NULL){
        return NULL;
    }

    rbtree_init(&cycle->timer_rbtree,&cycle->timer_sentinel,rbtree_insert_value);

    int n;
    connection_t *cs = cycle->connections;
    event_t *res = cycle->read_events;
    event_t *wes = cycle->write_events;
    n = 0;
    while(1){
        if(n >= connection_n - 1){
            cs[n].data = NULL;
            cs[n].read = &res[n];
            cs[n].write = &res[n];
            break;
        }
        cs[n].data = &cs[n+1];
        cs[n].read = &res[n];
        cs[n].write = &wes[n];
        n++;
    }

    //TODO init_config
    //TODO init_listening

    return cycle;
}

cycle_t *cycle_init(cycle_t *old_cycle){
    cycle_t *cycle;
    cycle= malloc(sizeof(cycle_t));

    if(cycle==NULL){
        return NULL;
    }
    cycle->connection_n = old_cycle->connection_n;

    cycle->connections = malloc(old_cycle->connection_n * sizeof(connection_t));
    if(cycle->connections==NULL){
        return NULL;
    }
    cycle->read_events = malloc(old_cycle->connection_n * sizeof(event_t));
    if(cycle->read_events==NULL){
        return NULL;
    }
    cycle->write_events = malloc(old_cycle->connection_n * sizeof(event_t));
    if(cycle->write_events==NULL){
        return NULL;
    }

    int n;
    connection_t *cs = cycle->connections;
    event_t *res = cycle->read_events;
    event_t *wes = cycle->write_events;
    n = 0;
    while(1){
        if(n >= old_cycle->connection_n - 1){
            cs[n].data = NULL;
            cs[n].read = &res[n];
            cs[n].write = &res[n];
            break;
        }
        cs[n].data = &cs[n+1];
        cs[n].read = &res[n];
        cs[n].write = &wes[n];
        n++;
    }

    //TODO init_config
    //TODO init_listening

    return cycle;
}

connection_t *get_free_connection(cycle_t *cycle){
    connection_t *conn = cycle->free_connection;
    cycle->free_connection = (connection_t*)cycle->free_connection->data;
    return conn;
}

int release_connection(cycle_t *cycle,connection_t *conn){
    conn->data = (void*)cycle->free_connection;
    //TODO 释放request
    cycle->free_connection = (connection_t*)conn;
    return OK;
}