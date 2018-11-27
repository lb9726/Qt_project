#include "widgetbase.h"
#include "effectbase.h"

RcBase::RcBase()
{
    m_ImgList.clear();
    m_ImgList.clear();
    m_EffectRender = 0;
}

RcBase::~RcBase()
{
    m_ImgList.clear();
    m_ImgList.clear();
}

void RcBase::LoadPixmap(int pKey, QPixmap pPixmap)
{
    if(pPixmap.isNull() == false)
    {
        m_ImgList.remove(pKey);
        m_ImgList.insert(pKey, pPixmap);
    }
}

QPixmap RcBase::LoadPixmap(QString pPixmapName, QSizeF pSize)
{
    if(pPixmapName.isEmpty())
        return QPixmap();
    FILEPATH_TYPE tmpType = GetFilePathType(pPixmapName);
    if(tmpType == PATH_TYPENONE || tmpType == PATH_TYPENETADDR)
        return QPixmap();
    RC_TYPE tmpRcType = GetRcType(pPixmapName);
    switch(tmpRcType)
    {
        case RC_BMP:
        case RC_JPG:
        case RC_PNG:
        {
            QPixmap tmpPixmap;
#ifdef RGB565  //>@主要是对图像进行像素调整
            QImage tmpImage;
            if(tmpImage.load(pPixmapName) == false)
                return QPixmap();
            if(ConvertImageTo565(tmpImage) == false)
                return QPixmap();
            tmpPixmap = QPixmap::fromImage(tmpImage);
#else
            if(tmpPixmap.load(pPixmapName) == false)
                return QPixmap();
#endif
            return ScalePixmap(tmpPixmap, pSize);
        }
        case RC_GIF:
        {
            break;
        }
        case RC_SVG:
        {
#ifdef  SURRPORT_SVG
            QSvgRenderer *tmpRenderer = new QSvgRenderer(pPixmapName);
            if(tmpRenderer->isValid() == false)
            {
                tmpRenderer->deleteLater();
                return QPixmap();
            }
            else
            {
                if(pSize.isEmpty())
                    return QPixmap();
                QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
                if(tmpImage.isNull())
                    return QPixmap();
                tmpImage.fill(Qt::transparent); //>@创建透明图层
                QPainter p(&tmpImage);
                tmpRenderer->render(&p);
                p.end();
#ifdef RGB565  //>@主要是对图像进行像素调整
                if(ConvertImageTo565(tmpImage) == false)
                    return QPixmap();
#endif
                return QPixmap::fromImage(tmpImage);
            }
#endif
            break;
        }
        case RC_FONT:
        {
            QPixmap tmpPixmap;

            return tmpPixmap;
        }
        default:break;
    }
    return QPixmap();
}

QPixmap RcBase::GetPixmap(int pKey, QSizeF pSize)
{
    QPixmap tmpPixmap;
#ifdef PIXBUFFER
    //>@首先查找m_EffectImage
    tmpPixmap = m_ImgList.value(pKey);
    if(tmpPixmap.isNull() == false)
    {
        return ScalePixmap(tmpPixmap, pSize);
    }
    //>@其次查找m_EffectPath，并将其插入m_EffectImage
    tmpPixmap = LoadPixmap(m_FileList.value(pKey), pSize);
    if(tmpPixmap.isNull() == false)
    {
        m_ImgList.remove(pKey);
        m_ImgList.insert(pKey, tmpPixmap);
        return tmpPixmap;
    }
#else
    tmpPixmap = LoadPixmap(m_FileList.value(pKey), pSize);
#endif
    return tmpPixmap;
}

bool RcBase::load(int pKey, QString pFile)
{
    if(!m_FileList.contains(pKey))
        m_FileList.insert(pKey, pFile);
    return false;
}

bool RcBase::load(int pKey, QPixmap pImg)
{
    if(!m_ImgList.contains(pKey))
        m_ImgList.insert(pKey, pImg);
    return false;
}

bool RcBase::load(QHash<int, QString> pFileList)
{
    m_FileList = pFileList;
    return true;
}

bool RcBase::load(QHash<int, QPixmap> pImgList)
{
    m_ImgList = pImgList;
    return true;
}

QPixmap RcBase::getNextImg()
{
    int tmpNextKey = -1;
    if(!m_FileList.isEmpty())
    {
        int tmpKey = m_ImgList.key(m_ImgPointer.mCurPointer);
        QList<int> tmpKeys = m_FileList.keys();
        qSort(tmpKeys.begin(), tmpKeys.end());
        int index = tmpKeys.indexOf(tmpKey);
        if((index+1) < tmpKeys.count())
            tmpNextKey = tmpKeys.at(index+1);
        else
            tmpNextKey = tmpKeys.first();
    }
    return GetPixmap(tmpNextKey);
}

QPixmap RcBase::getPrevImg()
{
    int tmpPreKey = -1;
    if(!m_FileList.isEmpty())
    {
        int tmpKey = m_ImgList.key(m_ImgPointer.mCurPointer);
        QList<int> tmpKeys = m_FileList.keys();
        qSort(tmpKeys.begin(), tmpKeys.end());
        int index = tmpKeys.indexOf(tmpKey);
        if((index-1) >= 0)
            tmpPreKey = tmpKeys.at(index-1);
        else
            tmpPreKey = tmpKeys.last();
    }
    return GetPixmap(tmpPreKey);
}

void RcBase::SetRender(AREA_OPERATE pOperate, EffectBase *pRender)
{
    if(pRender)
        m_EffectRenders.insert(pOperate, pRender);
    else
        m_EffectRenders.remove(pOperate);
}

bool RcBase::IsRendering(AREA_OPERATE pOperate)
{
    EffectBase *tmpRender = m_EffectRenders.value(pOperate);
    if(tmpRender)
        return (tmpRender->m_Animator.state() == QTimeLine::Running);
    return false;
}

bool RcBase::IsRendering()
{
    if(m_EffectRender)
        return (m_EffectRender->m_Animator.state() == QTimeLine::Running);
    return false;
}

void RcBase::Update()
{

}

void RcBase::StartRender()
{
    if(m_EffectRender)
    {
        m_EffectRender->setImage(0, m_ImgPointer.mCurPointer);
        m_EffectRender->setImage(1, m_ImgPointer.mLastPointer);
        m_EffectRender->Start();
    }
    else
        Update();
}

void RcBase::StopRender()
{
    if(m_EffectRender)
        m_EffectRender->Stop();
}

WidgetBase::WidgetBase(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f)
{
    m_Index = -1;
}

WidgetBase::~WidgetBase()
{

}

void WidgetBase::InitRC(QString pRcPath, bool pInititial)
{
    if(!pRcPath.isEmpty())
    {
        if(!pRcPath.endsWith('/'))
            pRcPath.append('/');
        pRcPath.replace("//", "/");
        QStringList tmpFileList = getAllFileInFloders(pRcPath);
        QString tmpFile;
        foreach(tmpFile, tmpFileList)
        {
            QString tmpFileName = getFileName(tmpFile);
            tmpFileName.remove("RC");
            if(tmpFileName.endsWith(".png", Qt::CaseInsensitive) ||
               tmpFileName.endsWith(".jpg", Qt::CaseInsensitive) ||
               tmpFileName.endsWith(".svg", Qt::CaseInsensitive) ||
               tmpFileName.endsWith(".bmp", Qt::CaseInsensitive))
            {
                int index = tmpFileName.indexOf('.');
                if(index)
                    tmpFileName = tmpFileName.mid(0, index);
                int tmpKey = GetKey(tmpFileName);
                if(tmpKey != -1)
                    load(tmpKey, tmpFile);
            }
        }
    }
    if(pInititial)
    {
        QList<int> tmpList = m_FileList.keys();
        if(!tmpList.isEmpty())
        {
            qSort(tmpList.begin(), tmpList.end());
            m_ImgPointer.mCurPointer = GetPixmap(tmpList.first());
        }
    }
}

void WidgetBase::paintEffect()
{
//    m_EffectRender = VAR_CAST<EffectBase*>(sender());
//    if(!m_EffectRender)
//        return;
    update();
}

void WidgetBase::paintEvent(QPaintEvent *e)
{
    if(IsRendering())
    {
        QPainter p(this);
        m_EffectRender->paint(&p, rect());
        p.end();
    }
    else
    {
        if(!m_ImgPointer.mCurPointer.isNull())
        {
            QPainter p(this);
            p.drawPixmap(rect(), m_ImgPointer.mCurPointer);
            p.end();
        }
        else  //>@必须添加如下代码，否则QSS不生效
        {
            QStyleOption opt;
            opt.init(this);
            QPainter p(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        }
    }
}

void WidgetBase::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            int tmpKey = pPara.toInt();
            switch(tmpKey)
            {
                case PREVRC:
                {
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
                    m_ImgPointer.mCurPointer = getPrevImg();
                    break;
                }
                case NEXTRC:
                {
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
                    m_ImgPointer.mCurPointer = getNextImg();
                    break;
                }
                default:
                {
                    if(tmpKey == m_Index)
                        return;
                    m_Index = tmpKey;
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
                    m_ImgPointer.mCurPointer = GetPixmap(tmpKey);
                    break;
                }
            }
            m_EffectRender = m_EffectRenders.value(pOperate);
            StartRender();
            break;
        }
        default:
            break;
    }
}


GWidgetBase::GWidgetBase(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
    QGraphicsWidget(parent, wFlags)
{

}

GWidgetBase::~GWidgetBase()
{

}

void GWidgetBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_EffectRender)
    {
        m_EffectRender->paint(painter, geometry());
    }
    else
    {
        QGraphicsWidget::paint(painter, option, widget);
    }
}

void GWidgetBase::paintEffect()
{
    m_EffectRender = VAR_CAST<EffectBase*>(sender());
    if(!m_EffectRender)
        return;
    update();
}

