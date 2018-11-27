#ifndef CONGFIGUREHOLDER_H
#define CONGFIGUREHOLDER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <windows.h>
#include "copyjob.h"

class ConfigureHolder : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureHolder(QObject *parent = 0);


    void InitDoc();
    void GetAppropriateJobNumber();

signals:
    void copyFileProgress(int pro);
public slots:
    bool createXml(QString dir);
    void clearParameters();    
    void startJobs();

    void orderFile(QString dir);
    bool copyReourceFile(QString src, QString dst);
    bool copyFileMulti(QString src, QString dst);

    QString GetFilePath(QString pFilePath);

    void copyProgressMult(int id, qint64 copyBytes);
    void jobFinished(int id);


    bool resetScreen(bool isReset); //是否重置设备默认资源配置

    bool updateMultiMedia(bool isMediaContentEnabled,//true更新多媒体,false不更新多媒体
                          bool videoOrPicture, //true视频,false图片
                          bool isfullscreen,//是否全屏
                          QString paths);//true视频,false图片

    bool updateParameterBasic(bool bConfigureParameter,//是否配置了参数
                              bool bBrightnessVolume,//是否配置了亮度和亮度
                              bool bScrollText,//是否配置了滚动字幕
                              bool bTitle,//是否配置了标题
                              bool bDateTime,//是否配置了时间日期格式
                              bool bStandby,
                              bool isScrollText,
                              bool isTitle,
                              bool isDateTime);//是否配置了节能模式

    bool updateBrightnessVolume(qint32 vBrightness,//亮度
                              qint32 vVolume);//音亮

    bool updateScrollText(QString vScrollText);//滚动字幕

    bool updateTitle(QString vTitle);//标题

    bool updateDateTime(QString vTimeFormat,QString vDateFormat);//时间,日期格式

    bool updateStandby(quint32 vStage1Interval,//第一阶段进入时间
                        quint8 vStage1Brightness,//第一阶段亮度
                        quint32 vStage2Interval,//第二阶段进入时间
                       quint8 vStage2Brightness);//第二阶段亮度

public:
    ///是否重置设备默认资源配置
    bool isReset;

    ///多媒体配置选项区域
    bool isMediaContentEnabled; //true —— 更新多媒体，false —— 不更新多媒体
    bool isfullscreen;
    bool videoOrPicture; //true —— 视频，false —— 图片
    QString videoPath;
    QStringList pictureList;

    ///参数配置选项区域
    bool bConfigureParameter;
    bool bBrightnessVolume;
    bool bScrollText;
    bool bTitle;
    bool bDateTime;
    bool bStandby;

    //bool bBrightnessVolume;
    bool isScrollText;
    bool isTitle;
    bool isDateTime;
    //bool bStandby;

    //bool isBrightnessVolume;
    quint8 vVolume;
    quint8 vBrightness;

    //bool isScrollText;
    QString vScrollText;

    //bool isTitle;
    QString vTitle;

    //bool isDateTime;
    QString vTimeFormat;
    QString vDateFormat;

    //bool isStandby;
    quint32 vStage1Interval;
    quint8 vStage1Brightness;
    quint32 vStage2Interval;
    quint8 vStage2Brightness;    

    QMap<int,CopyJob*> jobsMap;
    quint32 appropriateJobNumber;

    bool isMultiThreadCopying;
    qint64 fileTotalBytes;
    qint64 copyedBytes;
    qreal copyPercent;
    qint64 *perJobCopyedBytes;

    qint64 perJobLoad;
    qint64 lastJobLoad;


    QString toWindwosPath(QString path);
    QString usbPath;
};

#endif // CONGFIGUREHOLDER_H
