#include "data_client.h"
//将fgets函数多存入的一个\n处理掉
void you_hua(char *p)
{
	while(*p != '\0')
	{	
		if(*p == '\n')
		{
		 	*p ='\0';
		 	break;
		}	
		p++;
	}
}

//模拟与服务器连接函数,动态打印>>
void dynamic_print()
{
	int i = 3;
	while(i)
	{
		printf(L_CYAN" >> >> "NONE);
		fflush(stdout);
		i--;
		usleep(500000);
	}
	printf("\n\n");
}

//处理键盘输入字符的函数，返回值为int型的
int input_digit(char left, char right)
{
    char ch[50];
    char li;
    int flag = 0;
    do
    {
        fgets(ch, 49, stdin);
        you_hua(ch);
        flag = strlen(ch);
        li = ch[0];
        setbuf(stdin, NULL);/*清空键盘缓冲区里的无用字符*/
        if(!(li >= left && li <= right))
            printf("\033[;31m您输入的操作选项有误，请重新输入!!\033[0m\n");
    }
    while(!(li >= left && li <= right));
    return li - '0';
}

//处理客户端发来的群聊的请求
void deal_with_group_chat()
{

}


/***************************************************
  函数名:cutStr
  功能:将字符串str在字符c处分割，前后两段
  分别赋给left和right，left最大长度为n，str最大
  长度为max
  传入参数:char str[],char left[], int n, char right[],int max, char c
  返回值:无
 ***************************************************/
void cutStr(char str[], char left[], int n, char right[], int max, char c)
{
    int i,k,j;
    for(i = 0 ; i < n ;i++)
    {
        if(c == str[i])                             //c为str的分割符
            break;
    }
    if(i == n)
    {
        i = -1;
    }
    else
    {
        memset(left, 0, strlen(left));
        for(k = 0 ; k < i ; k++)
        {
            left[k] = str[k];         //c左边的字符串赋给left[]
        }
    }

    left[i] = '\0';
printf("left = %s\n", left);
printf("i = %d n = %d\n", i, n);
    for(j = i+1 ; j < max; j++)
    {
        if(str[j] == '\0')
            break;
        right[j-i-1] = str[j];     //c右边的字符串给right[]
    }
    if(j < max)
    {
        right[j-i-1] = '\0';
    }
    else
    {
        right[max] = '\0';
    }
    printf("right = %s\n", right);
}

/*
功能:获取当前系统时间
传入参数:char 类型
返回值:void 类型
意义:获取当前系统时间  需要包含头文件#include <time.h>
*/
void gettime(char *systime)
{
    char year[20], mon[10], day[10], hour[10], min[10], sec[10];
    time_t rawtime;
    struct tm *tm;
    time ( &rawtime );
    tm = localtime ( &rawtime );
    sprintf(year, "%d", tm->tm_year+1900);
    if( tm->tm_mon + 1 < 10)
        sprintf(mon, "0%d", tm->tm_mon + 1);
    else
        sprintf(mon, "%d", tm->tm_mon + 1);
    if( tm->tm_mday  < 10)
        sprintf(day, "0%d", tm->tm_mday);
    else
        sprintf(day, "%d", tm->tm_mday);
    if( tm->tm_hour < 10)
        sprintf(hour, "0%d", tm->tm_hour);
    else
        sprintf(hour, "%d", tm->tm_hour);
    if( tm->tm_min < 10)
        sprintf(min, "0%d", tm->tm_min);
    else
        sprintf(min, "%d", tm->tm_min);
    if( tm->tm_sec < 10)
        sprintf(sec, "0%d", tm->tm_sec);
    else
        sprintf(sec, "%d", tm->tm_sec);
    sprintf(systime, "%s-%s-%s-%s:%s:%s", year, mon, day, hour, min, sec);//printf("systime = %s\n",systime);
}

/*登录成功之后处理接收的服务器信息*/
void handle_servermsg_afterlogin_success(int *sockfd)
{
    int connfd = *sockfd;
    int nread;
    int fd;
//    char buf[1024];
    char str[1024];
    char systime[50];
    MESSAGE recvmessage;
    printf("chat_log = %s\n", chat_log);
    if((fd = open(chat_log, O_RDWR|O_CREAT|O_APPEND, 0777)) < 0)          //创建聊天记录文件
    {
        printf("打开聊天记录失败!");
        exit(1);
    }
    while(1)
    {
        printf(" %s() son child thread, in lines %d \n", __PRETTY_FUNCTION__, __LINE__);/********/
        nread = read(connfd, &recvmessage, sizeof(MESSAGE));    //接受文件
        printf("recvmessage.flag = %s\n", recvmessage.flag);
        if(nread == 0)
        {
            printf("与服务器的连接已断开,请检查!!!\n");
            close(fd);
            close(connfd);
            exit(0);
        }
        else if (strcmp(recvmessage.flag, GROUP_CHAT) == 0)             //接受群发信息
        {
            gettime(systime);
            memset(str, 0, strlen(str));
            sprintf(str, "%s %s 对大家说: %s\n", systime, recvmessage.name, recvmessage.msg);
            printf("%s", str);
            printf(" %s() now is in lines %d!\n", __PRETTY_FUNCTION__, __LINE__); /********/
            write(fd, str, strlen(str));                          //聊天信息写入文件
        }
        else if (strcmp(recvmessage.flag, "personal") == 0)         //接受私信
        {
            gettime(systime);
            memset(str, 0, strlen(str));
            sprintf(str, "%s %s 对你说: %s\n", systime, recvmessage.name, recvmessage.msg);
            printf("%s() %s in lines %d!!!", __PRETTY_FUNCTION__ , str, __LINE__);
            write(fd, str, strlen(str));                         //保存聊天记录
        }
        else if (strcmp(recvmessage.flag, "sermsg") == 0)          //系统提示信息
        {
            gettime(systime);
            printf("sermsg is server message\n");/********/
            memset(str, 0, strlen(str));
            sprintf(str, "%s系统信息: %s\n", systime, recvmessage.msg);
            printf("%s ", str);
            printf("%s() in lines %d \n", __PRETTY_FUNCTION__ , __LINE__);
            write(fd, str, strlen(str));
            continue;
        }
        else if (strcmp(recvmessage.flag, "view") == 0)       //收到查看在线用户标志
        {
            gettime(systime);
            memset(str, 0, strlen(str));
            sprintf(str, "%s在线用户:\n%s\n", systime, recvmessage.msg);
            printf("%s() %s in lines %d\n", __PRETTY_FUNCTION__ , str, __LINE__);
            continue;
        }
    }
}

