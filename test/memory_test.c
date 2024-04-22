#include "lib.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
    memory_pool_t *pool;

    pool = memory_pool_create();

    int *a = pmalloc(pool,sizeof(int));
    *a = 10;

    int *b = malloc(sizeof(int));
    *b = 100;
    pget(pool, b, free);

    assert(pool->size == 2);
    
    memory_pool_t* sub_pool = memory_pool_create();
    pget(pool, sub_pool, pfree_void);
    
    int *sub_a = pmalloc(sub_pool, sizeof(int));
    *sub_a = 11; 

    pfree(pool);

    assert(pool->size == 0);
    assert(*a != 10);
    assert(*b != 100);
    assert(*sub_a != 11);

    return 0;
}