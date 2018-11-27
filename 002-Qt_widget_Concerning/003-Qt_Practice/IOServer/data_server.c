#include "data_server.h"

/*
功能:文件判空函数
传入参数:char 类型的数组的文件名
返回值:int 类型
意义:返回ISTRUE表示为空，ISFALSE不为空
*/

Status jugde_file_isEmpty(char filename[50])
{
    int filepos = -1;
    FILE *file = fopen(filename, "rt");
    if(NULL == file)
    {
        printf("%s opening file %s occurs error in lines %d！\n",__PRETTY_FUNCTION__, filename, __LINE__);
        exit(0);
    }
    fseek(file, 0, SEEK_END);//将文件指针放到文件尾部
    filepos = ftell(file);//报告文件距离文件开头的位移量，如果为0，则表示文件为空
    if(0 == filepos)
    {
        fclose(file);
        file = NULL;
        return ISTRUE;     // 文件为空
    }
    else
    {
        fclose(file);
        file = NULL;
        return ISFALSE;   // 文件不为空
    }
}

/**********************************************************
功能:动态生成指针节点，尾插法。辅助read_file_use_fscanf函数的
使用:在read_file_use_fscanf函数中调用
传入参数:STNODE * 的头指针和 PERSONINFO people
返回值:void,对地址进行操作，无须返回头节点
**********************************************************/

void generate_node_for_login(reg_person_info people, Login_STNODE *head)
{
    Login_STNODE *p = (Login_STNODE *)malloc(sizeof(Login_STNODE)); //创建一个新结点p
    Login_STNODE *tmp = head;
    while(tmp->next != NULL)  //将指针移动到链表的尾部，即最后一个元素的地方,做尾插法操作
        tmp = tmp->next;
    tmp->next = p;
    p->personinfo = people;
    p->next = NULL;
    return ;
}

/*
功能:fopen读文件生成链表
传入参数:char 类型的数组文件名
返回值:void 类型
意义:生成链表
*/
//读信息到登录表中，从登录表中获取上次登录时间和次数
void read_login_table(char filename[], Login_STNODE *head)
{
    FILE *file = fopen(filename,"rb");
    reg_person_info people;
    if(NULL == file)
    {
        printf(" %s() opening file <%s> occurs error in lines %d！", __PRETTY_FUNCTION__, filename, __LINE__);
        exit(0);
    }
    int flag = jugde_file_isEmpty(filename);
    while(!feof(file) && (!flag))
    {
        fscanf(file,"%d %s %s %s\n",
            &people.reg_id,
            people.reg_name,
            people.reg_pwd,
            people.reg_time
            );//从文件读入记录
        generate_node_for_login(people, head);
    }//end of while
    fclose(file);
    file = NULL;
}

/*
功能:获取当前系统时间
传入参数:char 类型
返回值:void 类型
意义:获取当前系统时间  需要包含头文件#include <time.h>
*/
void get_time(char *systime)
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

/*
功能:写客户的注册信息表
传入参数:MESSAGE 类型
返回值:void 类型
意义:写客户注册表
*/
void write_register_table(MESSAGE message)
{
    reg_person_info user_info;
    user_info.reg_id = 1000;
    strcpy(user_info.reg_name, message.name);
    strcpy(user_info.reg_pwd, message.msg);
    get_time(user_info.reg_time);
//    user_info.logincounts = 0;
//    gettime(user_info.lastlogintime);
    FILE *fp = NULL;
    fp = fopen("./dat/register_info.txt","ab");
    judge_openfile("./dat/register_info.txt", fp);
    fprintf(fp, "%-10d %-17s %-17s %-30s \n",
            user_info.reg_id,
            user_info.reg_name,
            user_info.reg_pwd,
            user_info.reg_time
//            user_info.logincounts,
//            user_info.lastlogintime
            );//写文件操作
    fclose(fp);
    fp = NULL;
    return ;
}

/*
功能:判断分配空间是否成功
传入参数:指针类型
返回值:void 类型
意义:判断分配空间是否成功,成功继续运行，失败退出系统
*/
void judge_allocate(void *p)
{
    if(NULL == p)
    {
        printf("%s() phsical memory allocate failure in lines %d exit system!!\n", __PRETTY_FUNCTION__, __LINE__);
        exit(0);
    }
    else
    {
        return;
    }
}

/*
功能:判断打开文件是否成功
传入参数:char 和 文件指针类型
返回值:void 类型
意义:判断打开文件是否成功,成功继续运行，失败退出系统
*/
void judge_openfile(char filename[], FILE *fp)
{
    if (NULL == fp)
    {
        printf(" %s() opening file <%s> occurs error in lines %d！",__PRETTY_FUNCTION__, filename, __LINE__);
        exit(0);
    }
    else
    {
        return ;
    }
}

/*
功能:读文件生成注册了的人员信息表
传入参数:char 和 文件指针类型
返回值:void 类型
意义:生成注册了的人员信息表
*/
Login_STNODE * readfiletolist_for_login(char filename[30])
{
    Login_STNODE *head = (Login_STNODE *)malloc(sizeof(Login_STNODE));
    judge_allocate(head);
    head->next = NULL;
    read_login_table(filename, head);
    return head;
}

void print_personinfo(Login_STNODE *head)
{
    Login_STNODE *p = head->next;
    while(p != NULL)
    {
        printf("%d %s %s %s \n",
            p->personinfo.reg_id,
            p->personinfo.reg_name,
            p->personinfo.reg_pwd,
            p->personinfo.reg_time/*,
            p->personinfo.logincounts,
            p->personinfo.lastlogintime*/
            );//从文件读入记录
        p = p->next;
    }
}

/*
 *每当注册成功一个用户之后，都将其加入到注册表的链表的尾部，并且写入文件中
 */
void add_registerperson(Login_STNODE *head, reg_person_info msg)
{
    Login_STNODE *p = head;
    while (NULL != p->next)
    {
        p = p->next;
    }
    Login_STNODE *tmp = (Login_STNODE *)malloc(sizeof(Login_STNODE));
    judge_allocate(tmp);
    p->next = tmp ;
    tmp->personinfo = msg;
    tmp->next = NULL;
}

//登录成功的客户信息补全
void client_login_success(int fd, success_login *head, online_person_info *permsg, MESSAGE msg)
{
    permsg->client_sockfd = fd; printf("fd = %d\n", fd);
    permsg->login_id = get_min_id(head);printf("loginid = %d\n", permsg->login_id);
    strcpy(permsg->login_name, msg.name);printf("login_name = %s\n", permsg->login_name);
    strcpy(permsg->login_pwd, msg.msg); printf("login_pwd = %s\n", permsg->login_pwd);
    permsg->login_status = 1;
    permsg->speak_status = 1;
    permsg->logincounts = 0;
    get_time(permsg->lastlogintime);printf("lastlogintime = %s\n", permsg->lastlogintime);
}

//在线的登录成功的客户链表
success_login * online_client(success_login *head, online_person_info *permsg)
{
    success_login *p = head;
    while(NULL != p->next)
    {
        p = p->next;
    }
    success_login *tmp = (success_login *)malloc(sizeof(success_login));
    judge_allocate(tmp);
    p->next = tmp;
    tmp->perinfo = *permsg;
    tmp->next = NULL;
}

//打印在线客户端的信息
void print_online_client(success_login *head)
{
    success_login *p = head->next;
    while(NULL != p)
    {
        printf("%d %d %s %s %d %d %d %s \n", p->perinfo.client_sockfd, p->perinfo.login_id,
               p->perinfo.login_name, p->perinfo.login_pwd, p->perinfo.login_status,
               p->perinfo.speak_status, p->perinfo.logincounts, p->perinfo.lastlogintime);
        p = p->next;
    }
}

//将id编号存入整形数组中
void get_id_to_array(success_login *head,int a[])
{
    success_login *p = head->next;
    int i = 0;
    while(p != NULL)
    {
        a[i] = p->perinfo.login_id;
        i++;
        p = p->next;
    }
}

//数组排序从小到大，冒泡排序
void array_sort_small_to_big(int a[],int n)
{
    int i,j,temp;
    for(i = 0; i < n-1; i++ ) //外循环的趟数是数组个数减一
    {
        for(j = 0; j < n-i-1; j++)//每进行一次外循环内循环就减一
        {
            if(a[j] > a[j+1])//每一次外层循环内层循环都需要把相邻位置的两个数进行比较
            {
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
}

//获取记录的条数
int Get_Lines(success_login *head)
{

    int a = 0;
    success_login *p = head->next;
    while(p != NULL)
    {
        a++;
        p =  p->next;
    }//printf("a = %d\n",a);
    return a;
}

//遍历生成第一个不存在的最小的id编号
int get_min_id(success_login *head)
{
    int id = 1000;
    if(head->next == NULL)
    {
        return id;
    }
    int len = Get_Lines(head);            //获取记录条数
    int Lenarry[len];                    //记录条数作为数组长度
    get_id_to_array(head,Lenarry);       //将id存入数组中来
    array_sort_small_to_big(Lenarry, len);//进行一轮冒泡排序
    int i = 0;            //遍历生成第一个未使用的最小的id编号
    for(i = 0; i < len ; i++)
    {
        if(id == Lenarry[i])
        {
            id ++;
        }
        else
        {//printf("%d\n",id);
            break;
        }
    }
    return id;
}

/*服务器群发操作*/
void group_write_message(success_login *head, MESSAGE *buf)
{
    success_login *p = head->next;
    printf("%s(), in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);  /********/
    while(NULL != p)
    {
        printf("%s(), in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);  /********/
        if (p->perinfo.speak_status != 0 && (p->perinfo.client_sockfd > 0))
        {
            write(p->perinfo.client_sockfd, buf, sizeof(*buf));
            printf("%s() write to name = %s\n ", __PRETTY_FUNCTION__, p->perinfo.login_name);
        }
        p = p->next;
    }
}

/*处理群聊*/
void dealwith_chat_group(int fd, success_login *head, MESSAGE *buf)
{
//    MESSAGE buf;
//    memset(&buf, 0, sizeof(buf));
//    while (1)
//    {
//        int n = read(fd, &buf, sizeof(buf));
//        if (0 == n)
//        {
//            printf("用户未发送消息!\n");
//        }
//        else
//        {
//            group_write_message(head, &buf);
//        }
//    }
    group_write_message(head, buf);
    return ;
}

/*回收注册人员链表的系统资源*/
void destroy_Login_STNODE(Login_STNODE *head)
{
    if (NULL == head)
    {
        return;
    }
    Login_STNODE *p = head->next;
    Login_STNODE *q;
    while (NULL != p)
    {
        q = p->next; // 先记下p的下一个，释放当前的p，再把下一个赋值给p，循环，直到退出
        free(p);
        p = q;
    }
    free(head);
    printf("%s() 释放结束!! line in %d \n", __PRETTY_FUNCTION__, __LINE__);
    head = NULL;
}

/*回收回收在线人员链表的系统资源*/
void destroy_success_login(success_login *head)
{
    if (NULL == head)
    {
        return;
    }printf("1111\n");
    success_login *p = head->next;
    success_login *q;
    while (NULL != p)
    {
        q = p->next; // 先记下p的下一个，释放当前的p，再把下一个赋值给p，循环，直到退出
        free(p);
        p = q;printf("222\n");
    }
    free(head);
    printf("%s() 释放结束!! line in %d \n", __PRETTY_FUNCTION__, __LINE__);
    head = NULL;
}

/*用户退出聊天室时，将此用户从服务器的链表中删除*/
void user_exit_chatroom(int fd, success_login *head)
{
    success_login *cur_node = head->next;
    success_login *pre_node = head;
    while (NULL != cur_node)
    {
        if (fd == cur_node->perinfo.client_sockfd)
        {
            pre_node->next = cur_node->next;
            printf("remove fd = %d, name = %s from linklist and server\n", fd, cur_node->perinfo.login_name);
            free(cur_node);
            cur_node = NULL;
            return;
        }
        pre_node = cur_node;
        cur_node = cur_node->next;
    }
}











