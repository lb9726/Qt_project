#ifndef DATA_CLIENT_H
#define DATA_CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <curses.h>
#include <menu.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAXLINE 1024
#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8787
#define LEN 500
#define max(a,b) (a > b) ? a : b
#define Is_OK 1
#define Is_ERROR 0

#define NONE "\e[0m"
#define BLACK "\e[0;30m"
#define L_BLACK "\e[1;30m"
#define RED "\e[0;31m"
#define L_RED "\e[1;31m"
#define GREEN "\e[0;32m"
#define L_GREEN "\e[1;32m"
#define BROWN "\e[0;33m"
#define YELLOW "\e[1;33m"
#define BLUE "\e[0;34m"
#define L_BLUE "\e[1;34m"
#define PURPLE "\e[0;35m"
#define L_PURPLE "\e[1;35m"
#define CYAN "\e[0;36m"
#define L_CYAN "\e[1;36m"
#define GRAY "\e[0;37m"
#define WHITE "\e[1;37m"
 
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"
#define BLINK "\e[5m"
#define REVERSE "\e[7m"
#define HIDE "\e[8m"
#define CLEAR "\e[2J"
#define CLRLINE "\r\e[K" //or "\e[1K\r"

#define SERVER_MSG        "server_msg"         //服务器的消息
#define REGISTER          "register"           //注册
#define REGISTER_SUCCESS  "register_success"   //注册成功
#define REGISTER_FAILURE  "register_failure"   //注册失败
#define LOGIN_SUCCESS     "logic_success"      //登录成功
#define LOGIN_FAILURE     "logic_failure"      //登录失败
#define GROUP_CHAT        "group_chat"         //群聊
#define PERSONAL_CHAT     "personal_chat"      //私聊
#define VIEW_ONLINE       "view_online"        //查看在线人员
#define FILE_TRANS        "file_trans"         //文件传输
#define CHANGE_PWD        "change_pwd"         //修改密码
#define EXIT_CHATROOM     "exit_chatroom"      //离开聊天室

/*
enum INFO_TYPES
{
    server_msg = 0,
    group_chat,
    personal_chat,
    view_online,
    file_trans,
    change_pwd,
    exit
};
*/

char locname[20];               //本客户端用户名
char chat_log[100];             //创建登录成功的用户的聊天记录的新文件
typedef int Status;
//注册客户的信息结构体
typedef struct reg_person_info
{
    int reg_num;        /*注册人员的编号，是唯一的*/
    char reg_name[40];  /*注册人员的昵称*/
    char reg_pwd[40];   /*注册人员的密码*/
}reg_person_info;

/*
typedef struct _MESSAGE
{	
	int  pnum;         //归属人编号
	int  num;          //编号
	char name[64];     //用户名
	char msg[LEN];     //登录时是密码，聊天时就是聊天内容
	char flag[30];     //操作标志
	int  state;        //在线状态
}MESSAGE;
*/

//操作信息结构体
typedef struct MESSAGE
{
    int  pnum;           //归属人编号
//    int  num;            //好友（其他人）编号
    char flag[50];
    char name[64];       //归属人姓名
    char msg[MAXLINE];   //发送消息的主体
//    char regtime[40];    //用户注册时间
//    int  state;         //在线状态
//    int  clientfd;
//    char IP[30];
}MESSAGE;

//将fgets函数多存入的一个\n处理掉
void you_hua(char *p);

void dynamic_print();

//处理键盘输入字符的函数，返回值为int型的
int input_digit(char left, char right);

//处理客户端发来的群聊的请求
void deal_with_group_chat();

void cutStr(char str[],char left[], int n, char right[],int max, char c);

void gettime(char *systime);

/*登录成功之后处理接收的服务器信息*/
void handle_servermsg_afterlogin_success(int *sockfd);

#endif // DATA_CLIENT_H_INCLUDED














