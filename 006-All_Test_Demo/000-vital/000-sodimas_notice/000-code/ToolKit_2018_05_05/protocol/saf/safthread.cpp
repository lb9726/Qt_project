#include "safthread.h"
#include "define.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>

int SafThread::s_fd = 0;
quint8 SafThread::s_saf = 0;
quint8 SafThread::s_flm = 0;
quint8 SafThread::s_fld = 0;
quint8 SafThread::s_sgo = 0;
quint8 SafThread::s_talk = 0;
quint8 SafThread::s_sf = 0;

quint8 SafThread::s_dip1 = 0;
quint8 SafThread::s_dip2 = 0;
quint8 SafThread::s_dip3 = 0;
quint8 SafThread::s_dip4 = 0;
quint8 SafThread::s_switchValue = 0;

bool SafThread::s_gpioChangeFlag = false;
bool SafThread::s_parseFrameFlag = false;
bool SafThread::s_firstRun = true;

SafThread::SafThread(QObject *parent)
    :QThread(parent)
{
    m_safLast = 0;
    m_saftalk = 255;
    m_sgoLast = 0;
    m_sgoUpedge = 0;
    m_talkLast = 0;
    m_talkUpedge = 0;
    m_openingMessage = true;
    m_scroll = false;
    loadDriverFile();
//    start(QThread::HighestPriority);
}

void SafThread::run()
{
   while(1)
   {
       if(s_gpioChangeFlag)
       {
           parseFrame();
       }
       msleep(10);
   }
}

void SafThread::parseFrame()
{
    s_gpioChangeFlag = false;
    //sgo上升沿
    if(s_sgo == 1)
    {
        if(m_sgoLast == 0)
        {
            m_sgoUpedge = 1;
        }
        else
        {
            m_sgoUpedge = 0;
        }
        m_sgoLast = 1;
    }
    else
    {
        m_sgoUpedge = 0;
        m_sgoLast = 0;
    }
    //talk上升沿
    if(s_talk == 1)
    {
        if(m_talkLast == 0)
        {
            m_talkUpedge = 1;
        }
        else
        {
            m_talkUpedge = 0;
        }
        m_talkLast = 1;
    }
    else
    {
        m_talkUpedge = 0;
        m_talkLast = 0;
    }


  //qDebug() << "Start.....................";
    qDebug() << "   s_saf:" << s_saf << "  s_flm:" << s_flm << "   s_fld:" << s_fld << "   s_sgo:" << m_sgoUpedge
             << "   s_talk:" << s_talk << " s_sf:" << s_sf << "         s_dip1:" << s_dip1 << " s_dip2:" << s_dip2
             << "   s_dip3:" << s_dip3 <<"  s_dip4:" << s_dip4;

//    if(s_dip1 == 1)
//        ;//emit .....  发送大小屏切换

//    if(s_dip2 == 1)
//        ;//emit .....  进入待机模式

//    if(s_dip3 == 1)
//        ;//emit .....  进入自动测试模式

//    if(s_dip4 == 1)
//        ;//emit .....  声音开关
    if(s_saf==60)
     {
         emit sFrame60Loop(43,45);
     }
    else
    {
        emit sFrame60Loop(0,0);
    }
    if(s_firstRun)
    {
         emit switchValue(s_dip1, s_dip2, s_dip3, s_dip4);
         s_firstRun = false;
    }

    //箭头和到站钟
    if(s_sf)
    {
        emit sigArrowAction(0, 0, false);
        qDebug() << "s_flm:"  <<  s_flm  << "s_fld:" << s_fld << "scroll: 0";
        if(m_sgoUpedge)
        {
            if(m_openingMessage)
            {
                qDebug() << "emit sMessageAudio(44)";
                emit sigMainVolumeChange(44, -1);
            }
        }
    }
    else
    {
        if(s_saf == 52)
        {
            m_scroll = true;
        }
        else if(s_saf == 53)
        {
            m_scroll = false;
        }
        emit sigArrowAction((bool)s_flm, (bool)s_fld, m_scroll);
        qDebug() << "s_flm:"  <<  s_flm  << "s_fld:" << s_fld << "scroll:" << m_scroll;
        if(m_sgoUpedge)
        {
            if(m_openingMessage)
            {
                qDebug() << "emit sMessageAudio(44)";
                emit sigMainVolumeChange(44, -1);
            }
            if(s_flm == 1 && s_fld == 0)
            {
                emit sigMainVolumeChange(40, -1);
                qDebug() << "emit sMessageAudio(40)";
            }
            else if(s_flm == 0 && s_fld == 1)
            {
                qDebug() << "emit sMessageAudio(41)";
                emit sigMainVolumeChange(41, -1);
            }
            else if(s_flm == 1 && s_fld == 1)
            {
                qDebug() << "emit sMessageAudio(42)";
                emit sigMainVolumeChange(42, -1);
            }
        }
    }
    //楼层和楼层报站

    if(s_saf <= 39)
    {
        if(s_saf <= 19)
        {
            m_safLast  = s_saf;
        }
        else if(s_saf >= 21 && s_saf <= 29)
        {
            m_safLast = 0 - (s_saf - 20);
        }
        else if(s_saf >= 30)
        {
            m_safLast = s_saf - 10;
        }

        if(m_talkUpedge && m_saftalk != m_safLast)
        {
            m_saftalk = m_safLast;
            qDebug() << QString("emit sMessageAudio(%1)").arg(m_safLast);
            if(m_safLast >= 0)
            {
                emit sigFloorArrive(m_safLast%10, m_safLast/10%10, m_safLast/100, m_safLast,s_saf);
            }
            else
            {
                emit sigFloorArrive((-m_safLast)%10, -3, 0, m_safLast+40,s_saf);
            }
        }
        else
        {
            if(m_safLast >= 0)
            {
                emit sigFloorArrive(m_safLast%10, m_safLast/10%10, m_safLast/100, -1,s_saf);
            }
            else
            {
                emit sigFloorArrive((-m_safLast)%10, -3, 0, -1,s_saf);
            }
        }
    }

//    40:检修 Inspection
//    41:禁入 Access forbidden
//    46:消防 fire mode
//    47:Liftier
//    48:cell CLEEULE
//    51:logo
//    以上特殊功能只有图标，没有音频，因此插入后，重复发楼层帧不会重复播报

    else if(s_saf == 40)   //maintenance
    {
        emit sigShowFuncLogo(40, -1);
    }
    else if(s_saf == 41)   //out of service
    {
        emit sigShowFuncLogo(41, -1);
    }
    else if(s_saf == 44)   //overload
    {
        emit sigMainVolumeChange(-1, 43);            //bipbipbip
//        if(m_safLast >= 0)
//            emit sigFloorArrive(m_safLast%10, m_safLast/10%10, m_safLast/100, m_safLast);
//        else
//            emit sigFloorArrive((-m_safLast)%10, -3, 0, m_safLast+40);
        emit sigShowFuncLogo(42, -1);
        qDebug() << "bipbipbip";
        qDebug() << "The floor broadcast repeated";
    }
    else if(s_saf == 45)  //fullload
    {
        //emit sigMainVolumeChange(48, -1);  //客户已经将此音频设为静音，但插入此帧后楼层播报会重复一次

//        if(m_safLast >= 0)
//            emit sigFloorArrive(m_safLast%10, m_safLast/10%10, m_safLast/100, m_safLast);
//        else
//            emit sigFloorArrive((-m_safLast)%10, -3, 0, m_safLast+40);

        emit sigShowFuncLogo(43, -1);
        qDebug() << "The floor broadcast repeated";
    }
    else if(s_saf == 46)   //fireman
    {
        emit sigShowFuncLogo(44, -1);
    }
    else if(s_saf == 47)   //lift attendant
    {
        emit sigShowFuncLogo(45, -1);
    }
    else if(s_saf == 48)   //photo cell
    {
        emit sigShowFuncLogo(46, -1);
    }
    else if(s_saf == 56)     //door close
    {
        emit sigMainVolumeChange(-1, 43);      //bipbipbip
//        if(m_safLast >= 0)
//            emit sigFloorArrive(m_safLast%10, m_safLast/10%10, m_safLast/100, m_safLast);
//        else
//            emit sigFloorArrive((-m_safLast)%10, -3, 0, m_safLast+40);
        emit sigShowFuncLogo(47, -1);
        qDebug() << "bipbipbip";
        qDebug() << "The floor broadcast repeated";
    }
    else if(s_saf == 57)   //emergency mode
    {
        emit sigShowFuncLogo(48, -1);
    }
    else if(s_saf == 58)   //password
    {
        emit sigShowFuncLogo(49, -1);
    }
    else if(s_saf == 59)      //door close  message
    {
                       //显示器接收到Frame 59 时，显示上一个楼层帧对应的内容（楼层帧的帧号为1-39, 70）
        if(m_talkUpedge)
        {
            emit sigMainVolumeChange(45, -1);
            qDebug() << QString("emit sMessageAudio(45)");
        }
    }
//    60帧：只要发60帧即可显示图标以及播放音频，不需要SGO和TALK信号，插入此两帧后楼层播报会重复一次。
//    if(mSAFLine->mSafFrameNum == 60)
//    {
//        emit sMessageAudio(43);
//        emit sMessageAudio(mOldFrameNum);
//    }
 //     qDebug()    <<   "End.....................";
//      qDebug();
    //s_parseFrameFlag = false;
}

void SafThread::loadDriverFile()
{
    int flag;

    //在应用程序中捕捉SIGIO信号(这个信号是驱动程序发送出来的)
    signal(SIGIO, SafThread::my_signal_func);
    system("insmod /usr/bst/usrfs/gpio-sodimas-saf.ko");
    s_fd = open("/dev/saf", O_RDWR);
    if(s_fd < 0)
    {
        printf("Can't open\n");
        return;
    }
    printf("Open file id is:%d\n", s_fd);
    //将当前进程PID设置为fd文件所对应驱动程序将要发送SIGIO信号进程PID
    fcntl(s_fd, F_SETOWN, getpid());
    //获取fd的打开方式
    flag = fcntl(s_fd, F_GETFL);
    //讲fd的打开方式设置为FASYNC, 即支持异步通知
    fcntl(s_fd, F_SETFL, flag|FASYNC);
}

void SafThread::my_signal_func(int signum)
{
    //while(s_parseFrameFlag);
    //s_parseFrameFlag = true;

    quint8 Value[10];
//    unsigned char Cnt;

//    char *str[] = {"ArrowEnable:", "GongSta:", "ArrowUp:", "TalkSta:", "Floor:",
//                   "ArrowDn:", "DIP_4:", "DIP_3:", "DIP_2:", "DIP_1:"};

    read(s_fd, Value, 10);
//    for(Cnt = 0; Cnt < 10; Cnt++){
//        printf("%s%d ", str[Cnt], Value[Cnt]);
//    }
//    printf("/n");

//    qDebug() << "   s_saf:" << s_saf << "  s_flm:" << s_flm << "   s_fld:" << s_fld << "   s_sgo:" << s_sgo
//             << "   s_talk:" << s_talk << " s_sf:" << s_sf << "         s_dip1:" << s_dip1 << " s_dip2:" << s_dip2
//             << "   s_dip3:" << s_dip3 <<"  s_dip4:" << s_dip4;

//    qDebug() << "   s_saf:" << Value[4] << "  s_flm:" << Value[2] << "   s_fld:" << Value[5]
//             << "   s_sgo:" << Value[1] << "   s_talk:" << Value[3] << " s_sf:" << Value[0]
//            << Value[9] << Value[8] << Value[7] << Value[6];

//    if(s_firstRun)
//    {
//        if(s_dip1 == 1)
//            s_switchValue |= 1 << 0;
//        if(s_dip2 == 1)
//            s_switchValue |= 1 << 1;
//        if(s_dip3 == 1)
//            s_switchValue |= 1 << 2;
//        if(s_dip4 == 1)
//            s_switchValue |= 1 << 3;
//        qDebug() << "hello" << s_switchValue;
//    }
//    else
//    {
//        quint8 switchValue = 0;
//        if(s_dip1 == 1)
//            switchValue |= 1 << 0;
//        if(s_dip2 == 1)
//            switchValue |= 1 << 1;
//        if(s_dip3 == 1)
//            switchValue |= 1 << 2;
//        if(s_dip4 == 1)
//            switchValue |= 1 << 3;
//        qDebug() << "hello" << s_switchValue << switchValue;
//        if(switchValue != s_switchValue)
//        {
//            s_switchValue = switchValue;
//            return;
//        }
//    }

    //saf
    s_saf = Value[4];
    if(s_saf == 70)
    {
        s_saf = 0;
    }

    if(Value[2] == 0)
    {
        s_flm = 1;
    }
    else
    {
        s_flm = 0;
    }

    if(Value[5] == 0)
    {
        s_fld = 1;
    }
    else
    {
        s_fld = 0;
    }

    if(Value[1] == 0)
    {
        s_sgo = 1;
    }
    else
    {
        s_sgo = 0;
    }

    if(Value[3] == 0)
    {
        s_talk = 1;
    }
    else
    {
        s_talk = 0;
    }

    s_sf = Value[0];

    //拨码开关
    if(Value[9] == 0)
    {
        s_dip1 = 1;
    }
    else
    {
        s_dip1 = 0;
    }

    if(Value[8] == 0)
    {
        s_dip2 = 1;
    }
    else
    {
        s_dip2 = 0;
    }

    if(Value[7] == 0)
    {
        s_dip3 = 1;
    }
    else
    {
        s_dip3 = 0;
    }

    if(Value[6] == 0)
    {
        s_dip4 = 1;
    }
    else
    {
        s_dip4 = 0;
    }

    if(s_saf > 70 || (s_flm != 1 && s_flm != 0) || (s_fld != 1 && s_fld != 0) || (s_sgo != 1 && s_sgo != 0)
        || (s_talk != 1 && s_talk != 0) || (s_dip1 != 1 && s_dip1 != 0) || (s_dip2 != 1 && s_dip2 != 0)
        || (s_dip3 != 1 && s_dip3 != 0) || (s_dip4 != 1 && s_dip4 != 0) )
    return;
//    qDebug() << "   s_saf:" << s_saf << "  s_flm:" << s_flm << "   s_fld:" << s_fld << "   s_sgo:" << s_sgo
//             << "   s_talk:" << s_talk << " s_sf:" << s_sf << "         s_dip1:" << s_dip1 << " s_dip2:" << s_dip2
//             << "   s_dip3:" << s_dip3 <<"  s_dip4:" << s_dip4;
    s_gpioChangeFlag = true;
}
