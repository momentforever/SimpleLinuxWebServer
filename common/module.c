//
// Created by lzt11 on 2022/7/27.
//

#include "module.h"

int modules_num = 0;
int main_module_num = 0;
int protocol_module_num = 0;
int service_module_num = 0;
int route_module_num = 0;


void module_init(module_t* modules[]){
    for(int i = 0; modules[i] != NULL;i++){
        if(modules[i]->type & MAIN_MODULE){
            modules[i]->main_index = main_module_num;
            main_module_num+=1;
        }
        if(modules[i]->type & PROTOCOL_MODULE){
            modules[i]->main_index = protocol_module_num;
            protocol_module_num+=1;
        }
        if(modules[i]->type & SERVICE_MODULE){
            modules[i]->main_index = service_module_num;
            service_module_num+=1;
        }
        if(modules[i]->type & ROUTE_MODULE){
            modules[i]->main_index = route_module_num;
            route_module_num+=1;
        }
        modules_num+=1;
    }
    debugln("modules_num -> %d", modules_num);
    debugln("main_module_num -> %d", main_module_num);
    debugln("protocol_module_num -> %d", protocol_module_num);
    debugln("service_module_num -> %d", service_module_num);
    debugln("route_module_num -> %d", route_module_num);
}
