#ifndef CONGFIGUREHOLDER_H
#define CONGFIGUREHOLDER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>

#include <QStringList>
#include <QVariant>

class ConfigureHolder : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureHolder(QObject *parent = 0);
    ~ConfigureHolder();
    QString toWindwosPath(QString path);
    void InitDoc();

signals:
    void copyFileProgress(int pro);
    void resetDefaultClicked();
    void resetCanceled();
    void showPreviewSignal();
    void previewCheckedFalse();
    void startPicAnimation();
    void isFullScreenChanges(bool isfull);
    void horizontalAudioAutoPlay();
    void horizontalFullscreenAudioAutoPlay();
    void verticalAudioAutoPlay();
    void verticalFullscreenAudioAutoPlay();
    void stopAllAudioPlay();
    void startPlayAfterCloseNormal();
    void startPlayAfterCloseFullScreen();
    void checkFullScreen();

public slots:
    bool createXml(QString dir);
    void clearParameters();    

    void setDelDirPath(const QString path);
    bool delDir(const QString path);
    void startDelDir();
    void orderFile(QString dir);
    bool copyReourceFile(QString src, QString dst);

    QString GetFilePath(QString pFilePath);

    bool resetScreen(bool isReset); //是否重置设备默认资源配置
    void sendCheckedSignal();
    void sendCanceledSignal();
    void sendShowPreview();
    void sendPreviewCheckedFalse();
    void sendStartPicAnimation();
    void sendIsFullScreenSignal(bool isflag);
    void sendCheckFullScreenSignal(bool isflag);
    void sendHorizontalAutoPlay(bool isfull);
    void sendVerticalAutoPlay(bool isfull);
    void sendStopAudioPlay();
    void sendPlayAfterClose(bool isfull);
    void setBrightOrVolumeChanged(int para);

    bool updateMultiMedia(bool isMediaContentEnabled,//true更新多媒体,false不更新多媒体
                          bool videoOrPicture, //true视频,false图片
                          bool isAudio,
                          bool isfullscreen,//是否全屏
                          QString paths);//true视频,false图片
    QVariant retPicturePath(QString paths);
    bool updateParameterBasic(bool bConfigureParameter,//是否配置了参数
                              bool bBrightnessVolume,//是否配置了亮度和亮度
                              bool bScrollText,//是否配置了滚动字幕
                              bool bTitle,//是否配置了标题
                              bool bDateTime,//是否配置了时间日期格式
                              bool bStandby, //是否配置了节能模式
                              bool isScrollText,
                              bool isTitle,
                              bool isDateTime,
                              bool audioSel);

    bool updateParameterBasic1(bool bConfigureParameter,
                               bool bBrightnessVolume,
                               bool bScrollText,
                               bool bTitle,
                               bool isScrollText,
                               bool isTitle,
                               int scrollFlag,
                               int titleFlag);
    bool updateArea1ParaAudio(bool audioOn, bool audioOff);

    bool updateParameterBasic2(bool bConfigureParameter,
                               bool bDateTime,
                               bool bStandby,
                               bool isDateTime,
                               bool audioSel,
                               bool audiooff,
                               int timeFlag,
                               int audioFlag);

    bool updateBrightnessVolume(qint32 vBrightness,//亮度
                              qint32 vVolume);//音亮

    int judgeString(QString str);
    bool updateScrollText(QString vScrollText);//滚动字幕

    bool updateTitle(QString vTitle);//标题

    bool updateDateTime(QString vTimeFormat,QString vDateFormat);//时间,日期格式

    bool updateStandby(quint32 vStage1Interval,//第一阶段进入时间
                        quint8 vStage1Brightness,//第一阶段亮度
                        quint32 vStage2Interval,//第二阶段进入时间
                       quint8 vStage2Brightness);//第二阶段亮度
    bool updatePictureInterval(qint32 vPictureInterval);
    int getPicInterval();
    void setSystimeText(QString ptext, bool sysTimeOn);
    void setCountryIndex(int pCountryIndex, int pYysIndex);
    void setApnFlag(bool pApnOn);
    void setApnParameter(QString papnYysText, QString papnJrdText,
                         QString papnMccText, QString papnMncText,
                         QString papnUNameText, QString papnPasswdText);
    void setIpParameter(bool pIsAutoSettingFlag, QString pIpAddressText, QString psubnetText,
                        QString pdefaultGateWay, QString pdnsService);
    void setArea2ParameterAllFlag(bool pHidetimeFlag, bool pHideApnFlag, bool pIpSettingAutoFlag);

public:
    ///是否重置设备默认资源配置
    bool isReset;

    ///多媒体配置选项区域
    bool isMediaContentEnabled; //true —— 更新多媒体，false —— 不更新多媒体
    bool isfullscreen;
    bool videoOrPicture; //true —— 视频，false —— 图片
    bool audioSelect;    // 音频

    QString videoPath;
    QStringList pictureList;
    QStringList pictureList2;

    ///参数配置选项区域
    bool bConfigureParameter;
    bool bBrightnessVolume;
    int mBrightOrVolumeChanged;
    bool bScrollText;
    bool bTitle;
    bool bDateTime;
    bool bStandby;
    bool audioSwither;
    bool audioSwitherOff;
    int audioHasSelect;
    int timeHasSelect;

    bool isScrollText;
    bool isTitle;
    bool isDateTime;
    int scrollHasSelect;
    int titleHasSelect;

    quint8 vVolume;
    quint8 vBrightness;

    QString vScrollText;

    QString vTitle;

    QString vTimeFormat;
    QString vDateFormat;

    quint32 vStage1Interval;
    quint8 vStage1Brightness;
    quint32 vStage2Interval;
    quint8 vStage2Brightness;    
    qint32 vPicInterval;

    QString usbPath;
    qreal pro;

    QString sysTimeText;
    bool msysTimeOn;
    int apnCountryIndex;
    int apnYysIndex;
    QString apnYysText;
    QString apnJrdText;
    QString apnMccText;
    QString apnMncText;
    QString apnUNameText;
    QString apnPasswdText;
    bool apnOn;
    bool isAutoSettingFlag;
    QString IpAddressText;
    QString subnetText;
    QString defaultGateWay;
    QString dnsService;
};

#endif // CONGFIGUREHOLDER_H
