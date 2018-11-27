#ifndef MAIN_H
#define MAIN_H
#include "log.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <signal.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <string.h>
//#include <errno.h>
//#include <pthread.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <sys/stat.h>
#define PORT 5678
#define MAXLEN 1000

//struct message    /*消息结构体*/
//{
//    char flag[15];             /*标志位*/
//    char name[20];             /*用户名*/
//    char msg[MAXLEN];          /*消息内容*/
//    char addressee[20];        /*传输文件目的用户*/
//    int size;                  /*传输内容字节数*/
//};
int qid = -1,fd = -1;
int sockfd = -1;               /*套接字描述符*/
int savefilefd = -1;           /*保存文件描述符*/
char filefromname[20];         /*文件来源名*/
char chat_log[100];            /*聊天记录名*/
pthread_mutex_t lock ;         /*线程锁*/
void openfile(char name[30]);

#endif // MAIN_H
