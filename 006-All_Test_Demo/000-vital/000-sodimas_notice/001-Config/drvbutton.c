#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
static int fd;
static unsigned char PreFlag[24];
static unsigned char LightSta[16];

/*
        my_signal_func函数的处理事物及逻辑
        尽量越短越好。最好只作为数据传输。
        read完数据之后立即退出。主线程中再
        对read的结果做处理。
*/
void my_signal_func(int signum)
{
    //sodimas I/O输入总共有24个(0-23) 0无按下 1有按下
    //0-11:BC0-BC11
    //12-15:Elib1-Elib4
    //16-23:8位薄码开关
    unsigned char Value[24];
    unsigned char Cnt;
    unsigned char sta;

    read(fd, Value, 24);
    for(Cnt = 0; Cnt < 24; ++Cnt)
    {
        printf("%d ", Value[Cnt]);
        if(Value[Cnt]) // 有按键按下
        {
            PreFlag[Cnt] = 1; // 记录按下标志
        }
        else
        {
            if(PreFlag[Cnt]) // 松开按键
            {
                PreFlag[Cnt] = 0;// 清除按下标志
                /*
                可在此处将按钮信息发送给控制柜或是作为事件触发
                */
                /*test func: press on and again off*/
                if(Cnt < 16) // BC0-BC11 Elib1-Elib4按钮输入灯状态处理
                {
                    LightSta[Cnt] ^= 1;
                    if(LightSta[Cnt])
                    {
                        sta = 1;
                        lseek(fd, Cnt, SEEK_SET);
                        write(fd, &sta, 1);
                    }
                    else
                    {
                        sta = 0;
                        lseek(fd, Cnt, SEEK_SET);
                        write(fd, &sta, 1);
                    }
                }
                /*end of test*/
            }
        }
    }
    printf("\r\n");
}

int main(int argc, char **argv)
{
    int flag;
    unsigned int but, st;

    //在应用程序中捕捉SIGIO信号(这个信号是驱动程序发送出来的)
    memset(PreFlag, 0, 24);
    memset(LightSta, 0, 16);
    signal(SIGIO, my_signal_func);
    fd = open("/dev/canopen", O_RDWR);
    if(fd < 0)
    {
        printf("Can't open\n");
        return -1;
    }
    printf("Open file id is:%d\n", fd);
    //将当前进程PID设置为fd文件所对应驱动程序将要发送SIGIO信号进程PID
    fcntl(fd, F_SETOWN, getpid());
    //获取fd的打开方式
    flag = fcntl(fd, F_GETFL);
    //讲fd的打开方式设置为FASYNC, 即支持异步通知
    fcntl(fd, F_SETFL, flag|FASYNC);
    while(1)
    {
        printf("Please input button index and state:\n");
        scanf("%d %d", &but, &st);
        //sodimas I/O输出总共21个(0-20) 0灭灯 1亮灯
        //0-11:BC0-BC11
        //12-15:Elib1-Elib4
        //16-17: CAN_OK灯 CAN_FAIL灯
        //18-20: Back_Ext背光 Out_Fld、Out_Flm箭头
        if((but < 21)&&(st < 2))
        {
            lseek(fd, but, SEEK_SET);
            write(fd, &st, 1);
        }
        else
        {
            printf("arg error\n");
        }
    }
}
