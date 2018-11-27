#include "client.h"
int Init_nums = 2;
void handle_recv_msg(int sockfd, MESSAGE buf) 
{
    if (0 == strcmp(buf.flag, "注册成功")) 
    {
        printf("client recv msg is:%s\n", buf.flag);
        printf("您已经注册成功,请登录!\n");
        ui_login(sockfd);
    }
    else if (0 == strcmp(buf.flag, "登录成功"))
    {
        printf("in login cases client recv msg is:%s\n", buf.flag);
        printf(" %s(), name = %s\n ", __PRETTY_FUNCTION__, buf.name);
        strcpy(locname, buf.name);
        sprintf(chat_log, "./chat_records/%s.txt", buf.name);
        ui_mainchat(sockfd, &buf);
    }
    else if (0 == strcmp(buf.flag, "登录失败"))
    {
        printf("fuck is failed client recv msg is:%s\n", buf.flag);
        cirlce_login_failed(sockfd, &Init_nums, buf);
    }
}

void handle_connection(int sockfd)
{
    MESSAGE message;
    int maxfdp ;
    fd_set readfds;
    int n;
//    struct timeval tv;
    int retval = 0;
    int logintime = 0;
    while (1) 
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        maxfdp = sockfd;

//        tv.tv_sec = 5;
//        tv.tv_usec = 0;
        if (0 == logintime)
        {
            ui_register_login(sockfd);
            logintime ++;
        }
        printf("beyond login\n");
        retval = select(maxfdp + 1, &readfds, NULL, NULL, NULL);
        printf("retval = %d\n", retval);
        if (-1 == retval)
        {
            return ;
        }

        if (0 == retval)
        {
            printf("client timeout.\n");
            continue;
        }

        if (FD_ISSET(sockfd, &readfds))
        {
            n = read(sockfd, &message, MAXLINE);
            if (n <= 0)
            {
                fprintf(stderr,"client: server is closed,bye,bye!!\n");
                close(sockfd);
                FD_CLR(sockfd, &readfds);
                return;
            }
            handle_recv_msg(sockfd, message);
        }
    }
}

int main(int argc,char *argv[])
{
    int sockfd;
//    MESSAGE message;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);

    int retval = 0;
    retval = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (retval < 0) 
    {
        fprintf(stderr, "connect fail,error:%s\n", strerror(errno));
        return -1;
    }
    printf("client send to server .\n");
    handle_connection(sockfd);
    return 0;
}











