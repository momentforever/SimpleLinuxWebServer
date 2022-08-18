//
// Created by lzt11 on 2022/7/27.
//

#ifndef SOCKET_MODULE_H
#define SOCKET_MODULE_H

#include "lib.h"
#include "config.h"

typedef struct module_s module_t;
typedef int (*module_parse_handler_pt)(cJSON* config_fmt,config_node_t* config);

typedef struct command_s command_t;

struct command_s{
    char* name;
    // 凭此判断范围
    int part;
    // 凭此判断类型
    int main_type:8;
    int protocol_type:8;
    int service_type:8;
    int route_type:8;
    module_parse_handler_pt parse_handler;
    //others
};

struct module_s{
    int main_index;
    int protocol_index;
    int service_index;
    int route_idx;

    char* name;
    // 凭此声明内存区
    int type:4;

    module_parse_handler_pt pre_conf_handler;
    module_parse_handler_pt fin_conf_handler;

    module_parse_handler_pt init_main_handler;
    module_parse_handler_pt init_protocol_handler;
    module_parse_handler_pt init_service_handler;
    module_parse_handler_pt init_route_handler;

    command_t *commands[];
};

extern int modules_num;
extern int main_module_num;
extern int protocol_module_num;
extern int service_module_num;
extern int route_module_num;

void module_init(module_t* modules[]);

#define get_main_conf_while_parse()
#define get_protocol_conf_while_parse
#define get_service_conf_while_parse
#define get_route_conf_while_parse

#define get_main_conf_while_handle
#define get_protocol_conf_while_handle
#define get_service_conf_while_handle
#define get_route_conf_while_handle

#endif //SOCKET_MODULE_H
