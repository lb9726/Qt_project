#include "logic_server.h"
//检测客户端的登录是否成功
Status check_client_login_request(Login_STNODE * head, MESSAGE message)
{
    Login_STNODE *nhead = head;
    Login_STNODE *p = nhead->next;
    while (NULL != p)
    {
        if ((p->personinfo.reg_id == message.pnum || (0 == strcmp(p->personinfo.reg_name, message.name)))
            && (0 == strcmp(p->personinfo.reg_pwd, message.msg)))
        {
            return OK;
        }
        p = p->next;
    }
    return ERROR;
}
