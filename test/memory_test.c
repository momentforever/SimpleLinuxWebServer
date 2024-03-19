#include "lib.h"
#include "memory.h"
#include <stdio.h>


int main(int argc,char *argv[]){
    memory_pool_t *pool;

    pool = memory_pool_create();

    int *a = pmalloc(pool,sizeof(int));
    *a = 10;

    int *b = malloc(sizeof(int));
    *b = 100;
    pget(pool, b, free);

    assert(pool->size == 2);

    pfree(pool);

    assert(pool->size == 0);

    assert(*a != 10);
    assert(*b != 100);

    return 0;
}