#ifndef SOCKET_PROTOCOL_MODULE_H
#define SOCKET_PROTOCOL_MODULE_H

#include "lib.h"
#include "common.h"
#include "stl.h"
#include "event_module.h"
#include "cycle_module.h"

struct protocol_s{
    event_handler_pt read_event_handlers[PROTOCOL_NUM];
    event_handler_pt write_event_handlers[PROTOCOL_NUM];
};

int resigster_protocol_event_handler(event_handler_pt read_event_handler, 
                        event_handler_pt write_event_handler,
                        config_node_t *cn, 
                        config_t* c,
                        int type);

int set_protocol_event_handler(connection_t* conn);

#endif