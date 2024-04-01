//
// Created by lzt11 on 2022/10/20.
//

#include "protocol_module.h"
#include "request_module.h"
#include "lib.h"
#include "self_httpd.h"

int init_protocol_http_request(cJSON* data, config_node_t *cn, config_t* c);

static command_t http_request_commands[] = {
        NULL
};

module_t http_request_module = {
        MODULE_START,
        "http_request_module",
        SERVICE_MODULE,
        UNSET,
        UNSET,
        UNSET,
        init_protocol_http_request,
        UNSET,
        UNSET,
        http_request_commands,
};

void http_write_handler(event_t *ev){
    debugln("write_handler");
    // connection_t *con = ev->data;
    // request_handler((void*)&con->fd);
}

void http_read_handler(event_t *ev){
    debugln("read_handler");
    ev->active = ON;

    // connection_t *con = ev->data;
    // request_handler((void*)&con->fd);
    ev->active = OFF;
    ev->eof = ON;
}

int init_protocol_http_request(cJSON* data, config_node_t *cn, config_t* c){
    int result;
    if(!(cn->type == HTTP || cn->type == HTTPS)){
        return IGNORE;
    }
    result = resigster_protocol_event_handler(http_read_handler, http_write_handler, 
                                                cn, c, HTTP);
    if(result == ERROR){
        return ERROR;
    }
    return OK;
}
