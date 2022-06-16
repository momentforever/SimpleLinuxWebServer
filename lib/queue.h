//
// Created by lzt11 on 2022/5/13.
//

#ifndef SOCKET_QUEUE_H
#define SOCKET_QUEUE_H

#include "global.h"


typedef struct queue_node_s queue_node_t;
struct queue_node_s{
    queue_node_t *next;
    queue_node_t *before;
    //void* value;
};

#define queue_data(node,type,member) \
(type*)((void*)(node)-offsetof(type,member))

#define queue_init(h) \
    (h)->before = h;    \
    (h)->next = h;

#define queue_next(node) \
    (node)->next

#define queue_before(node) \
    (node)->before

#define queue_insert_begin(h,node) \
    (h)->next->before = node; \
    (node)->next = (h)->next; \
    (node)->before = h; \
    (h)->next = node;

#define queue_insert_end(h,node) \
    (h)->before->next = node; \
    (node)->before = (h)->before; \
    (node)->next = h; \
    (h)->before = node;

#define queue_empty(h) ((h)==(h)->next)

#endif //SOCKET_QUEUE_H
