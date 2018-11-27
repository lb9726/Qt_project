#ifndef DATA_SERVER_H
#define DATA_SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define IPADDR      "127.0.0.1"
#define PORT        8787
#define MAXLINE     1024
#define LISTENQ     5
#define SIZE        10
#define ISFALSE 0
#define ISTRUE 1
#define ERROR 0
#define OK 1

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


typedef int Status;
typedef struct server_context_st
{
    int cli_cnt;        /*客户端个数*/
    int clifds[SIZE];   /*客户端的个数*/
    fd_set allfds;      /*句柄集合*/
    int maxfd;          /*句柄最大值*/
} server_context_st;

//登录验证时的客户的信息结构体
typedef struct reg_person_info
{
    int reg_id;              /*注册人员的编号，是唯一的*/
    char reg_name[40];       /*注册人员的昵称*/
    char reg_pwd[40];        /*注册人员的密码*/
    char reg_time[50];       /*注册的时间*/
}reg_person_info;

//登录表的结构体
typedef struct online_person_info
{
    int client_sockfd;        /*登录人员的套接字描述符*/
    int login_id;             /*登录人员的编号，是唯一的*/
    char login_name[50];      /*登录人员的昵称*/
    char login_pwd[50];       /*登录人员的密码*/
    int login_status;         /*在线状态*/
    int speak_status;         /*发言状态*/
    int logincounts;          /*登录次数*/
    char lastlogintime[40];   /*上次登录的时间*/
}online_person_info;

/*
//操作信息结构体
typedef struct MESSAGE
{
    int  pnum;           //归属人编号
    int  num;            //好友（其他人）编号
    char name[64];       //归属人姓名
    char msg[MAXLINE];   //发送消息的主体
    char flag[50];
    char regtime[40];    //用户注册时间
    int  state;         //在线状态
    int  clientfd;
    char IP[30];
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


/*
//登录信息
typedef struct logmes
{
    int id;                  //编号
    char name[30];           //登录人员的姓名
    char passwd[30];         //登录人员的密码
    int logincounts;         //登录次数
    char lastlogintime[40];  //上次登录的时间
}LOGMES;
*/

//注册表中登录人员信息结点
typedef struct Login_STNODE
{
    reg_person_info personinfo;
    struct Login_STNODE *next;
}Login_STNODE;

//成功登录系统的在线人员的信息结点
typedef struct success_login
{
    online_person_info perinfo;
    struct success_login *next;
}success_login;

//文件判空函数
Status jugde_file_isEmpty(char filename[50]);

void generate_node_for_login(reg_person_info people, Login_STNODE *head);

void  read_login_table(char filename[], Login_STNODE *head);

void get_time(char *systime);

void write_register_table(MESSAGE message);

void judge_allocate(void *p);

void judge_openfile(char filename[], FILE *fp);

Login_STNODE * readfiletolist_for_login(char filename[30]);

void print_personinfo(Login_STNODE *head);

void add_registerperson(Login_STNODE *head, reg_person_info msg);

//登录成功的客户信息补全
void client_login_success(int fd, success_login *head, online_person_info *permsg, MESSAGE msg);

//在线的登录成功的客户链表
success_login * online_client(success_login *head, online_person_info *permsg);

//打印在线客户端的信息
void print_online_client(success_login *head);

//将id编号存入整形数组中
void get_id_to_array(success_login *head, int a[]);

//数组排序从小到大，冒泡排序
void array_sort_small_to_big(int a[], int n);

//获取记录的条数
int Get_Lines(success_login *head);

//遍历生成第一个不存在的最小的id编号
int get_min_id(success_login *head);

/*服务器群发操作*/
void group_write_message(success_login *head, MESSAGE *buf);

/*处理群聊*/
void dealwith_chat_group(int fd, success_login *head, MESSAGE *buf);

/*回收系统资源1*/
void destroy_Login_STNODE(Login_STNODE *head);

/*回收系统资源2*/
void destroy_success_login(success_login *head);

/*用户退出聊天室时，将此用户从服务器的链表中删除*/
void user_exit_chatroom(int fd, success_login *head);


#endif // DATA_SERVER_H











