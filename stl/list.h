//
// Created by lzt11 on 2022/5/11.
//

#ifndef SOCKET_LIST_H
#define SOCKET_LIST_H

#include "lib.h"

typedef struct list_node_s list_node_t;
struct list_node_s{
    list_node_t  *next;
    void* value;
};

typedef struct list_s list_t;
struct list_s{
    size_t size; //self size
    size_t value_size;
    list_node_t *begin;
    list_node_t *end;
};

list_t *list_create(size_t value_size);
list_node_t *list_node_push(list_t *list);
list_node_t *list_node_insert(list_t *list,void *before_data);
void list_node_delete(list_t *list,list_node_t *node);
void list_delete(list_t *list);
void list_delete_void(void *list);


#define list_is_empty(l) \
    ((l)->size > 0 ? 1 : 0)

#define list_init(l,value_size)     \
    (l)->value_size = value_size;   \
    (l)->end = NULL;                \
    (l)->begin = NULL;              \
    (l)->size = 0

#define list_size(l) \
    (l)->size

#define list_begin(l) \
    (l)->begin

#define list_end(l) \
    (l)->end

#endif //SOCKET_LIST_H
