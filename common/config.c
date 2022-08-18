//
// Created by lzt11 on 2022/5/10.
//

#include "config.h"
#include "lib.h"
#include "auto.h"

// 使用了全局变量
config_node_t* config_node_add(config_node_t *parent,int type){
    config_node_t *node;
    if(parent == NULL){
        //root
        node = malloc(sizeof(config_node_t));
        if(node == NULL){
            return NULL;
        }
        memset(node,0, sizeof(config_node_t));
        node->type = MAIN;
        node->part = MAIN_MODULE;
        node->parent = NULL;
        node->sons = list_create(sizeof(config_node_t));
        node->data= malloc(main_module_num*sizeof(void*));
        memset(node->data,0,main_module_num*sizeof(void*));
        return node;
    }


    list_node_t *ln = list_node_push(parent->sons);
    if(ln == NULL){
        return NULL;
    }

    node = ln->value;

    node->parent = parent;
    node->type = type;
    node->sons = list_create(sizeof(config_node_t));
    //malloc data
    if(parent->part == MAIN_MODULE){
        node->part = PROTOCOL_MODULE;
        node->data= malloc(protocol_module_num*sizeof(void*));
        memset(node->data,0,protocol_module_num*sizeof(void*));
    }else if(parent->part == PROTOCOL_MODULE){
        node->part = SERVICE_MODULE;
        node->data= malloc(service_module_num*sizeof(void*));
        memset(node->data,0,service_module_num*sizeof(void*));
    }else if(parent->part == SERVICE_MODULE*sizeof(void*)){
        node->part = ROUTE_MODULE;
        node->data= malloc(route_module_num*sizeof(void*));
        memset(node->data,0,route_module_num*sizeof(void*));
    }else{
        return NULL;
    }
    return node;
}

int main_translate(char* name){
    if(name == NULL){
        return MAIN;
    }
    if(strcasecmp(name,"main") == 0){
        return MAIN;
    }else{
        return MAIN;
    }
}

int protocol_translate(char* name){
    if(name == NULL){
        return UNSET;
    }

    if(strcasecmp(name,"tcp") == 0){
        return TCP;
    }else if(strcasecmp(name,"udp") == 0){
        return UDP;
    }else if(strcasecmp(name,"http") == 0){
        return HTTP;
    }else if(strcasecmp(name,"https")==0){
        return HTTPS;
    }else if(strcasecmp(name,"grpc")==0){
        return GRPC;
    }else{
        return UNSET;
    }
}

int service_translate(char* name){
    if(name == NULL){
        return UNSET;
    }
    if(strcasecmp(name,"upstream") == 0){
        return UPSTREAM;
    }else if(strcasecmp(name,"server") == 0){
        return SERVER;
    }else{
        return UNSET;
    }
}

int route_translate(char* name){
    if(name == NULL){
        return UNSET;
    }
    if(strcasecmp(name,"location") == 0){
        return LOCATION;
    }else{
        return UNSET;
    }
}


int config_init(config_t* config){
    //引擎解析 json
    cJSON* json_main = config->fmt;

    config->root = config_node_add(NULL,main_translate(cJSON_GetStringValue(
            cJSON_GetObjectItem(json_main,"type"))));

    config_node_t *main = config->root;

    for(int module_i = 0;modules[module_i]!=NULL;module_i++){
        if(modules[module_i]->pre_conf_handler!=UNSET){
            modules[module_i]->pre_conf_handler(json_main,main);
        }
    }

    for(int module_i = 0;modules[module_i]!=NULL;module_i++){
        if(modules[module_i]->init_main_handler!=UNSET){
            modules[module_i]->init_main_handler(json_main,main);
        }
        //if(modules[module_i]->type & main->part){
            for(int command_i = 0;modules[module_i]->commands[command_i] != NULL;command_i++){
                if((modules[module_i]->commands[command_i]->part & main->part)
                &&(modules[module_i]->commands[command_i]->main_type & main->type)){
                    modules[module_i]->commands[command_i]->parse_handler(json_main,main);
                }
            }
        //}
    }

    cJSON* json_protocols = cJSON_GetObjectItem(json_main,"protocol");

    int json_protocols_size = cJSON_GetArraySize(json_protocols);
    int jpi;
    cJSON* json_protocol;
    for(jpi = 0; jpi < json_protocols_size; jpi++){
        json_protocol = cJSON_GetArrayItem(json_protocols,jpi);

        config_node_t *protocol = config_node_add(main, protocol_translate(cJSON_GetStringValue(
                cJSON_GetObjectItem(json_protocol,"type"))));

        for(int module_i = 0;modules[module_i]!=NULL;module_i++){
            if(modules[module_i]->init_protocol_handler!=UNSET){
                modules[module_i]->init_protocol_handler(json_protocol,protocol);
            }
            //if(modules[module_i]->type & protocol->part){
                for(int command_i = 0;modules[module_i]->commands[command_i] != NULL;command_i++){
                    if((modules[module_i]->commands[command_i]->part & protocol->part)
                       &&(modules[module_i]->commands[command_i]->protocol_type & protocol->type)){
                        modules[module_i]->commands[command_i]->parse_handler(json_protocol,protocol);
                    }
                }
            //}
        }

        cJSON* json_services = cJSON_GetObjectItem(json_protocol,"service");

        int json_services_size = cJSON_GetArraySize(json_services);
        int jsi;
        cJSON* json_service;
        for(jsi = 0; jsi < json_services_size; jsi++){
            json_service = cJSON_GetArrayItem(json_services,jsi);
            //service_translate(cJSON_GetStringValue(
            //                    cJSON_GetObjectItem(json_service,"type")))
            config_node_t *service = config_node_add(protocol,SERVER);

            for(int module_i = 0;modules[module_i]!=NULL;module_i++){
                if(modules[module_i]->init_service_handler!=UNSET){
                    modules[module_i]->init_service_handler(json_service,service);
                }
                //if(modules[module_i]->type & service->part){
                    for(int command_i = 0;modules[module_i]->commands[command_i] != NULL;command_i++){
                        if((modules[module_i]->commands[command_i]->part & service->part)
                           &&(modules[module_i]->commands[command_i]->service_type & service->type)){
                            modules[module_i]->commands[command_i]->parse_handler(json_service,service);
                        }
                    }
                //}
            }

            cJSON* json_routes = cJSON_GetObjectItem(json_service,"route");

            int json_routes_size = cJSON_GetArraySize(json_routes);
            int jri;
            cJSON* json_route;
            for(jri = 0; jri < json_routes_size; jri++){
                json_route = cJSON_GetArrayItem(json_routes,jri);

                config_node_t *route = config_node_add(service, route_translate(cJSON_GetStringValue(
                        cJSON_GetObjectItem(json_route,"type"))));

                for(int module_i = 0;modules[module_i]!=NULL;module_i++){
                    if(modules[module_i]->init_route_handler!=UNSET){
                        modules[module_i]->init_route_handler(json_route,route);
                    }
                    //if(modules[module_i]->type & route->part){
                        for(int command_i = 0;modules[module_i]->commands[command_i] != NULL;command_i++){
                            if((modules[module_i]->commands[command_i]->part & route->part)
                               &&(modules[module_i]->commands[command_i]->route_type & route->type)){
                                modules[module_i]->commands[command_i]->parse_handler(json_route,route);
                            }
                        }
                    //}

                }
            }
        }
    }

    for(int module_i = 0;modules[module_i]!=NULL;module_i++){
        if(modules[module_i]->fin_conf_handler!=UNSET){
            modules[module_i]->fin_conf_handler(json_main,main);
        }
    }

    return OK;
}

config_t* config_create(){
    config_t* config = malloc(sizeof(config_t));

    if(config == NULL){
        return NULL;
    }
    //read config
    FILE *config_fd = fopen("../config.json","r");

    if(config_fd == NULL){
        return NULL;
    }
    //查找文件尾偏移
    fseek(config_fd,0,SEEK_END);
    int length;
    //计算长度
    length = ftell(config_fd);
    config->str = malloc((length + 1) * sizeof(char));
    //回到文件头
    rewind(config_fd);
    length = fread(config->str,1,length,config_fd);
    config->str[length] = '\0';
    fclose(config_fd);

    config->fmt = cJSON_Parse(config->str);

    config_init(config);

    debug("Finish config create.\n");
    return config;
}