//
// Created by lzt11 on 2022/10/11.
//

#ifndef SOCKET_MEMORY_H
#define SOCKET_MEMORY_H



typedef struct memory_pool_node_s memory_pool_node_t;
typedef struct memory_pool_s memory_pool_t;

typedef void (*memory_pool_node_free_pt)(void* ptr);

struct memory_pool_node_s{
    memory_pool_node_t *next;
    memory_pool_node_free_pt handler;
    void* ptr;
};

struct memory_pool_s{
    memory_pool_node_t* begin;
    memory_pool_node_t* end;
    size_t size;
};

memory_pool_t* memory_pool_create();

#define memory_pool_init(mp) \
    (mp)->begin = NULL; \
    (mp)->end = NULL; \
    (mp)->size = 0


void* pmalloc(memory_pool_t *pool,size_t size);
void pget(memory_pool_t *pool,void* ptr, memory_pool_node_free_pt free_handler);

void pfree(memory_pool_t *pool);

void memory_pool_delete(memory_pool_t* pool);

#endif //SOCKET_MEMORY_H