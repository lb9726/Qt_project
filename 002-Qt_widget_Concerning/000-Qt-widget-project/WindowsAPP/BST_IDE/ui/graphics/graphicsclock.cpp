#include "graphicsclock.h"

GraphicsClock::GraphicsClock(QGraphicsItem *parent):
    GraphicsOperate(parent)
{
    m_Diff = 0;
    m_ItemWidth = m_ItemHeight = 0;
}

void GraphicsClock::ReleaseElementInfos()
{
    QList<CLOCKE_ELEMENT> tmpList = m_ClockElement.keys();
    for(int i=tmpList.count()-1; i>=0; i--)
    {
        CLOCK_ELEMENTINFO *tmpElement = m_ClockElement.take(tmpList.at(i));
        if(tmpElement)
            delete tmpElement;
    }
    m_ClockElement.clear();
}

void GraphicsClock::CalcElementInfo(CLOCK_TYPE pType)
{
    ReleaseElementInfos();
    if(pType == ANALOG)
    {
        //>@根据区域大小计算时间布局
        int hw = ::qMin(size().width(), size().height());
        int hh = hw;
        m_ItemWidth = (qreal)hw / 20.0;
        m_ItemHeight = ::qMin((int)m_ItemWidth, hh);
        //>@下面几行代码要放在for(int i 循环之前，因为在其中使用m_PointerLength来计算指针的RECT。
        m_PointerLength[Element_HourPointer] = (qreal)(hw / 2.0) * 6.0 / 10.0;
        m_PointerLength[Element_MinutePointer] = (qreal)(hw / 2.0) * 6.5 / 10.0;
        m_PointerLength[Element_SecondPointer] = (qreal)(hw / 2.0) * 8.0 / 10.0;
        m_PointerAngle[Element_HourPointer][Last] = 0;
        m_PointerAngle[Element_MinutePointer][Last] = 0;
        m_PointerAngle[Element_SecondPointer][Last] = 0;
        for(int i=Element_HourPointer; i<=Element_Week; i++)
        {
            CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
            tmpEle->mAnimateFlag = false;
            switch(i)
            {
                case Element_Dot:
                {
                    int w = (qreal)hw / 10.0;
                    tmpEle->mRect = QRectF(hw/2.0-w/2.0,hw/2.0-w/2.0, w, w);
                    tmpEle->mRcPointer.mLastPointer = ClockDot;
                    tmpEle->mRcPointer.mCurPointer = ClockDot;
                    break;
                }
                case Element_Year0:
                case Element_Year1:
                case Element_Year2:
                case Element_Year3:
                case Element_Month0:
                case Element_Month1:
                case Element_Day0:
                case Element_Day1:
                    tmpEle->mRect = QRectF(m_ItemWidth*(i-Element_Year0+5),
                                          (qreal)hh / 3.0,
                                          m_ItemWidth,
                                          m_ItemHeight);
                    tmpEle->mRcPointer.mLastPointer = Text0;
                    tmpEle->mRcPointer.mCurPointer = Text0;
                    break;
                case Element_Minus0:
                case Element_Minus1:
                    tmpEle->mRect = QRectF(m_ItemWidth*(i-Element_Year0+5),
                                          (qreal)hh / 3.0,
                                          m_ItemWidth,
                                          m_ItemHeight);
                    tmpEle->mRcPointer.mLastPointer = ClockMinus;  //>@ClockMinus指绘制-符号
                    tmpEle->mRcPointer.mCurPointer = ClockMinus;
                    break;
                case Element_Week:
                    tmpEle->mRect = QRectF(m_ItemWidth*12,
                                          m_ItemHeight*10 - m_ItemHeight/2.0,
                                          m_ItemWidth*4,
                                          m_ItemHeight);
                    tmpEle->mRcPointer.mLastPointer = ClockSun;  //>@ClockSun表示星期日
                    tmpEle->mRcPointer.mCurPointer = ClockSun;
                    break;
                default:
                    delete tmpEle;
                    continue;
            }
            m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
        }
    }
    else if(pType == DIGITAL)
    {
        int hw = size().width();
        int hh = size().height();
        switch (m_Parttern)
        {
            case PTN_V_YMDWHMS:
            {
                m_ItemWidth = (qreal)hw/10.0;
                m_ItemHeight = (qreal)hh/4.0;
                for(int i=Element_Year0; i<=Element_Second1; i++)
                {
                    CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
                    tmpEle->mAnimateFlag = false;
                    if(i < Element_Week)        //>@年月日其区域
                    {
                        int j = i-Element_Year0;
                        tmpEle->mRect = QRectF(m_ItemWidth*j, 0, m_ItemWidth, m_ItemHeight);
                        if(i == Element_Minus0 || i == Element_Minus1)
                        {
                            tmpEle->mRcPointer.mLastPointer = ClockMinus;
                            tmpEle->mRcPointer.mCurPointer = ClockMinus;
                        }
                        else
                        {
                            tmpEle->mRcPointer.mLastPointer = Text0;
                            tmpEle->mRcPointer.mCurPointer = Text0;
                        }
                    }
                    else if(i == Element_Week)  //>@星期区域
                    {
                        tmpEle->mRect = QRectF(m_ItemWidth*3, m_ItemHeight*3/2, m_ItemWidth*4, m_ItemHeight);
                        tmpEle->mRcPointer.mLastPointer = ClockSun;
                        tmpEle->mRcPointer.mCurPointer = ClockSun;
                    }
                    else                    //>@11~19 时分秒区域
                    {
                        int j = i-Element_Hour0;
                        tmpEle->mRect = QRectF(m_ItemWidth*(j+1), m_ItemHeight*3, m_ItemWidth, m_ItemHeight);
                        if(i == Element_Colon0 || i == Element_Colon1)
                        {
                            tmpEle->mRcPointer.mLastPointer = ClockColon;
                            tmpEle->mRcPointer.mCurPointer = ClockColon;
                        }
                        else
                        {
                            tmpEle->mRcPointer.mLastPointer = Text0;
                            tmpEle->mRcPointer.mCurPointer = Text0;
                        }
                    }
                    m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
                }
                break;
            }
            case PTN_H_MDYHM:
            {
                m_ItemWidth = (qreal)hw/16.0;     //>@MM-DD-YYYY HH:mm
                m_ItemHeight = (qreal)hh;
                for(int i=Element_Year0; i<=Element_Second1; i++)
                {
                    int j;
                    if(i>=Element_Year0 && i<= Element_Year3)
                        j = i-Element_Year0+6;
                    else if(i==Element_Minus0)
                        j = 2;
                    else if(i==Element_Minus1)
                        j = 5;
                    else if(i==Element_Colon0)
                        j = 13;
                    else if(i>=Element_Month0 && i<= Element_Day1)
                        j = i-Element_Month0;
                    else if(i>=Element_Hour0 && i<=Element_Minute1)
                        j = i-Element_Hour0+11;
                    else
                        continue;

                    CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
                    tmpEle->mAnimateFlag = false;
                    tmpEle->mRect = QRectF(m_ItemWidth*j, 0, m_ItemWidth, m_ItemHeight);
                    if(i == Element_Minus0 || i == Element_Minus1)
                    {
                        tmpEle->mRcPointer.mLastPointer = ClockMinus;
                        tmpEle->mRcPointer.mCurPointer = ClockMinus;
                    }
                    else if(i == Element_Colon0)
                    {
                        tmpEle->mRcPointer.mLastPointer = ClockColon;
                        tmpEle->mRcPointer.mCurPointer = ClockColon;
                    }
                    else
                    {
                        tmpEle->mRcPointer.mLastPointer = Text0;
                        tmpEle->mRcPointer.mCurPointer = Text0;
                    }
                    m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
                }
                break;
            }
            case PTN_H_YMDWHMS:
            {
                m_ItemWidth = (qreal)hw/24.0;   //>@YYYY-MM-DD WWWW HH:mm:SS
                m_ItemHeight = (qreal)hh;
                for(int i=Element_Year0; i<=Element_Second1; i++)
                {
                    CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
                    tmpEle->mAnimateFlag = false;
                    if(i < Element_Week)        //>@年月日其区域
                    {
                        int j = i-Element_Year0;
                        tmpEle->mRect = QRectF(m_ItemWidth*j, 0, m_ItemWidth, m_ItemHeight);
                        if(i == Element_Minus0 || i == Element_Minus1)
                        {
                            tmpEle->mRcPointer.mLastPointer = ClockMinus;
                            tmpEle->mRcPointer.mCurPointer = ClockMinus;
                        }
                        else
                        {
                            tmpEle->mRcPointer.mLastPointer = Text0;
                            tmpEle->mRcPointer.mCurPointer = Text0;
                        }
                    }
                    else if(i == Element_Week)  //>@星期区域
                    {
                        int j = i-Element_Year0;
                        tmpEle->mRect = QRectF(m_ItemWidth*(j+1), 0, m_ItemWidth*4, m_ItemHeight);
                        tmpEle->mRcPointer.mLastPointer = ClockSun;
                        tmpEle->mRcPointer.mCurPointer = ClockSun;
                    }
                    else                    //>@11~19 时分秒区域
                    {
                        int j = i-Element_Year0;
                        tmpEle->mRect = QRectF(m_ItemWidth*(j+5), 0, m_ItemWidth, m_ItemHeight);
                        if(i == Element_Colon0 || i == Element_Colon1)
                        {
                            tmpEle->mRcPointer.mLastPointer = ClockColon;
                            tmpEle->mRcPointer.mCurPointer = ClockColon;
                        }
                        else
                        {
                            tmpEle->mRcPointer.mLastPointer = Text0;
                            tmpEle->mRcPointer.mCurPointer = Text0;
                        }
                    }
                    m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
                }
                break;
            }
            case PTN_MOJI_MDWHM:
            {
                m_ItemWidth = (qreal)hw/10.0;
                m_ItemHeight = (qreal)hh/2.5;
                break;
            }
            default:
                return;
        }
    }
}

void GraphicsClock::CalcAnalogClockPointer()
{
    for(int i=Element_HourPointer; i<=Element_SecondPointer; i++)
    {
        CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
        tmpEle->mAnimateFlag = false;
        switch(i)
        {
            case Element_HourPointer:
            case Element_MinutePointer:
            case Element_SecondPointer:
            {
                QPixmap tmpPixmap = GetPixmap(i+ClockHourPointer);
                if(tmpPixmap.isNull())
                {
                    IDE_TRACE();
                    break;
                }
                IDE_TRACE_INT(i);
                qreal ww,hh;
                int w = tmpPixmap.width();
                int h = tmpPixmap.height();
                if(w >= h)
                {
                    ww = m_PointerLength[i];
                    qreal scale = (qreal)w / ww;
                    hh = (qreal)h / scale;
                    tmpEle->mRect = QRectF(0,-hh/2.0, ww, hh);
                }
                else
                {
                    hh = m_PointerLength[i];
                    qreal scale = (qreal)h / hh;
                    ww = (qreal)w / scale;
                    tmpEle->mRect = QRectF(-ww/2.0, 0, ww, hh);
                }
                tmpEle->mRcPointer.mLastPointer = i+ClockHourPointer;
                tmpEle->mRcPointer.mCurPointer = i+ClockHourPointer;
                break;
            }
            default:
                break;
        }
        m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
    }
}

bool GraphicsClock::InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent)
{
    if(pLabel.compare(PARA_PATTERN, Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare(QString("V_YMDWHMS")))
            m_Parttern = PTN_V_YMDWHMS;
        else if(!pContent.compare(QString("H_YMDWHMS")))
            m_Parttern = PTN_H_YMDWHMS;
        else if(!pContent.compare(QString("H_MDYHM")))
            m_Parttern = PTN_H_MDYHM;
        else if(!pContent.compare(QString("MOJI_MDWHM")))
            m_Parttern = PTN_MOJI_MDWHM;
    }
    else if(pLabel.compare("Style", Qt::CaseInsensitive) == 0)
    {
        m_Type = GetClockType(pContent);
    }
    return true;
}

CLOCK_TYPE GraphicsClock::GetClockType(QString pType)
{
    CLOCK_TYPE tmpType = DIGITAL;
    if(!pType.compare(QString("Digital")))
        tmpType = DIGITAL;
    else if(!pType.compare(QString("Analog")))
        tmpType = ANALOG;
    return tmpType;
}

bool GraphicsClock::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;
    CalcElementInfo(m_Type);
    CalcAnalogClockPointer();

#ifdef N329
    m_EffectRC.insert(2000, CreateRcInfo(QPixmap(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(size().toSize())));
#endif

    m_MinSizeLen = ::qMin(size().width(), size().height());
    m_SavedTime.fromChar(3,5,2,1,1,1);
    updateEffect(CLOCK_INFO(0,1,1,2,0,0));
    return true;
}

bool GraphicsClock::PaintEffect(QPainter *p)
{
    if(m_Type == ANALOG)
    {
        //>@绘制背景、日期、星期
        DrawPixmap(p, m_BufferPixmap[2], QRectF(0,0,m_MinSizeLen,m_MinSizeLen));
        //>@绘制指针
        int index = m_Animator.currentFrame();
        int tmpFrameCount;
        for(int i=Element_HourPointer;i<=Element_SecondPointer;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            p->save();
            p->translate(m_MinSizeLen/2, m_MinSizeLen/2);
            if(tmpEle->mAnimateFlag && m_AreaAnimate)
            {
                tmpFrameCount = m_AreaAnimate->mFrameEnd - m_AreaAnimate->mFrameStart;
                m_Diff = (qreal)(m_PointerAngle[i][Current] - m_PointerAngle[i][Last]) / tmpFrameCount;
                p->rotate(m_PointerAngle[i][Last] + m_Diff*index);
            }
            else
            {
                p->rotate(m_PointerAngle[i][Current]);
            }
            DrawPixmap(p, GetPixmap(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect);
            p->restore();
        }
        //>@绘制Dot
        CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value(Element_Dot);
        if(tmpEle)
        {
            DrawPixmap(p, GetPixmap(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect);
        }
    }
    else if(m_Type == DIGITAL)
    {
        //>@绘制背景、日期、星期
        DrawPixmap(p, m_BufferPixmap[2], rect());
        //>@绘制时间
        for(int i=Element_Hour0;i<=Element_Second1;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            m_PixmapPointer.mLastPointer = GetPixmap(tmpEle->mRcPointer.mLastPointer);
            m_PixmapPointer.mCurPointer = GetPixmap(tmpEle->mRcPointer.mCurPointer);
            if(tmpEle->mAnimateFlag == false || m_AreaAnimate == 0)
            {
                Paint2D_None(p, tmpEle->mRect);
            }
            else
            {
                switch(m_AreaAnimate->mEffectType)
                {
                case EffectTypeRoll:
                    Paint2D_Roll(p, tmpEle->mRect);
                    break;
                case EffectTypeFlip:
                    Paint2D_Flip(p, tmpEle->mRect);
                    break;
                case EffectTypeZoom:
                    Paint2D_Zoom(p, tmpEle->mRect);
                    break;
                case EffectTypeRotate:
                    Paint2D_Rotate(p, tmpEle->mRect);
                    break;
                case EffectTypeSlipCycle:
                    Paint2D_Slipcycle(p, tmpEle->mRect);
                    break;
                case EffectTypeTransparent:
                    Paint2D_Transparent(p, tmpEle->mRect);
                    break;
                default:
                    Paint2D_None(p, tmpEle->mRect);
                    break;
                }
            }
        }
    }
    return true;
}

void GraphicsClock::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                CLOCK_INFO tmpTime(0,1,1,2,0,0);
                updateEffect(tmpTime);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                CLOCK_INFO tmpTime(3,5,2,1,1,1);
                updateEffect(tmpTime);
            }
            break;
        }
        case OPERATE_CHANGE:
        {
            QDateTime tmpValue = pPara.toDateTime();
            CLOCK_INFO tmpTime(tmpValue);
            updateEffect(tmpTime);
            break;
        }
        default:
            break;
    }
}

void GraphicsClock::updateEffect(CLOCK_INFO pTime)
{
    if(m_EffectValid == false)
        return;
    if(pTime.isValid() == false)
    {
        IDE_DEBUG("DateTime is invalid,please check it!");
        return;
    }
    bool flag = false;  //>@如果时间有改变则....
    //>@确定数字时钟各部位的图片
    quint32 tmpNewYear = pTime.mYear + 2000;
    quint32 tmpOldYear = m_SavedTime.mYear + 2000;
    CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value(Element_Year0);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewYear0 = tmpNewYear / 1000;
//        quint32 tmpOldYear0 = tmpOldYear / 1000;
//        if(tmpNewYear0 != tmpOldYear0)
//        {
//            tmpEle->mAnimateFlag = true;
//            tmpEle->mRcPointer.mLastPointer = tmpOldYear0;
//            tmpEle->mRcPointer.mCurPointer = tmpNewYear0;
//            IDE_TRACE_INT(tmpEle->mRcPointer.mLastPointer);
//            IDE_TRACE_INT(tmpEle->mRcPointer.mCurPointer);
//            flag = true;
//        }
        tmpEle->mAnimateFlag = true;
        tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
        tmpEle->mRcPointer.mCurPointer = tmpNewYear0;
    }
    tmpEle = m_ClockElement.value(Element_Year1);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewYear1 = tmpNewYear%1000/100;
//        quint32 tmpOldYear1 = tmpOldYear%1000/100;
//        if(tmpNewYear1 != tmpOldYear1)
//        {
//            tmpEle->mAnimateFlag = true;
//            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
//            tmpEle->mRcPointer.mCurPointer = tmpNewYear1;
//            flag = true;
//        }
        tmpEle->mAnimateFlag = true;
        tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
        tmpEle->mRcPointer.mCurPointer = tmpNewYear1;
    }
    tmpEle = m_ClockElement.value(Element_Year2);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewYear2 = tmpNewYear%100/10;
        quint32 tmpOldYear2 = tmpOldYear%100/10;
        if(tmpOldYear2 != tmpNewYear2)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewYear2;
            flag = true;
        }
    }
    tmpEle = m_ClockElement.value(Element_Year3);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewYear3 = tmpNewYear%10;
        quint32 tmpOldYear3 = tmpOldYear%10;
        if(tmpNewYear3 != tmpOldYear3)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewYear3;
            flag = true;
        }
    }
    //>@判断年处是否有改变，如果有，则更新m_BufferPixmap[0]
    if(flag)
    {
#ifdef N329
        QImage tmpImage = GetPixmap(2000).toImage();
        if(tmpImage.isNull())
            return;
#else
        QImage tmpImage(size().toSize(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return;
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter p(&tmpImage);
        if(m_Type == ANALOG)
            DrawPixmap(&p, GetPixmap(BGRC), QRectF(0,0,m_MinSizeLen,m_MinSizeLen));
        else if(m_Type == DIGITAL)
            DrawPixmap(&p, GetPixmap(BGRC), rect());
        for(int i=Element_Year0;i<=Element_Minus0;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            DrawPixmap(&p, GetPixmap(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect);
        }
        p.end();
        m_BufferPixmap[0] = QPixmap::fromImage(tmpImage);
    }

    tmpEle = m_ClockElement.value(Element_Month0);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewMonth0 = pTime.mMonth/10;
        quint32 tmpOldMonth0 = m_SavedTime.mMonth/10;
        if(tmpNewMonth0 != tmpOldMonth0)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewMonth0;
            flag = true;
        }
    }
    tmpEle = m_ClockElement.value(Element_Month1);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewMonth1 = pTime.mMonth%10;
        quint32 tmpOldMonth1 = m_SavedTime.mMonth%10;
        if(tmpNewMonth1 != tmpOldMonth1)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewMonth1;
            flag = true;
        }
    }
    //>@判断月处是否有改变，如果有，则更新m_BufferPixmap[1]
    if(flag)
    {
#ifdef N329
        QImage tmpImage = GetPixmap(2000).toImage();
        if(tmpImage.isNull())
            return;
#else
        QImage tmpImage(size().toSize(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return;
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter p(&tmpImage);
        DrawPixmap(&p, m_BufferPixmap[0], rect());
        for(int i=Element_Month0;i<=Element_Minus1;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            DrawPixmap(&p, GetPixmap(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect);
        }
        p.end();
        m_BufferPixmap[1] = QPixmap::fromImage(tmpImage);
    }

    tmpEle = m_ClockElement.value(Element_Day0);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewDay0 = pTime.mDay/10;
        quint32 tmpOldDay0 = m_SavedTime.mDay/10;
        if(tmpNewDay0 != tmpOldDay0)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewDay0;
            flag = true;
        }
    }
    tmpEle = m_ClockElement.value(Element_Day1);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        quint32 tmpNewDay0 = pTime.mDay%10;
        quint32 tmpOldDay0 = m_SavedTime.mDay%10;
        if(tmpNewDay0 != tmpOldDay0)
        {
            tmpEle->mAnimateFlag = true;
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = tmpNewDay0;
            flag = true;
        }
    }
    //>@week
    tmpEle = m_ClockElement.value(Element_Week);
    if(tmpEle != 0)
    {
        tmpEle->mAnimateFlag = false;
        if(m_SavedTime.mWeekDay != pTime.mWeekDay)
        {
            tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
            tmpEle->mRcPointer.mCurPointer = 10 + pTime.mWeekDay/*1~7*/ - 1;
            tmpEle->mAnimateFlag = true;
            flag = true;
        }
    }
    //>@判断日处是否有改变，如果有，则更新m_BufferPixmap[2]
    if(flag)
    {
#ifdef N329
        QImage tmpImage = GetPixmap(2000).toImage();
        if(tmpImage.isNull())
            return;
#else
        QImage tmpImage(size().toSize(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return;
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter p(&tmpImage);
        DrawPixmap(&p, m_BufferPixmap[1], rect());
        for(int i=Element_Day0;i<=Element_Week;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            DrawPixmap(&p, GetPixmap(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect);
        }
        p.end();
        m_BufferPixmap[2] = QPixmap::fromImage(tmpImage);
    }

    //>@查看时钟部分
    if(m_Type == DIGITAL)
    {
        tmpEle = m_ClockElement.value(Element_Hour0);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewHour0 = pTime.mHour/10;
            quint32 tmpOldHour0 = m_SavedTime.mHour/10;
            if(tmpNewHour0 != tmpOldHour0)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewHour0;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
        tmpEle = m_ClockElement.value(Element_Hour1);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewHour1 = pTime.mHour%10;
            quint32 tmpOldHour1 = m_SavedTime.mHour%10;
            if(tmpNewHour1 != tmpOldHour1)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewHour1;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }

        tmpEle = m_ClockElement.value(Element_Minute0);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewMinute0 = pTime.mMinute/10;
            quint32 tmpOldMinute0 = m_SavedTime.mMinute/10;
            if(tmpNewMinute0 != tmpOldMinute0)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewMinute0;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
        tmpEle = m_ClockElement.value(Element_Minute1);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewMinute1 = pTime.mMinute%10;
            quint32 tmpOldMinute1 = m_SavedTime.mMinute%10;
            if(tmpNewMinute1 != tmpOldMinute1)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewMinute1;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }


        tmpEle = m_ClockElement.value(Element_Second0);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewSecond0 = pTime.mSecond/10;
            quint32 tmpOldSecond0 = m_SavedTime.mSecond/10;
            if(tmpNewSecond0 != tmpOldSecond0)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewSecond0;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
        tmpEle = m_ClockElement.value(Element_Second1);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            quint32 tmpNewSecond1 = pTime.mSecond%10;
            quint32 tmpOldSecond1 = m_SavedTime.mSecond%10;
            if(tmpNewSecond1 != tmpOldSecond1)
            {
                tmpEle->mRcPointer.mLastPointer = tmpEle->mRcPointer.mCurPointer;
                tmpEle->mRcPointer.mCurPointer = tmpNewSecond1;
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
    }
    else if(m_Type == ANALOG)
    {
        tmpEle = m_ClockElement.value(Element_HourPointer);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            if(m_SavedTime.mHour != pTime.mHour)
            {
                //>@用来解决当从60变为1时出现的指针跳变
                if(pTime.mHour == 0)
                {
                    m_PointerAngle[0][Last] = 6.0 * (-1 + pTime.mMinute / 60.0) + 180;
                    m_PointerAngle[0][Current] = 6.0 * pTime.mMinute / 60.0 + 180;
                }
                else
                {
                    m_PointerAngle[0][Last] = m_PointerAngle[0][Current];
                    m_PointerAngle[0][Current] = 30.0 * ((pTime.mHour + pTime.mMinute / 60.0)) + 180;
                }
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
        //>@Minute
        tmpEle = m_ClockElement.value(Element_MinutePointer);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            if(m_SavedTime.mMinute != pTime.mMinute)
            {
                //>@用来解决当从60变为1时出现的指针跳变
                if(pTime.mMinute == 0)
                {
                    m_PointerAngle[1][Last] = 6.0 * (-1 + pTime.mSecond / 60.0) + 180;
                    m_PointerAngle[1][Current] = 6.0 * pTime.mSecond / 60.0 + 180;
                }
                else
                {
                    m_PointerAngle[1][Last] = m_PointerAngle[1][Current];
                    m_PointerAngle[1][Current] = 6.0 * (pTime.mMinute + pTime.mSecond / 60.0) + 180;
                }
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
        //>@Second
        tmpEle = m_ClockElement.value(Element_SecondPointer);
        if(tmpEle != 0)
        {
            tmpEle->mAnimateFlag = false;
            if(m_SavedTime.mSecond != pTime.mSecond)
            {
                //>@用来解决当从60变为1时出现的指针跳变
                if(pTime.mSecond == 0)
                {
                    m_PointerAngle[2][Last] = -6.0 + 180;
                    m_PointerAngle[2][Current] = 0 + 180;
                }
                else
                {
                    m_PointerAngle[2][Last] = m_PointerAngle[2][Current];
                    m_PointerAngle[2][Current] = 6.0 * (pTime.mSecond) + 180;
                }
                tmpEle->mAnimateFlag = true;
                flag = true;
            }
        }
    }

    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    if(flag)
    {
        if(m_SavedTime.fromClock(pTime) == false)
        {
            IDE_DEBUG("DateTime is invalid,please check it!");
        }
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = OPERATE_NONE;
    //>@执行STEP0中的内容
    OperateStep0();
}

DesignClock::DesignClock(QGraphicsItem *parent):
    GraphicsClock(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("UPDATE"));
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignClock::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        for(int i=0;i<tmpList.count();i++)
        {
            QAction* tmpAction = tmpList.at(i);
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignClock::ExecAction(QAction *pAction)
{
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    updateEffect(CLOCK_INFO(tmpDateTime.date().year(),
                            tmpDateTime.date().month(),
                            tmpDateTime.date().day(),
                            tmpDateTime.time().hour(),
                            tmpDateTime.time().minute(),
                            tmpDateTime.time().second()));
}

QSize DesignClock::GetRcSize(QString pRcName)
{
    CLOCKE_ELEMENT tmpEleType;
    int tmpKey = GetKey(pRcName);
    if(tmpKey >= 0 && tmpKey <= 9)
        tmpEleType = Element_Year0;
    else if(tmpKey >= 10 && tmpKey <= 16)
        tmpEleType = Element_Week;
    else if(tmpKey == 17)
        tmpEleType = Element_Colon0;
    else if(tmpKey == 18)
        tmpEleType = Element_Minus0;
    else if(tmpKey == 19)
        tmpEleType = Element_HourPointer;
    else if(tmpKey == 20)
        tmpEleType = Element_MinutePointer;
    else if(tmpKey == 21)
        tmpEleType = Element_SecondPointer;
    else if(tmpKey == 22)
        tmpEleType = Element_Dot;
    else
        return size().toSize();

    CLOCK_ELEMENTINFO *tmpEleInfo = m_ClockElement.value(tmpEleType);
    if(tmpEleInfo == 0)
        return size().toSize();
    return tmpEleInfo->mRect.size().toSize();
}
