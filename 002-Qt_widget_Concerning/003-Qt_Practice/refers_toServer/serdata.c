#include"serdata.h"
int getMaxFd(int * pint,int size)
{
    int max = pint[0];
    int i = 0;
    for(i=1;i < size;i++)
    {
        if(max < pint[i])
        {
            max = pint[i];
        }
    }
    return max;
}

void addFd(int * pint,int size,int fd)
{
    int i = 0;
    for(i = 0; i < size;i++)
    {
        if(-1 == pint[i])
        {
            pint[i] = fd;
            break;
        }
    }
}
void setAllFd(int * pint,int size,fd_set * fdset)
{
    int i = 0;
    for(i = 0; i < size;i++)
    {
        if(-1 != pint[i])
        {
            FD_SET(pint[i],fdset);
        }
    }

}


int overlap(struct message meag)
{

    LNODE *p=LHead.next;
    while(p != NULL)
    {

        if(strcmp(p->data.name,meag.name)==0)
        {

                return 1;
        }
        p=p->next;
    }
    return 0 ;
}

int login(struct message *meag,clientinf *clientNode)
{
    int i;
    char buf[MAXLINE];
    i=login_check(meag);
    if(i==1)
    {
        printf("%s Login success!\n",(*meag).name);
        memset((*meag).msg,0,strlen((*meag).msg));
        strcpy((*meag).msg,"login,success!");
        strcpy((*clientNode).name,(*meag).name);
        insertend(clientNode);
        write((*clientNode).decr,meag,sizeof(struct message));

        LNODE *L=LHead.next;
        strcpy((*meag).flag,"sermsg");
        sprintf((*meag).msg,"欢迎 %s 来到聊天室!",(*meag).name);
        while(L!=NULL)
        {
            write(L->data.decr,meag,sizeof(struct message));
            L=L->next;
        }
    }
    else
    {
        printf("%s Login failed!\n",(*meag).name);
        strcpy((*meag).msg,"login failed!");
        write((*clientNode).decr,meag,sizeof(struct message));
    }
}


int login_check(struct message *meag)
{
    DestroyLinklist();
    Readfiletolinklist();
  //  CoverBookNode();
    List *L=Head.next;
    while(L!=NULL)
    {

       if(strcmp(L->user.name,(*meag).name)==0)
        {
            if(strcmp(L->user.passwd,(*meag).msg)==0)
            {
                return 1;

            }
        }

        L=L->next;
    }
    return;
}

void CoverBookNode()
{
     List *p=Head.next;
     printf("注册了的用户如下\n");
     while(p!=NULL)
     {
         printf("%-5s\n",p->user.name);
         p=p->next;
     }
     printf("\n");
     return;
}

int insertend(clientinf *clientNode)
{
    LNODE *pnode=malloc(sizeof(LNODE));
    pnode->data=(*clientNode);
    pnode->next=NULL;

    pnode->next=LHead.next;
    LHead.next=pnode;

    return;
}

//用户注册
int reg(struct message *meag,clientinf *clientNode)
{
    int i;
    char buf[MAXLINE];
    i=reg_check(meag);
    if(i==1)
    {

        printf("%s 注册成功!\n",(*meag).name);
        strcpy((*meag).msg,"注册成功!");
        strcpy((*clientNode).name,(*meag).name);
        write((*clientNode).decr,meag,sizeof(struct message));
    }
    else
    {
        printf("%s  注册失败!\n",(*meag).name);
        strcpy((*meag).msg,"注册失败!");
        write((*clientNode).decr,meag,sizeof(struct message));
    }
}

//用户注册检查
int reg_check(struct message *meag)
{
    if(strlen(meag->name)>20 || strlen(meag->msg)>20)
        return 0;
    Readfiletolinklist();
    List *p=Head.next;
    while(p!=NULL)
    {
        if(strcmp(p->user.name,meag->name)==0)
        {
                return 0;
        }
        p=p->next;
    }
    User user;
    strcpy(user.name,meag->name);
    strcpy(user.passwd,meag->msg);
    insert(user);
    Readlinklisttofile();
    DestroyLinklist();
    return 1;
}

int insert(User user)
{
    List *pnode=malloc(sizeof(LNODE));
    pnode->user=user;
    pnode->next=NULL;

    pnode->next=Head.next;
    Head.next=pnode;

    return;
}

void Readfiletolinklist()
{
    FILE *fp;
    User user;
    List *p=NULL;
    fp=fopen("personinf.txt","rb");
    if(fp==NULL)
    {
        printf("读取文件出错！！\n");
        exit(0);
    }
    else
    {

        while((fread(&user,sizeof(struct _User),1,fp))==1)
        {

            List *p=malloc(sizeof(struct _List));
            if(Head.next==NULL)
            {
                Head.next=p;
                p->next=NULL;
                p->user=user;
            }
            else
            {
                List *tmp=&Head;

                while(tmp->next!=NULL)
                    tmp=tmp->next;
                tmp->next=p;
                p->next=NULL;
                p->user=user;
            }

        }
        free(p);
    }

    fclose(fp);
    fp=NULL;
    return ;

}

void Readlinklisttofile()
{
    FILE *fp;
    List head=Head;
    fp=fopen("personinf.txt","wb");
    while(head.next!=NULL)
    {
        fwrite(&head.next->user,sizeof(struct _User),1,fp);
        head.next=head.next->next;
    }
    fclose(fp);
    fp=NULL;
    return ;
}

void DestroyLinklist()
{
    List *p = Head.next;
    while(p!=NULL)
    {
        Head.next = p->next;
        free(p);
        p = Head.next;
    }
    return;
}


int all(struct message *meag,clientinf *clientNode)
{
    LNODE *p=LHead.next;
    while(p!=NULL)
    {
        if(strcmp(p->data.name,(*meag).name)==0)
        {
            if(p->data.speak==0)
            {
                strcpy((*meag).flag,"sermsg");
                sprintf((*meag).msg,"sorry,you speak limited by manager!\n");
                write(p->data.decr,meag,sizeof(struct message));
                return 0;
            }

            else if(strcmp((*meag).msg,"")!=0)
            {
                LNODE *L=LHead.next;
                strcpy((*meag).name,(*clientNode).name);
                while(L!=NULL)
                {
                    write(L->data.decr,meag,sizeof(struct message));
                    L=L->next;
                }
                return 1;
            }
        }
        p=p->next;
    }
}

int personal(struct message *meag,clientinf *clientNode)
{
     LNODE *p=LHead.next;
     while(p!=NULL)
     {
         if(strcmp(p->data.name,(*meag).name)==0)
         {
             if(p->data.speak==0)
             {
                 strcpy((*meag).flag,"sermsg");
                 sprintf((*meag).msg,"sorry,you speak limited by manager!\n");
                 write(p->data.decr,meag,sizeof(struct message));
                 return 0;
             }
         }
         p=p->next;
     }
     clientinf data;
     strcpy(data.name,(*meag).addressee);
     LNODE *L=LHead.next;
     strcpy((*meag).name,(*clientNode).name);
     while(L!=NULL)
     {
         if(strcmp(data.name,L->data.name)==0)
         {
             if(strcmp((*meag).msg,"")!=0)
             {
                 write(L->data.decr,meag,sizeof(struct message));
                 return 1;
             }
         }
         L=L->next;

     }
     strcpy((*meag).flag,"sermsg");
     //sprintf((*meag).msg,"the user not online\n");
     write((*clientNode).decr,meag,sizeof(struct message));
     return 0;
}

//查看在线人数
int view(struct message *meag,clientinf *clientNode)
{
    char str[MAXLINE],buf[MAXLINE];
    int i=1;
    LNODE *L=LHead.next;
    bzero(buf,strlen(buf));
    while(L!=NULL)
    {
        bzero(str,strlen(str));
        if(L->data.speak==1)
        {
            sprintf(str,"%d. %s  online\n",i,L->data.name);
            strcat(buf,str);
        }
        else
        {
              sprintf(str,"%d. %s  speak limited\n",i,L->data.name);
              strcat(buf,str);
        }
        L=L->next;
        i++;
    }
    strcpy((*meag).name,(*clientNode).name);
    strcpy((*meag).msg,buf);
    write((*clientNode).decr,meag,sizeof(struct message));
}

//用户离开聊天室函数
int uexit(struct message *meag,clientinf *clientNode)
{
    LNODE *L=&LHead;
    while(L->next!=NULL)
    {
        if(L->next->data.decr==(*clientNode).decr)
        {
            strcpy((*clientNode).name,L->next->data.name);
            LNODE *q=L->next;
            L->next=L->next->next;
            free(q);
            q=NULL;
            break;
        }
        L=L->next;
    }
    strcpy((*meag).flag,"sermsg");
    printf("%s 离开聊天室!\n",(*clientNode).name);
    sprintf((*meag).msg,"%s leave the chatroom\n",(*clientNode).name);
    LNODE *p=LHead.next;
    while(p!=NULL)
    {
        write(p->data.decr,meag,sizeof(struct message));
        p=p->next;
    }
}


