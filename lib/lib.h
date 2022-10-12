//
// Created by lzt11 on 2022/6/8.
//

#ifndef SOCKET_LIB_H
#define SOCKET_LIB_H


#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <bits/time.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#include <sys/epoll.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "cJSON.h"


#define ALL (-1)
#define ETERNITY (-1)
#define MAX_EVENT_NUM 1024
#define DEFAULT_PORT 80
#define TRUE 0
#define FALSE 1

#define ON ((unsigned) 1)
#define OFF ((unsigned) 0)

#define SERV 0
#define CLNT 1

#define OK 0
#define ERROR 1
#define AGAIN 2

#define W (unsigned)0
#define R (unsigned)1

#ifdef WITH_DEBUG
#include <assert.h>
#define debug(...) printf(__VA_ARGS__); printf("\n")
#define debug_assert(...) assert(__VA_ARGS__)
#else
#define debug_assert()
#define debug()
#endif

extern int g_epoll_fd;

typedef struct cycle_s cycle_t;
extern cycle_t *g_cycle;

typedef struct timer_rbtree_s timer_rbtree_t;
extern timer_rbtree_t *g_event_timer_tree;


typedef struct event_s event_t;
typedef struct connection_s connection_t;
typedef struct listening_s listening_t;

typedef void (*connection_handler_pt)(connection_t *c);


#define LISTENING_MODULE 1
#define EPOLL_MODULE 2
#define EVENT_MODULE 3

#define MAIN_MODULE     0b0001
#define PROTOCOL_MODULE 0b0010
#define SERVICE_MODULE  0b0100
#define ROUTE_MODULE    0b1000

#define MAIN            0b00000001
#define ALL_MAIN        0b11111111

#define TCP             0b00000001
#define UDP             0b00000010
#define HTTP            0b00000100
#define HTTPS           0b00001000
#define GRPC            0b00010000
#define ALL_PROTOCOL    0b11111111

#define UPSTREAM        0b0001
#define SERVER          0b0010
#define ALL_SERVICE     0b11111111


#define LOCATION        0b00000001
#define ALL_ROUTE       0b11111111

#define UNSET           0b00000000

#define MODULE_START 0,0,0,0

#endif //SOCKET_LIB_H
