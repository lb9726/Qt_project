#include "graphicsbutton.h"

GraphicsButton::GraphicsButton(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
}

bool GraphicsButton::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;
    m_ButtonState = PIC_DARK;
    emit sUpdateCom(0);
    return true;
}

bool GraphicsButton::PaintEffect(QPainter *p)
{
    if(m_AreaAnimate == 0)
    {
        //>@首先绘制按钮底图
        DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(PIC_DARK):(PIC_LIGHT)), rect());
        //>@最终绘制文字
        DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(TEXT_DARK):(TEXT_LIGHT)), rect());
        return false;
    }
    switch(m_AreaAnimate->mEffectType)
    {
        case EffectTypeSpread:
        {
            //>@首先绘制按钮底图
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(PIC_DARK):(PIC_LIGHT)), rect());
            //>@其次特效绘图
            m_PixmapPointer.mLastPointer = GetPixmap(PIC_EFFECT);
            m_PixmapPointer.mCurPointer = m_PixmapPointer.mLastPointer;
            Paint2D_Spread(p, rect());
            //>@最终绘制文字
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(TEXT_DARK):(TEXT_LIGHT)), rect());
            break;
        }
        case EffectTypeWheel:
        {
            //>@首先绘制按钮底图
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(PIC_DARK):(PIC_LIGHT)), rect());
            //>@其次特效绘图
            m_PixmapPointer.mLastPointer = GetPixmap(PIC_EFFECT);
            m_PixmapPointer.mCurPointer = m_PixmapPointer.mLastPointer;
            Paint2D_Wheel(p, rect());
            //>@最终绘制文字
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(TEXT_DARK):(TEXT_LIGHT)), rect());
            break;
        }
        case EffectTypeRotate:
        {
            QImage tmpImage(size().toSize(), QImage::Format_ARGB32_Premultiplied);
            if(tmpImage.isNull())
                return false;
            tmpImage.fill(Qt::transparent);
            QPainter painter(&tmpImage);
            //>@首先绘制按钮底图
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(PIC_DARK):(PIC_LIGHT)), rect());
            //>@绘制文字
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(TEXT_DARK):(TEXT_LIGHT)), rect());
            painter.end();
            //>@最终特效绘图
            m_PixmapPointer.mCurPointer = m_PixmapPointer.mLastPointer = QPixmap::fromImage(tmpImage);
            Paint2D_Rotate(p, rect());
            break;
        }
        case EffectTypeSlipCycle:
        {
            //>@首先绘制按钮底图
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(PIC_DARK):(PIC_LIGHT)), rect());
            //>@其次特效绘图
            m_PixmapPointer.mLastPointer = GetPixmap(PIC_EFFECT);
            m_PixmapPointer.mCurPointer = m_PixmapPointer.mLastPointer;
            Paint2D_Slipcycle(p, rect());
            //>@最终绘制文字
            DrawPixmap(p, GetPixmap((m_ButtonState == PIC_DARK)?(TEXT_DARK):(TEXT_LIGHT)), rect());
            break;
        }
        default:
            Paint2D_None(p, rect());
            break;
    }
    return true;
}

void GraphicsButton::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            if(m_ButtonState == PIC_LIGHT)
            {
                updateEffect(OPERATE_KEYRELEASE, pPara);
                return;
            }
            m_ButtonState = PIC_LIGHT;
            emit reportEffect(OPERATE_KEYPRESS, QVariant(GetCurType()));
            break;
        }
        case OPERATE_KEYLIGHT:
        {
            if(m_ButtonState == PIC_LIGHT)
                return;
            pOperate = OPERATE_KEYPRESS;
            m_ButtonState = PIC_LIGHT;
            break;
        }
        case OPERATE_KEYRELEASE:
        {
            if(m_ButtonState == PIC_DARK)
                return;
            if(m_Animator.state() == QTimeLine::Running)
                return;
            m_ButtonState = PIC_DARK;
            emit reportEffect(OPERATE_KEYCANCEL, QVariant(GetCurType()));
            break;
        }
        default:
            return;
    }
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    m_OperateInfo[STEP0].mValid = true;
    m_OperateInfo[STEP0].mOperate = pOperate;
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = OPERATE_NONE;
    //>@执行STEP0中的内容
    OperateStep0();
}

//#################################################################################
//#################################################################################

DesignButton::DesignButton(QGraphicsItem *parent) :
    GraphicsButton(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("None"));
    m_ActionGroup->addAction(tr("Press"));
    m_ActionGroup->addAction(tr("Release"));
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignButton::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignButton::ExecAction(QAction *pAction)
{
    if(pAction->text().compare("Press")==0)
        updateEffect(OPERATE_KEYPRESS, QVariant());
    else if(pAction->text().compare("Release")==0)
        updateEffect(OPERATE_KEYRELEASE, QVariant());
}

//#################################################################################
//#################################################################################

GraphicsFloorButton::GraphicsFloorButton(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    mRow = mColunm = mCount = 0;
    mWidth = mHeight = mTextWidth = mTextHeight = 0;
    mRSpace = mCSpace = mItemLSpace = mItemRSpace = 0;
}

bool GraphicsFloorButton::InitSubEffectPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent)
{
    if(pLabel.compare(PARA_PATTERN, Qt::CaseInsensitive) == 0)
    {
        if(pContent == QString("Grid"))
            m_Pattern = PTN_FB_GRID;
        else if(pContent == QString("Ellipse"))
            m_Pattern = PTN_FB_ELLIPSE;
        else if(pContent == QString("Guide"))
            m_Pattern = PTN_FB_GUIDE;
    }
    else if(pLabel.compare(PARA_RSPACE, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            mRSpace = dec;
    }
    else if(pLabel.compare(PARA_CSPACE, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            mCSpace = dec;
    }
    else if(pLabel.compare(PARA_AMOUNT, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            mCount = dec;
    }
    else if(pLabel.compare(PARA_ROW, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            mRow = dec;
    }
    else if(pLabel.compare(PARA_COLUMN, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            mColunm = dec;
    }
    return true;
}

void GraphicsFloorButton::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_ADD:
        {
            int tmpNewFloor = pPara.toInt();
            FloorBtnInfo* tmpFloor = m_FloorBtnInfoList.at(tmpNewFloor);
            if(tmpFloor && tmpFloor->mButtonItem)
                tmpFloor->mButtonItem->updateEffect(OPERATE_KEYLIGHT, QVariant());
            break;
        }
        case OPERATE_DEL:
        {
            int tmpDelFloor = pPara.toInt();
            FloorBtnInfo* tmpFloor = m_FloorBtnInfoList.at(tmpDelFloor);
            if(tmpFloor && tmpFloor->mButtonItem)
                tmpFloor->mButtonItem->updateEffect(OPERATE_KEYRELEASE, QVariant());
            break;
        }
        default:
            return;
    }
}

quint32 GraphicsFloorButton::GetMappedType(quint32 pType)
{
    return pType;
}

//>@Type->MappedType
bool GraphicsFloorButton::AddButton(quint32 pType, GraphicsButton *&pButton, QRectF pRect)
{
    if(pButton)
        pButton->deleteLater();
    pButton = new GraphicsButton(this);
    pButton->SetComGeometory(pRect);
    pButton->SetType(pType);
    //>@获取资源
    //>@映射信号到显示数字
    quint32 tmpMappedType = GetMappedType(pType);
    FLOOR_INFO tmpFloor(tmpMappedType);
    QHash<int, RC_INFO*> tmpRcList;
    tmpRcList.insert(PIC_DARK, m_EffectRC.value(PIC_DARK));
    tmpRcList.insert(PIC_LIGHT, m_EffectRC.value(PIC_LIGHT));
    tmpRcList.insert(PIC_EFFECT, m_EffectRC.value(PIC_EFFECT));
    tmpRcList.insert(TEXT_DARK, CreateRcInfo(GetBtnRc(pRect.size(), tmpFloor, false)));
    tmpRcList.insert(TEXT_LIGHT, CreateRcInfo(GetBtnRc(pRect.size(), tmpFloor, true)));
    pButton->InitComponent(tmpRcList, m_EffectGroup);
    //>@建立信号连接
    connect(pButton, SIGNAL(reportEffect(AREA_OPERATE,QVariant)), this, SIGNAL(reportEffect(AREA_OPERATE,QVariant)));
    return true;
}

bool GraphicsFloorButton::InitFloorBtnInfoList()
{
    ReleaseFloorBtnInfoList();
    switch(m_Pattern)
    {
        case PTN_FB_GRID:
        {
            qreal width = size().width();
            qreal height = size().height();
            if(mCount == 0 || mRow == 0 || mColunm == 0)
                return false;
            mWidth = width/(qreal)mColunm;
            mHeight = height/(qreal)mRow;
            mTextHeight = mHeight * 3.0 / 4.0;
            mTextWidth = mTextHeight * 3.0 / 5.0;
            int row = -1, column = -1;
            for(quint32 i=0;i<mCount;i++)
            {
                if(i%mColunm == 0)
                    row += 1;
                column = i - row*mColunm;
                FloorBtnInfo *tmpFloorBtnInfo = new FloorBtnInfo;
                tmpFloorBtnInfo->mRect = QRect(column*mWidth+mCSpace,
                                               row*mHeight+mRSpace,
                                               mWidth-2*(mCSpace),
                                               mHeight-2*(mRSpace));
                AddButton(i, tmpFloorBtnInfo->mButtonItem, tmpFloorBtnInfo->mRect);
                m_FloorBtnInfoList.append(tmpFloorBtnInfo);
            }
            break;
        }
        case PTN_FB_ELLIPSE:
        {
            qreal width = size().width();
            qreal height = size().height();
            if(mCount == 0)
                return false;
            qreal tmpAngelDiff = 2*PI/(qreal)mCount;
            qreal tmpBeginAngle = 0;  //>@从0度开始排列
            qreal tmpTwirlA = width / 3.0;   //>@取椭圆的长半径a为1/3的长度，b为1/3的长度
            qreal tmpTwirlB = height / 3.0;
            int halfcount = (int)((qreal)mCount / 2.0 - 1);
            mWidth = tmpTwirlA/(qreal)halfcount;
            mHeight = tmpTwirlB/(qreal)halfcount;
            mTextHeight = mHeight * 2.0 / 3.0;
            mTextWidth = mTextHeight * 3.0 / 5.0;
            QPointF tmpCentrePoint = QPointF(width/2.0, height/2.0);
            for(quint32 i=0;i<mCount;i++)
            {
                FloorBtnInfo *tmpFloorBtnInfo = new FloorBtnInfo;
                qreal tmpAngle = RegularAngle(tmpBeginAngle + tmpAngelDiff * i);
                QPointF tmpItemCentrePoint = tmpCentrePoint + QPointF(tmpTwirlA*qCos(tmpAngle), -tmpTwirlB*qSin(tmpAngle));
                tmpFloorBtnInfo->mRect = QRect(tmpItemCentrePoint.x() - mWidth/2.0 + mCSpace,
                                               tmpItemCentrePoint.y() - mHeight/2.0 + mRSpace,
                                               mWidth - 2*(mCSpace),
                                               mHeight - 2*(mRSpace));
                AddButton(i, tmpFloorBtnInfo->mButtonItem, tmpFloorBtnInfo->mRect);
                m_FloorBtnInfoList.append(tmpFloorBtnInfo);
            }
            break;
        }
        case PTN_FB_GUIDE:
        {
            break;
        }
        default:
            return false;
    }
    return true;
}

void GraphicsFloorButton::ReleaseFloorBtnInfoList()
{
    for(int i=m_FloorBtnInfoList.count()-1;i>=0;i--)
    {
        FloorBtnInfo *tmpFloorBtnInfo = m_FloorBtnInfoList.takeAt(i);
        if(tmpFloorBtnInfo == 0)
            continue;
        delete tmpFloorBtnInfo;
    }
}

int GraphicsFloorButton::PrepareBtnChar(char pAscii, bool pLight)
{
    if(pAscii >= '0' && pAscii <= '9')
        return pLight ? (pAscii-'0'+14) : (pAscii-'0');
    else if(pAscii >= 'A' && pAscii <= 'D')
        return pLight ? (pAscii-'A'+24) : (pAscii-'A'+10);
    return -1;
}

QPixmap GraphicsFloorButton::GetBtnRc(QSizeF pSize, FLOOR_INFO pFloorInfo, bool pLight)
{
    //>@准备资源
    QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@创建透明图层
    QPainter painter(&tmpImage);
    painter.translate(pSize.width()/2.0, pSize.height()/2.0);
    //>@绘制文字
    if(pFloorInfo.mHundredBits != 0xff)
    {
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mHundredBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(-mTextWidth*3.0/2.0, -mTextHeight/2.0, mTextWidth, mTextHeight));
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mTenBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(-mTextWidth/2.0, -mTextHeight/2.0, mTextWidth, mTextHeight));
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mSingleBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(mTextWidth/2.0, -mTextHeight/2.0, mTextWidth, mTextHeight));
    }
    else if(pFloorInfo.mTenBits != 0xff)//>@表示没有百位有十位
    {
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mTenBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(-mTextWidth, -mTextHeight/2.0, mTextWidth, mTextHeight));
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mSingleBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(0, -mTextHeight/2.0, mTextWidth, mTextHeight));
    }
    else if(pFloorInfo.mSingleBits != 0xff)   //>@3456
    {
        DrawPixmap(&painter,
                   GetPixmap(PrepareBtnChar(pFloorInfo.mSingleBits, pLight), QSizeF(mTextWidth,mTextHeight)),
                   QRectF(-mTextWidth/2.0, -mTextHeight/2.0, mTextWidth, mTextHeight));
    }
    painter.end();
    return QPixmap::fromImage(tmpImage);
}

bool GraphicsFloorButton::Start()
{
    if(GraphicsComponent::Start() == false)
        return false;
    InitFloorBtnInfoList();
    return true;
}


//#################################################################################
//#################################################################################


DesignFloorButton::DesignFloorButton(QGraphicsItem *parent) :
    GraphicsFloorButton(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("None"));
    m_ActionGroup->addAction(tr("Press"));
    m_ActionGroup->addAction(tr("Release"));
    tmpDefault->setChecked(true);
}

QSize DesignFloorButton::GetRcSize(QString pRcName)
{
    int tmpKey = GetKey(pRcName);
    if((tmpKey >= 0 && tmpKey <= 13) || (tmpKey >= 50 && tmpKey <= 63))
        return QSize(mTextWidth, mTextHeight);
    return QSize(mWidth-mCSpace*2, mHeight-mRSpace*2);
}

QList<QAction*> DesignFloorButton::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignFloorButton::ExecAction(QAction *pAction)
{
    if(pAction->text().compare("Press")==0)
    {
        int count = m_FloorBtnInfoList.count();
        FloorBtnInfo *tmpFloor = m_FloorBtnInfoList.at(qrand() % count);
        if(tmpFloor)
            tmpFloor->mButtonItem->updateEffect(OPERATE_KEYPRESS, QVariant());
    }
    else if(pAction->text().compare("Release")==0)
    {
        int count = m_FloorBtnInfoList.count();
        FloorBtnInfo *tmpFloor = m_FloorBtnInfoList.at(qrand() % count);
        if(tmpFloor)
            tmpFloor->mButtonItem->updateEffect(OPERATE_KEYRELEASE, QVariant());
    }
}

