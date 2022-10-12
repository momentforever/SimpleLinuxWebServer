//
// Created by lzt11 on 2022/10/11.
//

#include "stl.h"
#include "lib.h"
#include "memory.h"

memory_pool_t* memory_pool_create(){
    memory_pool_t * pool = malloc(sizeof(memory_pool_t));
    if(pool == NULL)return NULL;
    memory_pool_init(pool);
    return pool;
}

void pget(memory_pool_t *pool,void* ptr, memory_pool_node_free_pt free_handler){
    // init node
    memory_pool_node_t* node = malloc(sizeof(memory_pool_node_t));
    if(node == NULL)return;
    node->next = NULL;
    node->ptr = ptr;
    node->handler = free_handler;

    //add node
    if(pool->size == 0){
        pool->begin = node;
        pool->end = node;
    }else{
        debug_assert(pool->end->next == NULL);

        pool->end->next = node;
        pool->end = node;
    }
    pool->size += 1;
}

void* pmalloc(memory_pool_t *pool,size_t size){
    //init ptr
    void* ptr = malloc(size);
    if(ptr == NULL)return NULL;
    memset(ptr,0,size);
    // init node
    memory_pool_node_t* node = malloc(sizeof(memory_pool_node_t));
    if(node == NULL)return NULL;
    node->next = NULL;
    node->ptr = ptr;
    node->handler = free;

    //add node
    if(pool->size == 0){
        pool->begin = node;
        pool->end = node;
    }else{
        debug_assert(pool->end->next == NULL);

        pool->end->next = node;
        pool->end = node;
    }
    pool->size += 1;

    return ptr;
}

void pfree(memory_pool_t *pool){
    if(pool->size == 0)return;
    memory_pool_node_t *node = pool->begin;
    memory_pool_node_t *next = pool->begin->next;

    while(1){
        node->handler(node->ptr);
        free(node);

        node=next;
        if(next==NULL)break;

        next=next->next;
    }
    pool->size = 0;
}

void memory_pool_delete(memory_pool_t* pool){
    pfree(pool);
    free(pool);
}

