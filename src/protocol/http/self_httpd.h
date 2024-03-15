//
// Created by lzt11 on 2021/12/28.
//

#ifndef SOCKET_SELF_HTTPD_H
#define SOCKET_SELF_HTTPD_H

#include "lib.h"

#define BUF_SIZE 1024
#define SMALL_BUF 100
#define EPOLL_SIZE 1000

void request_handler(void *arg);
void send_data(FILE *fp,char *ct,char *file_name);

void send_400(FILE *fp);
void send_404(FILE *fp);
void send_405(FILE *fp);
void error_handling(char *message);

static int counter = 0;

#endif //SOCKET_SELF_HTTPD_H
