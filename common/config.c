//
// Created by lzt11 on 2022/5/10.
//

#include "config.h"
#include "global.h"


config_t* config_create(){
    config_t *config;
    //TODO 内存池
    config = malloc(sizeof(config_t));
    if(config == NULL){
        return NULL;
    }
    config_init(config);
    return config;
}

