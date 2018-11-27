#ifndef LOG_H
#define LOG_H
#include "./client.h"
int  MenuUI();

//模拟与服务器连接函数,动态打印
void printdot();

//用户注册
int Register(struct message *a);

//用户登录
void log_user(struct message *a);

//功能:登录成功进入聊天模式
int login_success(struct message *a);

//功能帮助界面
int help(char str[]);

#endif // LOG_H

