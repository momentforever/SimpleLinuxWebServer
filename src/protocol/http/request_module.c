//
// Created by lzt11 on 2022/10/20.
//

#include "config.h"
#include "memory.h"
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

void http_request_init(http_request_t* hr){
    hr->conf = NULL;
    hr->pool = memory_pool_create();
    hr->conn = NULL;

    hr->header_in = NULL;
    hr->args = NULL;
    hr->uri = NULL;
    hr->exten = NULL;
    hr->unparsed_uri = NULL;
}

void http_pre_handler(connection_t *conn){
    http_request_t* hr = pmalloc(conn->pool,sizeof(http_request_t));
    if(hr == NULL){
        return;
    }
    http_request_init(hr);
    
    conn->data = hr;
}


void http_read_handler(event_t *ev){
    debugln("read_handler");
    ev->active = ON;
    
    connection_t *conn = ev->data;
    // printf("start request_handler\n");  
    int clnt_sock = conn->fd;
    char req_line[SMALL_BUF];
    FILE *clnt_read;
    FILE *clnt_write;

    char filter[3][30];

    if((clnt_read = fdopen(clnt_sock,"r"))==NULL){
        error_handling("fopen() error");
    }
    int temp_clnt_write;
    if((temp_clnt_write = dup(clnt_sock))==-1){
        error_handling("dup() error");
    }
    if((clnt_write = fdopen(temp_clnt_write,"w"))==NULL){
        error_handling("fopen() error");
    }

    printf("=====================\n");
    fgets(req_line,SMALL_BUF,clnt_read);
    printf("%s",req_line);
    //    GET / HTTP/1.1
    //    Host: 127.0.0.1:10005
    //    User-Agent: curl/7.68.0
    //    Accept: */*
    //
    char example_line[SMALL_BUF];
    while(!feof(clnt_read) && strcmp(example_line,"\r\n") != 0){
        fgets(example_line,SMALL_BUF,clnt_read);
        printf("%s",example_line);
    }
    printf("=====================\n");
    //strstr 查找字符串
    //是否为HTTP请求
    if(strstr(req_line,"HTTP/")==NULL){
        send_400(clnt_write);
        // 半关闭-写
        shutdown(fileno(clnt_read),SHUT_WR);
        fclose(clnt_write);
        // 关闭
        fclose(clnt_read);
        return;
    }
    //strcpy 字符串值复制
    //strtok 字符串分割
    //strcpy(method,strtok(req_line," /"));
    strcpy(filter[0],strtok(req_line," "));
    printf("request method: %s\n",filter[0]);

    //file_name
    //strcpy(file_name,strtok(NULL," /"));
    strcpy(filter[1],strtok(NULL," "));
    printf("request path: %s\n",filter[1]);
    //other
    strcpy(filter[2],strtok(NULL," "));
    printf("request other: %s\n",filter[2]);
    // 方法只允许GET
    if(strcmp(filter[0],"GET")!=0){
        send_405(clnt_write);
        // 半关闭-写
        shutdown(fileno(clnt_read),SHUT_WR);
        fclose(clnt_write);
        // 关闭
        fclose(clnt_read);
        return;
    }
    //正常请求
    send_data(clnt_write,"text/html",filter[1]);

    // 半关闭-写
    shutdown(fileno(clnt_read),SHUT_WR);
    fclose(clnt_write);
    // 关闭
    fclose(clnt_read);

    ev->active = OFF;
    ev->eof = ON;
}

void http_write_handler(event_t *ev){
    debugln("write_handler");
    // connection_t *con = ev->data;
    ev->active = ON;

    ev->active = OFF;
    ev->eof = ON;
}

int init_protocol_http_request(cJSON* data, config_node_t *cn, config_t* c){
    int result;
    if(!(cn->type == HTTP || cn->type == HTTPS)){
        return IGNORE;
    }
    result = resigster_protocol_event_handler(http_read_handler,
                                                http_write_handler,
                                                http_pre_handler,
                                                cn, c, HTTP);
    if(result == ERROR){
        return ERROR;
    }
    return OK;
}
