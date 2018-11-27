#include "graphicsfloor.h"
#include "graphicsscene.h"
#include "logicthememanager.h"

GraphicsFloor::GraphicsFloor(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    m_IsDiscreted = false;
    m_FloorAlign = Qt::AlignCenter;
}

//>@323248 -> 323249
MAP_INFO* GraphicsFloor::LoadMap(QDomElement pElement)
{
    if(pElement.isNull())
        return (MAP_INFO *)0;
    quint32 tmpSrcNum = GetKey(pElement.tagName());
    quint32 tmpHundred = (tmpSrcNum/10000);
    quint32 tmpTen = (tmpSrcNum%10000/100);
    quint32 tmpSignal = (tmpSrcNum%100);
    int tmpKey = (tmpHundred<<16)+(tmpTen<<8)+tmpSignal;
    MAP_INFO *tmpInfo = m_MapList.value(tmpKey);
    if(!tmpInfo)
    {
        tmpInfo = new MAP_INFO;
        m_MapList.insert(tmpKey, tmpInfo);
    }
    bool ret = false;
    quint32 tmpMap;
    quint32 tmpPara;
    if(pElement.text().count() > 2)
    {
        tmpMap = pElement.text().toUInt(&ret, 10);
        tmpPara = 0;
    }
    else if(pElement.text().count() <= 2)
    {
        tmpMap = pElement.text().toUInt(&ret, 10);
        tmpPara = 1;
    }
    else
    {
        MAP_INFO *tmpDelInfo = m_MapList.take(tmpKey);
        if(tmpDelInfo)
            delete tmpDelInfo;
        return (MAP_INFO *)0;
    }
    if(ret)
    {
        tmpHundred = (tmpMap/10000);
        tmpTen = (tmpMap%10000/100);
        tmpSignal = (tmpMap%100);
        tmpInfo->mMap = (tmpHundred<<16)+(tmpTen<<8)+tmpSignal;
        tmpInfo->mParaHash.insert(0, tmpPara);  //>@zai
    }
    else
    {
        tmpInfo->mMap = -1;
        tmpInfo->mParaHash.insert(0, tmpPara);
    }
    ParserParameter(tmpInfo, pElement.attribute(D_PARAMETER));
    return tmpInfo;
}

bool GraphicsFloor::InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent)
{
    if(pLabel.compare("Pattern", Qt::CaseInsensitive) == 0)
    {
        m_IsDiscreted = (bool)(!pContent.compare("Discrete", Qt::CaseInsensitive));
    }
    else if(pLabel.compare("Align", Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare("Center", Qt::CaseInsensitive) == 0)
            m_FloorAlign = Qt::AlignCenter;
        else if(pContent.compare("Left", Qt::CaseInsensitive) == 0)
            m_FloorAlign = Qt::AlignLeft;
        else if(pContent.compare("Right", Qt::CaseInsensitive) == 0)
            m_FloorAlign = Qt::AlignRight;
    }
    else if(pLabel.compare("Font", Qt::CaseInsensitive) == 0)
    {
        m_FloorFont.setFamily(pContent);
    }
    else if(pLabel.compare("Weigth", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("DemiBold", Qt::CaseInsensitive))
            m_FloorFont.setWeight(QFont::DemiBold);
        else if(!pContent.compare("Bold", Qt::CaseInsensitive))
            m_FloorFont.setWeight(QFont::Bold);
        else
            m_FloorFont.setWeight(QFont::Normal);
    }
    else if(pLabel.compare("Color", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("red", Qt::CaseInsensitive))
            m_FloorColor = Qt::red;
        else if(!pContent.compare("blue", Qt::CaseInsensitive))
            m_FloorColor = Qt::blue;
        else if(!pContent.compare("white", Qt::CaseInsensitive))
            m_FloorColor = Qt::white;
        else
            m_FloorColor = Qt::black;
    }
    return true;
}

bool GraphicsFloor::Start()
{
    bool ret = GraphicsOperate::Start();

    if(m_RcStyle)
    {
        if(m_RcStyle->mRcType == COMRC_TEXT)  //>@根据大小，计算字体大小
        {
            int fontsize=1;
            while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
            {
                int newfontsize;
                newfontsize = fontsize+1;
                m_FloorFont.setPointSize(newfontsize);
                QFontMetricsF fm(m_FloorFont);
                qreal pixelsWide = fm.width("8");
                qreal pixelsHigh = fm.height();
                if(pixelsWide*3 > size().width() || pixelsHigh > size().height())
                {
                    m_FloorFont.setPointSize(fontsize);
                    break;
                }
                fontsize = newfontsize;
            }
            m_FloorNumSize = size();
        }
#ifdef N329
        m_EffectRC.insert(BGRC, CreateRcInfo(QPixmap(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(size().toSize())));
#endif
    }
    m_PixmapPointer.mLastPointer.fill(Qt::transparent);
    m_PixmapPointer.mCurPointer.fill(Qt::transparent);
    m_FloorNumInfo = INVALIDRC;
    return true;
}

bool GraphicsFloor::PaintEffect(QPainter *p)
{
#ifndef D_SUPPORT_EFFECT
    return PaintFloor(m_FloorNumInfo, p);
#else
    return GraphicsComponent::PaintEffect(p);
#endif
}

bool GraphicsFloor::PaintFloor(FLOOR_INFO pFloorInfo, QPainter *p)
{
    if(!p)
        return false;
    if(pFloorInfo.mReserve == 1)
    {
        QString tmpFile = m_UiContainer.data()->m_UiManager->m_FileDir + QString("/Floor/Resource/Image/T%1.png").arg(pFloorInfo.toInt()&0x000000ff);
        DrawPixmap(p, QPixmap(tmpFile), rect());
    }
    else
    {
        if(m_RcStyle->mRcType == COMRC_IMAGE)
        {
            QRectF tmpComRect = rect();
            if(!m_IsDiscreted || (pFloorInfo.mTenBits==0 && pFloorInfo.mHundredBits==0))
            {
                m_FloorNumSize = QSizeF(tmpComRect.width(), tmpComRect.height());
            }
            else if(pFloorInfo.mHundredBits==0)
            {
                m_FloorNumSize = QSizeF(tmpComRect.width() / 2.0, tmpComRect.height());
            }
            else
            {
                m_FloorNumSize = QSizeF(tmpComRect.width() / 3.0, tmpComRect.height());
            }
            QRectF tmpRcRect[3];
            QPixmap tmpHundredRcPix = GetPixmap(pFloorInfo.mHundredBits, m_FloorNumSize);
            QPixmap tmpTenRcPix = GetPixmap(pFloorInfo.mTenBits, m_FloorNumSize);
            QPixmap tmpSingleRcPix = GetPixmap(pFloorInfo.mSingleBits, m_FloorNumSize);
            if(m_FloorAlign == Qt::AlignCenter)
            {
    #ifdef N329
                QList<QPixmap> tmpPixList;
                if(!tmpHundredRcPix.isNull())
                    tmpPixList.append(tmpHundredRcPix);
                if(!tmpTenRcPix.isNull())
                    tmpPixList.append(tmpTenRcPix);
                if(!tmpSingleRcPix.isNull())
                    tmpPixList.append(tmpSingleRcPix);
                switch(tmpPixList.count())
                {
                    case 1:
                    {
                        tmpTenRcPix = tmpPixList.takeFirst();
                        tmpRcRect[Ten] = QRectF(tmpComRect.center()-QPointF(m_FloorNumSize.width()/2.0, m_FloorNumSize.height()/2.0), m_FloorNumSize);
                        break;
                    }
                    case 2:
                    {
                        tmpTenRcPix = tmpPixList.takeFirst();
                        tmpSingleRcPix = tmpPixList.takeFirst();
                        tmpRcRect[Ten] = QRectF(QPointF(m_FloorNumSize.width()/2.0, 0), m_FloorNumSize);
                        tmpRcRect[Single] = QRectF(QPointF(m_FloorNumSize.width()*3.0/2.0, 0), m_FloorNumSize);
                        break;
                    }
                    case 3:
                    {
                        tmpRcRect[Hundred] = QRectF(QPointF(0, 0), m_FloorNumSize);
                        tmpRcRect[Ten] = QRectF(QPointF(tmpRcRect[Hundred].right(), 0), m_FloorNumSize);
                        tmpRcRect[Single] = QRectF(QPointF(tmpRcRect[Ten].right(), 0), m_FloorNumSize);
                        break;
                    }
                    default:
                        return false;
                }
    #else
                if(tmpHundredRcPix.isNull())
                {
                    if(tmpTenRcPix.isNull())
                    {
                        if(tmpSingleRcPix.isNull())
                            return false;
                        tmpRcRect[Single] = QRectF(tmpComRect.center()-QPointF(m_FloorNumSize.width()/2.0, m_FloorNumSize.height()/2.0), m_FloorNumSize);
                    }
                    else
                    {
                        tmpRcRect[Ten] = QRectF(QPointF(m_FloorNumSize.width()/2.0, 0), m_FloorNumSize);
                        tmpRcRect[Single] = QRectF(QPointF(m_FloorNumSize.width()*3.0/2.0, 0), m_FloorNumSize);
                    }
                }
                else
                {
                    //>@考虑到百位有数据，但十个位没有数据的情况
                    if(tmpSingleRcPix.isNull() && tmpTenRcPix.isNull())
                    {
                        tmpRcRect[Hundred] = QRectF(QPointF(m_FloorNumSize.width(), 0), m_FloorNumSize);
                    }
                    else
                    {
                        tmpRcRect[Hundred] = QRectF(QPointF(0, 0), m_FloorNumSize);
                        tmpRcRect[Ten] = QRectF(QPointF(tmpRcRect[Hundred].right(), 0), m_FloorNumSize);
                        tmpRcRect[Single] = QRectF(QPointF(tmpRcRect[Ten].right(), 0), m_FloorNumSize);
                    }
                }
    #endif
            }
            else if(m_FloorAlign == Qt::AlignLeft)
            {
                if(tmpHundredRcPix.isNull())
                {
                    if(tmpTenRcPix.isNull())
                    {
                        if(tmpSingleRcPix.isNull())
                            return false;
                        tmpRcRect[Single] = QRectF(QPointF(0, 0), m_FloorNumSize);
                    }
                    else
                    {
                        tmpRcRect[Ten] = QRectF(QPointF(0, 0), m_FloorNumSize);
                        tmpRcRect[Single] = QRectF(QPointF(tmpRcRect[Ten].right(), 0), m_FloorNumSize);
                    }
                }
                else
                {
                    tmpRcRect[Hundred] = QRectF(QPointF(0, 0), m_FloorNumSize);
                    tmpRcRect[Ten] = QRectF(QPointF(tmpRcRect[Hundred].right(), 0), m_FloorNumSize);
                    tmpRcRect[Single] = QRectF(QPointF(tmpRcRect[Ten].right(), 0), m_FloorNumSize);
                }
            }
            else
            {
                tmpRcRect[Hundred] = QRectF(QPointF(0, 0), m_FloorNumSize);
                tmpRcRect[Ten] = QRectF(QPointF(tmpRcRect[Hundred].right(), 0), m_FloorNumSize);
                tmpRcRect[Single] = QRectF(QPointF(tmpRcRect[Ten].right(), 0), m_FloorNumSize);
            }
            if(!tmpRcRect[Hundred].isNull())
                DrawPixmap(p, tmpHundredRcPix, tmpRcRect[Hundred]);
            if(!tmpRcRect[Ten].isNull())
                DrawPixmap(p, tmpTenRcPix, tmpRcRect[Ten]);
            if(!tmpRcRect[Single].isNull())
                DrawPixmap(p, tmpSingleRcPix, tmpRcRect[Single]);
        }
        else if(m_RcStyle->mRcType == COMRC_TEXT)
        {
            char *tmpText = (char *)malloc(4);
            if(pFloorInfo.mHundredBits < 0x20 || pFloorInfo.mHundredBits > 0x71)
                pFloorInfo.mHundredBits = 0x20;
            if(pFloorInfo.mTenBits < 0x20 || pFloorInfo.mTenBits > 0x71)
                pFloorInfo.mTenBits = 0x20;
            if(pFloorInfo.mSingleBits < 0x20 || pFloorInfo.mSingleBits > 0x71)
                pFloorInfo.mSingleBits = 0x20;
            tmpText[0] = pFloorInfo.mHundredBits;
            tmpText[1] = pFloorInfo.mTenBits;
            tmpText[2] = pFloorInfo.mSingleBits;
            tmpText[3] = '\0';
            p->setFont(m_FloorFont);
            p->setPen(m_FloorColor);
            p->drawText(QRectF(QPointF(0,0), m_FloorNumSize), m_FloorAlign, tmpText);
            free(tmpText);
        }
        else
            return false;
    }
    return true;
}

QPixmap GraphicsFloor::GenerateFloorImage(FLOOR_INFO pFloorInfo)
{
    QPixmap tmpPixmap;
    if(m_RcStyle->mRcType == COMRC_IMAGE)
    {
        if(m_IsDiscreted)
        {
            //>@准备资源
#ifdef N329
            QImage tmpImage = GetPixmap(BGRC).toImage();
            if(tmpImage.isNull())
                return false;
#else
            QImage tmpImage(size().width(), size().height(), QImage::Format_ARGB32_Premultiplied);
            if(tmpImage.isNull())
                return false;
            tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
            QPainter painter(&tmpImage);
#ifdef PIXQUALITY
            painter.setRenderHint(QPainter::Antialiasing);
#endif
            //>@绘制文字
            if(PaintFloor(pFloorInfo, &painter))
                tmpPixmap = QPixmap::fromImage(tmpImage);
        }
        else
        {
            int tmpValue = pFloorInfo.toIndex();
            IDE_TRACE_INT(tmpValue);
            tmpPixmap = GetPixmap(tmpValue, size());
            IDE_TRACE_INT(tmpPixmap.isNull());
        }
    }
    else if(m_RcStyle->mRcType == COMRC_TEXT)
    {
        //>@准备资源
#ifdef N329
        QImage tmpImage = GetPixmap(BGRC).toImage();
        if(tmpImage.isNull())
            return false;
#else
        QImage tmpImage(size().width(), size().height(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return false;
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter painter(&tmpImage);
#ifdef PIXQUALITY
        painter.setRenderHint(QPainter::Antialiasing);
#endif
        if(PaintFloor(pFloorInfo, &painter))
            tmpPixmap = QPixmap::fromImage(tmpImage);
    }
    return tmpPixmap;
}

void GraphicsFloor::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsOperate::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            bool ok = false;
            quint32 index = (quint32)(pPara.toUInt(&ok))&0x00ffffff;
            if(!ok)
                return;
            FLOOR_INFO tmpFloorInfo;
            quint32 tmpPara = 0;
            //>@如果遇到NEXTRC，则默认播放下一张
            if(index == NEXTRC)
            {
                tmpFloorInfo = m_FloorNumInfo;
                tmpFloorInfo.Rise();
                index = tmpFloorInfo.toInt();
            }
            if(m_MapList.contains(index))
            {
                MAP_INFO *tmpInfo = m_MapList.value(index);
                if(tmpInfo)
                {
                    tmpPara = tmpInfo->mParaHash.value(0).toUInt();
                    index = tmpInfo->mMap;
                }
                IDE_TRACE_INT(index);
            }
            tmpFloorInfo = index;
            tmpFloorInfo.mReserve = tmpPara;
            updateEffect(tmpFloorInfo, OPERATE_HALT);
            break;
        }
        default:
            break;
    }
}

void GraphicsFloor::updateEffect(FLOOR_INFO pFloorInfo, AREA_OPERATE pOperate)
{
    bool tmpFloorNumChange = false;
    bool tmpOperateChange = false;
    //>@比较楼层是否改变
    if(pFloorInfo.mHundredBits != m_FloorNumInfo.mHundredBits ||
       pFloorInfo.mTenBits != m_FloorNumInfo.mTenBits ||
       pFloorInfo.mSingleBits != m_FloorNumInfo.mSingleBits ||
       pFloorInfo.mReserve != m_FloorNumInfo.mReserve)
    {
        tmpFloorNumChange = true;
    }
#ifndef D_SUPPORT_EFFECT
    if(tmpFloorNumChange)
    {
        m_FloorNumInfo = pFloorInfo;
        update();
    }
#else
    //>@比较操作是否改变
    if(pOperate != m_OperateInfo[STEP1].mOperate)
        tmpOperateChange = true;
    if(tmpFloorNumChange == false && tmpOperateChange == false)
        return;
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    //>@准备资源
    if(tmpFloorNumChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
        m_PixmapPointer.mCurPointer = GenerateFloorImage(pFloorInfo);
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = pOperate;
    m_FloorNumInfo = pFloorInfo;
    OperateStep0();
#endif
}

DesignFloor::DesignFloor(QGraphicsItem *parent) :
    GraphicsFloor(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("Number"));
    m_ActionGroup->addAction(QString("012"));
    m_ActionGroup->addAction(QString("123"));
    m_ActionGroup->addAction(QString("234"));
    m_ActionGroup->addAction(QString("345"));
    m_ActionGroup->addAction(QString("456"));
    m_ActionGroup->addAction(QString("567"));
    m_ActionGroup->addAction(QString("678"));
    m_ActionGroup->addAction(QString("789"));
    tmpDefault->setChecked(true);
}

QSize DesignFloor::GetRcSize(QString pRcName)
{
    int index = GetKey(pRcName);
    if(index == BGRC)
        return size().toSize();
    else
        return QSize(size().width()/3.0, size().height());
    return size().toSize();
}

QList<QAction*> DesignFloor::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        bool hasNumber = false;
        bool hasLetter = false;
        bool hasMixed = false;
        QList<int> tmpKeyList = m_EffectRC.keys();
        for(int i=0;i<tmpKeyList.count();i++)
        {
            if(tmpKeyList.at(i)>=0 && tmpKeyList.at(i)<=9)
            {
                hasNumber = true;
                break;
            }
        }
        for(int i=0;i<tmpKeyList.count();i++)
        {
            if(tmpKeyList.at(i)>=10 && tmpKeyList.at(i)<=35)
            {
                hasLetter = true;
                break;
            }
        }
        if(hasNumber && hasLetter)
            hasMixed = true;

        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setDisabled(true);
            QString tmpString = tmpAction->text();
            if(tmpString.contains("Number") && (hasNumber || hasMixed))
            {
                tmpAction->setEnabled(true);
            }
            else if(tmpString.contains("Letter") && (hasLetter || hasMixed))
            {
                tmpAction->setEnabled(true);
            }
            else if(tmpString.contains("Mixed") && hasMixed)
            {
                tmpAction->setEnabled(true);
            }
        }
    }
    return tmpList;
}

void DesignFloor::ExecAction(QAction *pAction)
{
    updateEffect(OPERATE_CHANGE, pAction->text());
}




