#include "bootlogo.h"
//#include "bggen.h"

BootLogo::BootLogo()
{
    Init();
}

BootLogo::BootLogo(QRect pRect, QString pFile, QString pPattern)
{
    Init();
    SetRect(pRect);
    SetFile(pFile);
    SetPattern(pPattern);
}

void BootLogo::Init()
{
    m_ShowLogo = true;
    m_PrgInfo.mPrgValue = -1;
    m_MsgInfo.mMaxMsgRow = -1;
    m_MsgInfo.mMsgQueue.append(GetAppVersion());
}

bool BootLogo::SetProgress(int pValue)
{
    if(pValue == m_PrgInfo.mPrgValue)
        return false;
    if(pValue == INVALIDRC)  //>@表示不改变当前进度条值
        return true;
    else if(pValue > 100)
        m_PrgInfo.mPrgValue = 100;
    else if(pValue < 0)
        m_PrgInfo.mPrgValue = 0;
    else
        m_PrgInfo.mPrgValue = pValue;
    return true;
}

int BootLogo::GetProgress()
{
    return m_PrgInfo.mPrgValue;
}

int BootLogo::GetMsgCount()
{
    return m_MsgInfo.mMsgQueue.count();
}

bool BootLogo::ShowMessage(QString pMsg)
{
    if(m_ShowLogo)
        return true;
    if(!pMsg.isEmpty())
    {
        if(m_MsgInfo.mMsgQueue.count() < m_MsgInfo.mMaxMsgRow)
        {
            m_MsgInfo.mMsgQueue.append(pMsg);
        }
        else
        {
            m_MsgInfo.mMsgQueue.removeFirst();
            m_MsgInfo.mMsgQueue.append(pMsg);
        }
        IDE_TRACE_STR(pMsg);
        return true;
    }
    return false;
}

void BootLogo::SetVersion(QString pVer)
{
    m_VerInfo.mContent = pVer;
    IDE_TRACE_STR(pVer);
}

bool BootLogo::SetFile(QString pFile)
{
    if(m_BgFile.compare(pFile))
        m_BgFile = pFile;
    if(m_BgRect.isEmpty())
        return false;
    if(pFile.isEmpty() || !QFile::exists(pFile))  //>@使用默认配色
    {
        BaseBgGen tmpGen;
        tmpGen.SetPattern(m_Pattern);
        tmpGen.SetFullScreen(true);
        tmpGen.Resize(m_BgRect.size());
        tmpGen.Refresh();
        if(!tmpGen.m_Image.isNull())
            m_BgPixmap = QPixmap::fromImage(tmpGen.m_Image);
        tmpGen.deleteLater();
    }
    else
    {
        QImage tmpImage(m_BgFile);
        if(!tmpImage.isNull())
            m_BgPixmap = QPixmap::fromImage(tmpImage.scaled(m_BgRect.size()));
    }
    return !m_BgPixmap.isNull();
}

void BootLogo::ShowLogoOrMsg(bool pLogo)
{
    m_ShowLogo = pLogo;
}

void BootLogo::SetRencode(QPixmap pPixmap)
{
    mRencodePix = pPixmap;
}

bool BootLogo::SetRect(QRect pRect)
{
    m_BgRect = pRect;
    if(!m_BgRect.isEmpty())
    {
        //>@设置二维码的大小为屏幕最小边的1/5
        int len = qMin(m_BgRect.width(), m_BgRect.height());
        int w = len / 5.0;
        int margin = len / 10.0;
        mRencodeRect = QRect(m_BgRect.x()+margin, m_BgRect.bottom()-w-margin, w, w);
    }
    return SetFile(m_BgFile);
}

bool BootLogo::SetPattern(QString pPattern)
{
    m_LogoPixmap = m_PrgPixmap = m_MsgPixmap = QPixmap();
    if(m_BgRect.isEmpty())
        return false;
    if(!pPattern.compare("bst", Qt::CaseInsensitive))
    {
        m_Pattern = 0;
    }
    else if(!pPattern.compare("Sodimas", Qt::CaseInsensitive))
    {
        m_Pattern = 1;
    }
    else
        return false;
    int len = qMin(m_BgRect.width(), m_BgRect.height());
    //>@获取Progress信息
    int tmpPenWid = 10;
    int tmpX = m_BgRect.width()/6.0;
    int tmeY = m_BgRect.height()/2.0;
    int tmeW = m_BgRect.width()*2.0/3.0;
    int tmeH = 44;//tmeW/15.0;
    m_PrgRect = QRect(tmpX, tmeY, tmeW, tmeH);
    QRect tmpPrgRect = QRect(QPoint(0,0), m_PrgRect.size());
#ifdef N329
    QImage tmpPrgImage = m_TransparentPix.toImage().scaled(m_PrgRect.size());
    if(tmpPrgImage.isNull())
        return false;
#else
    QImage tmpPrgImage(m_PrgRect.size(), QImage::Format_ARGB32_Premultiplied);
    if(tmpPrgImage.isNull())
        return false;
    tmpPrgImage.fill(Qt::transparent); //>@创建透明图层
#endif
    QPainter tmpPrgPainter(&tmpPrgImage);
    tmpPrgPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    tmpPrgPainter.setRenderHint(QPainter::Antialiasing);
    //>@绘制进度条外框
    tmpPrgPainter.save();
    tmpPrgPainter.setBrush(QBrush(QColor(55,70,79)));
    int round = tmpPrgRect.height()/2;
    tmpPrgPainter.drawRoundedRect(tmpPrgRect, round, round);
    tmpPrgPainter.restore();
    //>@绘制进度条底图及顶图
    tmpPrgPainter.save();
    QLinearGradient pgBaseGradient(QPointF(0,tmpPenWid/2), QPointF(0,tmeH-tmpPenWid));
    pgBaseGradient.setColorAt(0, QColor(241,241,241,255));
    pgBaseGradient.setColorAt(0.5, QColor(202,202,202,255));
    pgBaseGradient.setColorAt(1, QColor(241,241,241,255));//设置渐变的颜色和路径比例
    tmpPrgPainter.setPen(Qt::NoPen);
    tmpPrgPainter.setBrush(QBrush(pgBaseGradient));
    m_PrgInfo.mPrgValueRect = tmpPrgRect.adjusted(tmpPenWid, tmpPenWid, -tmpPenWid, -tmpPenWid);
    round = m_PrgInfo.mPrgValueRect.height()/2;
    tmpPrgPainter.drawRoundedRect(m_PrgInfo.mPrgValueRect, round, round);
    tmpPrgPainter.restore();
    int fontheight = tmpPrgRect.height()*2.0/3.0;
    m_PrgInfo.mPrgTextFontSize = CalcFontSizeFromSize(QString("Arial"), QSize(100,fontheight), PRIOR_HEIGHT);
    m_PrgInfo.mPrgTextFont = QFont("Arial", m_PrgInfo.mPrgTextFontSize, QFont::DemiBold, false);
    tmpPrgPainter.end();
    m_PrgPixmap = QPixmap::fromImage(tmpPrgImage);

    if(m_ShowLogo)
    {
        //>@Logo
        QImage tmpLogoImage = QImage(D_FSRCPATH + D_BOOTDIR + "logo.png");
        if(!tmpLogoImage.isNull())
        {
            QSize tmpLogoSize = tmpLogoImage.size();
            tmeH = len/5.0;
            tmeY = (m_BgRect.height()-len)/2.0 + tmeH;
            tmeW = (qreal)tmeH / (qreal)tmpLogoSize.height() * (qreal)tmpLogoSize.width();
            tmpX = (m_BgRect.width()-tmeW)/2.0;
            m_LogoRect = QRect(tmpX, tmeY, tmeW, tmeH);
            m_LogoPixmap = QPixmap::fromImage(tmpLogoImage.scaled(m_LogoRect.size()));
        }
    }
    else
    {
        //>@获取Msg信息
        //>@绘制消息底图
        tmpX = m_BgRect.width()/4.0;
        tmeY = m_BgRect.height()/5.0;
        tmeW = m_BgRect.width()/2.0;
        tmeH = m_BgRect.height()*3.0/10.0;
        m_MsgRect = QRect(tmpX, tmeY, tmeW, tmeH);
        QRect tmpMsgRect = QRect(QPoint(0,0), m_MsgRect.size());
#ifdef N329
        QImage tmpMsgImage = m_TransparentPix.toImage().scaled(m_MsgRect.size());
        if(tmpMsgImage.isNull())
            return false;
#else
        QImage tmpMsgImage(m_MsgRect.size(), QImage::Format_ARGB32_Premultiplied);
        if(tmpPrgImage.isNull())
            return false;
        tmpMsgImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter tmpMsgPainter(&tmpMsgImage);
        tmpMsgPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        tmpMsgPainter.setRenderHint(QPainter::Antialiasing);
        tmpMsgPainter.save();
        tmpMsgPainter.setBrush(QBrush(QColor(238, 239, 239)));
        round = tmpMsgRect.height()/10;
        tmpMsgPainter.drawRoundedRect(tmpMsgRect, round, round);
        int ydiff = 3;
        int xdiff = 3;
        QRect tmpNewMsgRect = tmpMsgRect.adjusted(xdiff,ydiff,-xdiff,-ydiff);
        round = tmpNewMsgRect.height()/10;
        tmpMsgPainter.drawRoundedRect(tmpNewMsgRect, round, round);
        tmpX += xdiff*3;
        tmeY += ydiff*4;
        tmeW -= xdiff*6;
        tmeH -= 8*ydiff;
        m_MsgInfo.mMsgTextRect = QRect(tmpX, tmeY, tmeW, tmeH);
        m_MsgInfo.mMsgTextFont = QFont("Arial", 12, QFont::DemiBold, false);
        QFontMetricsF fm(m_MsgInfo.mMsgTextFont);
        m_MsgInfo.mMaxMsgRow = m_MsgInfo.mMsgTextRect.height() / fm.height();
        tmpMsgPainter.restore();
        tmpMsgPainter.end();
        m_MsgPixmap = QPixmap::fromImage(tmpMsgImage);
    }

    //>@显示版本号
    tmeH = len/10.0;
    tmeY = (m_BgRect.height()-len)/2.0 + len - tmeH*2;
    tmeW = m_BgRect.width();
    tmpX = 0;
    m_VerInfo.mRect = QRect(tmpX, tmeY, tmeW, tmeH*2);
    m_VerInfo.mFont = QFont("Arial", 18, QFont::DemiBold, false);
    m_VerInfo.mColor = Qt::white;
    return true;
}

