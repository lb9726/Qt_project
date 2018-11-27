#ifndef BOOTLOGO_H
#define BOOTLOGO_H

#include "global.h"

typedef struct{
    int                mPrgValue;
    QRect              mPrgValueRect;
    int                mPrgTextFontSize;
    QFont              mPrgTextFont;
} PRG_INFO;

typedef struct{
    QStringList        mMsgQueue;
    QRect              mMsgTextRect;
    QFont              mMsgTextFont;
    int                mMaxMsgRow;
} LOG_INFO;

typedef struct{
    QString            mContent;
    QRect              mRect;
    QFont              mFont;
    QColor             mColor;
} VERSION_INFO;

class BootLogo
{
public:
    BootLogo();
    BootLogo(QRect pRect, QString pFile, QString pPattern);

    void Init();
    bool ShowMessage(QString pMsg);
    int GetMsgCount();
    bool SetProgress(int pValue);
    int GetProgress();

    void SetVersion(QString pVer);

    bool SetFile(QString pFile);
    bool SetRect(QRect pRect);
    bool SetPattern(QString pPattern);

    void ShowLogoOrMsg(bool pLogo);
    void SetRencode(QPixmap pPixmap);

public:
    QRect         m_BgRect;
    QString       m_BgFile;
    QPixmap       m_BgPixmap;   //>@背景

    int           m_Pattern;

    QPixmap       m_LogoPixmap; //>@Logo
    QRect         m_LogoRect;

    QPixmap       m_PrgPixmap;  //>@进度
    QRect         m_PrgRect;
    PRG_INFO      m_PrgInfo;

    QPixmap       m_MsgPixmap;  //>@日志
    QRect         m_MsgRect;
    LOG_INFO      m_MsgInfo;

    VERSION_INFO  m_VerInfo;

    QRect         mRencodeRect;
    QPixmap       mRencodePix;
    bool          m_ShowLogo;
#ifdef N329
    QPixmap       m_TransparentPix;
#endif
};

#endif // BOOTLOGO_H
