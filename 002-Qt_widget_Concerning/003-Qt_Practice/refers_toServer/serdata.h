#ifndef SERDATA_H
#define SERDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#include <sys/stat.h>
#include <fcntl.h>


#define MAXLINE 1000

struct message
{
    char flag[15];
    char name[20];
    char msg[MAXLINE];
    char addressee[20];
    int size;
};

typedef struct _clientinf
{
    char name[20];
    struct sockaddr_in addr_in;
    int decr;
    pthread_t pid;
    int speak;

}clientinf;

typedef struct _User
{
    char name[20];
    char passwd[20];
}User;


typedef struct _LNODE
{
    clientinf data;
    struct _LNODE *next;
}LNODE;

LNODE LHead;


typedef struct _List
{
    User user;
    struct _List *next;
}List;

List Head;

int getMaxFd(int * pint,int size);

void addFd(int * pint,int size,int fd);

void setAllFd(int * pint,int size,fd_set * fdset);

int fd;

int overlap(struct message meag);

int login(struct message *meag,clientinf *clientNode);

int login_check(struct message *meag);

void CoverBookNode();

int insertend(clientinf *clientNode);

int reg(struct message *meag,clientinf *clientNode);

int reg_check(struct message *meag);

int insert(User user);

void Readfiletolinklist();

void Readlinklisttofile();

void DestroyLinklist();

int all(struct message *meag,clientinf *clientNode);

int personal(struct message *meag,clientinf *clientNode);

int view(struct message *meag,clientinf *clientNode);


int uexit(struct message *meag,clientinf *clientNode);



#endif // SERDATA_H

