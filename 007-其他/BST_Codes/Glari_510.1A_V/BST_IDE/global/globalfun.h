#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include "define.h"

#include "mdpparser.h"
#include "uiparser.h"

//>@---------------基础函数-------------------
inline bool StrToInt(int& pNum, int pBase, QString pKey)
{
    bool flag = false;
    pNum = pKey.toInt(&flag, pBase);
    if(flag)
        return true;
    return false;
}

class NUM_INFO
{
public:
    NUM_INFO()
    {
        mChanged = false;
        mNum = 0;
        mMinNum = 0;
        mMaxNum = 0;
    }
    bool setnum(int pNum)
    {
        if(pNum == mNum)
            return true;
        mChanged = true;
        if(pNum < mMinNum)
        {
            mNum = mMinNum;
            return false;
        }
        else if(pNum > mMaxNum)
        {
            mNum = mMaxNum;
            return false;
        }
        else
        {
            mNum = pNum;
            return true;
        }
        return false;
    }
    void setnum(int pNum, int pMinNum, int pMaxNum)
    {
        mMinNum = pMinNum;
        mMaxNum = pMaxNum;
        if(pNum < mMinNum)
            mNum = mMinNum;
        else if(pNum > mMaxNum)
            mNum = mMaxNum;
        else
            mNum = pNum;
    }
public:
    int mNum;
    int mMinNum;
    int mMaxNum;
    bool mChanged;
};


inline QString GetPcName()
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
                return stringList.at(1);
        }
    }
    return QString();
}

inline QString GetAppVersion()
{
    QString tmpVersion;
    QFile tmpFile(":/Version/rc/Version/version.txt");
    if(tmpFile.open(QIODevice::ReadOnly))
    {
        tmpVersion = tmpFile.readLine(1024);
    }
    tmpFile.close();
    return tmpVersion;
}

inline QString GetMachineName()
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
                return stringList.at(1);
        }
    }
    return QString();
}

inline QString GetLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
            return address.toString();
    }
    return 0;
}

inline QStringList GetLocalIPListFromNet(QString pNetName)
{
    QStringList tmpIPList;
    if(pNetName.isEmpty())
        return tmpIPList;
    QNetworkInterface tmpInterface = QNetworkInterface::interfaceFromName(pNetName);
    if(tmpInterface.isValid() == false)
        return tmpIPList;
    QList<QHostAddress> list = tmpInterface.allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
       {
           tmpIPList.append(address.toString());
       }
    }
    return tmpIPList;
}

inline QStringList GetLocalIPList()  //获取ip地址
{
    QStringList tmpIPList;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
       {
           tmpIPList.append(address.toString());
       }
    }
    return tmpIPList;
}

inline QString getUserName()  //获取用户名
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
            }
        }
    }
    return false;
}

inline bool ConvertImageTo565(QImage &pImg)
{
    if(pImg.isNull())
        return false;
    int width=pImg.width();
    int height=pImg.height();
    int r,g,b;
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            QPoint tmpPoint = QPoint(j, i);
            QRgb tmpRgb = pImg.pixel(tmpPoint);
            r = qRed(tmpRgb) >> 2;
            g = qGreen(tmpRgb) >> 2;
            b = qBlue(tmpRgb) >> 2;
            pImg.setPixel(tmpPoint, ((tmpRgb & 0xff000000) | (r<<16) | (g<<8) | (b)));
        }
    }
    return true;
}

QString getUiString(UI_TYPE pType);

qreal RegularAngle(qreal angle);
qreal CalDiffBeginAngle(qreal angle);

//>@从磅换算到像素
/*   Point实际是磅，也就是 1/72 inch
     DPI: Dot per inch，在显示器上也就是每英寸包含的像素。英尺的换算为 1 cm = 0.39 inch, 1 inch = 2.54 cm。
     DPI 可以通过如下计算得来：
            水平 dpi = 水平 resolution * 2.54 / 显示器宽度
            垂直 dpi = 垂直 resolution * 2.54 / 显示器高度
     pixel pitch（点距）: 像素点间距离。可以用25.4mm / DPI得到。
     字体在屏幕上的实际大小 = 字体像素大小 * 点距
*/
typedef enum{
    PRIOR_WIDTH = 0,
    PRIOR_HEIGHT
} PRIOR;
#define DPI 96 //>@Dot per inch
int CalcPixelFromPoint(int pPoint);
int CalcPointFromPixel(int pPixel);
int CalcFontSizeFromPixel(int pPixel);
int CalcFontSizeFromRect(QString pFontType, QSize pSize);
int CalcFontSizeFromRect(QString pFontType, QSize pSize, PRIOR pPrior);
int CalcFontSizeFromSize(QString pFontType, QSize pSize, PRIOR pPrior);
QSize CalcSizeFromFontSize(QString pFontType, int pFontSize);
QSize CalcSizeFromFontSize(QString pFontType, QChar pChar, int pFontSize);
QSize CalcSizeFromFontSize(QString pFontType, QString pText, int pFontSize);

QPointF CalcIntersectPos(QRectF pRect, QLineF pLine);

void WriteText(QPainter *pPainter, QString pText, QRect pRect, QFont pFont, QColor pColor);
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType=QString("Arial"), QColor pColor=Qt::black, PRIOR pPrior=PRIOR_WIDTH);
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, int pFontSize);

inline QPixmap ScalePixmap(QPixmap pPixmap, QSizeF pSize)
{
    QSize tmpSize = pSize.toSize();
    if(!(tmpSize.isEmpty()) && (pPixmap.size() != tmpSize))
        return pPixmap.scaled(tmpSize);
    return pPixmap;
}

qreal normalizeAngle(qreal angle);

inline QBrush GetLinearGradientBrush(QPointF pStartPos, QColor pStartColor, QPointF pEndPos, QColor pEndColor)
{
    QLinearGradient linearGradient(pStartPos, pEndPos);
    linearGradient.setColorAt(0, pStartColor);
    linearGradient.setColorAt(1, pEndColor);//设置渐变的颜色和路径比例
    return QBrush(linearGradient);
}

bool isFileExist(QString FileName);
bool delFile(QString FileName);

bool is_file(QString fullFileName);

bool StringToInt(QString pString,int &tmp);

QString getFileIconName(QString pFileName);

FILE_TYPE getFileType(QString FileName);
FILE_TYPE getFileType(FILE_INFO *pFileInfo);
QString   getFileSuffix(FILE_TYPE pFileType);
QString   getNewFileName(FILE_TYPE pType);
//RC_TYPE   getRcType(QString FileName);
QString   regularFile(QString pRcName, QString pSrcFile, QString pDstPath, QSize pSize);

int GetKeyType(QString pKey);

QString getParentDirectory(QString DirPath);
QString GetDirectoryName(QString DirPath);

QString getFileSuffix(QString pFileName);
QString getFileName(QString FilePath);
QString getFileDirectory(QString FilePath);

QStringList findFiles(const QDir &dir,
                      const QString &fileName,
                      const QString &txt,
                      Qt::CaseSensitivity sensitive);
QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder);
QStringList ErgodicDirectory(const QString d);
QStringList SearchFiles(QString pDirPath, QString pSuffix, bool pSubfolder=false);

QStringList getAllInFloders(QString pDir);
QStringList getAllFileInFloders(QString pDir);

//>@---------------MDP依赖函数-------------------

class MDPConfigParaInfo
{
public:
    enum Permission{RO,RW,EDIT};
public:
    MDPConfigParaInfo(const QDomElement &pElement);
    ~MDPConfigParaInfo();
    bool IsNull();

    typedef struct{
        QString                     Name;
        QList<MDPConfigParaInfo *>  ParaInfoList;
    } SubConfigParaInfo;

public:
    QString                     mName;
    Permission                  mPermission;
    UI_TYPE                mParaType;
    QString                     mParas;
    QString                     mValue;
    QList<SubConfigParaInfo*>   mSubInfos;
};

class MDPConfigInfo
{
public:
    MDPConfigInfo(const QDomElement &pElement);
    ~MDPConfigInfo();
    bool IsNull();

public:
    QString                     mName;
    bool                        mDrag;
    QString                     mRc;
    QRectF                      mRectF;
    int                         mZValue;
    QList<MDPConfigParaInfo *>  mParaInfoList;
};

inline bool ParseIP(QString pIP, QHostAddress &pIPAddr, quint16 &pPort, QString &pCode)
{
    QStringList tmpList = pIP.split(':');
    if(tmpList.count() < 1)
        return false;
    QHostAddress tmpAddr(tmpList.at(0));
    if(tmpAddr.isNull())
        return false;
    if(tmpList.count() > 1)
    {
        if(tmpList.count() == 3)
        {
            bool ok=false;
            quint16 tmpPort = tmpList.at(1).toUShort(&ok, 10);
            if(!ok)
                tmpPort = 0;
            pPort = tmpPort;
        }
        else
        {
            pPort = 0;
        }
        pCode = tmpList.at(2);
    }
    pIPAddr = tmpAddr;
    return true;
}

inline bool CheckIP(QString pIP)
{
    QHostAddress tmpIP;
    quint16 tmpPort;
    QString tmpCode;
    return ParseIP(pIP, tmpIP, tmpPort, tmpCode);
}


//>@---------------UI依赖函数-------------------

COM_TYPE getComponetType(QString ComponentName);
QString getComponetName(COM_TYPE ComponentType);
QString getComponetPath(COM_TYPE pType);
COM_TYPE getParentComType(QString ComPath);
COM_TYPE getParentComType(COM_TYPE pType);
QString getCurComName(QString ComPath);
QString getParentComPath(QString ComPath);
QString getComPath(QString ComName);
COM_TYPE getCurComType(QString ComPath);

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
void SetFuzzy(QGraphicsItem *pItem, qreal pBlurRadius);
void SetColor(QGraphicsItem *pItem, QColor pColor);
void SetShadow(QGraphicsItem *pItem, QColor pColor, qreal pBlurRadius, QPointF pPointF);
void SetOpacity(QGraphicsItem *pItem, qreal pOpacity);
void ClearFuzzy(QGraphicsItem *pItem);
void ClearColor(QGraphicsItem *pItem);
void ClearShadow(QGraphicsItem *pItem);
void ClearOpacity(QGraphicsItem *pItem);

void Effect_Fuzzy(QGraphicsItem *Item,qreal n);
void Effect_color(QGraphicsItem *Item,int r, int g, int b);
void Effect_Shadow(QGraphicsItem *Item,qreal dx, qreal dy);
void Effect_Transparent(QGraphicsItem *Item,qreal opacity);
#endif

//>@---------------转换函数-------------------
typedef enum{
    Audio_Start,
    Audio_PathErr,
    Audio_Suncess,
    Audio_Suncess_Cover,
    Audio_Suncess_Cancel,
    Audio_TimeOut,
    Audio_Failed
} AUDIO_CONVERT_STATE;

typedef enum{
    Video_Start,
    Video_PathErr,
    Video_Suncess,
    Video_Suncess_Cover,
    Video_Suncess_Cancel,
    Video_TimeOut,
    Video_Failed
} VIDEO_CONVERT_STATE;

typedef struct{
    QString                     m_FileName;
    QString                     m_FilePath;
    QString                     m_DstPath;
    quint32                     m_FileLenth;
}CONVERT_FILE_INFO;

typedef struct{
    quint16                     m_TopCut;
    quint16                     m_BottomCut;
    quint16                     m_LeftCut;
    quint16                     m_RightCut;
}CUT_SET;

QString GetStrFromKey(QString pString, QString pKey,
                      quint8 pStart, quint8 pLenth,
                      Qt::CaseSensitivity pCaps);
QString GetStrBehindKey(QString pString, QString pKey,
                        quint8 pStart, quint8 pLenth,
                        Qt::CaseSensitivity pCaps);
QString GetStrBetweenPreAndAfter(QString pString,
                                 QString pPre, QString pAfter,
                                 int pPreindent, int pAfterindent,
                                 Qt::CaseSensitivity pCaps);

bool ConvertImage(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution);
bool ConvertImage(QString pSrcFileName,
                  QString pDstFileName,
                  QSize pResolution);
bool ConvertImage(QString pFileName, QSize pSize, PRIOR pPrior);
QImage GetScaledImage(QString pFileName, QSize pSize, PRIOR pPrior);
QSize GetScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior);

bool ConvertAudio(QString pSrcFileName, QString pDstFileName);

bool ConvertVideo(QString pSrcFileName, QString pDstFileName, QSize pResolution);
bool ConvertVideo(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution);

//>@---------------函数-------------------
#ifdef WINDOWS
size_t _Scanf(pInputCharFun InputFun, const char* fmt,...);
size_t _Printf(pOutputStrFun OutputStrFun, const char* fmt,...);
size_t _Printf(pOutputCharFun OutputCharFun, const char* fmt,...);
#endif

//>@---------------日志函数-------------------
bool InitLogPath(QString pLogPath, QString pLogName, int pMaxLogFileNum);

//>@---------------公有类-------------------------

class GraphicsSceneBase : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsSceneBase(QObject *parent, QRect pRect);
public:
    QRectF            m_Rect;
};

class GraphicsViewBase : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewBase(QWidget *parent, QRect pRect);
    GraphicsViewBase(QWidget *parent);
public:
    GraphicsSceneBase *m_Scene;
};

//>@---------------解析类-------------------------

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps);

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps);

/************************************************************************
//>@
************************************************************************/
typedef struct{
    RC_TYPE                     mType;
    QString                     mPath;
    QString                     mName;
    quint32                     mSize;
    QSize                       mResolution;
    quint32                     mLenth;
}Media_Info;
//>@
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}GRABSCREEN_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}WATERMARk_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}TEXT_SET;

class MULTIMEDIA_CONVERT : public QObject
{
    Q_OBJECT
public:
    MULTIMEDIA_CONVERT(QObject *parent = 0);
    ~MULTIMEDIA_CONVERT();
    void KillProcess()
    {
        mMutex->lock();
        if(m_Process != 0)
        {
            disconnect(m_Process, SIGNAL(readyReadStandardOutput()), 0, 0);
            disconnect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)), 0, 0);
            m_Process->kill();
            static quint8 tmpState;
            while((tmpState = m_Process->state()))
            {
                QtSleep(20);
            }
            m_Process->waitForFinished(1000);
            delete m_Process;
            m_Process = 0;
        }
        mMutex->unlock();
    }
    bool StartProcess(QString pCmd, int pTimeout)
    {
        if(pCmd.isEmpty())
            return false;
        mTaskState = Start;
        KillProcess();
        m_Process = new QProcess(0);
        m_Process->setProcessChannelMode(QProcess::MergedChannels);
        connect(m_Process, SIGNAL(readyReadStandardOutput()),this, SLOT(slot_CatchStandardOutput()));
        connect(m_Process,
                SIGNAL(finished(int, QProcess::ExitStatus)),
                this,
                SLOT(slot_Finished(int, QProcess::ExitStatus)),
                Qt::DirectConnection);
        m_Process->start(pCmd);
        m_Process->waitForFinished(pTimeout);
        KillProcess();
        switch(mTaskState)
        {
            case Success:return true;
            default:return false;
        }
    }
    bool PreHandel(QString pSrc, QString pDst)
    {
        QFile srcFile(pSrc);
        if(!srcFile.exists())
        {
            return false;
        }
        QString dstDir = getFileDirectory(pDst);
        QDir dir(dstDir);
        if(!dir.exists())
        {
            if(!dir.mkpath(dstDir))
            {
                return false;
            }
        }
        QFile dstFile(pDst);
        if(dstFile.exists())
        {
            dstFile.setPermissions(QFile::WriteOther);
            if(!dstFile.remove())
            {
                return false;
            }
        }
        return true;
    }
    bool GetFileInfo(QString pSrc, int pTimeout);
    bool ConvertAudio(QString pSrc, QString pDst, int pTimeout);
    bool ConvertVideo(QString pSrc, QSize pResolution, int pFrame, QString pDst, int pTimeout);
    bool ConvertVideo(QString pSrc, QRect pRect, QSize pResolution, int pFrame, QString pDst, int pTimeout);
//    rm/flv/avi/rmvb
    Media_Info          m_FileInfo;
    TaskState           mTaskState;
    QMutex*             mMutex;
private:
    RC_TYPE GetSrcType(QString pStr)
    {
        if(pStr.contains("mp3",Qt::CaseInsensitive))
            return RC_MP3;
        if(pStr.contains("wav",Qt::CaseInsensitive))
            return RC_WAV;
        if(pStr.contains("mp4",Qt::CaseInsensitive))
            return RC_MP4;
        if(pStr.contains("avi",Qt::CaseInsensitive))
            return RC_AVI;
        if(pStr.contains("rmvb",Qt::CaseInsensitive))
            return RC_RMVB;
        if(pStr.contains("flv",Qt::CaseInsensitive))
            return RC_FLV;
        return RC_INVALID;
    }
    quint32 GetLenth(QString pLenth)
    {
        QStringList tmpStrList = SplitString(pLenth,":");
        quint8  tmpSecond = 0,
                tmpMinute = 0,
                tmpHour = 0;
        quint32 tmpLenth = 0;
        if(tmpStrList.count() != 3)
            return 0;
        bool tmpOk = false;
        tmpSecond = tmpStrList.at(2).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpMinute = tmpStrList.at(1).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpHour = tmpStrList.at(0).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpLenth = tmpHour*60*60 + tmpMinute*60 +tmpSecond;
        return tmpLenth;
    }
    QSize GetResolution(QString pKey)
    {
        QStringList keyList = pKey.split(",", QString::SkipEmptyParts, Qt::CaseInsensitive);
        for(int i=0;i<keyList.count();i++)
        {
            QString subKey = keyList.at(i);
            QStringList subKeyList = subKey.split("x", QString::SkipEmptyParts, Qt::CaseInsensitive);
            if(subKeyList.count() == 2)
            {
                quint16 w = subKeyList.at(0).toInt();
                quint16 h = subKeyList.at(1).toInt();
                QSize size(w, h);
                return size;
            }
        }
        return QSize();
    }
    quint8 GetProgress(QString pKey, quint32 pLenth)
    {
        QStringList keyList = SplitString(pKey,".");
        if(keyList.isEmpty())
            return 0;
        bool ok = false;
        quint16 curPos = keyList.at(0).toInt(&ok,10);
        if(!ok)
            return 0;
        curPos = ((float)curPos/(float)pLenth)*100;
        curPos = (curPos > 100)? 100 : curPos;
        return curPos;
    }
    QString SizeToStr(QSize pSize)
    {
        int width = pSize.width();
        int height = pSize.height();
        width = width - width%10;
        height = height - height%10;
        QString size = QString("%1x%2").arg(width).arg(height);
        return size;
    }

    QProcess            *m_Process;
signals:
    void sProgress(quint8 pVaule);
private slots:
    void slot_CatchStandardOutput();
    void slot_Finished(int, QProcess::ExitStatus);
};

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect, QPixmap pBgPixmap=QPixmap());

class xmlParse : public QObject, public QDomDocument
{
public:
    xmlParse();
    xmlParse(QString fileName);
    ~xmlParse();

    static const QString XmlHead;
    static const QString XmlSuffix;

    QString errorStr;
    int errorLine;
    int errorColumn;

    bool m_Valid;

    bool m_IsChanged;

    QString m_FileDir;
    QString m_FileName;

    QDomElement itemElement;

    //>@根据路径查找
    bool GetItemElement(QString itemPath);
    //>@以元素itemDomElement为根路径进行查找
    bool GetItemElement(QDomElement itemDomElement, QString itemPath);

    //>@模糊查找
    bool SearchItemElement(QString itemName);

    //>@获取元素值 多态
    QString GetItemElementValue();
    QString GetItemElementValue(QString itemPath);

    //>@遍历属性
    QList<QDomAttr> GetItemElementAttrs(QDomElement itemDomElement);
    QList<QDomAttr> GetItemElementAttrs();
    QList<QDomAttr> GetItemElementAttrs(QString itemPath);

    //创建元素节点
    bool CreateItemElement(QString itemPath);
    bool CreateItemElement(QDomElement pParElement, QString pName, QString pValue);

    //>@保存某一个元素为XML文本
    bool SaveItemElement(QDomElement itemDomElement, QString& pContent);     //>@保存到pContent中
    bool SaveItemElement(QString &pContent);
    bool SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice);   //>@保存到IODevice中
    bool SaveItemElement(QIODevice *pDevice);

    bool ModifyItemElement(QString itemPath, QString value);
    bool ModifyItemElement(QDomElement itemDomElement, QString value);
    bool SetText(QDomElement itemDomElement, QString value);

    bool ClearItemElement(QDomElement itemDomElement);
    bool DeleteItemElement(QDomElement itemDomElement);

    //获取当前元素所有的子节点
    QDomNodeList GetChildNodes()
    {
        return itemElement.childNodes();
    }

    bool write(QIODevice *device);
    bool read(QIODevice *device);
    bool Open();
    bool New();
    bool Save();
    bool SaveAs(QString fileName);
    bool Rename(QString fileName);
};

//>@注意key不能为“/”，否则会死机
class iniParse : public QSettings
{
public:
    iniParse(QString pfileName);
    void setProperty(QString key,QString value) { setValue(key, value); }
    void setProperty(QString key,int value)     { setValue(key, value); }

    bool getProperty(QString pkey, QString &pValue);
    int  getProperty(QString pkey, int &pValue);

    /*
    可以直接读取中文的QSTRING的，不过真正的意义在于把QSETTING的读取返回值放入 到QByteArray中才是关键。
    QTextCodec *codec=QTextCodec ::codecForName("GBK"); //设定字符集为GBK（大小写均可，还可用GB18030代替）
    QString outStr = codec->toUnicode(myArray); //这里就好解释了，通过译码器将字节数组中的值转化为UNICODE的QSTRING就O了
    */
};

#endif // GLOBALFUN_H
