//
// Created by lzt11 on 2022/6/8.
//

#ifndef SOCKET_GLOBAL_H
#define SOCKET_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define ALL (-1)
#define ETERNITY (-1)
#define MAX_EVENT_NUM 1024
#define DEFAULT_PORT 80
#define TRUE 0
#define FALSE 1

#define SERV 0
#define CLNT 1

#define OK 0
#define ERROR 1

#define UDP 1
#define TCP 2

#endif //SOCKET_GLOBAL_H
