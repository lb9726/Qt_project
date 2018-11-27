#ifndef SERVER_H
#define SERVER_H
#include "logic_server.h"
//服务器初始化失败的话，释放分配的内存
void server_uninit();

//为服务器初始化做准备
int server_init();

//服务器端，调用socket生成套接字描述符,bind,listen，返回监听套接字
int create_server_proc(const char* ip, int port);

//处理是否接受客户端的请求
int accept_client_proc(int srvfd);

//向客户端发送信息
int handle_client_msg(int fd, MESSAGE buf, Login_STNODE * head, success_login *online_head);

//接收客户端的信息
void recv_client_msg(fd_set *readfds, Login_STNODE * head, success_login *online_head);

//select 轮询，处理客户信息
void handle_client_proc(int srvfd, Login_STNODE * head, success_login *online_head);

//忽略ctrl +c 键的处理函数
void sign_Handler(int signNO);

#endif // SERVER_H
