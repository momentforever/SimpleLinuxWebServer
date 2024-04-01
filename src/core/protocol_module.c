#include "protocol_module.h"
#include "config.h"
#include "lib.h"
#include "memory.h"
#include "module.h"
#include "listening_module.h"

int init_main_protocol(cJSON* data,config_node_t *cn, config_t* c);

static command_t protocol_commands[] = {
        NULL
};

module_t protocol_module = {
    MODULE_START,
    "protocol_moudle",
    MAIN_MODULE,
    UNSET,
    UNSET,
    init_main_protocol,
    UNSET,
    UNSET,
    UNSET,
    protocol_commands,
};

int init_main_protocol(cJSON* data,config_node_t *cn, config_t* c){
    protocol_t* cp;
    cp = pmalloc(c->pool, sizeof(protocol_t));
    if(cp == NULL){
        return ERROR;
    }
    cn->data[protocol_module.main_index] = cp;
    return OK;
}

int resigster_protocol_event_handler(event_handler_pt read_event_handler, 
                        event_handler_pt write_event_handler,
                        config_node_t *cn, 
                        config_t* c,
                        int type){
    debugln("resgister part: %d, type: %d", cn->part, type);
    if(cn->part != PROTOCOL_MODULE){
        return ERROR;
    }
    int pos = find_one_pos(type);
    protocol_t *cp = cn->parent->data[protocol_module.main_index];
    if(pos >= PROTOCOL_NUM){
        return ERROR;
    }
    debugln("register pos %d", pos);
    cp->read_event_handlers[pos] = read_event_handler;
    cp->write_event_handlers[pos] = write_event_handler;
    return OK;
}

int set_protocol_event_handler(connection_t* conn){
    config_node_t *cn = conn->listening->cn->parent;
    debugln("set part: %d, type: %d", cn->part, cn->type);
    if(cn->part != PROTOCOL_MODULE){
        return ERROR;
    }
    protocol_t *cp = cn->parent->data[protocol_module.main_index];
    int pos = find_one_pos(cn->type);

    conn->read->handler = cp->read_event_handlers[pos];
    conn->write->handler = cp->write_event_handlers[pos];
    return OK;
}