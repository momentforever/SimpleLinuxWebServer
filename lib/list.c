//
// Created by lzt11 on 2022/5/11.
//

#include "list.h"
#include "global.h"

list_t *list_create(size_t value_size){
    list_t *list;
    list = malloc(sizeof(list_t));
    if(list == NULL){
        return NULL;
    }
    list->value_size = value_size;
    list->end = NULL;
    list->begin = NULL;
    list->size = 0;
    return list;
}

void *list_node_push(list_t *list){

    list_node_t *node;

    node = malloc(sizeof(list_node_t));
    if(node == NULL){
        return NULL;
    }
    node->next = NULL;

    node->value = malloc(list->value_size);
    if(node->value==NULL){
        return NULL;
    }

    if(list->begin==NULL){
        list->begin = node;
        list->end = node;
    }else{
        list->end->next = node;
        list->end=node;
    }

    list->size += 1;

    return node->value;
}