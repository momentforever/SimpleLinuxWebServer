//
// Created by lzt11 on 2021/12/28.
//

#include "self_httpd.h"
#include "lib.h"
#include <linux/limits.h>
#include <string.h>

void request_handler(void* arg){
    printf("start request_handler\n");
    int clnt_sock = *((int *)arg);
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

}

void send_data(FILE *fp,char *ct,char *file_name){
    debugln("start send_data");
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048 \r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type,"Content-type:%s\r\n\r\n",ct);

    char abs_path[PATH_MAX] = "../resources";
    strcat(abs_path, file_name);
    printf("send path:%s\n",abs_path);

    if(abs_path[strlen(abs_path)-1]=='/'){
        send_404(fp);
        return;
    }
    send_file = fopen(abs_path,"r");
    //send_file = fopen(file_name,"r");
    //printf("send_file:%p\n",send_file);
    if (send_file == NULL)
    {
        printf("file path not exist\n");
        send_404(fp);
        return;
    }

    //传输头消息
    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);

    //传输请求数据
    //读取静态页面
    while(fgets(buf,BUF_SIZE,send_file)!=NULL){
        fputs(buf,fp);
        //切换状态
        fflush(fp);
    }
    fflush(fp);
}

void send_405(FILE *fp){
    printf("response 405\n");
    char protocol[] = "HTTP/1.0 405 Method Not Allowed\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Method Not Allowed!!"
                     "</font></body></html>";

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void send_400(FILE *fp){
    printf("response 404\n");
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Bad Request!!"
                     "</font></body></html>";

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void send_404(FILE *fp){
    printf("response 404\n");
    char protocol[] = "HTTP/1.0 404 Page Not Found\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Page Not Found!!"
                     "</font></body></html>\r\n";
    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    //TODO not exit
    exit(0);
}