
#include "./client.h"

/***************************************************
  函数名:handlerecvmsg
  功能:消息接收
  传入参数:int *sockfd
  返回值:无
 ***************************************************/
void handlerecvmsg(int *sockfd)
{
    int connfd = *sockfd;
    int nread;
    char buf[1024];
    char str[1024];
    struct message recvmsg;
    time_t timep;                              //显示当前时间
    if((fd = open(chat_log,O_RDWR|O_CREAT|O_APPEND,0777)) < 0)          //创建聊天记录文件
    {
        printf("打开聊天记录失败!");
        exit(1);
    }
    while(1)
    {
        printf(" %s() son child thread, in lines %d \n", __PRETTY_FUNCTION__, __LINE__);/********/
        nread = recv(connfd,&recvmsg,sizeof(struct message),0);    //接受文件
        printf("recvmsg.flag = %s\n", recvmsg.flag);
        if(nread == 0)
        {
            printf("与服务器的连接已断开,请检查!!!\n");
            close(fd);
            close(connfd);
            exit(0);
        }
        else if (strcmp(recvmsg.flag,"all") == 0)             //接受群发信息
        {
            time (&timep);
            memset(str,0,strlen(str));
            sprintf(str,"%s%s 对大家说: %s\n",ctime(&timep),recvmsg.name,recvmsg.msg);
            printf("%s",str);
            printf(" %s() now is in lines %d!\n", __PRETTY_FUNCTION__, __LINE__); /********/
            write(fd,str,strlen(str));                          //聊天信息写入文件
        }
        else if (strcmp(recvmsg.flag,"personal") == 0)         //接受私信
        {
            time (&timep);
            memset(str,0,strlen(str));
            sprintf(str,"%s%s 对你说: %s\n",ctime(&timep),recvmsg.name,recvmsg.msg);
            printf("%s() %s in lines %d!!!", __PRETTY_FUNCTION__ , str, __LINE__);
            write(fd,str,strlen(str));                         //保存聊天记录
        }
        else if (strcmp(recvmsg.flag,"sermsg") == 0)          //系统提示信息
        {
            printf("sermsg is server message\n");/********/
            /*ctime  会在字符串后面加上\n*/
            time (&timep);
            memset(str,0,strlen(str));
            sprintf(str,"%s系统信息: %s\n",ctime(&timep),recvmsg.msg);
            printf("%s ",str);
            printf("%s() in lines %d \n", __PRETTY_FUNCTION__ , __LINE__);
            write(fd,str,strlen(str));
            continue;
        }
        else if (strcmp(recvmsg.flag,"view") == 0)       //收到查看在线用户标志
        {
            time (&timep);
            memset(str,0,strlen(str));
            sprintf(str,"%s在线用户:\n%s\n",ctime(&timep),recvmsg.msg);
            printf("%s() %s in lines %d\n", __PRETTY_FUNCTION__ ,str,  __LINE__);
            continue;
        }
    }
}

/***************************************************
  函数名:cutStr
  功能:将字符串str在字符c处分割，前后两段
  分别赋给left和right，left最大长度为n，str最大
  长度为max
  传入参数:char str[],char left[], int n, char right[],int max, char c
  返回值:无
 ***************************************************/
void cutStr(char str[],char left[], int n, char right[],int max, char c)
{
    int i,k,j;
    for(i = 0 ; i < n ;i++)
    {
        if(str[i] == c)                             //c为str的分割符
            break;
    }
    if(i == n)
    {
        i = -1;
    }
    else
    {
        memset(left,0,strlen(left));
        for(k = 0 ; k < i ; k++)
        {
            left[k] = str[k];         //c左边的字符串赋给left[]
        }
    }
    for(j = i+1 ; j < max;j++)
    {
        if(str[j] == '\0')
            break;
        right[j-i-1] = str[j];     //c右边的字符串给right[]
    }
    left[i] = '\0';
    if(j < max)
    {
        right[j-i-1] = '\0';
    }
    else
    {
        right[max] = '\0';
    }
}















