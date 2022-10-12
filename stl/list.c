//
// Created by lzt11 on 2022/5/11.
//

#include "list.h"
#include "lib.h"

// 由我申请，由我释放


list_t *list_create(size_t value_size){
    list_t *list;
    list = malloc(sizeof(list_t));
    if(list == NULL)return NULL;
    list_init(list,value_size);
    return list;
}

list_node_t *list_node_push(list_t *list){
    list_node_t *node;
    node = malloc(sizeof(list_node_t));
    if(node == NULL)return NULL;
    node->next = NULL;

    node->value = malloc(list->value_size);
    if(node->value == NULL)return NULL;
    memset(node->value,0,sizeof(list->value_size));

    if(list->size==0){
        list->begin = node;
        list->end = node;
    }else{
        list->end->next = node;
        list->end=node;
    }

    list->size += 1;

    return node;
}

list_node_t *list_node_insert(list_t *list,void *before_data){
    //search
    list_node_t *before_node;
    for(before_node = list->begin;before_node != NULL; before_node = before_node->next){
        //address compare
        if(before_node->value == before_data)break;
    }
    if(before_node == NULL){
        return NULL;
    }

    list_node_t *node;
    node = malloc(sizeof(list_node_t));
    if(node == NULL){
        return NULL;
    }
    node->next = before_node->next;

    node->value = malloc(list->value_size);
    if(node->value == NULL){
        return NULL;
    }

    before_node->next = node;
    if(before_node == list->end){
        list->end = before_node;
    }

    list->size += 1;
    return node;
}

void list_node_delete(list_t *list,list_node_t *node){
    if(list->size == 0){
        return;
    }
    if(node == list->begin){
        // size == 1
        if(list->begin == list->end){
            list->end = NULL;
            list->begin = NULL;
        }else{
            list->begin = node->next;
        }
    }else{
        list_node_t *before_node;
        for(before_node = list->begin;before_node != list->end; before_node = before_node->next){
            //address compare
            if(before_node->next == node)break;
        }
        if(before_node == list->end){
            // not found
            return;
        }

        before_node->next = node->next;

        if(node == list->end){
            list->end = before_node;
        }
    }

    list->size -= 1;
    free(node->value);
    free(node);
}

void list_delete_void(void *list){
    list_delete((list_t*)list);
}

void list_delete(list_t *list){
    if(list->size == 0){
        free(list);
        return;
    }

    list_node_t *next_node,*node;
    node = list->begin;
    next_node = node->next;
    for(;next_node != NULL;next_node = next_node->next){
        free(node->value);
        free(node);
        node = next_node;
    }
    // size = 0
    free(list);
}

//void list_demo(){
//    list_t *l = list_create(sizeof(int));
//
//    list_node_t *n;
//    int* i;
//    n = list_node_push(l);
//
//    i = n->value;
//    *i = 10;
//
//    list_is_empty(l);
//    list_size(l);
//
//    n = list_node_insert(l,n);
//    *i = 15;
//
//    list_node_delete(l,n);
//
//    list_delete(l);
//}