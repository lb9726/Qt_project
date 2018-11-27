#ifndef LOGIC_CLIENT_H
#define LOGIC_CLIENT_H

#include "data_client.h"

// 验证注册输入的用户名是否合法
Status check_input_name(char username[]);

// 验证登录输入的密码是否合法
Status check_input_pwd(char pwd[]);

// 验证注册两次输入的密码长度是否正确是否一致
Status check_register_pwd( char userpwd[], char pwdagain[]);

#endif // LOGIC_CLIENT_H_INCLUDED
