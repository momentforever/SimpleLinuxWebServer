//
// Created by lzt11 on 2022/5/11.
//

#ifndef SOCKET_LIST_H
#define SOCKET_LIST_H

#include "global.h"

typedef struct list_node_s list_node_t;
struct list_node_s{
    list_node_t  *next;
    void* value;
};

typedef struct list_s list_t;
struct list_s{
    size_t size;
    size_t value_size;
    list_node_t *begin;
    list_node_t *end;
};

list_t *list_create(size_t value_size);
void *list_node_push(list_t *node);

//insert sort

#endif //SOCKET_LIST_H
