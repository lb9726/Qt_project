#ifndef UI_CLIENT
#define UI_CLIENT

#include "logic_client.h"

//1.客户注册
int ui_register(int fd);

//2.客户端登录
int ui_login(int sockfd);

//3.登录注册总界面
int ui_register_login(int sockfd);

//4.私聊界面
int ui_private_chat(int sockfd);

//5.群聊界面
int ui_group_chat(int sockfd);

//6.聊天主界面
int ui_mainchat(int sockfd, MESSAGE *message);

//7.主功能界面
int ui_main(int sockfd);

/*登录时有3次机会，如果登录错误大于等于3次时，直接退出客户端*/
void cirlce_login_failed(int sockfd, int *n, MESSAGE message);

#endif // LOGIC_CLIENT_H
