#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MAXLEN 1000

struct message                  //消息结构体
{
    char flag[15];              //标志位
    char name[20];              //用户名
    char msg[MAXLEN];           //消息内容
    char addressee[20];         //传输文件目的用户
    int size;                   //传输内容字节数
};
extern int qid,fd;
extern int sockfd;              //套接字描述符
extern int savefilefd;          //保存文件描述符
extern char filefromname[20];   //文件来源名
extern char chat_log[100];      //聊天记录名
extern pthread_mutex_t lock ;   //线程锁
char filefromuser[20];          //文件发送者用户名
char locname[20];               //本客户端用户名

void handlerecvmsg(int *sockfd);

void cutStr(char str[],char left[], int n, char right[],int max, char c);

#endif //CLIENT_H


