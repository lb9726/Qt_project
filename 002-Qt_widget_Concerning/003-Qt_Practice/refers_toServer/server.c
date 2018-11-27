
#include"server.h"
int main(int argc, char **argv)
{
    struct sockaddr_in cliaddr, servaddr;
    struct message meag;

    clientinf clientNode;
    fd_set rset;
    socklen_t clilen;
    time_t timep;

    char str[MAXLINE];
    int listenfd, connfd;
    int maxfd;
    int ret;
    int clientfd[64] = {0};
    int i = 0;

    //  strcpy(clientNode.name,"*****");
    clientNode.speak=1;
    fd=open("chat.txt",O_RDWR|O_CREAT|O_APPEND,0777);
    if(fd<0)
    {
        perror("open fail:");
        exit(-1);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(clientfd,0xff,sizeof(clientfd));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(servaddr.sin_addr));
    servaddr.sin_port = htons(5678);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    maxfd = listenfd;
    for ( ; ; )
    {
        FD_ZERO(&rset);
        FD_SET(listenfd,&rset);
        setAllFd(clientfd,64,&rset);
        maxfd = getMaxFd(clientfd,64);
        if(maxfd < listenfd)
        {
            maxfd = listenfd;
        }
        ret = select(maxfd+1,&rset,NULL,NULL,NULL);
        if(ret <= 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if(FD_ISSET(listenfd,&rset))
        {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
            addFd(clientfd,64,connfd);
        }
        for(i = 0; i < 64;i++)
        {
            if(clientfd[i] > 0)
            {
                if(FD_ISSET(clientfd[i],&rset))
                {
                    int len;

                    len = read(clientfd[i],&meag,sizeof(meag));
                    clientNode.decr=clientfd[i];
                    strcpy(clientNode.name,meag.name);
                    if(len == 0)
                    {
                        uexit(&meag,&clientNode);
                        fflush(stdout);
                        close(clientfd[i]);
                        FD_CLR(clientfd[i],&rset);
                        clientfd[i] = -1;
                        continue;
                    }

                    write(1,meag.flag,strlen(meag.flag));//打印标识符
                    printf("\n");

                    if(strcmp(meag.flag,"login")==0)
                    {
                        //printf("%s  %s\n",meag.name,meag.msg);

                        if(overlap(meag)==0)
                        {
                            login(&meag,&clientNode);
                        }
                        else
                        {
                            strcpy(meag.msg,"overlap");
                            write(clientNode.decr,&meag,sizeof(struct message));
                        }
                        continue;
                    }
                    else if(strcmp(meag.flag,"reg")==0)
                    {
                        reg(&meag,&clientNode);
                        continue;
                    }
                    else if(strcmp(meag.flag,"all")==0)
                    {
                        if(all(&meag,&clientNode)==1)
                        {
                            bzero(str,strlen(str));
                            time(&timep);
                            sprintf(str,"%s%s TO %s: %s\n",ctime(&timep),meag.name,meag.addressee,meag.msg);
                            printf("%s",str);
                            write(fd,str,strlen(str));
                        }

                    }
                    else if(strcmp(meag.flag,"personal")==0)
                    {
                        if(personal(&meag,&clientNode)==1)
                        {
                            bzero(str,strlen(str));
                            time(&timep);
                            sprintf(str,"%s%s TO %s: %s\n",ctime(&timep),meag.name,meag.addressee,meag.msg);
                            printf("%s",str);
                        }   write(fd,str,strlen(str));

                    }
                    else if(strcmp(meag.flag,"view")==0)
                    {
                        view(&meag,&clientNode);
                        continue;
                    }

                }
            }
        }
    }
    close(listenfd);
    return 0;
}









