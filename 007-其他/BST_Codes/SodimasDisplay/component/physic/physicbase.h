#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QObject>
#include <QDomElement>
#include <QPointer>
//#include "themeparser.h"
//#include "../logic/themelg.h"
#include "../globalsetting.h"

class LcdPh;
class GlobalSetting;
class PhysicBase : public QObject
{
    Q_OBJECT
public:
    PhysicBase(GlobalSetting *globalSetting);

    ~PhysicBase();
    virtual bool initDevice(PH_Type pPHType, QDomElement pElement);///初始化第一步
    virtual bool SetEnable(bool pFlag, DEV_MODE pDevMode, bool pForce=false);    ///初始化第三步

    bool setElementText(QDomElement pElement, QString pValue);
    bool setPara(QString pParaName, QString pParaValue, bool pSyncUI=true);
    bool setPara(QDomElement pElement, QString pParaValue, bool pSyncUI=true);
    bool setPara(QDomElement &pElement, XmlParse *pParse);
    QString getPara(QString pParaName);
    QString getPara(QDomElement pElement);
    bool getPara(QDomElement &pElement, XmlParse *pParse);

    //获取本节点中的重要信息
    virtual QString GetInformation() { return QString(); }
    virtual bool SetValue(QString pValue) {return false;}
    virtual QString GetValue(QString pValue) { return QString(); }

    bool saveMdp();

public slots:
    ///初始化第二步
    virtual void slot_Initialize();   //当物理、逻辑设备、主题全部加载好后执行此函数 初始化完毕函数
signals:
//    void sOperateTheme(AREA_OPERATE pOperate, QVariant pPara);
//    void sInfo(QString pMsg, int pDelay=3000, DEV_MODE pMode=DM_NORMAL);
//    void sWarn(QString pContent, int pDelay=3000, DEV_MODE pMode=DM_NORMAL);
//    void sError(QString pContent, int pDelay=3000, DEV_MODE pMode=DM_NORMAL);
//    void sProgress(quint32 pValue);
//    void sProgress(quint32 pPrg, quint32 pExtraPrg);
//    void sProgress(QString pMsg, quint32 pValue);
//    void sProgress(QString pMsg, quint32 pValue, quint32 pExtraPrg);

//    void sBeep(QString);      //发送播报beep信号
//    void sIcon(QString);      //发送显示图标信号
//    void sShowIcon(QString);  //发送显示图标信号
//    void sHideIcon(QString);  //发送隐藏图标信号
//    void sFeedLCD();          //亮屏操作
//    void sSetDevices(bool pEnable, DEV_MODE pDevMode);

public:
    bool modifyBool(QDomElement &pElement, bool &pDstValue, bool pNewValue);
    bool modifyEnum(QDomElement &pElement, QString &pDstValue, QString pNewValue);
    bool modifyNum(QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue=-1, int pMaxValue=-1);
    bool modifyText(QDomElement &pElement, QString &pDstValue, QString pNewValue);

public:
    bool m_initialized;
    bool m_enabled;

    GlobalSetting      *m_globalSetting;
    ThemeLg            *mThemeManager;
    LcdPh              *m_LCD;
    QDomElement         m_NodeElement;

    QString             mDevName;
    DEV_MODE            mDevMode;
};


/*
    virtual bool isSequential() const { return true; }
    virtual bool open(OpenMode mode){ return true; }
    virtual void close(){}

    virtual qint64 pos() const  {return 0;}
    virtual qint64 size() const  {return 0;}
    virtual bool seek(qint64 pos)  {return true;}
    virtual bool atEnd() const  {return true;}
    virtual bool reset()  {return true;}

    virtual qint64 bytesAvailable() const  {return 0;}
    virtual qint64 bytesToWrite() const  {return 0;}

    virtual bool canReadLine() const  {return true;}

    virtual qint64 write(QByteArray data, QString pAddr) {return 0;} //向某一地址的设备发送数据
    virtual qint64 reply(QByteArray data) {return QIODevice::write(data);}
    virtual inline qint64 write(const QByteArray &data) {return (QIODevice::write(data));}
    virtual QByteArray readAll(){return (QIODevice::readAll());}

    virtual bool waitForReadyRead(int msecs)  {return true;}
    virtual bool waitForBytesWritten(int msecs)  {return true;}

    virtual qint64 readData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 readLineData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 writeData(const char *data, qint64 len)  {return 0;}
    virtual bool isOpen()const{return (QIODevice::isOpen());}

    virtual bool copy(QString pSrc, QString pDst)  {return copyFile(pSrc, pDst);}
    virtual bool cut(QString pSrc, QString pDst)  {return cutFile(pSrc, pDst);}
    virtual bool del(QString pPath)  {return delFile(pPath);}

    virtual bool mkdir(QString pPath)  {return creatPath(pPath);}
    virtual bool copydir(QString pSrc, QString pDst, bool subfolder) {return copyFolder(pSrc, pDst, subfolder);}
    virtual bool movedir(QString pSrc, QString pDst)  {return moveFolder(pSrc, pDst);}
    virtual bool deldir(QString pDir)  {return delFolder(pDir);}
    virtual bool cleardir(QString pDir)  {return clearFolder(pDir);}
*/
#endif // DEVICEBASE_H
