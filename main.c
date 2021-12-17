// TODO 1.实现一个简单的web服务器,通过epoll OK
// TODO 2.尝试实现一个非阻塞的web服务器
// TODO 3.尝试实现一个异步的web服务器

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100
#define EPOLL_SIZE 50

void request_handler(void *arg);
void send_data(FILE *fp,char *ct,char *file_name);

void send_400(FILE *fp);
void send_404(FILE *fp);
void send_405(FILE *fp);
void error_handling(char *message);

static int counter = 0;

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int i;

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    printf("socket initing...\n");

    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }
    if(listen(serv_sock,SOMAXCONN) == -1){
        error_handling("listen() error");
    }

    epfd = epoll_create(EPOLL_SIZE); //可以忽略这个参数，填入的参数为操作系统参考
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;//需要读取数据的情况
    event.data.fd = serv_sock;
    epoll_ctl(epfd,EPOLL_CTL_ADD, serv_sock,&event);//例程epfd中添加文件描述符serv_sock，目的是监听event中的事件

    while(1){

        event_cnt = epoll_wait(epfd,ep_events,EPOLL_SIZE,-1); //获取改变了的文件描述符，返回数量
        //event_cnt = epoll_wait(epfd,ep_events,EPOLL_SIZE,0);

        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }
        for(i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == serv_sock){
                printf("counter:%d\n",counter++);
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_adr, &adr_sz);
                request_handler(&clnt_sock);
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void request_handler(void* arg){
    int clnt_sock = *((int *)arg);
    char req_line[SMALL_BUF];
    FILE *clnt_read;
    FILE *clnt_write;

    char filter[3][30];

    char method[10];
    char ct[15];
    char file_name[30];

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


    fgets(req_line,SMALL_BUF,clnt_read);
    //GET / HTTP/1.1
    //GET /index.html HTTP/1.1
    printf("%s\n",req_line);

    //strstr 查找字符串
    //是否为HTTP请求
    if(strstr(req_line,"HTTP/")==NULL){
        send_400(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    //strcpy 字符串值复制
    //strtok 字符串分割
    // method = req_line[0]
    // file_name = req_line[1]

    // method
    //strcpy(method,strtok(req_line," /"));
    strcpy(filter[0],strtok(req_line," "));
    printf("method: %s\n",filter[0]);

    //file_name
    //strcpy(file_name,strtok(NULL," /"));
    strcpy(filter[1],strtok(NULL," "));
    printf("file_name: %s\n",filter[1]);
    //other
    strcpy(filter[2],strtok(NULL," "));
    printf("other: %s\n",filter[2]);

    // 方法只允许GET
    if(strcmp(filter[0],"GET")!=0){
        send_405(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }

    //正常请求
    //strcpy(ct,content_type(file_name));
    strcpy(ct,"text/html");
    strcpy(file_name,filter[1]);
    send_data(clnt_write,ct,file_name);
    fclose(clnt_read);
    fclose(clnt_write);
}

void send_data(FILE *fp,char *ct,char *file_name){
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048 \r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type,"Content-type:%s\r\n\r\n",ct);

    char abs_path[] = "/root/projects/Socket";
    strcat(abs_path,file_name);
    printf("abs_path:%s\n",abs_path);

    if(abs_path[strlen(abs_path)-1]=='/'){
        send_404(fp);
        return;
    }
    send_file = fopen(abs_path,"r");
    //send_file = fopen(file_name,"r");
    printf("send_file:%p\n",send_file);
    if (send_file == NULL)
    {
        send_404(fp);
        return;
    }

    //传输头消息
    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);

    //传输请求数据
    while(fgets(buf,BUF_SIZE,send_file)!=NULL){
        fputs(buf,fp);
        //切换状态
        fflush(fp);
    }
    fflush(fp);
}

void send_405(FILE *fp){
    char protocol[] = "HTTP/1.0 405 Method Not Allowed\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Method Not Allowed!!"
                     "</font></body></html>";

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void send_400(FILE *fp){
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048 \r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Bad Request!!"
                     "</font></body></html>";

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void send_404(FILE *fp){
    char protocol[] = "HTTP/1.0 404 Page Not Found\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048 \r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Page Not Found!!"
                     "</font></body></html>";

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);
    fputs(content,fp);
    fflush(fp);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(0);
}