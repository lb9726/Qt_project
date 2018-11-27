#include "logic_client.h"

// 验证注册输入的用户名是否合法
Status check_input_name(char username[])
{
    char *p = username;
    char c = '\0';
    if (strlen(username) >= 10)
    {
        printf(RED"用户名长度大于等于10,过长！\n"NONE);
        return Is_ERROR;
    }
    if (0 == strlen(username))
    {
        printf(RED"用户名输入不能为空！\n"NONE);
        return Is_ERROR;
    }
    while (*p != '\0')
    {
        c = *p;
        p ++;
        if ('\0' == *p && ' ' == c)
        {
            printf(RED"用户名末尾不能包含空格!\n"NONE);
            return Is_ERROR;
        }
    }
//    printf("username = %s",username);
    return Is_OK;
}

// 验证登录输入的密码是否合法
Status check_input_pwd(char pwd[])
{
    if (0 == strlen(pwd))
    {
        printf(RED"密码输入不能为空！\n"NONE);
        return Is_ERROR;
    }
}

// 验证注册两次输入的密码长度是否正确是否一致
Status check_register_pwd( char userpwd[], char pwdagain[])
{
    if (0 == strlen(userpwd))
    {
        printf(RED"密码输入不能为空！\n"NONE);
        return Is_ERROR;
    }
    else if (strlen(userpwd) <= 4 || strlen(userpwd) >= 20)
    {
        printf(RED"密码长度不能短于4个字符,不能大于20个字符！\n"NONE);
        return Is_ERROR;
    }
    //printf("userpwd = %s, pwdagain = %s\n",userpwd, pwdagain);
    if (0 != strcmp(userpwd, pwdagain))
    {
        printf(RED"两次输入的密码不一致！\n"NONE);
        return Is_ERROR;
    }
    return Is_OK;
}
