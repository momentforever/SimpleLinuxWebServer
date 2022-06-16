//
// Created by lzt11 on 2022/5/10.
//

#ifndef SOCKET_CONFIG_H
#define SOCKET_CONFIG_H

#include "global.h"

typedef struct config_s config_t;

typedef void (*config_handler_pt)();

struct config_s{
    int port;
    //char* server_name;
    int timeout;
    config_handler_pt read_handler;//location
    config_handler_pt write_handler;//location
};

config_t* creat_config();


#define config_init(c) \
    (c)->port = ALL; \
    (c)->timeout = ETERNITY; \
    (c)->read_handler = NULL; \
    (c)->write_handler = NULL;

#endif //SOCKET_CONFIG_H
