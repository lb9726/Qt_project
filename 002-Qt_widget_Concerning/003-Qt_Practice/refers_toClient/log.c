#include "./log.h"

int  MenuUI()
{
    int iChoice = 0;
    system("clear");
    do
    {
        printf("\n\t****************************************************************\n");
        printf("\t*                                                              *\n");
        printf("\t*               ——————欢迎进入多功能聊天室——————               *\n");
        printf("\t*                                                              *\n");
        printf("\t****************************************************************\n");
        printf("\t                                                                \n");
        printf("\t                        相关操作选项                           \n");
        printf("\t                                                                \n");
        printf("\t****************************************************************\n");
        printf("\t*                        0.退    出                            *\n");
        printf("\t*                        1.注    册                            *\n");
        printf("\t*                        2.登    录                            *\n");
        printf("\t****************************************************************\n");
        printf("请输入功能编号1~3, 0 表示退出:");
        scanf("%d",&iChoice);
     }while((iChoice != 1) && (iChoice != 2) && (iChoice != 0)&& (iChoice != 3));
     return iChoice;
}

//模拟与服务器连接函数,动态打印
void printdot()
{
    int i = 3;
    while(i)
    {
        printf(" >> ");
        fflush(stdout);
        i--;
        sleep(1);
    }
    printf("\n");
}

//用户注册
int Register(struct message *a)
{
    char password_t[MAXLEN];
    do
    {
        printf("请输入用户名:");
        memset((*a).name,0,strlen((*a).name));
        setbuf(stdin,NULL);
        scanf("%s",(*a).name);
    }while(strlen((*a).name)>20);

    while(1)
    {
        printf("请输入密  码:");
        memset((*a).msg,0,strlen((*a).msg));
        setbuf(stdin,NULL);
        scanf("%s",(*a).msg);
        printf("请再输入密码:");
        memset(password_t,0,strlen(password_t));
        setbuf(stdin,NULL);
        scanf("%s",password_t);

        if(strcmp((*a).msg,password_t) != 0 || strlen((*a).msg)>20 || strlen(password_t)>20)
        {
            printf("密码出错！\n");
        }
        else
        {
            break;
        }
    }

    strcpy((*a).flag,"reg");
    send(sockfd,a,sizeof(*a),0);
    printf("正在注册，请稍等");
    fflush(stdout);
    printdot();
    recv(sockfd,a,sizeof(*a),0);
    printf("来自服务器的消息:%s\n",(*a).msg);
    sleep(2);
}

//用户登录
void log_user(struct message *a)
{
    do
    {
        printf("请输入用户名:");
        memset((*a).name,0,strlen((*a).name));
        scanf("%s",(*a).name);
        strcpy(locname,(*a).name);
        printf("请输入密  码:");
        memset((*a).msg,0,strlen((*a).msg));
        scanf("%s",(*a).msg);
    }while(strlen((*a).name)>20 || strlen((*a).msg)>20);
    strcpy((*a).flag,"login");
    send(sockfd,a,sizeof(*a),0);                  //向服务器发送登录信息
    printf("正在登录，请稍等");
    fflush(stdout);
    printdot();
    recv(sockfd,a,sizeof(*a),0);
    printf("123来自服务器的消息:%s\n",(*a).msg);
}

 //修改密码
 void findpasswd()
 {

 }

//功能:登录成功进入聊天模式
int login_success(struct message *a)
{
    char str[MAXLEN];
    char buf[MAXLEN];
    time_t timep;
    pthread_t pid;
    sprintf(chat_log,"./chat/%s.txt",(*a).name);
    if((fd = open(chat_log,O_RDWR|O_CREAT|O_APPEND,0777)) < 0)
    {
        printf("打开聊天记录失败!");
        exit(1);
    }
    pthread_create(&pid,NULL,(void*)handlerecvmsg,(void *)&sockfd);   //创建接受消息线程
    setbuf(stdin,NULL);
    strcpy((*a).flag,"all");
    printf("%s您好，如需帮助请输入：help\n",locname);
    while(1)
    {
        printf("%s(), in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);  /********/
        memset((*a).msg,0,strlen((*a).msg));
        memset(str,0,strlen(str));
        printf(" jiejie\n");    /********/
        //usleep(100000);

        printf("%s() hello in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);   /********/
        printf("TO %s:\n",(*a).flag);
        setbuf(stdin, NULL);
        scanf("%s",str);
        if(1 == help(str))                            //提示信息
        {
            continue;
        }

        strcpy((*a).name,locname);
        strcpy(buf,(*a).flag);
        cutStr(str,(*a).flag,15,(*a).msg,MAXLEN,'#'); //调用字符切割函数

        if(strcmp((*a).flag,"exit") == 0)
        {
            return 0;
        }
        if(strcmp((*a).flag,"view") == 0)             //请求查看在线用户
        {
            send(sockfd,a,sizeof((*a)),0);
            strcpy((*a).flag,buf);  printf("%s() in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);   /********/
            continue;
        }
        if(strcmp((*a).flag,"all") == 0)      //群聊
        {
            send(sockfd,a,sizeof(*a),0);
            continue;
        }
        if (strcmp((*a).flag,"trans") == 0)
        {
            strcpy((*a).flag,buf); printf("%s() in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);   /********/
        }
        else if (strcmp((*a).flag,"history") == 0)  // 打开对应的用户的聊天纪录
        {
            openfile((*a).name);
        }
        else
        {
            strcpy(buf,(*a).flag);
            strcpy((*a).addressee,(*a).flag);
            strcpy((*a).flag,"personal");
            send(sockfd,a,sizeof(*a),0);             //发送私信
            strcpy((*a).flag,buf);
            time (&timep);
            memset(str,0,strlen(str));
            sprintf(str,"%s你对 %s 说: %s\n",ctime(&timep),(*a).flag,(*a).msg);
            printf("123 %s",str);  printf("%s() in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);   /********/
            write(fd,str,strlen(str));              //写入聊天记录文件中
        }

    }
    return 1;
}

//功能帮助界面
int help(char str[])
{
    if(strcmp(str,"help") == 0)
    {
        printf("%s 用户，您好：\n",locname);
        printf("all#hello!-------------对所有人说hello!\n");
        printf("用户名#要说的话!--------跟该用户私聊!\n");
        printf("view#------------------查看在线用户\n");
        printf("history#---------------查看聊天记录\n");
        printf("exit#------------------退出\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
















