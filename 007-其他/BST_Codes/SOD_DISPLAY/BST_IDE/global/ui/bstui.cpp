#include "bstui.h"
#ifdef IDE
#include "mainwindow.h"
#endif
#include "devfun.h"

void HideFocus(QWidget *pWidget)
{
    if(pWidget)
    {
        QList<QAbstractButton*> tmpList = pWidget->findChildren<QAbstractButton *>();
        QWidget *tmpWid = 0;
        foreach(tmpWid, tmpList)
        {
            if(tmpWid)
                tmpWid->setFocusPolicy(Qt::NoFocus);
        }
    }
}

BstUiBase::BstUiBase()
{
    m_IsChanged = false;
    m_LogicState = STATE_UNFOCUS;
}

void BstUiBase::FocusInEvent()
{
    if(m_LogicState == STATE_UNFOCUS)
    {
        m_LogicState = STATE_FOCUS;
    }
    PaintBorder();
}

void BstUiBase::FocusOutEvent()
{
    if(m_LogicState != STATE_UNFOCUS)
    {
        m_LogicState = STATE_UNFOCUS;
    }
    PaintBorder();
}

void BstUiBase::MouseEvent(QMouseEvent* e)
{
    QEvent::Type type = e->type();
    switch(m_LogicState)
    {
        //>@‘???”–μ?μ????? ?????μ??è????μ????ì????
        case STATE_UNFOCUS:
        {
            if(type == QEvent::MouseButtonPress)       //>@??????μ??￡ ?
            {
#ifdef IDE
                m_LogicState = STATE_EDITABLE;
#else
                m_LogicState = STATE_FOCUS;
#endif
                PaintBorder();
            }
            else if(type == QEvent::MouseButtonDblClick)       //>@÷±?”??????±????￡ ?
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
            break;
        }
        //>@‘?μ?μ????? ?????μ??è????
        case STATE_FOCUS:
        {
            if(type == QEvent::MouseButtonDblClick)       //>@à′????????±????￡ ?
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
#ifdef IDE
            else if(type == QEvent::MouseButtonPress)       //>@??????μ??￡ ?
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
#endif
            break;
        }
        //>@‘???±??????¨??μ????ì????
        case STATE_EDITABLE:
        {
            ProcessMouseEvent(e);
            break;
        }
        default:
            break;
    }
}

//>@????±￡?êμ?m_LogicState???¨??????????????????
void BstUiBase::KeyEvent(QKeyEvent *e)
{
    switch(m_LogicState)
    {
        //>@‘???”–μ?μ????? ?????μ??è????μ????ì????
        case STATE_UNFOCUS:
        {
            //>@do nothing
            break;
        }
        //>@‘?μ?μ????? ?????μ??è????￡¨μ???￡?Enter￡?????????±??????¨
        case STATE_FOCUS:
        {
            if(e->key() == Qt::Key_Enter)
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
            break;
        }
        //>@‘???±??????¨??μ????ì????￡¨μ???￡?ESC￡????à????±??????¨￡¨??????μ????¨
        case STATE_EDITABLE:
        {
            if(e->key() == Qt::Key_Escape)
            {
                m_LogicState = STATE_FOCUS;
                PaintBorder();
            }
            else
                ProcessKeyEvent(e);
            break;
        }
        default:
            break;
    }
    IDE_TRACE_INT(m_LogicState);
}

void BstUiBase::WheelEvent(QWheelEvent* e)
{
    switch(m_LogicState)
    {
        //>@‘???”–μ?μ????? ?????μ??è????μ????ì????
        case STATE_UNFOCUS:
        {
            //>@do nothing
            break;
        }
        //>@‘?μ?μ????? ?????μ??è????
        case STATE_FOCUS:
        {
            //>@do nothing
            break;
        }
        //>@‘???±??????¨??μ????ì????
        case STATE_EDITABLE:
        {
            ProcessWheelEvent(e);
            break;
        }
        default:
            break;
    }
}

void BstUiBase::ProcessMouseEvent(QMouseEvent* e)
{

}

void BstUiBase::ProcessKeyEvent(QKeyEvent *e)
{

}

void BstUiBase::ProcessWheelEvent(QWheelEvent* e)
{

}

bool BstUiBase::isFocus()
{
    return false;
}

//>@???????¨???ê÷????¨???¨??μ?StyleSheet
void BstUiBase::PaintBorder()
{

}

Label::Label(QString text, QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_DeleteOnClose);
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

    setFixedHeight(25);
    setFixedWidth(100);
    setFocusPolicy(Qt::NoFocus);
    SetFocus(false);

    SetText(text);
}

void Label::SetText(QString text)
{
    m_Content = text;
//    QFontMetricsF fm(m_Font);
//    qreal pixelsWide = fm.width(m_Content);
//    if(pixelsWide > width())
//        setFixedWidth(pixelsWide+6);
    update();
}

void Label::SetFocus(bool falg)
{
    if(falg)
    {
        m_Color = Qt::red;
        m_Font = QFont("Arial", 13, QFont::Bold);
    }
    else
    {
        m_Color = Qt::white;
        m_Font = QFont("Arial", 12, QFont::Bold);
    }
}

void Label::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setFont(m_Font);
    p.setBrush(m_Color);
    p.setPen(m_Color);
    p.drawText(rect(), Qt::AlignRight|Qt::AlignVCenter, m_Content+QString(":"));
}

TipLabel::TipLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent)
{
    mAnimateEn = false;
}

TipLabel::TipLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent)
{
    mAnimateEn = false;
    connect(&mTimeLine, SIGNAL(frameChanged(int)), this ,SLOT(slot_FrameChanged(int)));
    connect(&mTimeLine, SIGNAL(stateChanged(QTimeLine::State)), this, SLOT(slot_StateChanged(QTimeLine::State)));
}

TipLabel::~TipLabel()
{
    if(mTimeLine.state() != QTimeLine::NotRunning)
        mTimeLine.stop();
}

void TipLabel::SetText(QString pContent)
{
    mContent = pContent;
    int size = mContent.size();
    mTimeLine.setFrameRange(0, size);
    mTimeLine.setDuration(size*50);
    mTimeLine.setCurveShape(QTimeLine::LinearCurve);
    mTimeLine.setLoopCount(0);
    mTimeLine.start();
}

void TipLabel::SetAnimEnable(bool pEnable)
{
    mAnimateEn = pEnable;
    if(mAnimateEn)
    {
        if(mTimeLine.state() == QTimeLine::NotRunning)
        {
            SetText(mContent);
        }
    }
    else
    {
        if(mTimeLine.state() != QTimeLine::NotRunning)
            mTimeLine.stop();
        setText(mContent);
    }
}

void TipLabel::slot_FrameChanged(int pCount)
{
    QFontMetrics fm(font());
    int tmpWidth0 = fm.averageCharWidth();
    int tmpWidth1 = fm.width(mContent);
    int tmpWidth2 = width();
    int maxCount = tmpWidth2 / tmpWidth0;
    int contentCount = mContent.count();
    QString tmpBuffer;
    if(pCount >= contentCount)
    {
    }
    else
    {

    }
}

void TipLabel::slot_StateChanged(QTimeLine::State pState)
{
    setText(mContent);
}

gifplayer::gifplayer(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("background-color: transparent;");
    setScaledContents(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_GifMovie = 0;

    m_Repeat = false;
    m_GifMovie = new QMovie;
    connect(m_GifMovie, SIGNAL(finished()), this, SLOT(slot_Finished()));
    connect(m_GifMovie, SIGNAL(error(QImageReader::ImageReaderError)), this, SLOT(slot_Error(QImageReader::ImageReaderError)));
    setMovie(m_GifMovie);
}

gifplayer::gifplayer(QString pGifFile, bool pRepeat, QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("background-color: transparent;");
    setScaledContents(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_Repeat = false;
    m_GifMovie = new QMovie;
    connect(m_GifMovie, SIGNAL(finished()), this, SLOT(slot_Finished()));
    connect(m_GifMovie, SIGNAL(error(QImageReader::ImageReaderError)), this, SLOT(slot_Error(QImageReader::ImageReaderError)));
    setMovie(m_GifMovie);

    Play(pGifFile, pRepeat);
}

void gifplayer::Play(QString pGifFile, bool pRepeat)
{
    if(!QFile::exists(pGifFile))
        return;
    if(m_GifMovie->state() != QMovie::NotRunning)
        m_GifMovie->stop();
    m_Repeat = pRepeat;
    m_GifMovie->setFileName(pGifFile);
    m_GifMovie->start();
    Play();
}

void gifplayer::Play()
{
    if(m_GifMovie)
    {
        show();
        m_GifMovie->start();
    }
}

void gifplayer::Stop()
{
    if(m_GifMovie)
    {
        hide();
        m_GifMovie->stop();
    }
}

void gifplayer::slot_Finished()
{
    if(m_Repeat)
        m_GifMovie->start();
    else
        Stop();
    IDE_TRACE();
}

void gifplayer::slot_Error(QImageReader::ImageReaderError pErr)
{
    Stop();
    IDE_TRACE_INT(pErr);
}


PreviewLabel::PreviewLabel(QWidget *parent) :
    QLabel(parent)
{
    setScaledContents(true);
    m_GifMovie = 0;
#ifdef SURRPORT_SVG
    m_SvgRender = 0;
#endif
    m_MediaProcess = 0;
}

PreviewLabel::~PreviewLabel()
{
    Release();
}

void PreviewLabel::resizeEvent(QResizeEvent *e)
{
    emit UpdatePreview();
}

void PreviewLabel::Release()
{
    if(m_GifMovie)
    {
        m_GifMovie->deleteLater();
        m_GifMovie = 0;
    }
#ifdef SURRPORT_SVG
    if(m_SvgRender)
    {
        m_SvgRender->deleteLater();
        m_SvgRender = 0;
    }
#endif
    if(m_MediaProcess)
    {
        m_MediaProcess->deleteLater();
        m_MediaProcess = 0;
    }
}

void PreviewLabel::Preview(QString pFile)
{
    RC_TYPE tmpRcType = getRcType(pFile);
    switch(tmpRcType)
    {
        case RC_BMP:
        case RC_JPG:
        case RC_PNG:
        {
            ShowPixmap(pFile);
            break;
        }
        case RC_GIF:
        {
            ShowGIF(pFile);
            break;
        }
        case RC_SVG:
        {
            ShowSVG(pFile);
            break;
        }
        case RC_QML:
        {
            ShowQML(pFile);
            break;
        }
        case RC_FONT:
        {
            break;
        }
        case RC_MP3:
        case RC_WAV:
        case RC_MP4:
        case RC_AVI:
        case RC_RMVB:
        case RC_FLV:
        {
            PlayMedia(pFile);
            break;
        }
        default:
            break;
    }
}

void PreviewLabel::ShowPixmap(QString pFilePath)
{
    Release();
    setPixmap(QPixmap(pFilePath));
}

void PreviewLabel::ShowGIF(QString pFilePath)
{
    Release();
    if(!m_GifMovie)
    {
        m_GifMovie = new QMovie(this);
        setMovie(m_GifMovie);
    }
    if(!QFile::exists(pFilePath))
        return;
    if(m_GifMovie->state() != QMovie::NotRunning)
        m_GifMovie->stop();
    m_GifMovie->setFileName(pFilePath);
    m_GifMovie->start();
}

void PreviewLabel::ShowSVG(QString pFilePath)
{
#ifdef SURRPORT_SVG
    Release();
    if(m_SvgRender)
        m_SvgRender->deleteLater();
    m_SvgRender = new QSvgRenderer(pFilePath);
    if(m_SvgRender->isValid())
    {
        QPainter p(this);
        m_SvgRender->render(&p);
        p.end();
    }
#endif
}

void PreviewLabel::ShowQML(QString pFilePath)
{
    Release();
}

void PreviewLabel::PlayMedia(QString pFilePath)
{
    Release();
    if(!m_MediaProcess)
        m_MediaProcess = new QProcess;
    FILEPATH_TYPE tmpFileType = getRcFileType(pFilePath);
    if(tmpFileType == PATH_TYPENETADDR)
    {
        //>@????μ±???????? ”?μ
        unsigned long id = (unsigned long)winId();
        QString tmpCmd = QString("%1 %2 -nocache -slave -wid %3 &").arg(D_MPLAYER_PATH).arg(pFilePath).arg(id);
        m_MediaProcess->start(tmpCmd);
    }
    else
    {
        RC_TYPE tmpRcType = getRcType(pFilePath);
        if(tmpRcType >= RC_MP4)
        {
            unsigned long id = (unsigned long)winId();
            QString tmpCmd = QString("%1 %2 -framedrop -wid %3 &").arg(D_MPLAYER_PATH).arg(pFilePath).arg(id);
            m_MediaProcess->start(tmpCmd);
        }
    }
}

ToolContainer::ToolContainer(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setWindowFlags(Qt::Window);
}

void ToolContainer::keyPressEvent(QKeyEvent *event)
{
   switch(event->key())
   {
   case Qt::Key_Up:
       focusNextChild();
       break;
   case Qt::Key_Down:
       focusPreviousChild();
       break;
   default:
       QWidget::keyPressEvent(event);
   }
}

bool ToolContainer::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() >= QEvent::MouseButtonPress &&
        event->type() <= QEvent::FocusOut)
    {
        return QWidget::eventFilter(target, event);
    }
}

GroupBox::GroupBox(QWidget* parent) :
    QGroupBox(parent)
{

}

GroupBox::GroupBox(const QString &title, QWidget* parent) :
    QGroupBox(title, parent)
{

}

void GroupBox::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void GroupBox::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}

void GroupBox::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

Groupbox::Groupbox(QWidget *parent) :
    QWidget(parent)
{
    //installEventFilter(this);
    m_VLayout = new QVBoxLayout(this);
    m_VLayout->setSpacing(4);
    m_VLayout->setContentsMargins(20, 70, 20, 20);
    setFocusPolicy(Qt::NoFocus);
}

Groupbox::Groupbox(QString pText, QWidget *parent) :
    QWidget(parent)
{
    //installEventFilter(this);
    m_VLayout = new QVBoxLayout(this);
    m_VLayout->setSpacing(4);
    m_VLayout->setContentsMargins(20, 70, 20, 20);
    setFocusPolicy(Qt::NoFocus);

    m_Content = pText;
}

Groupbox::~Groupbox()
{
    if(m_VLayout)
    {
        m_VLayout->deleteLater();
    }
}

void Groupbox::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.setBrush(m_BgBrush);
    p.setPen(m_BgPenColor);
    p.drawRoundedRect(m_BgRect, 15, 15, Qt::AbsoluteSize);

    p.setBrush(m_FgBrush);
    p.setPen(m_FgPenColor);
    p.drawRoundedRect(m_FgRect, 13, 13, Qt::AbsoluteSize);

    //>@?ê÷??￡??–???
    p.setPen(Qt::NoPen);
    p.setBrush(m_TGlassBrush);
    p.drawRect(m_TGlassRect);
    p.setBrush(m_TGlassBrush2);
    p.drawRect(m_TGlassRect);

    //>@?ê÷?μ?????‘?
    p.setBrush(m_BGlowBrush);
    p.setPen(Qt::NoPen);
    p.drawEllipse(m_BGlowRect);//‘???”?μ???±í??????

    //>@?ê÷????÷
    p.setFont(m_ContentFont);
    p.setBrush(m_ContentColor);
    p.setPen(m_ContentColor);
    p.drawText(m_TGlassRect, Qt::AlignHCenter|Qt::AlignVCenter, m_Content);

//    p.setBrush(m_LightBrush);
//    p.setPen(Qt::NoPen);
//    p.drawRect(m_LightRect);
}

bool Groupbox::event(QEvent *e)
{
    if(e->type() == QEvent::HoverEnter)
    {
        m_FgBrush = QBrush(QColor(20, 40, 60, 255));
        update();
    }
    else if(e->type() == QEvent::HoverLeave)
    {
        m_FgBrush = QBrush(QColor(1, 12, 24, 255));
        update();
    }
    return QWidget::event(e);
}

void Groupbox::resizeEvent(QResizeEvent *e)
{
    InitBrush(QRect(QPoint(0,0), e->size()));
}

//>@?úμ?????????“‘????￡¨?ì????“‘àê“?????
void Groupbox::InitBrush(QRect pRect)
{
    m_BgRect = pRect;
    QLinearGradient tmpBgGradient(m_BgRect.topLeft(), m_BgRect.bottomLeft());
    tmpBgGradient.setColorAt(0, QColor(54,183,251,180));
    tmpBgGradient.setColorAt(0.4, QColor(17,116,192,180));
    tmpBgGradient.setColorAt(0.5, QColor(5, 50, 171, 240));
    tmpBgGradient.setColorAt(0.6, QColor(17,116,192, 180));
    tmpBgGradient.setColorAt(1, QColor(54,183,251,180));//??÷???±?μ?—’?′??????±???
    m_BgBrush = QBrush(tmpBgGradient);
    m_BgPenColor = QColor(54,183,251,180);

    m_FgRect = pRect.adjusted(4,4,-4,-4);
    m_FgBrush = QBrush(QColor(1, 12, 24, 255));
    m_FgPenColor = QColor(13,37,68,255);

    int x = m_FgRect.x();
    int y = m_FgRect.y();
    int w = m_FgRect.width();
    int h = m_FgRect.height();
    int tmpW = w*6/5.0;
    m_BGlowRect = QRect(x-w/10.0, y+h-w/5.0, tmpW, tmpW);
    QRadialGradient tmpBGlowGradient(m_BGlowRect.x()+m_BGlowRect.width()/2,
                                   m_BGlowRect.y()+m_BGlowRect.height()/2,
                                   m_BGlowRect.width()/2);
    tmpBGlowGradient.setColorAt(0,QColor(240, 238, 250, 80));
    tmpBGlowGradient.setColorAt(1.0,QColor(245,245,255,0));//??÷???±?μ?—’?′??????±???
    m_BGlowBrush = QBrush(tmpBGlowGradient);

    m_TGlassRect = QRect(x, y+25, w, 35);
    QLinearGradient tmpTGlassGradient(m_TGlassRect.topLeft(), m_TGlassRect.bottomLeft());
    tmpTGlassGradient.setColorAt(0, QColor(79, 120, 165, 200));
    tmpTGlassGradient.setColorAt(0.45, QColor(55, 92, 136, 200));
    tmpTGlassGradient.setColorAt(0.46, QColor(20, 62, 117, 200));
    tmpTGlassGradient.setColorAt(1, QColor(13,91,173,200));//??÷???±?μ?—’?′??????±???
    m_TGlassBrush = QBrush(tmpTGlassGradient);

    QLinearGradient tmpTGlassGradient2(m_TGlassRect.topLeft(), m_TGlassRect.topRight());
    tmpTGlassGradient2.setColorAt(0, QColor(1, 12, 24, 255));
    tmpTGlassGradient2.setColorAt(0.4, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(0.5, QColor(1, 12, 24, 0));
    tmpTGlassGradient2.setColorAt(0.6, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(1, QColor(1, 12, 24, 255));//??÷???±?μ?—’?′??????±???
    m_TGlassBrush2 = QBrush(tmpTGlassGradient2);

    m_ContentFont = QFont("Arial", 15, QFont::DemiBold);
    m_ContentColor = QColor(221, 222, 223, 255);

    m_LightRect = QRect(x, y+260, w, 80);
    m_LightBrush = QBrush(QColor(255,199,0,120));
}

Slider::Slider(QWidget *parent)
    :QSlider(parent)
{
    setSingleStep(5); //>@?¨????÷????§??5
    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(slot_ValueChanged(int)));
}

Slider::~Slider()
{
    QObject::disconnect(this, SIGNAL(valueChanged(int)), 0, 0);
}

void Slider::bindLabel(QLabel *pLabel)
{
    mLabel = pLabel;
}

void Slider::slot_ValueChanged(int pVaule)
{
    if(singleStep() <= 0)
        return;
    static int oldValue = 0;
    int diff = pVaule%singleStep();
    if(diff)
        pVaule -= diff;
    if(pVaule == oldValue)
        return;
    oldValue = pVaule;

    if(!mLabel.isNull())
        mLabel.data()->setText(QString("%1%").arg(pVaule));

    emit sValueChanged(pVaule);
}

MySlider::MySlider(QWidget *parent)
    :QSlider(parent),
    mQesp(0)
{
    oldValue = 0x00ffffff;
    setSingleStep(5); //>@?¨????÷????§??5
    SetType(0);
}

MySlider::~MySlider()
{
    QObject::disconnect(this, SIGNAL(valueChanged(int)), 0, 0);
}

void MySlider::SetType(int pType)
{
    switch(pType)
    {
        case 0:
            QObject::disconnect(this, SIGNAL(sliderReleased(int)), 0, 0);
            QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(slot_ValueChanged(int)));
            break;
        case 1:
            QObject::disconnect(this, SIGNAL(valueChanged(int)), 0, 0);
            QObject::connect(this, SIGNAL(sliderReleased()), this, SLOT(slot_SliderReleased()));
            break;
        default:
            break;
    }
}

void MySlider::WriteValue(int pValue)
{
    if(!mLabel.isNull())
    {
        int tmpValue = GetValue(pValue);
        mLabel.data()->setText(mFormat.arg(tmpValue));
    }
    setValue(pValue);
}

void MySlider::bindLabel(QLabel *pLabel, QString pFormat, QString pEquation)
{
    mLabel = pLabel;
    mFormat = pFormat;
    mEquation = pEquation;
}

int MySlider::GetValue(int pVaule)
{
    //>@?ò?°?ò ?
    int tmpValue = -1;
    if(!mEquation.compare(QString("x/1000")))
    {
        tmpValue = pVaule / 1000;
    }
    else if(!mEquation.compare(QString("x/60000")))
    {
        tmpValue = pVaule / 60000;
    }
    else
    {
        tmpValue = pVaule;
    }
    return tmpValue;
}

void MySlider::slot_SliderReleased()
{
    if(singleStep() <= 0)
        return;
    int pVaule = value();
    int diff = pVaule%singleStep();
    if(diff)
        pVaule -= diff;
    if(pVaule == oldValue)
        return;
    oldValue = pVaule;

    if(!mLabel.isNull())
    {
        //>@?ò?°?ò ?
        int tmpValue = GetValue(pVaule);
        mLabel.data()->setText(mFormat.arg(tmpValue));
    }

    QString tmpMessage = property("valueChanged").toString().arg(pVaule);
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage, false);
}

void MySlider::slot_ValueChanged(int pVaule)
{
    if(singleStep() <= 0)
        return;
    static int oldValue = 0;
    int diff = pVaule%singleStep();
    if(diff)
        pVaule -= diff;
    if(pVaule == oldValue)
        return;
    oldValue = pVaule;

    if(!mLabel.isNull())
    {
        //>@?ò?°?ò ?
        int tmpValue = GetValue(pVaule);
        mLabel.data()->setText(mFormat.arg(tmpValue));
    }

    QString tmpMessage = property("valueChanged").toString().arg(pVaule);
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage, false);
}

ScrollArea::ScrollArea(QWidget *parent):
    QScrollArea(parent)
{
#ifndef QT_V453
    m_HAnimation = m_VAnimation = 0;
#endif
    m_SliderDirection = HVBoth;

    installEventFilter(this);
}

void ScrollArea::UpdateEventFilter()
{
    QList<QWidget*> tmpBtnList = findChildren<QWidget *>();
    QWidget *tmpBtn = 0;
    foreach(tmpBtn, tmpBtnList)
    {
        if(tmpBtn)
            tmpBtn->installEventFilter(this);
    }
}

void ScrollArea::resizeEvent(QResizeEvent *event)
{
//    QSize areaSize = viewport()->size();
//    QSize widgetSize = widget()->size();
//    m_HScrollValue.mMinNum = 0;
//    m_VScrollValue.mMinNum = 0;
//    m_HScrollValue.mMaxNum = widgetSize.width() - areaSize.width();
//    m_VScrollValue.mMaxNum = widgetSize.height() - areaSize.height();

//    horizontalScrollBar()->setPageStep(widgetSize.width());
//    horizontalScrollBar()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
//    verticalScrollBar()->setPageStep(widgetSize.height());
//    verticalScrollBar()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);

    QScrollArea::resizeEvent(event);
}

void ScrollArea::InitMovePara(DIRECTION pDirection, int pHScrollSize, int pHSubWidSize, int pVScrollSize, int pVSubWidSize)
{
//    m_SliderDirection = pDirection;
//    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
//    {
//        m_HScrollBar = horizontalScrollBar();
//        m_HScrollValue.mMinNum = 0;

//        QSize areaSize = viewport()->size();
//        QSize widgetSize = widget()->size();

//        if(!m_HScrollBar.isNull())
//        {
//            m_HScrollValue.mMaxNum = widgetSize.width() - areaSize.width();
//            m_HScrollBar->setPageStep(widgetSize.width());
//            m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
//            m_HRelDiff = 1;
//        }
//#ifndef QT_V453
//        m_HAnimation = new QPropertyAnimation(m_HScrollBar.data(), "value");
//        m_HAnimation->setEasingCurve(QEasingCurve::OutQuint);
//#endif
//    }
//    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
//    {
//        m_VScrollBar = verticalScrollBar();
//        m_VScrollValue.mMinNum = 0;

//        QSize areaSize = viewport()->size();
//        QSize widgetSize = widget()->size();

//        if(!m_VScrollBar.isNull())
//        {
//            m_VScrollValue.mMaxNum = widgetSize.height() - areaSize.height();
//            m_VScrollBar->setPageStep(widgetSize.height());
//            m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
//            m_VRelDiff = 1;
//        }
//#ifndef QT_V453
//        m_VAnimation = new QPropertyAnimation(m_VScrollBar.data(), "value");
//        m_VAnimation->setEasingCurve(QEasingCurve::OutQuint);
//#endif
//    }

    m_SliderDirection = pDirection;
    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        m_HScrollBar = horizontalScrollBar();
        int tmpMaxNum = 0;
        if(pHScrollSize == -1 || pHSubWidSize == -1)
            tmpMaxNum = widget()->width()-width();
        else
        {
            if(widget())
                widget()->setFixedWidth(pHSubWidSize);
            tmpMaxNum = pHSubWidSize - pHScrollSize;
        }
        if(!m_HScrollBar.isNull())
        {
            m_HScrollBar.data()->setRange(0, tmpMaxNum);
            m_HRelDiff = 1;
        }
#ifndef QT_V453
        m_HAnimation = new QPropertyAnimation(m_HScrollBar.data(), "value");
        m_HAnimation->setEasingCurve(QEasingCurve::OutQuint);
#endif
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        m_VScrollBar = verticalScrollBar();
        int tmpMaxNum = 0;
        if(pVScrollSize == -1 || pVSubWidSize == -1)
            tmpMaxNum = widget()->height()-height();
        else
        {
            if(widget())
                widget()->setFixedHeight(pVSubWidSize);
            tmpMaxNum = pVSubWidSize - pVScrollSize;
        }
        if(!m_VScrollBar.isNull())
        {
            m_VScrollBar.data()->setRange(0, tmpMaxNum);
            m_VRelDiff = 1;
        }
#ifndef QT_V453
        m_VAnimation = new QPropertyAnimation(m_VScrollBar.data(), "value");
        m_VAnimation->setEasingCurve(QEasingCurve::OutQuint);
#endif
    }
}

bool ScrollArea::eventFilter(QObject *target, QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent *>(event);
    if(e/* && target == this*/)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            m_IsMoving = false;
            if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
            {
#ifndef QT_V453
                if(m_HAnimation)
                    m_HAnimation->stop();
#endif
            }
            if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
            {
#ifndef QT_V453
                if(m_VAnimation)
                    m_VAnimation->stop();
#endif
            }

            m_MouseBaseTime = QTime::currentTime();
            m_MouseTime[0] = 0;
            m_MousePosBk[0] = e->globalPos();

            m_MouseStatus = QEvent::MouseButtonPress;
        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(m_MouseStatus == QEvent::MouseButtonPress)
            {
                m_IsMoving = true;

                m_MousePosBk[1] = m_MousePosBk[0];
                m_MousePosBk[0] = e->globalPos();

                m_MouseTime[1] = m_MouseTime[0];
                m_MouseTime[0] = QTime::currentTime().msecsTo(m_MouseBaseTime);

                int diff = 0;

                if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
                {
                    diff = m_MousePosBk[1].x() - m_MousePosBk[0].x();
                    if(diff != 0)
                    {
                        if(!m_HScrollBar.isNull())
                        {
                            int tmpValue = m_HScrollBar.data()->value()+diff*m_HRelDiff;
                            if(tmpValue < 0)
                                tmpValue = 0;
                            else if(tmpValue > m_HScrollBar.data()->maximum())
                                tmpValue = m_HScrollBar.data()->maximum();
                            if(tmpValue != m_HScrollBar.data()->value())
                                m_HScrollBar.data()->setValue(tmpValue);
                        }
                    }
                }
                if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
                {
                    diff = m_MousePosBk[1].y() - m_MousePosBk[0].y();
                    if(diff != 0)
                    {
                        if(!m_VScrollBar.isNull())
                        {
                            int tmpValue = m_VScrollBar.data()->value()+diff*m_VRelDiff;
                            if(tmpValue < 0)
                                tmpValue = 0;
                            else if(tmpValue > m_VScrollBar.data()->maximum())
                                tmpValue = m_VScrollBar.data()->maximum();
                            if(tmpValue != m_VScrollBar.data()->value())
                                m_VScrollBar.data()->setValue(tmpValue);
                        }
                    }
                }

                return true;
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            m_MouseStatus = QEvent::MouseButtonRelease;
            if(m_IsMoving)
            {
                //>@计算速度
                if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
                {
                    qreal tmpLenDiff = m_MousePosBk[0].x() - m_MousePosBk[1].x();
                    qreal tmpTimeDiff = m_MouseTime[1] - m_MouseTime[0];
                    if(tmpTimeDiff && tmpLenDiff)
                        m_XVelocity = tmpLenDiff*1000 / tmpTimeDiff;  //>@ px/us
                    else
                        m_XVelocity = 0;
                }
                if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
                {
                    int tmpLenDiff = m_MousePosBk[0].y() - m_MousePosBk[1].y();
                    int tmpTimeDiff = m_MouseTime[1] - m_MouseTime[0]; //>@sceTo特殊，需要使用前一次减去后一次的。
                    if(tmpTimeDiff && tmpLenDiff)
                        m_YVelocity = tmpLenDiff*1000 / tmpTimeDiff;  //>@ px/us
                    else
                        m_YVelocity = 0;
                }

#ifndef QT_V453
                if(m_HAnimation && (m_SliderDirection == Horizontal || m_SliderDirection == HVBoth))
                {
                    if(qAbs(m_XVelocity) >= 150)  //>@下为正
                    {
                        m_HAnimation->setStartValue(m_HScrollBar.data()->value());
                        m_HAnimation->setEndValue(m_HScrollBar.data()->value() - m_XVelocity/10);
                        m_HAnimation->setDuration(m_XVelocity);
                        m_HAnimation->start();
                        //m_HAnimation->setCurrentTime(m_XVelocity/2);
                    }
                }
                if(m_VAnimation && (m_SliderDirection == Vertical || m_SliderDirection == HVBoth))
                {
                    if(qAbs(m_YVelocity) >= 150)
                    {
                        m_VAnimation->setStartValue(m_VScrollBar.data()->value());
                        m_VAnimation->setEndValue(m_VScrollBar.data()->value() - m_YVelocity/10);
                        m_VAnimation->setDuration(m_YVelocity);
                        m_VAnimation->start();
                        //m_VAnimation->setCurrentTime(m_YVelocity/2);
                    }
                }
#endif
                m_IsMoving = false;
                return true; //>@返回true通知Qt已经处理了这个事件，如果返回false，Qt将会把事件传递给目标控件
            }
        }
    }
    return QScrollArea::eventFilter(target,event);
}

MySpinbox::MySpinbox(QWidget *parent)
    :QSpinBox(parent),
    mQesp(0)
{
    setSingleStep(5); //>@?¨????÷????§??5
    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(slot_ValueChanged(int)));
}

MySpinbox::~MySpinbox()
{
    QObject::disconnect(this, SIGNAL(valueChanged(int)), 0, 0);
}

void MySpinbox::WriteValue(int pValue)
{
    if(!mLabel.isNull())
    {
        int tmpValue = GetValue(pValue);
        mLabel.data()->setText(mFormat.arg(tmpValue));
    }
    setValue(pValue);
}

void MySpinbox::bindLabel(QLabel *pLabel, QString pFormat, QString pEquation)
{
    mLabel = pLabel;
    mFormat = pFormat;
    mEquation = pEquation;
}

int MySpinbox::GetValue(int pVaule)
{
    //>@?ò?°?ò ?
    int tmpValue = -1;
    if(!mEquation.compare(QString("x/1000")))
    {
        tmpValue = pVaule / 1000;
    }
    else
    {
        tmpValue = pVaule;
    }
    return tmpValue;
}

void MySpinbox::slot_ValueChanged(int pVaule)
{
    if(singleStep() <= 0)
        return;
    static int oldValue = 0;
    int diff = pVaule%singleStep();
    if(diff)
        pVaule -= diff;
    if(pVaule == oldValue)
        return;
    oldValue = pVaule;

    if(!mLabel.isNull())
    {
        //>@?ò?°?ò ?
        int tmpValue = GetValue(pVaule);
        mLabel.data()->setText(mFormat.arg(tmpValue));
    }

    QString tmpMessage = property("valueChanged").toString().arg(pVaule);
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage, false);
}

Button::Button(QString pText, bool pCheckable, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    m_Content = pText;
    m_Checkable = pCheckable;
    setChecked(false);
}

Button::Button(QString pText, bool pCheckable, QPixmap pNormalImg, QPixmap pDownImg, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    m_CheckState = false;
    m_Content = pText;
    m_Checkable = pCheckable;
    setChecked(false);
    m_NormalImage = pNormalImg;
    m_DownImage = pDownImg;
}

void Button::setChecked(bool falg)
{
    m_CheckState = falg;
    if(falg)
    {
        if(m_Checkable)
            m_Color = QColor(105, 255, 67, 240);
        else
            m_Color = QColor(24, 72, 187, 240);
        m_Font = QFont("Arial", 13, QFont::DemiBold);
    }
    else
    {
        m_Color = Qt::black;
        m_Font = QFont("Arial", 12, QFont::DemiBold);
    }
    update();
    emit sPressed(falg);
}

void Button::mousePressEvent(QMouseEvent* e)
{
    //if(e->pos().x() < 25)
    {
        bool flag = !m_CheckState;
        setChecked(flag);
        emit sPressed(flag);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *e)
{
    if(!m_Checkable)
        setChecked(false);
}

void Button::focusOutEvent(QFocusEvent *e)
{
    if(!m_Checkable)
        setChecked(false);
}

void Button::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    if(isChecked())
        p.drawPixmap(rect(), m_DownImage);
    else
        p.drawPixmap(rect(), m_NormalImage);
    if(!m_Content.isEmpty())
    {
        p.setFont(m_Font);
        p.setBrush(m_Color);
        p.setPen(m_Color);
        QRect tmpRect = rect();
        int x = tmpRect.width()/4;
        int y = 0;
        int w = x*3;
        int h = tmpRect.height();
        p.drawText(QRect(x, y, w, h), Qt::AlignHCenter|Qt::AlignVCenter, m_Content);
    }
}

MyPushButton::MyPushButton(QWidget *parent) :
    QPushButton(parent),
    mQesp(0)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(slot_Toggled(bool)));
}

MyPushButton::~MyPushButton()
{

}

void MyPushButton::slot_Toggled(bool pChecked)
{
    if(pChecked)
    {
        QString tmpMessage = property("state").toString();
        //>@??à?????
        IDE_DEBUG(tmpMessage);
        if(tmpMessage.isEmpty()||(!mQesp))
            return;
        mQesp->ProcessCommand(0, tmpMessage);
    }
}

MyRadioButton::MyRadioButton(QWidget *parent)
    :QRadioButton(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(slot_Toggled(bool)));
}

MyRadioButton::~MyRadioButton()
{
    QObject::disconnect(this, SIGNAL(toggled()), 0, 0);
}

void MyRadioButton::slot_Toggled(bool pChecked)
{
    if(pChecked)
    {
        QString tmpMessage = this->property("state").toString();
        //>@??à?????
        IDE_DEBUG(tmpMessage);
        if(tmpMessage.isEmpty()||(!mQesp))
            return;
        mQesp->ProcessCommand(0, tmpMessage);
    }
}


Switch::Switch(QString pSwitch, QString pOnText, QString pOffText, QWidget *parent) :
    Button(QString(), true, parent)
{
    QSize tmpSize = QSize(50, 25);
    setFixedSize(tmpSize);

    m_OnText = pOnText;
    m_OffText = pOffText;
    if(!pSwitch.compare(pOnText, Qt::CaseInsensitive))
        setChecked(true);
    else
        setChecked(false);
    m_DownImage = QPixmap(":/SetBar/rc/SetBar/switch-on.png").scaled(tmpSize);
    m_NormalImage = QPixmap(":/SetBar/rc/SetBar/switch-off.png").scaled(tmpSize);

    connect(this, SIGNAL(sPressed(bool)), this, SLOT(slot_ButtonClicked(bool)));
}

void Switch::slot_ButtonClicked(bool flag)
{
    emit sChanged(this);
}


ButtonList::ButtonList(QStringList pParaList, QWidget *parent)
    : QHBoxLayout(parent)
{
    QSize tmpBtnSize = QSize(75, 25);
    QPixmap tmpNormalImg = QPixmap(":/SetBar/rc/SetBar/button-off.png").scaled(tmpBtnSize);
    QPixmap tmpDownImg = QPixmap(":/SetBar/rc/SetBar/button-on.png").scaled(tmpBtnSize);
    for(int i=0;i<pParaList.count();i++)
    {
        Button *tmpButton=new Button(pParaList.at(i), false, tmpNormalImg, tmpDownImg);
        tmpButton->setFixedSize(tmpBtnSize);
        connect(tmpButton, SIGNAL(sPressed(bool)), this, SLOT(slot_ButtonClicked(bool)));
        addWidget(tmpButton);
    }
}

ButtonList::~ButtonList()
{
}

void ButtonList::slot_ButtonClicked(bool flag)
{
    Button *tmpButton = VAR_CAST<Button *>(sender());
    if(tmpButton)
    {
        if(flag)
            emit sChanged(tmpButton);
    }
}

CheckList::CheckList(QStringList pParaList, QStringList pSelectList, QWidget *parent)
    : QHBoxLayout(parent)
{
    QSize tmpBtnSize = QSize(75, 25);
    m_SelectList.clear();
    QPixmap tmpNormalImg = QPixmap(":/SetBar/rc/SetBar/check-off.png").scaled(tmpBtnSize);
    QPixmap tmpDownImg = QPixmap(":/SetBar/rc/SetBar/check-on.png").scaled(tmpBtnSize);
    for(int i=0;i<pParaList.count();i++)
    {
        QString tmpText = pParaList.at(i);
        Button *tmpCheckBtn=new Button(tmpText, true, tmpNormalImg, tmpDownImg);
        tmpCheckBtn->setFixedSize(tmpBtnSize);
        connect(tmpCheckBtn, SIGNAL(sPressed(bool)), this, SLOT(slot_ButtonClicked(bool)));
        for(int j=0;j<pSelectList.count();j++)
        {
            QString tmpSelectText = pSelectList.at(j);
            if(tmpSelectText.compare(tmpText, Qt::CaseInsensitive) == 0)
            {
                tmpCheckBtn->setChecked(true);
                break;
            }
        }
        addWidget(tmpCheckBtn);
    }
    m_SelectList = pSelectList;
}

CheckList::~CheckList()
{
}

void CheckList::slot_ButtonClicked(bool flag)
{
    emit sChanged(this);
}

void CheckList::InitCheckList(QStringList pList)
{
    for(int i=0;i<this->count();i++)
    {
        Button *tmpCheckBtn = VAR_CAST<Button *>(itemAt(i)->widget());
        if(!tmpCheckBtn)
            continue;
        QString tmpString = tmpCheckBtn->text();
        bool searchFlag = false;
        for(int j=0;j<pList.count();j++)
        {
            QString tmpText = pList.at(j);
            if(tmpText.compare(tmpString, Qt::CaseInsensitive) == 0)
            {
                searchFlag = true;
                break;
            }
        }
        tmpCheckBtn->setChecked(searchFlag);
    }
}

void CheckList::RestoreCheckList()
{
    InitCheckList(m_SelectList);
}

Button *CheckList::GetCheckbox(int index)
{
    if(index < 0)
        return (Button *)0;
    if(index >= count())
        return (Button *)0;
    return VAR_CAST<Button *>(itemAt(index)->widget());
}

Button *CheckList::GetCheckbox(QString pName)
{
    if(pName.isEmpty())
        return (Button *)0;
    for(int i=0;i<count();i++)
    {
        Button *tmpButton = VAR_CAST<Button *>(itemAt(i)->widget());
        if(!tmpButton)
            continue;
        if(tmpButton->text().compare(pName) == 0)
            return tmpButton;
    }
    return (Button *)0;
}

QStringList CheckList::GetSelectList()
{
    QStringList tmpList;
    if(count())
    {
        for(int i=0;i<count();i++)
        {
            Button *tmpCheckBtn = VAR_CAST<Button *>(itemAt(i)->widget());
            if(tmpCheckBtn->isChecked())
                tmpList.append(tmpCheckBtn->text());
        }
    }
    return tmpList;
}

QString CheckList::GetSelectText(QStringList pList)
{
    QString tmpString;
    if(pList.count()>0)
    {
        int i=0;
        for(;i<pList.count()-1;i++)
        {
            tmpString.append(pList.at(i));
            tmpString.append(';');
        }
        tmpString.append(pList.at(i));
    }
    return tmpString;
}

QString CheckList::GetSelectText()
{
    QStringList tmpList = GetSelectList();
    return GetSelectText(tmpList);
}

BaseToolBar::BaseToolBar(QWidget *parent) :
    QToolBar(parent)
{
#ifdef IDE
    mMainWindow = VAR_CAST<MainWindow*>(parent);
#endif
}

BaseToolBar::~BaseToolBar()
{

}

Checkbox::Checkbox(QWidget *parent) :
    QCheckBox(parent), BstUiBase()
{
    setMinimumHeight(25);
    setMaximumHeight(25);
    PaintBorder();
}
void Checkbox::mousePressEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

//>@÷?“?”?”????? ?±í“???—°÷–??±?μ?????
void Checkbox::mouseMoveEvent(QMouseEvent* e)
{

}

void Checkbox::mouseDoubleClickEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

void Checkbox::wheelEvent(QWheelEvent *e)
{
    WheelEvent(e);
}

void Checkbox::keyPressEvent( QKeyEvent *e )
{
    KeyEvent(e);
}

void Checkbox::focusInEvent(QFocusEvent *e)
{
    FocusInEvent();
}

void Checkbox::focusOutEvent(QFocusEvent *e)
{
    FocusOutEvent();
}

void Checkbox::ProcessMouseEvent(QMouseEvent* e)
{
    toggle();
}

void Checkbox::ProcessKeyEvent(QKeyEvent *e)
{
    bool checkflag = isChecked();
    if(e->key() == Qt::Key_Up)
    {
        if(!checkflag)
            setChecked(true);
    }
    else if(e->key() == Qt::Key_Down)
    {
        if(checkflag)
            setChecked(false);
    }
    else if(e->key() == Qt::Key_Enter)
    {
        toggle();
    }
}

void Checkbox::ProcessWheelEvent(QWheelEvent *e)
{
    //>@???÷“???????15??￡¨“???μ?μ?delta??120￡¨“ú?à????μ?????￡?“??????÷????μ?μ??? ?￡???delta()/8/15
    int tmpDelta = e->delta();
    tmpDelta /= 8;
    tmpDelta /= 15;
    bool checkflag = isChecked();
    if(tmpDelta > 0)
    {
        if(!checkflag)
            setChecked(true);
    }
    else if(tmpDelta < 0)
    {
        if(checkflag)
            setChecked(false);
    }
}

bool Checkbox::isFocus()
{
    return hasFocus();
}

void Checkbox::PaintBorder()
{
    switch(m_LogicState)
    {
        case STATE_UNFOCUS: //>@??÷?±???????
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 1px groove blue;");
            break;
        }
        case STATE_FOCUS:
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 2px groove magenta;");  //groove???€  //>@solid μ–?
            break;
        }
        case STATE_EDITABLE:
        {
            setStyleSheet(QString("background-color: rgba(%1);color:rgba(%2);border: 2px groove magenta;").arg(D_EDITCOLOR).arg(D_TEXTCOLOR));
            break;
        }
        default:
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 1px groove blue;");
            break;
        }
    }
}

MyCheckBox::MyCheckBox(QWidget *parent)
    :QCheckBox(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(stateChanged(int)), this, SLOT(slot_StateChanged(int)));
}

MyCheckBox::~MyCheckBox()
{
    mLabel = 0;
    QObject::disconnect(this, SIGNAL(stateChanged(int)), 0, 0);
}

bool MyCheckBox::GetStatus()
{
    return isChecked();
}

void MyCheckBox::SetStatus(bool pStatus)
{
    if(mLabel)
    {
        if(pStatus)
            mLabel.data()->setText(mCheckedText);
        else
            mLabel.data()->setText(mUncheckedText);
    }
    setChecked(pStatus);
}

void MyCheckBox::bindLabel(QLabel *pLabel, QString pCheckedText, QString pUncheckedText)
{
    mLabel = pLabel;
    mCheckedText = pCheckedText;
    mUncheckedText = pUncheckedText;
}

void MyCheckBox::slot_StateChanged(int pCheckFlag)
{
    QString tmpKeyWords = pCheckFlag?QString("state1"):QString("state0");
    QString tmpMessage = property(tmpKeyWords.toUtf8().data()).toString();
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage);
}

ENetAddrEdit::ENetAddrEdit(QString pIP, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    m_FocusAreaIndex = -1;
    QSize tmpSize = QSize(171, 25);
    setFixedSize(tmpSize);

    connect(&m_FlowTimer, SIGNAL(timeout()), this, SLOT(slot_FlowChange()));
    m_FlowTimer.setInterval(100);

    m_AreaRect[MinusOn] = QRect(0, 0, 20, 20);
    m_AreaRect[AddOn] = QRect(tmpSize.width()-20, 0, 20, 20);
    m_AreaRect[IPNum1] = QRect(23, 0, 30, 25);
    m_AreaRect[IPNum2] = m_AreaRect[IPNum1].adjusted(32, 0, 32, 0);
    m_AreaRect[IPNum3] = m_AreaRect[IPNum2].adjusted(32, 0, 32, 0);
    m_AreaRect[IPNum4] = m_AreaRect[IPNum3].adjusted(32, 0, 32, 0);

    m_Pixmap[BgOff] = QPixmap(":/SetBar/rc/SetBar/ip-edit.png").scaled(tmpSize);
    m_Pixmap[MinusOn] = QPixmap(":/SetBar/rc/SetBar/ip-minus-on.png").scaled(tmpSize);
    m_Pixmap[AddOn] = QPixmap(":/SetBar/rc/SetBar/ip-add-on.png").scaled(tmpSize);
    m_FocusPixmap = QPixmap(":/SetBar/rc/SetBar/ip-light.png").scaled(m_AreaRect[IPNum1].size());
    m_Color = QColor(250, 250, 250, 240);
    m_FocusColor = QColor(0, 24, 124, 255);
    m_Font = QFont("Arial", 12, QFont::DemiBold);
    m_FocusFont = QFont("Arial", 13, QFont::Bold);
    SetState(BgOff);
    SetIP(pIP);
}

void ENetAddrEdit::SetState(int pState)
{
    if(pState < (int)BgOff && pState > (int)AddOn)
        pState = (int)BgOff;
    m_State = (ENET_STATE)pState;
    if(m_State == MinusOn || m_State == AddOn)
        m_FlowTimer.start();
    else
        m_FlowTimer.stop();
    m_CurPixmap = m_Pixmap[pState];
    update();
}

void ENetAddrEdit::SetIP(QString pIP)
{
    m_IPAddr = ENET_ADDR(pIP);
    update();
}

void ENetAddrEdit::SetAreaIP(int index, quint8 pNum)
{
    if(index > (int)IPNum4 || index < (int)IPNum1)
        return;
    index -= IPNum1;
    if(m_IPAddr.mAddr[index] != pNum)
    {
        m_IPAddr.mAddr[index] = pNum;
        update();
    }
}

void ENetAddrEdit::slot_FlowChange()
{
    if(m_State == MinusOn)
    {
        SetAreaIP(m_FocusAreaIndex, GetAreaIP(m_FocusAreaIndex)-1);
    }
    else if(m_State == AddOn)
    {
        SetAreaIP(m_FocusAreaIndex, GetAreaIP(m_FocusAreaIndex)+1);
    }
}

quint8 ENetAddrEdit::GetAreaIP(int pAreaIndex)
{
    return m_IPAddr.mAddr[pAreaIndex-IPNum1];
}

void ENetAddrEdit::GotoArea(int index)
{
    if(m_FocusAreaIndex != index)
    {
        m_FocusAreaIndex = index;
        update();
    }
}

QString ENetAddrEdit::GetIP()
{
    return m_IPAddr.toQString();
}

void ENetAddrEdit::MoveAreaRight()
{
    int tmpIndex = m_FocusAreaIndex;
    tmpIndex++;
    if(tmpIndex > (int)AddOn)
        tmpIndex = (int)BgOff;
    GotoArea(tmpIndex);
}

void ENetAddrEdit::mousePressEvent(QMouseEvent* e)
{
    if(m_AreaRect[MinusOn].contains(e->pos()))
    {
        SetState(MinusOn);
        SetAreaIP(m_FocusAreaIndex, GetAreaIP(m_FocusAreaIndex)-1);
    }
    else if(m_AreaRect[AddOn].contains(e->pos()))
    {
        SetState(AddOn);
        SetAreaIP(m_FocusAreaIndex, GetAreaIP(m_FocusAreaIndex)+1);
    }
    else
    {
        for(int i=IPNum1;i<=IPNum4;i++)
        {
            if(m_AreaRect[i].contains(e->pos()))
            {
                GotoArea(i);
                return;
            }
        }
        GotoArea(-1);
    }
}

void ENetAddrEdit::mouseReleaseEvent(QMouseEvent *e)
{
    SetState(BgOff);
}

void ENetAddrEdit::focusInEvent(QFocusEvent *e)
{
    SetState(BgOff);
    GotoArea(IPNum1);
}

void ENetAddrEdit::focusOutEvent(QFocusEvent *e)
{
    SetState(BgOff);
    GotoArea(-1);
    emit sChanged(this);
}

void ENetAddrEdit::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.drawPixmap(rect(), m_CurPixmap);
    if(m_FocusAreaIndex >= IPNum1 && m_FocusAreaIndex <= IPNum4)
    {
        p.drawPixmap(m_AreaRect[m_FocusAreaIndex], m_FocusPixmap);
    }
    for(int i=IPNum1;i<=IPNum4;i++)
    {
        quint8 tmpInt = GetAreaIP(i);
        QString tmpData = QString("%1%2%3").arg(tmpInt/100)
                                           .arg(tmpInt/10%10)
                                           .arg(tmpInt%10);
        p.save();
        if(i == m_FocusAreaIndex)
        {
            p.setFont(m_FocusFont);
            p.setBrush(m_FocusColor);
            p.setPen(m_FocusColor);
        }
        else
        {
            p.setFont(m_Font);
            p.setBrush(m_Color);
            p.setPen(m_Color);
        }
        p.drawText(m_AreaRect[i], Qt::AlignHCenter|Qt::AlignVCenter, tmpData);
        p.restore();
    }
}


LineEdit::LineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

LineEdit::LineEdit(const QString &str, QWidget *parent)
    :QLineEdit(str, parent)
{

}

LineEdit::~LineEdit()
{

}

void LineEdit::focusInEvent(QFocusEvent *e)
{
    emit sActive();
}

void LineEdit::focusOutEvent(QFocusEvent *e)
{
    emit sUnActive();
}

FileSystemModel::FileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 0) {
        QString path  = QDir::toNativeSeparators(filePath(index));
        if (path.endsWith(QDir::separator()))
            path.chop(1);
        return path;
    }

    return QFileSystemModel::data(index, role);
}

CompleterLineEdit::CompleterLineEdit(QWidget *parent, COMPLETER_MODE pMode) :
    QLineEdit(parent)
{
    m_Completer = 0;
    changeModel(pMode);
}

void CompleterLineEdit::CreateCompleter()
{
    ReleaseCompleter();
    m_Completer = new QCompleter;
    m_Completer->setCompletionMode(QCompleter::PopupCompletion);
    m_Completer->setCaseSensitivity(Qt::CaseInsensitive);
#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    m_Completer->setMaxVisibleItems(10);
#endif
    m_Completer->setWrapAround(true);
    setCompleter(m_Completer);
}

void CompleterLineEdit::ReleaseCompleter()
{
    if(m_Completer)
        m_Completer->deleteLater();
}

CompleterLineEdit::~CompleterLineEdit()
{
    if(m_Completer)
        m_Completer->deleteLater();
}

QAbstractItemModel *CompleterLineEdit::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(m_Completer);
    QStringList words;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }
    if (!fileName.contains(QLatin1String("countries.txt")))
        return new QStringListModel(words, m_Completer);
    QStandardItemModel *m = new QStandardItemModel(words.count(), 2, m_Completer);
    for (int i = 0; i < words.count(); ++i)
    {
        QModelIndex countryIdx = m->index(i, 0);
        QModelIndex symbolIdx = m->index(i, 1);
        QString country = words[i].mid(0, words[i].length() - 2).trimmed();
        QString symbol = words[i].right(2);
        m->setData(countryIdx, country);
        m->setData(symbolIdx, symbol);
    }

    return m;
}

void CompleterLineEdit::changeModel(COMPLETER_MODE mode)
{
    CreateCompleter();

    switch (mode)
    {
        case Mode_UnsortedFS:
            { // Unsorted QFileSystemModel
                QFileSystemModel *fsModel = new QFileSystemModel(m_Completer);
                fsModel->setRootPath("");
                m_Completer->setModel(fsModel);
            }
            break;
        case Mode_FullpathFS:
            {   // FileSystemModel that shows full paths
                FileSystemModel *fsModel = new FileSystemModel(m_Completer);
                m_Completer->setModel(fsModel);
                fsModel->setRootPath("");
            }
            break;
        case Mode_Country:
            { // Country List
                m_Completer->setModel(modelFromFile(":/Completer/rc/Completer/countries.txt"));
                QTreeView *treeView = new QTreeView;
                m_Completer->setPopup(treeView);
                treeView->setRootIsDecorated(false);
                treeView->header()->hide();
                treeView->header()->setStretchLastSection(false);
#ifndef QT5
                treeView->header()->setResizeMode(0, QHeaderView::Stretch);
                treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
#endif
            }
            break;
        case Mode_Word:
            { // Word list
                m_Completer->setModel(modelFromFile(":/Completer/rc/Completer/wordlist.txt"));
                m_Completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
            }
            break;
        default:
            ReleaseCompleter();
            return;
    }
}

MyLineEdit::MyLineEdit(QWidget *parent)
    :LineEdit(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(editingFinished()), this, SLOT(slot_TextChanged()));
}

MyLineEdit::~MyLineEdit()
{
    QObject::disconnect(this, SIGNAL(returnPressed()), 0, 0);
}

QString MyLineEdit::GetText()
{
    return text();
}

void MyLineEdit::SetText(QString pText)
{
    setText(pText);
    slot_TextChanged();
}

void MyLineEdit::slot_TextChanged()
{
    static QString oldValue;
    QString tmpString = text();
    if(tmpString.isEmpty())
        return;
    if(!oldValue.compare(tmpString))
        return;
    oldValue = tmpString;
    QString tmpMessage = this->property("textChanged").toString().arg(tmpString);
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage);
}

TextEdit::TextEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setStyleSheet(QString("background: qradialgradient(cx:0, cy:0, "
                          "radius: 1, fx:0.5, fy:0.5, stop:0 rgba(200, 200, 170, 60%), stop:1 rgba(5, 50, 171, 100%));"
                          "font-size:15px;font-weight: bold;color:white;font-family:\"Arial\";"
                          "border-radius: 9px;"));
    setFocusPolicy(Qt::StrongFocus);
    setMinimumHeight(25);
    setMaximumHeight(25);
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_TextChanged(QString)));
}

TextEdit::TextEdit(QString pText, QWidget *parent) :
    QLineEdit(parent)
{
    setStyleSheet(QString("background: qradialgradient(cx:0, cy:0, "
                          "radius: 1, fx:0.5, fy:0.5, stop:0 rgba(200, 200, 170, 60%), stop:1 rgba(5, 50, 171, 60%));"
                          "font-size:15px;font-weight: bold;color:white;font-family:\"Arial\";"
                          "border-radius: 9px;"));
    setMinimumHeight(25);
    setMaximumHeight(25);
    setText(pText);
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_TextChanged(QString)));
}

void TextEdit::slot_TextChanged(QString)
{
    emit sChanged(this);
}

void TextEdit::UpdateSelection(int pos)
{
    if(pos >= 0 && pos < text().count())
    {
        setSelection(pos, 1);
    }
    else if(pos < 0)
    {
        setCursorPosition(-1);
        deselect();
    }
    else if(pos >= text().count())
    {
        setCursorPosition(text().count());
        deselect();
    }
}

//>@?á???? ??÷?ú’??÷??￡¨‘ú??????????––‘???
void TextEdit::UpdateSelectionData(Qt::Key pKey)
{
    if(hasSelectedText() && selectedText().count() == 1)
    {
        QString tmpChange;
        int start = selectionStart();
        QString tmpData = selectedText();
        //>@?–?? ??÷???? ? ??÷
        char tmp = tmpData.toLatin1().at(0);
        IDE_TRACE_INT(tmp);
        if(tmp >= 0x30 && tmp <= 0x39)
        {
            if(pKey == Qt::Key_Up)
            {
                tmpChange = QString(QChar((tmp+1 > 0x39)?(0x39):(tmp+1)));
            }
            else if(pKey == Qt::Key_Down)
            {
                tmpChange = QString(QChar((tmp-1 < 0x30)?(0x30):(tmp-1)));
            }
        }
        else if(tmp >= 0x41 && tmp <= 0x5a)
        {
            if(pKey == Qt::Key_Up)
            {
                tmpChange = QString(QChar((tmp+1 > 0x5a)?(0x5a):(tmp+1)));
            }
            else if(pKey == Qt::Key_Down)
            {
                tmpChange = QString(QChar((tmp-1 < 0x41)?(0x41):(tmp-1)));
            }
        }
        else if(tmp >= 0x61 && tmp <= 0x7a)
        {
            if(pKey == Qt::Key_Up)
            {
                tmpChange = QString(QChar((tmp+1 > 0x7a)?(0x7a):(tmp+1)));
            }
            else if(pKey == Qt::Key_Down)
            {
                tmpChange = QString(QChar((tmp-1 < 0x61)?(0x61):(tmp-1)));
            }
        }
        else
        {
            return;
        }
        insert(tmpChange);
        UpdateSelection(start);
        update();
    }
}


ComboBox::ComboBox(QStringList pParaList, QString pSelectPara, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    m_CurIndex = -1;
    QSize tmpSize = QSize(125, 25);
    setFixedSize(tmpSize);
    m_AreaRect[MinusOn] = QRect(5, 3, 20, 19);
    m_AreaRect[AddOn] = QRect(100, 3, 20, 19);

    m_Pixmap[BgOff] = QPixmap(":/SetBar/rc/SetBar/combox-off.png").scaled(tmpSize);
    m_Pixmap[MinusOn] = QPixmap(":/SetBar/rc/SetBar/combox-minus-on.png").scaled(tmpSize);
    m_Pixmap[AddOn] = QPixmap(":/SetBar/rc/SetBar/combox-add-on.png").scaled(tmpSize);

    m_Color = QColor(250, 250, 250, 240);
    m_Font = QFont("Arial", 12, QFont::DemiBold);

    SetState(BgOff);
    InitEnumList(pParaList, pSelectPara);
}

void ComboBox::InitEnumList(QStringList pList, QString pValue)
{
    m_EnumList = pList;
    setCurrentText(pValue);
}

void ComboBox::SetState(int pState)
{
    if(pState < 0 && pState > 2)
        pState = 0;
    m_State = (COMBOX_STATE)pState;
    m_CurPixmap = m_Pixmap[pState];
    update();
}

bool ComboBox::setCurrentText(QString pText)
{
    if(m_EnumList.isEmpty())
    {
        setCurrentIndex(-1);
        return false;
    }
    if(!pText.isEmpty())
    {
        for(int i=0;i<m_EnumList.count();i++)
        {
            if(pText.compare(m_EnumList.at(i), Qt::CaseInsensitive) == 0)
            {
                setCurrentIndex(i);
                return true;
            }
        }
    }
    setCurrentIndex(0);
    return false;
}

void ComboBox::setCurrentIndex(int pIndex)
{
    if(pIndex != m_CurIndex)
    {
        if(pIndex >= m_EnumList.count())
            m_CurIndex = m_EnumList.count()-1;
        else
            m_CurIndex = pIndex;
        emit sChanged(this);
    }
    update();
}

QString ComboBox::currentText()
{
    if(m_CurIndex >= m_EnumList.count())
        m_CurIndex = m_EnumList.count()-1;
    if(m_CurIndex >= 0)
        return m_EnumList.at(m_CurIndex);
    return QString();
}

int ComboBox::currentIndex()
{
    if(m_CurIndex >= m_EnumList.count())
        m_CurIndex = m_EnumList.count()-1;
    return m_CurIndex;
}

void ComboBox::mousePressEvent(QMouseEvent* e)
{
    if(m_AreaRect[MinusOn].contains(e->pos()))
    {
        setCurrentIndex(m_CurIndex-1);
        SetState(MinusOn);
    }
    else if(m_AreaRect[AddOn].contains(e->pos()))
    {
        setCurrentIndex(m_CurIndex+1);
        SetState(AddOn);
    }
    else
    {
        SetState(BgOff);
    }
}

void ComboBox::mouseReleaseEvent(QMouseEvent *e)
{
    SetState(BgOff);
}

void ComboBox::focusInEvent(QFocusEvent *e)
{
    SetState(BgOff);
}

void ComboBox::focusOutEvent(QFocusEvent *e)
{
    SetState(BgOff);
}

void ComboBox::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.drawPixmap(rect(), m_CurPixmap);

    p.setFont(m_Font);
    p.setBrush(m_Color);
    p.setPen(m_Color);
    p.drawText(rect().adjusted(25, 0, -25, 0), Qt::AlignHCenter|Qt::AlignVCenter, currentText());
}

MyComboBox::MyComboBox(QWidget *parent)
    :QComboBox(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentIndexChanged(int)));
}

MyComboBox::~MyComboBox()
{
    QObject::disconnect(this, SIGNAL(currentIndexChanged(int)), 0, 0);
}

void MyComboBox::SetCurrentIndex(int pIndex)
{
    setCurrentIndex(pIndex);
    slot_CurrentIndexChanged(pIndex);
}

void MyComboBox::slot_CurrentIndexChanged(int index)
{
    QString tmpMessage = this->property("currentIndexChanged").toString().arg(itemText(index));
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage);
}

ComboListView::ComboListView(QWidget* parent) :
    QListWidget(parent)
{
    setViewMode(QListWidget::ListMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

bool ComboListView::AddCombox(QComboBox* pCmb)
{
    if(m_cmboxs.contains(pCmb))
        return false;
    m_cmboxs.append(pCmb);
    return true;
}

bool ComboListView::DelCombox(QComboBox* pCmb)
{
    return m_cmboxs.removeOne(pCmb);
}

bool ComboListView::AddItem(QString pName, QVariant pData)
{
    if(m_fslist.contains(pName))
    {
        return false;
    }

    m_bchecked.append(false);
    m_data.append(pData);

    QListWidgetItem *item = new QListWidgetItem(pName);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    item->setData(Qt::UserRole+1, pData);
    insertItem(model()->rowCount(), item);

    QCheckBox* box = new QCheckBox(pName);
    setItemWidget(item,  box);
    //???”??—°???¨??±?–????€
    QObject::connect(box, SIGNAL(stateChanged(int)), this, SLOT(set_select_item(int)));

    m_checklist.append(box);
    m_fslist.append(pName);

    return true;
}

bool ComboListView::DelItem(QString pName)
{
    int index = m_fslist.indexOf(pName);
    if(index < 0)
    {
        return false;
    }

    m_bchecked.removeAt(index);
    m_data.removeAt(index);

    QListWidgetItem *item = takeItem(index);
    if(item)
    {
        QCheckBox* box = VAR_CAST<QCheckBox*>(itemWidget(item));
        removeItemWidget(item);
        if(box)
            box->deleteLater();
        delete item;
    }

    m_checklist.removeAt(index);
    m_fslist.removeAt(index);
    return true;
}

void ComboListView::GetSelectDatas(QList<QVariant> &data)
{
    for( int i=0; i<m_bchecked.size(); i++)
    {
        if( m_bchecked.at(i)== true)
            data.append( m_data.at(i) );
    }
}

//QCheckBox??—°???????ì
void ComboListView::set_select_item(int state)
{
    QCheckBox*  box = qobject_cast<QCheckBox*>(sender());

    int off = -1;
    for( off = 0;off<m_checklist.size(); off++)
    {
        if( box == m_checklist.at(off) )
        {
            m_bchecked[off] = (box->checkState() ==  Qt::Checked) ? true : false;
            break;
        }
    }

    if(off < m_cmboxs.count())
    {
        QString strfs;
        for( int i=0; i<m_bchecked.size(); i++)
        {
            if( m_bchecked.at(i)== true)
            {
                strfs += m_fslist.at(i);
                strfs += ",";
            }
        }
        m_cmboxs.at(off)->setEditText( strfs );
    }
}

ComboList::ComboList(QWidget* parent) :
    QComboBox(parent)
{

}

void ComboList::SetModel(ComboListView *pView, bool pFirst)
{
    if(pView->AddCombox(this))
    {
        setModel(pView->model());    //??÷?Model
        if(pFirst)
            setView(pView);               //??÷? ”??
        setEditable(true);            //?à??–?“??′QComboBox
    }
}


SpinEdit::SpinEdit(int pValue, int minValue, int maxValue, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    QSize tmpSize = QSize(85, 25);
    setFixedSize(tmpSize);

    m_AreaRect[MinusOn] = QRect(5, 3, 20, 19);
    m_AreaRect[NumOn] = QRect(25, 3, 35, 19);
    m_AreaRect[AddOn] = QRect(60, 3, 20, 19);

    m_Pixmap[BgOff] = QPixmap(":/SetBar/rc/SetBar/spin-off.png").scaled(tmpSize);
    m_Pixmap[NumOn] = QPixmap(":/SetBar/rc/SetBar/spin-on.png").scaled(tmpSize);
    m_Pixmap[MinusOn] = QPixmap(":/SetBar/rc/SetBar/spinminus-on.png").scaled(tmpSize);
    m_Pixmap[AddOn] = QPixmap(":/SetBar/rc/SetBar/spinadd-on.png").scaled(tmpSize);

    m_Color = QColor(250, 250, 250, 240);
    m_Font = QFont("Arial", 12, QFont::DemiBold);

    connect(&m_FlowTimer, SIGNAL(timeout()), this, SLOT(slot_FlowChange()));
    m_FlowTimer.setInterval(100);

    SetState(BgOff);

    m_MinValue = minValue;
    m_MaxValue = maxValue;
    setValue(pValue);
}

void SpinEdit::SetState(int pState)
{
    if(pState < 0 && pState > 3)
        pState = 0;
    m_State = (SPIN_STATE)pState;
    if(m_State == MinusOn || m_State == AddOn)
        m_FlowTimer.start();
    else if(m_State == BgOff || m_State == NumOn)
        m_FlowTimer.stop();
    m_CurPixmap = m_Pixmap[pState];
    update();
}

void SpinEdit::setValue(int pValue)
{
    int oldValue = m_Value;
    if(pValue < m_MinValue)
        m_Value = m_MinValue;
    else if(pValue > m_MaxValue)
        m_Value = m_MaxValue;
    else
        m_Value = pValue;
    if(m_Value != oldValue)
        emit sChanged(this);
    update();
}

void SpinEdit::slot_FlowChange()
{
    if(m_State == MinusOn)
    {
        setValue(m_Value-1);
    }
    else if(m_State == AddOn)
    {
        setValue(m_Value+1);
    }
}

void SpinEdit::mousePressEvent(QMouseEvent* e)
{
    for(int i=1;i<4;i++)
    {
        if(m_AreaRect[i].contains(e->pos()))
        {
            if(i == MinusOn)
            {
                setValue(m_Value-1);
            }
            else if(i == AddOn)
            {
                setValue(m_Value+1);
            }
            SetState(i);
            return;
        }
    }
    SetState(BgOff);
}

void SpinEdit::mouseReleaseEvent(QMouseEvent *e)
{
    SetState(BgOff);
}

void SpinEdit::keyPressEvent( QKeyEvent *e )
{
    if(m_State == NumOn)
    {
        if(e->key() >= Qt::Key_0 && e->key() >= Qt::Key_9)
        {

        }
        else if(e->key() == Qt::Key_Enter)
        {
            //SetValue();
        }
    }
}

void SpinEdit::focusInEvent(QFocusEvent *e)
{
    SetState(BgOff);
}

void SpinEdit::focusOutEvent(QFocusEvent *e)
{
    SetState(BgOff);
}

void SpinEdit::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.drawPixmap(rect(), m_CurPixmap);

    p.setFont(m_Font);
    p.setBrush(m_Color);
    p.setPen(m_Color);
    p.drawText(m_AreaRect[NumOn], Qt::AlignHCenter|Qt::AlignVCenter, QString("%1").arg(m_Value));
}


DateTimeEdit::DateTimeEdit(QDateTime pDateTime, QWidget *parent) :
    QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    m_FocusAreaIndex = -1;
    QSize tmpSize = QSize(226, 25);
    setFixedSize(tmpSize);

    connect(&m_FlowTimer, SIGNAL(timeout()), this, SLOT(slot_FlowChange()));
    m_FlowTimer.setInterval(100);

    m_AreaRect[MinusOn] = QRect(0, 0, 20, 20);
    m_AreaRect[AddOn] = QRect(tmpSize.width()-20, 0, 20, 20);
    m_AreaRect[DateYear] = QRect(24, 0, 30, 25);
    m_AreaRect[DateMonth] = m_AreaRect[DateYear].adjusted(32, 0, 32, 0);
    m_AreaRect[DateDay] = m_AreaRect[DateMonth].adjusted(32, 0, 32, 0);
    m_AreaRect[TimeHour] = m_AreaRect[DateDay].adjusted(32, 0, 32, 0);
    m_AreaRect[TimeMinute] = m_AreaRect[TimeHour].adjusted(32, 0, 32, 0);
    m_AreaRect[TimeSecond] = m_AreaRect[TimeMinute].adjusted(32, 0, 32, 0);

    m_Pixmap[BgOff] = QPixmap(":/SetBar/rc/SetBar/time-edit.png").scaled(tmpSize);
    m_Pixmap[MinusOn] = QPixmap(":/SetBar/rc/SetBar/time-minus-on.png").scaled(tmpSize);
    m_Pixmap[AddOn] = QPixmap(":/SetBar/rc/SetBar/time-add-on.png").scaled(tmpSize);
    m_FocusPixmap = QPixmap(":/SetBar/rc/SetBar/ip-light.png").scaled(m_AreaRect[DateYear].size());

    m_Color = QColor(250, 250, 250, 240);
    m_FocusColor = QColor(0, 24, 124, 255);
    m_Font = QFont("Arial", 12, QFont::DemiBold);
    m_FocusFont = QFont("Arial", 13, QFont::Bold);

    SetState(BgOff);
    SetDateTime(pDateTime);
}

void DateTimeEdit::SetState(int pState)
{
    if(pState < (int)BgOff && pState > (int)AddOn)
        pState = (int)BgOff;
    m_State = (DATETIME_STATE)pState;
    if(m_State == MinusOn || m_State == AddOn)
        m_FlowTimer.start();
    else
        m_FlowTimer.stop();
    m_CurPixmap = m_Pixmap[pState];
    update();
}

void DateTimeEdit::SetDateTime(QDateTime pDateTime)
{
    m_DateTime[0] = pDateTime.date().year();
    m_DateTime[1] = pDateTime.date().month();
    m_DateTime[2] = pDateTime.date().day();
    m_DateTime[3] = pDateTime.time().hour();
    m_DateTime[4] = pDateTime.time().minute();
    m_DateTime[5] = pDateTime.time().second();
    update();
}

QDateTime DateTimeEdit::GetDateTime()
{
    QDate tmpDate(m_DateTime[0], m_DateTime[1], m_DateTime[2]);
    QTime tmpTime(m_DateTime[3], m_DateTime[4], m_DateTime[5]);
    return QDateTime(tmpDate, tmpTime);
}

void DateTimeEdit::SetAreaTime(int index, quint8 pNum)
{
    if(index > (int)TimeSecond || index < (int)DateYear)
        return;
    index -= (int)DateYear;
    if(m_DateTime[index] != pNum)
    {
        m_DateTime[index] = pNum;
        update();
    }
}

quint8 DateTimeEdit::GetAreaTime(int pAreaIndex)
{
    return m_DateTime[pAreaIndex-DateYear];
}

void DateTimeEdit::GotoArea(int index)
{
    if(m_FocusAreaIndex != index)
    {
        m_FocusAreaIndex = index;
        update();
    }
}

void DateTimeEdit::MoveAreaRight()
{
    int tmpIndex = m_FocusAreaIndex;
    tmpIndex++;
    if(tmpIndex > (int)TimeSecond)
        tmpIndex = (int)DateYear;
    GotoArea(tmpIndex);
}

void DateTimeEdit::slot_FlowChange()
{
    if(m_State == MinusOn)
    {
        SetAreaTime(m_FocusAreaIndex, GetAreaTime(m_FocusAreaIndex)-1);
    }
    else if(m_State == AddOn)
    {
        SetAreaTime(m_FocusAreaIndex, GetAreaTime(m_FocusAreaIndex)+1);
    }
}

void DateTimeEdit::mousePressEvent(QMouseEvent* e)
{
    if(m_AreaRect[MinusOn].contains(e->pos()))
    {
        SetState(MinusOn);
        SetAreaTime(m_FocusAreaIndex, GetAreaTime(m_FocusAreaIndex)-1);
    }
    else if(m_AreaRect[AddOn].contains(e->pos()))
    {
        SetState(AddOn);
        SetAreaTime(m_FocusAreaIndex, GetAreaTime(m_FocusAreaIndex)+1);
    }
    else
    {
        for(int i=DateYear;i<=TimeSecond;i++)
        {
            if(m_AreaRect[i].contains(e->pos()))
            {
                GotoArea(i);
                return;
            }
        }
        GotoArea(-1);
    }
}

void DateTimeEdit::mouseReleaseEvent(QMouseEvent *e)
{
    SetState(BgOff);
}

void DateTimeEdit::focusInEvent(QFocusEvent *e)
{
    SetState(BgOff);
    GotoArea(DateYear);
}

void DateTimeEdit::focusOutEvent(QFocusEvent *e)
{
    SetState(BgOff);
    GotoArea(-1);
    emit sChanged(this);
}

void DateTimeEdit::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.drawPixmap(rect(), m_CurPixmap);
    if(m_FocusAreaIndex >= DateYear && m_FocusAreaIndex <= TimeSecond)
    {
        p.drawPixmap(m_AreaRect[m_FocusAreaIndex], m_FocusPixmap);
    }
    QString tmpData;
    for(int i=DateYear;i<=TimeSecond;i++)
    {
        quint8 tmpInt = GetAreaTime(i);
        if(i == 0)  //>@?í??à??? ?￡¨??”??????? ?
        {
            tmpData = QString("%1%2%3%4").arg(tmpInt/1000)
                                         .arg(tmpInt/100%10)
                                         .arg(tmpInt/10%10)
                                         .arg(tmpInt%10);
        }
        else
        {
            tmpData = QString("%1%2").arg(tmpInt/10%10)
                                     .arg(tmpInt%10);
        }
        p.save();
        if(i == m_FocusAreaIndex)
        {
            p.setFont(m_FocusFont);
            p.setBrush(m_FocusColor);
            p.setPen(m_FocusColor);
        }
        else
        {
            p.setFont(m_Font);
            p.setBrush(m_Color);
            p.setPen(m_Color);
        }
        p.drawText(m_AreaRect[i], Qt::AlignHCenter|Qt::AlignVCenter, tmpData);
        p.restore();
    }
}

MyTimeEdit::MyTimeEdit(QWidget *parent)
    :QTimeEdit(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(timeChanged(QTime)), this, SLOT(slot_TimeChanged(QTime)));
}

MyTimeEdit::~MyTimeEdit()
{
    QObject::disconnect(this, SIGNAL(slot_TimeChanged(QTime)), 0, 0);
}

void MyTimeEdit::slot_TimeChanged(QTime pTime)
{

    quint32 tmpTime = pTime.hour()*3600000 + pTime.minute()*60000 + pTime.second()*1000;
    QString tmpMessage = this->property("valueChanged").toString().arg(tmpTime);
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage);
}

MyDateTimeEdit::MyDateTimeEdit(QWidget *parent)
    :QDateTimeEdit(parent),
    mQesp(0)
{
    QObject::connect(this, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(slot_DateTimeChanged(QDateTime)));
}

MyDateTimeEdit::~MyDateTimeEdit()
{
    QObject::disconnect(this, SIGNAL(dateTimeChanged(QDateTime)), 0, 0);
}

void MyDateTimeEdit::slot_DateTimeChanged(QDateTime pDateTime)
{
    QString tmpMessage = this->property("dateTimeChanged").toString().arg(pDateTime.toString("yyyy-mm-dd-hh-mm-ss-zzz"));
    //>@??à?????
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    mQesp->ProcessCommand(0, tmpMessage);
}

ProgressBar::ProgressBar(int pValue, QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_DeleteOnClose);
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

    setFocusPolicy(Qt::NoFocus);
    InitBrush();

    setProgress(pValue);
}

void ProgressBar::InitBrush()
{
    m_BgPenColor = QColor(133, 209, 145, 240);
    m_BgBrush = QBrush(m_BgPenColor);

    m_TGlassPenColor = QColor(20, 62, 117, 200);
    m_TGlassBrush = QBrush(m_TGlassPenColor);

    m_ContentFont = QFont("Arial", 15, QFont::DemiBold);
    m_ContentColor = QColor(221, 222, 223, 255);
}

void ProgressBar::setProgress(int pValue)
{
    m_ValidRect = QRect(0,0,((qreal)pValue/100.0)*width(),height());
    update();
}

void ProgressBar::setMessage(QString pMsg)
{
    m_Content = pMsg;
    update();
}

void ProgressBar::setErrorStatus(int pErrorLevel)
{
    m_BgPenColor.setRed(133+pErrorLevel);
    m_BgBrush = QBrush(m_BgPenColor);
    update();
}

void ProgressBar::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.setBrush(m_BgBrush);
    p.setPen(m_BgPenColor);
    p.drawRect(m_ValidRect);

    //>@?ê÷??￡??–???
    p.setPen(m_TGlassPenColor);
    p.setBrush(m_TGlassBrush);
    p.drawRect(QRect(0,0,m_ValidRect.width(),m_ValidRect.height()/2.0));

    //>@?ê÷????÷
    p.setFont(m_ContentFont);
    p.setBrush(m_ContentColor);
    p.setPen(m_ContentColor);
    p.drawText(rect(), Qt::AlignHCenter|Qt::AlignVCenter, m_Content);
}

bool ProgressBar::event(QEvent *e)
{
    if(e->type() == QEvent::HoverEnter)
    {
        m_BgPenColor.setBlue(195);
        m_BgBrush = QBrush(m_BgPenColor);
        update();
    }
    else if(e->type() == QEvent::HoverLeave)
    {
        m_BgPenColor.setBlue(145);
        m_BgBrush = QBrush(m_BgPenColor);
        update();
    }
    return QWidget::event(e);
}

ThreadMessageBox::ThreadMessageBox(MSG_LEVEL type, const QString &title, const QString &message):
    m_type(type), m_title(title), m_message(message)
{
}

void ThreadMessageBox::show(MSG_LEVEL type, const QString &title, const QString &message)
{
    QEventLoop eventLoop;
    ThreadMessageBox* messageBox = new ThreadMessageBox(type, title, message);
    connect(messageBox, SIGNAL(destroyed()), &eventLoop, SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
}

void ThreadMessageBox::readyShow(void)
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0, this, SLOT(onShow()));
}

void ThreadMessageBox::onShow(void)
{
    if(m_type == LEVEL_INFO)
        QMessageBox::information(NULL, m_title, m_message);
    else if(m_type == LEVEL_WARN)
        QMessageBox::warning(NULL, m_title, m_message);
    else if(m_type == LEVEL_ERROR)
        QMessageBox::critical(NULL, m_title, m_message);
    else if(m_type == LEVEL_QUESTION)
        QMessageBox::question(NULL, m_title, m_message);
    else
        QMessageBox::information(NULL, m_title, m_message);

    this->deleteLater();
}

/**
//>@UpQTreeWidget
*/
UpQTreeWidget::UpQTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    setAlternatingRowColors(true);
}

UpQTreeWidget::~UpQTreeWidget()
{
}

void UpQTreeWidget::ClearAllItem()
{
    if(topLevelItemCount() <= 0)
        return;
    QTreeWidgetItem* tmpItem;
    for(int i=topLevelItemCount()-1;i>=0;i--)
    {
        tmpItem = takeTopLevelItem(i);
        if(tmpItem)
        {
            ClearTreeItem(tmpItem);
            delete tmpItem;
        }
    }
}

//>@????parentItemμ?à?”–?”??μ?
void UpQTreeWidget::ClearTreeItem(QTreeWidgetItem *parentItem)
{
    if(parentItem == 0)
        return;
    QList<QTreeWidgetItem*> ItemList = parentItem->takeChildren();
    if(ItemList.count() <= 0)
        return;
    QTreeWidgetItem* tmpItem;
    for(int i=ItemList.count()-1;i>=0;i--)
    {
        tmpItem = ItemList.takeAt(i);
        if(tmpItem)
        {
            ClearTreeItem(tmpItem);
            delete tmpItem;
        }
    }
}

//>@?μ????item
QTreeWidgetItem *UpQTreeWidget::DelTreeItem(QTreeWidgetItem *pItem)
{
    if(pItem == 0)
        return 0;
    QTreeWidgetItem* tmpParentItem = pItem->parent();
    if(tmpParentItem == 0)
        return 0;
    ClearTreeItem(pItem);
    tmpParentItem->removeChild(pItem);
    delete pItem;
    return tmpParentItem;
}

void UpQTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    emit sDragEnter(event);
}

void UpQTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    emit sDragLeave(event);
}

void UpQTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    emit sDragMove(event);
}

void UpQTreeWidget::dropEvent(QDropEvent *event)
{
    emit sDrop(event);
}

void UpQTreeWidget::startDrag(Qt::DropActions actions)
{
    emit sStartDrag(actions);
}
//>@--UpMouseQTreeWidget
UpMouseQTreeWidget::UpMouseQTreeWidget(QWidget *parent):
    UpQTreeWidget(parent)
{
    setAlternatingRowColors(true);
}

UpMouseQTreeWidget::~UpMouseQTreeWidget()
{

}

void UpMouseQTreeWidget::mousePressEvent(QMouseEvent *event)
{
    emit sMousePressEvent(event);
}
/**
//>@UpQTableWidget
*/
UpQTableWidget::UpQTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    setAlternatingRowColors(true);
}

UpQTableWidget::~UpQTableWidget()
{
}

void UpQTableWidget::ClearAllItem()
{
    if(rowCount())
    {
        for(int j=rowCount()-1;j>=0;j--)
        {
            removeRow(j);
        }
    }
}

void UpQTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    emit sDragEnter(event);
}

void UpQTableWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    emit sDragLeave(event);
}

void UpQTableWidget::dragMoveEvent(QDragMoveEvent *event)
{
    emit sDragMove(event);
}

void UpQTableWidget::dropEvent(QDropEvent *event)
{
    emit sDrop(event);
}

void UpQTableWidget::startDrag(Qt::DropActions actions)
{
    emit sStartDrag(actions);
}

/**
//>@UpQListWidget
*/
UpQListWidget::UpQListWidget(QWidget *parent):
    QListWidget(parent)
{
}

UpQListWidget::~UpQListWidget()
{
}

void UpQListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    emit sDragEnter(event);
}

void UpQListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    emit sDragLeave(event);
}

void UpQListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    emit sDragMove(event);
}

void UpQListWidget::dropEvent(QDropEvent *event)
{
    emit sDrop(event);
}

void UpQListWidget::startDrag(Qt::DropActions actions)
{
    emit sStartDrag(actions);
}
//>@--UpSelecQListWidget
UpSelecQListWidget::UpSelecQListWidget(QWidget *parent):
    UpQListWidget(parent)
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_ItemClicked(QListWidgetItem*)));
}

UpSelecQListWidget::~UpSelecQListWidget()
{
    disconnect(this, SIGNAL(itemClicked(QListWidgetItem*)), 0, 0);
}

void UpSelecQListWidget::slot_ItemClicked(QListWidgetItem* pItem)
{
    QAbstractItemView::SelectionMode mode = this->selectionMode();
    switch(mode)
    {
        case SingleSelection:
        {
            if(pItem->checkState() == Qt::Checked)
            {
                for(int i=0;i<this->count();i++)
                {
                    QListWidgetItem* item = this->item(i);
                    item->setCheckState(Qt::Unchecked);
                }
                pItem->setCheckState(Qt::Checked);
            }
            break;
        }
        default:break;
    }
}
/**
//>@UpQGraphicsView******************************************************
*/
UpQGraphicsView::UpQGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    m_FitInView = true;
}

UpQGraphicsView::~UpQGraphicsView()
{

}

void UpQGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
}

void UpQGraphicsView::resizeEvent(QResizeEvent *event)
{
//    QGraphicsView::resizeEvent(event);
    if(m_FitInView)
    {
        fitInView(sceneRect(), Qt::IgnoreAspectRatio);
        sceneRect().setSize(size());
    }
}

void UpQGraphicsView::dropEvent(QDropEvent *event)
{
    emit sDropEvent(event);
}

#ifndef QT_V453

QListWidgetItem* GetListItem(QListWidget *pListWidget, QString pPath, QString pName)
{
    if(pListWidget)
    {
        QList<QListWidgetItem*> tmpList = pListWidget->findItems(pName, Qt::MatchExactly);
        for(int i=0;i<tmpList.count();i++)
        {
            QListWidgetItem *tmpItem = tmpList.at(i);
            if(tmpItem && (!tmpItem->data(D_LISTITEM_PATH).toString().compare(pPath, Qt::CaseInsensitive)))
            {
                return tmpItem;
            }
        }
    }
    return (QListWidgetItem*)0;
}

QListWidgetItem* AddListItem(QListWidget *pListWidget, QString pPath, QString pName)
{
    if(!pListWidget)
        return (QListWidgetItem*)0;
    //>@??÷?÷???????
    QListWidgetItem *tmpItem = GetListItem(pListWidget, pPath, pName);
    if(tmpItem)
        return tmpItem;
    tmpItem = new QListWidgetItem(pName);
    tmpItem->setData(D_LISTITEM_PATH, pPath);
    tmpItem->setData(D_LISTITEM_NAME, pName);
    pListWidget->addItem(tmpItem);
    return tmpItem;
}

bool DelListItem(QListWidget *pListWidget, QListWidgetItem *pItem)
{
    if(pListWidget == 0)
        return false;
    if(!pItem)
        return false;
    int row = pListWidget->row(pItem);
    pListWidget->takeItem(row);
    delete pItem;
    return true;
}

bool DelListItem(QListWidget *pListWidget, QString pPath, QString pName)
{
    if(pListWidget == 0)
        return false;
    QListWidgetItem *tmpItem = GetListItem(pListWidget, pPath, pName);
    return DelListItem(pListWidget, tmpItem);
}

ListWidget::ListWidget(QWidget *parent):
    UpQListWidget(parent)
{
    m_SliderDirection = HVBoth;
    m_AlignEnable = false;
    m_AlignMode = 1;
    m_AnimatEnable = false;
}

void ListWidget::slot_ItemCountChanged(QSizeF pMatrix)
{
    if(pMatrix.isEmpty())  //>@?à?????? ?”?isnull
    {
        if(viewMode() == ListMode)
            pMatrix = QSize(1, count());
        else if(viewMode() == IconMode)
            pMatrix = QSize(count(), 1);
    }
    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        m_HScrollValue.mMinNum = 0;
        m_HScrollValue.mMaxNum = m_ItemSize.width()*pMatrix.width() - width();
        m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        m_VScrollValue.mMinNum = 0;
        m_VScrollValue.mMaxNum = m_ItemSize.height()*pMatrix.height() - height();
        m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
    }
}

void ListWidget::SetAlignMode(bool pEnable, int pMode, bool pAnimat)
{
    m_AlignEnable = pEnable;
    m_AlignMode = pMode;
    m_AnimatEnable = pAnimat;
}

/*
    pMinMatrix÷??ó–°––?– ?￡¨??÷?μ?“???÷μ????
    pMatrix÷? μ? μ?––?– ?
*/
void ListWidget::InitMovePara(DIRECTION pDirection, QSizeF pMinMatrix, QSizeF pMatrix)
{
    m_SliderDirection = pDirection;
    m_MinMatrix = pMinMatrix;
    int itemcount = count();
    if(itemcount < 1)
        return;
    if(pMatrix.isEmpty())  //>@?à?????? ?”?isnull
    {
        if(viewMode() == ListMode)
            pMatrix = QSize(1, count());
        else if(viewMode() == IconMode)
            pMatrix = QSize(count(), 1);
    }
    QSize tmpSize = size();
    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        qreal w = tmpSize.width() / pMinMatrix.width();
        setHorizontalScrollMode(ScrollPerPixel);
        m_HScrollBar = horizontalScrollBar();
        m_HScrollValue.mMinNum = 0;
        m_HScrollValue.mMaxNum = w*pMatrix.width() - tmpSize.width();
        m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
        //>@m_RelDiff±ì ? ?±í????1???òà?￡¨scroolbar–?“?“???????
        m_HRelDiff = 1;
        m_ItemSize.setWidth(w);

        if(m_AlignEnable)
        {
            if(m_AnimatEnable)
            {
                m_HAnimation = new QPropertyAnimation(m_HScrollBar.data(), "value");
                m_HAnimation->setEasingCurve(QEasingCurve::OutBack);
            }
        }
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        qreal h = tmpSize.height() / pMinMatrix.height();
        setVerticalScrollMode(ScrollPerPixel);
        m_VScrollBar = verticalScrollBar();
        m_VScrollValue.mMinNum = 0;
        m_VScrollValue.mMaxNum = h*pMatrix.height() - tmpSize.height();
        m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
        m_VRelDiff = 1;
        m_ItemSize.setHeight(h);

        if(m_AlignEnable)
        {
            if(m_AnimatEnable)
            {
                m_VAnimation = new QPropertyAnimation(m_VScrollBar.data(), "value");
                m_VAnimation->setEasingCurve(QEasingCurve::OutBack);
            }
        }
    }
}

void ListWidget::mousePressEvent(QMouseEvent *e)
{
    m_IsMoving = false;

    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        if(!m_HScrollBar.isNull())
        {
            m_HScrollValue.setnum(m_HScrollBar.data()->value());
        }
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        if(!m_VScrollBar.isNull())
        {
            m_VScrollValue.setnum(m_VScrollBar.data()->value());
        }
    }

    m_MousePosBk = e->globalPos();   //>@“???“? ?”?globalPos￡¨?ò‘ú?·???÷?ò?°pos ±?????÷??°￡
    m_MouseStatus = QEvent::MouseButtonPress;

    QListWidget::mousePressEvent(e);
}

void ListWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_MouseStatus == QEvent::MouseButtonPress)
    {
        m_IsMoving = true;
        QPoint new_pos = e->globalPos();
        int diff = 0;

        if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
        {
            diff = m_MousePosBk.x() - new_pos.x();
            if(diff != 0)
            {
                if(!m_HScrollBar.isNull())
                {
                    m_HScrollValue.setnum(m_HScrollValue.mNum+diff*m_HRelDiff);
                    m_HScrollBar.data()->setValue(m_HScrollValue.mNum);
                }
            }
        }
        if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
        {
            diff = m_MousePosBk.y() - new_pos.y();
            if(diff != 0)
            {
                if(!m_VScrollBar.isNull())
                {
                    m_VScrollValue.setnum(m_VScrollValue.mNum+diff*m_VRelDiff);
                    m_VScrollBar.data()->setValue(m_VScrollValue.mNum);
                }
            }
        }

        m_MousePosBk = new_pos;
    }
    else
    {
        QListWidget::mouseMoveEvent(e);
    }
}

void ListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_MouseStatus = QEvent::MouseButtonRelease;
    if(m_IsMoving)
    {
        m_IsMoving = false;
        QListWidgetItem *tmpItem;
        if(m_AlignMode == 1)
        {
            tmpItem = itemAt(rect().topLeft());
        }
        else if(m_AlignMode == 2)
        {
            tmpItem = itemAt(rect().center());
        }
        else
        {
            tmpItem = itemAt(rect().topLeft());
        }
        int tmpRow = row(tmpItem);
        if(m_AlignEnable)
        {
            if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
            {
                if(m_AnimatEnable)
                {
                    m_HAnimation->setStartValue(m_VScrollBar.data()->value());
                    if(m_AlignMode == 1)
                    {
                        m_HAnimation->setEndValue(tmpRow * m_ItemSize.height());
                    }
                    else if(m_AlignMode == 2)
                    {
                        m_HAnimation->setEndValue((tmpRow-1) * m_ItemSize.height()  + m_ItemSize.height()*1.5-height()/2.0);
                    }
                    m_HAnimation->setDuration(200);
                    m_HAnimation->start();
                }
                else
                {
                    if(m_AlignMode == 1)
                    {

                    }
                    else if(m_AlignMode == 2)
                    {
                        SetListCurRow(this, tmpRow);
                    }
                }
            }
            if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
            {
                if(m_AnimatEnable)
                {
                    m_VAnimation->setStartValue(m_VScrollBar.data()->value());
                    if(m_AlignMode == 1)
                    {
                        m_VAnimation->setEndValue(tmpRow * m_ItemSize.height());
                    }
                    else if(m_AlignMode == 2)
                    {
                        m_VAnimation->setEndValue((tmpRow-1)*m_ItemSize.height() + m_ItemSize.height()*1.5-height()/2.0);
                    }
                    m_VAnimation->setDuration(200);
                    m_VAnimation->start();
                }
                else
                {
                    if(m_AlignMode == 1)
                    {

                    }
                    else if(m_AlignMode == 2)
                    {
                        SetListCurRow(this, tmpRow);
                    }
                }
            }
        }
        emit sItemChanged(tmpRow);
    }
    QListWidget::mouseReleaseEvent(e);
}

QListWidgetItem* GetListCurItem(QListWidget *pList)
{
    if(!pList)
        return (QListWidgetItem*)0;
    return pList->itemAt(pList->rect().center());
}

int GetListCurRow(QListWidget *pList)
{
    if(!pList)
        return -1;
#if 1
    return pList->currentRow();
#else
    QListWidgetItem *current = pList->itemAt(pList->rect().center());
    if(!current)
        return -1;
    return pList->row(current);
#endif
}

bool SetListCurRow(QListWidget *pList, int pRow, bool pMiddle)
{
    if(!pList || pRow == -1)
        return false;
    if(pMiddle)
        pList->setCurrentRow(pRow-1);
    else
        pList->setCurrentRow(pRow);
    pList->scrollTo(pList->currentIndex(), QAbstractItemView::PositionAtCenter);
    if(pMiddle)
        pList->setCurrentRow(pRow);
    return true;
}

TimeListWidget::TimeListWidget(QWidget *parent):
    ListWidget(parent)
{
    //InitMovePara(Vertical);
}

void TimeListWidget::slot_Plus()
{
    SetListCurRow(this, currentRow()+1);
}

void TimeListWidget::slot_Minus()
{
    SetListCurRow(this, currentRow()-1);
}


TableWidget::TableWidget(QWidget *parent):
    UpQTableWidget(parent)
{
    setAlternatingRowColors(false);
    m_SliderDirection = HVBoth;
}

void TableWidget::slot_ItemCountChanged(QSizeF pMatrix)
{
    if(pMatrix.isEmpty())  //>@?à?????? ?”?isnull
    {
        pMatrix = QSizeF(rowCount(), columnCount());
    }
    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        m_HScrollValue.mMinNum = 0;
        m_HScrollValue.mMaxNum = m_ItemSize.width()*pMatrix.width() - width();
        m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        m_VScrollValue.mMinNum = 0;
        m_VScrollValue.mMaxNum = m_ItemSize.height()*pMatrix.height() - height();
        m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
    }
}

void TableWidget::InitMovePara(DIRECTION pDirection, QSizeF pMinMatrix, QSizeF pMatrix)
{
    m_SliderDirection = pDirection;
    m_MinMatrix = pMinMatrix;
    if(pMatrix.isEmpty())  //>@?à?????? ?”?isnull
    {
        pMatrix = QSizeF(columnCount(), rowCount());
    }
    QSize tmpSize = size();
    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        qreal w = tmpSize.width() / pMinMatrix.width();
        setHorizontalScrollMode(ScrollPerPixel);
        m_HScrollBar = horizontalScrollBar();
        m_HScrollValue.mMinNum = 0;
        m_HScrollValue.mMaxNum = w*pMatrix.width() - tmpSize.width();
        m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
        //>@m_RelDiff±ì ? ?±í????1???òà?￡¨scroolbar–?“?“???????
        m_HRelDiff = 1;
        m_ItemSize.setWidth(w);
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        qreal h = tmpSize.height() / pMinMatrix.height();
        setVerticalScrollMode(ScrollPerPixel);
        m_VScrollBar = verticalScrollBar();
        m_VScrollValue.mMinNum = 0;
        m_VScrollValue.mMaxNum = h*pMatrix.height() - tmpSize.height();
        m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
        m_VRelDiff = 1;
        m_ItemSize.setHeight(h);
    }
}

void TableWidget::mousePressEvent(QMouseEvent *e)
{
    m_IsMoving = false;

    if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
    {
        if(!m_HScrollBar.isNull())
        {
            m_HScrollValue.setnum(m_HScrollBar.data()->value());
        }
    }
    if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
    {
        if(!m_VScrollBar.isNull())
        {
            m_VScrollValue.setnum(m_VScrollBar.data()->value());
        }
    }

    m_MousePosBk = e->globalPos();   //>@“???“? ?”?globalPos￡¨?ò‘ú?·???÷?ò?°pos ±?????÷??°￡
    m_MouseStatus = QEvent::MouseButtonPress;

    QTableWidget::mousePressEvent(e);
}

void TableWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_MouseStatus == QEvent::MouseButtonPress)
    {
        m_IsMoving = true;
        QPoint new_pos = e->globalPos();
        int diff = 0;

        if(m_SliderDirection == Horizontal || m_SliderDirection == HVBoth)
        {
            diff = m_MousePosBk.x() - new_pos.x();
            if(diff != 0)
            {
                if(!m_HScrollBar.isNull())
                {
                    m_HScrollValue.setnum(m_HScrollValue.mNum+diff*m_HRelDiff);
                    m_HScrollBar.data()->setValue(m_HScrollValue.mNum);
                }
            }
        }
        if(m_SliderDirection == Vertical || m_SliderDirection == HVBoth)
        {
            diff = m_MousePosBk.y() - new_pos.y();
            if(diff != 0)
            {
                if(!m_VScrollBar.isNull())
                {
                    m_VScrollValue.setnum(m_VScrollValue.mNum+diff*m_VRelDiff);
                    m_VScrollBar.data()->setValue(m_VScrollValue.mNum);
                }
            }
        }

        m_MousePosBk = new_pos;
    }
    QTableWidget::mouseMoveEvent(e);
}

void TableWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_MouseStatus = QEvent::MouseButtonRelease;
    if(m_IsMoving)
    {
        m_IsMoving = false;
    }
    QTableWidget::mouseReleaseEvent(e);
}

TreeWidget::TreeWidget(QWidget *parent):
    UpQTreeWidget(parent)
{
    setAlternatingRowColors(false);
}

void TreeWidget::InitMovePara(DIRECTION pDirection)
{
    m_SliderDirection = pDirection;
    if(m_SliderDirection == Horizontal)
    {
        m_HScrollBar = horizontalScrollBar();
        m_HScrollValue.mMinNum = 0;
        m_HScrollValue.mMaxNum = width()*2;
        m_HScrollBar.data()->setRange(m_HScrollValue.mMinNum, m_HScrollValue.mMaxNum);
        //>@m_RelDiff±ì ? ?±í????1???òà?￡¨scroolbar–?“?“???????
        m_HRelDiff = (qreal)(m_HScrollValue.mMaxNum-m_HScrollValue.mMinNum)/(qreal)(width())/2;
    }
    else
    {
        m_VScrollBar = verticalScrollBar();
        m_VScrollValue.mMinNum = 0;
        m_VScrollValue.mMaxNum = height()*2;
        m_VScrollBar.data()->setRange(m_VScrollValue.mMinNum, m_VScrollValue.mMaxNum);
        m_VRelDiff = (qreal)(m_VScrollValue.mMaxNum-m_VScrollValue.mMinNum)/(qreal)(height())/2;
    }
}

void TreeWidget::mousePressEvent(QMouseEvent *e)
{
    m_IsMoving = false;

    if(m_SliderDirection == Horizontal)
    {
        m_HScrollValue.setnum(m_HScrollBar.data()->value());
    }
    else
    {
        m_VScrollValue.setnum(m_VScrollBar.data()->value());
    }

    m_MousePosBk = e->globalPos();   //>@“???“? ?”?globalPos￡¨?ò‘ú?·???÷?ò?°pos ±?????÷??°￡
    m_MouseStatus = QEvent::MouseButtonPress;

    QTreeWidget::mousePressEvent(e);
}

void TreeWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_MouseStatus == QEvent::MouseButtonPress)
    {
        m_IsMoving = true;
        QPoint new_pos = e->globalPos();
        int diff = 0;

        if(m_SliderDirection == Horizontal)
        {
            diff = m_MousePosBk.x() - new_pos.x();
            if(diff != 0)
            {
                m_HScrollValue.setnum(m_HScrollValue.mNum+diff*m_HRelDiff);
                m_HScrollBar.data()->setValue(m_HScrollValue.mNum);
            }
        }
        else
        {
            diff = m_MousePosBk.y() - new_pos.y();
            if(diff != 0)
            {
                m_VScrollValue.setnum(m_VScrollValue.mNum+diff*m_VRelDiff);
                m_VScrollBar.data()->setValue(m_VScrollValue.mNum);
            }
        }

        m_MousePosBk = new_pos;
    }
    QTreeWidget::mouseMoveEvent(e);
}

void TreeWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_MouseStatus = QEvent::MouseButtonRelease;
    if(m_IsMoving)
    {
        m_IsMoving = false;
    }
    QTreeWidget::mouseReleaseEvent(e);
}

#endif

DieselInstrument::DieselInstrument(QWidget *parent) :
    QWidget(parent)
{
}

void DieselInstrument::init()
{
    mValue= 0;
    update();
}

void DieselInstrument::setValue(int newvalue)
{
    if(mValue == newvalue)
        return;
    mValue= newvalue;
    update();
}

void DieselInstrument::paintEvent(QPaintEvent *event)
{
    //??÷?÷?’?μ?????μ?
    static const QPoint hourHand[8] =
    {
        QPoint(4, -20),
        QPoint(4, -60),
        QPoint(1, -70),
        QPoint(1, -80),
        QPoint(-1, -80),
        QPoint(-1, -70),
        QPoint(-4, -60),
        QPoint(-4,-20)
    };
    // ??÷?—’?′
    QColor pointer(200,0,0);
    QColor hourColor(255, 255, 255);
    QColor minuteColor(255,122,29);
    QColor textColor(255, 255, 255);

    int side = qMin(width(), height());                //?°?ó–°÷μ
    QPainter tmPainter(this);
    tmPainter.setRenderHint(QPainter::Antialiasing, true);//????????
    tmPainter.setPen(Qt::black);
    tmPainter.setBrush(Qt::black);

    tmPainter.translate(width() / 2, height() / 2);       //?′???÷÷?“??ì????????‘?μ?

    QRectF rect(-(side/2-6), -(side/2-6), side-12, side-12);
    int startAngle = -54 * 16;
    int spanAngle = (180+110) * 16;
    tmPainter.drawChord(rect, startAngle, spanAngle);

//    tmPainter.fillRect(rect(), QBrush(Qt::black));       //??÷?±???—’?′
    tmPainter.scale(side / 300.0, side / 300.0);          //??à????¨??–°
    //??÷?÷?’?± –?”??ó??—’?′
    tmPainter.setPen(Qt::NoPen);
    tmPainter.setBrush(pointer);
    //??÷? ±????
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(hourColor);
    for (int i = 0; i < 9; ++i)
    {
        tmPainter.drawLine(-53, 73, -62, 85);
        tmPainter.rotate(36.0);
    }
    tmPainter.restore();
    //??÷??÷????
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(minuteColor);
    for (int j = 0; j < 8; ++j)
    {
        tmPainter.drawLine(-73, 53, -85, 62);
        tmPainter.rotate(36.0);
    }
    tmPainter.restore();
    //??÷?????÷μ
    tmPainter.setPen(textColor);
    tmPainter.drawText(-61, 57,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(0));//?”￡?x,y￡??? ?,????30,?ì??30
    tmPainter.drawText(-90, 15,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(10));
    tmPainter.drawText(-90, -35,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(20));
    tmPainter.drawText(-62, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(30));
    tmPainter.drawText(-15, -90,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(40));
    tmPainter.drawText(32, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(50));
    tmPainter.drawText(62, -31,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(60));
    tmPainter.drawText(60, 20,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(70));
    tmPainter.drawText(35, 55,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(80));
    //??‘?
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(QPen(Qt::white,2,Qt::SolidLine));
    tmPainter.setBrush(QBrush(Qt::NoBrush));
    tmPainter.drawEllipse(-20,-20,40,40);
    tmPainter.restore();
    //??÷?’?
    tmPainter.save();
    tmPainter.setPen(Qt::NoPen);
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.rotate(180+36+mValue*288/80);
    tmPainter.drawConvexPolygon(hourHand, 8);
    tmPainter.restore();
    //?ó???ê‘??°
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::red,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,-54*16,72*16);
    tmPainter.restore();
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::yellow,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,18*16,144*16);
    tmPainter.restore();
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::white,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,162*16,72*16);
    tmPainter.restore();
    //??‘??? ??÷
    QFont font;
    font.setPointSize(12);
    setFont(font);

    tmPainter.drawText(-13, -11,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(mValue));
}


TimeInstrument::TimeInstrument(QWidget *parent)
    : QWidget(parent)
{
    m_EffectValid = false;
    m_Timer = 0;
    connect(&m_Animator, SIGNAL(finished()), SLOT(OperateStep1()));
    connect(&m_Animator, SIGNAL(frameChanged(int)), SLOT(update()));
}

void TimeInstrument::Initial(CLOCK_TYPE pType)
{
    if(pType == DIGITAL)
    {
        m_Type = DIGITAL;
        m_AreaAnimate.mEffectType = EffectTypeFlip;
        m_AreaAnimate.mDirection = Vertical;    //>@???¨??–????ú
        m_AreaAnimate.mOrientation = Positive;  //>@??–?’??ê–ú
        m_AreaAnimate.mFrameStart = 0;
        m_AreaAnimate.mFrameEnd = 20;
        m_AreaAnimate.mAnimateTime = 600;
        m_AreaAnimate.mAnimateType = QTimeLine::EaseInOutCurve;

        CalcElementInfo(m_Type);

        LoadPixmap(BGRC, ":/Clock/rc/Clock/clock.png");
        LoadPixmap(Text0, ":/Clock/rc/Clock/num-0.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text1, ":/Clock/rc/Clock/num-1.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text2, ":/Clock/rc/Clock/num-2.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text3, ":/Clock/rc/Clock/num-3.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text4, ":/Clock/rc/Clock/num-4.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text5, ":/Clock/rc/Clock/num-5.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text6, ":/Clock/rc/Clock/num-6.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text7, ":/Clock/rc/Clock/num-7.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text8, ":/Clock/rc/Clock/num-8.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text9, ":/Clock/rc/Clock/num-9.png", QSize(m_ItemWidth, m_ItemHeight));

        LoadPixmap(ClockColon, ":/Clock/rc/Clock/num-maohao.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockMinus, ":/Clock/rc/Clock/num--.png", QSize(m_ItemWidth, m_ItemHeight));

        LoadPixmap(ClockMon, ":/Clock/rc/Clock/1.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockTue, ":/Clock/rc/Clock/2.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockWed, ":/Clock/rc/Clock/3.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockThu, ":/Clock/rc/Clock/4.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockFri, ":/Clock/rc/Clock/5.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockSat, ":/Clock/rc/Clock/6.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockSun, ":/Clock/rc/Clock/7.png", QSize(4*m_ItemWidth, m_ItemHeight));
    }
    else if(pType == ANALOG)
    {
        m_Type = ANALOG;
        m_AreaAnimate.mEffectType = EffectTypeRoll;
        m_AreaAnimate.mDirection = Vertical;    //>@???¨??–????ú
        m_AreaAnimate.mOrientation = Positive;  //>@??–?’??ê–ú
        m_AreaAnimate.mFrameStart = 0;
        m_AreaAnimate.mFrameEnd = 20;
        m_AreaAnimate.mAnimateTime = 600;
        m_AreaAnimate.mAnimateType = QTimeLine::EaseInOutCurve;

        CalcElementInfo(m_Type);
        LoadPixmap(BGRC, ":/Clock/rc/Clock/clock.png");

        LoadPixmap(Text0, ":/Clock/rc/Clock/num-0.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text1, ":/Clock/rc/Clock/num-1.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text2, ":/Clock/rc/Clock/num-2.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text3, ":/Clock/rc/Clock/num-3.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text4, ":/Clock/rc/Clock/num-4.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text5, ":/Clock/rc/Clock/num-5.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text6, ":/Clock/rc/Clock/num-6.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text7, ":/Clock/rc/Clock/num-7.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text8, ":/Clock/rc/Clock/num-8.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(Text9, ":/Clock/rc/Clock/num-9.png", QSize(m_ItemWidth, m_ItemHeight));

        LoadPixmap(ClockColon, ":/Clock/rc/Clock/num-maohao.png", QSize(m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockMinus, ":/Clock/rc/Clock/num--.png", QSize(m_ItemWidth, m_ItemHeight));

        LoadPixmap(ClockMon, ":/Clock/rc/Clock/1.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockTue, ":/Clock/rc/Clock/2.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockWed, ":/Clock/rc/Clock/3.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockThu, ":/Clock/rc/Clock/4.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockFri, ":/Clock/rc/Clock/5.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockSat, ":/Clock/rc/Clock/6.png", QSize(4*m_ItemWidth, m_ItemHeight));
        LoadPixmap(ClockSun, ":/Clock/rc/Clock/7.png", QSize(4*m_ItemWidth, m_ItemHeight));

        LoadPixmap(ClockHourPointer, ":/Clock/rc/Clock/hour.png");
        LoadPixmap(ClockMinutePointer, ":/Clock/rc/Clock/minute.png");
        LoadPixmap(ClockSecondPointer, ":/Clock/rc/Clock/second.png");
        LoadPixmap(ClockDot, ":/Clock/rc/Clock/dot.png");

        CalcAnalogClockPointer();
    }
    m_EffectValid = true;

    m_SavedTime.fromChar(3,5,2,1,1,1);
    CLOCK_INFO tmpTime(0,1,1,2,0,0);
    updateEffect(tmpTime);

    if(m_Timer == 0)
    {
        m_Timer = new QTimer(this);
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    }
    m_Timer->start(1000);
}

void TimeInstrument::UpdateTime()
{
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    CLOCK_INFO tmpClock(tmpDateTime.date().year()-2000,
                        tmpDateTime.date().month(),
                        tmpDateTime.date().day(),
                        tmpDateTime.time().hour(),
                        tmpDateTime.time().minute(),
                        tmpDateTime.time().second());
    updateEffect(tmpClock);
}

void TimeInstrument::OperateStep0()
{
    if(m_EffectValid == false)
        return;
    update();
    //>@÷?––STEP0÷–μ?????￡¨?á??÷?––??????￡¨?? ‘÷?––STEP1÷–μ?????
    if(m_OperateInfo[STEP0].mValid)
    {
        StartTimeline(m_AreaAnimate.mFrameStart,
                      m_AreaAnimate.mFrameEnd,
                      m_AreaAnimate.mAnimateTime,
                      m_AreaAnimate.mAnimateType);
        return;
    }
    OperateStep1();
}

void TimeInstrument::OperateStep1()
{
    if(m_EffectValid == false)
        return;
    //>@??±?? ‘?￡¨????±??????¨“?’????¨
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
    update();
    if(m_OperateInfo[STEP1].mValid)
    {
        StartTimeline(m_AreaAnimate.mFrameStart,
                      m_AreaAnimate.mFrameEnd,
                      m_AreaAnimate.mAnimateTime,
                      m_AreaAnimate.mAnimateType);
    }
}

void TimeInstrument::CalcElementInfo(CLOCK_TYPE pType)
{
    if(pType == ANALOG)
    {
        //>@??????”ú??–°??à? ±?????÷
        int hw = ::qMin(width(), height());
        int hh = hw;
        m_ItemWidth = (qreal)hw / 20.0;
        m_ItemHeight = ::qMin((int)m_ItemWidth, hh);
        //>@???ê??––????“???‘?for(int i —???÷???￡¨“ú??‘???÷– ?”?m_PointerLength????à?÷?’?μ?RECT°￡
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
                    tmpEle->mRcPointer.mLastPointer = ClockMinus;  //>@ClockMinus÷??ê÷?-????
                    tmpEle->mRcPointer.mCurPointer = ClockMinus;
                    break;
                case Element_Week:
                    tmpEle->mRect = QRectF(m_ItemWidth*12,
                                          m_ItemHeight*10 - m_ItemHeight/2.0,
                                          m_ItemWidth*4,
                                          m_ItemHeight);
                    tmpEle->mRcPointer.mLastPointer = ClockSun;  //>@ClockSun±ì ?–????’
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
        int hw = width();
        int hh = height();
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
                    if(i < Element_Week)        //>@?í‘??’????”ú
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
                    else if(i == Element_Week)  //>@–?????”ú
                    {
                        tmpEle->mRect = QRectF(m_ItemWidth*3, m_ItemHeight*3/2, m_ItemWidth*4, m_ItemHeight);
                        tmpEle->mRcPointer.mLastPointer = ClockSun;
                        tmpEle->mRcPointer.mCurPointer = ClockSun;
                    }
                    else                    //>@11~19  ±?÷????”ú
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
            case PTN_H_YMDWHMS:
            {
                m_ItemWidth = (qreal)hw/24.0;
                m_ItemHeight = (qreal)hh;
                for(int i=Element_Year0; i<=Element_Second1; i++)
                {
                    CLOCK_ELEMENTINFO *tmpEle = new CLOCK_ELEMENTINFO;
                    tmpEle->mAnimateFlag = false;
                    if(i < Element_Week)        //>@?í‘??’????”ú
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
                    else if(i == Element_Week)  //>@–?????”ú
                    {
                        int j = i-Element_Year0;
                        tmpEle->mRect = QRectF(m_ItemWidth*(j+1), 0, m_ItemWidth*4, m_ItemHeight);
                        tmpEle->mRcPointer.mLastPointer = ClockSun;
                        tmpEle->mRcPointer.mCurPointer = ClockSun;
                    }
                    else                    //>@11~19  ±?÷????”ú
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

void TimeInstrument::CalcAnalogClockPointer()
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
                QPixmap tmpPixmap = m_EffectImage.value(i+ClockHourPointer);
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
        }
        m_ClockElement.insert((CLOCKE_ELEMENT)i, tmpEle);
    }
}

void TimeInstrument::paintEvent(QPaintEvent *event)
{
    if(m_EffectValid == false)
        return;
    QPainter p(this);
    if(m_Type == ANALOG)
    {
        int bgw = ::qMin(width(), height());
        //>@?ê÷?±???
        DrawPixmap(p, m_EffectImage.value(BGRC), QRect(0,0,bgw,bgw));
        //>@?ê÷??’??°?–???
        for(int i=Element_Year0;i<=Element_Week;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            DrawPixmap(p, m_EffectImage.value(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect.toRect());
        }
        //>@?ê÷?÷?’?
        int index = m_Animator.currentFrame();
        int tmpFrameCount;
        for(int i=Element_HourPointer;i<=Element_SecondPointer;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            p.save();
            QTransform transform;
            transform.translate(bgw/2, bgw/2);
            if(tmpEle->mAnimateFlag)
            {
                tmpFrameCount = m_AreaAnimate.mFrameEnd - m_AreaAnimate.mFrameStart;
                m_Diff = (qreal)(m_PointerAngle[i][Current] - m_PointerAngle[i][Last]) / tmpFrameCount;
                transform.rotate(m_PointerAngle[i][Last] + m_Diff*index);
            }
            else
            {
                transform.rotate(m_PointerAngle[i][Current]);
            }
            p.setTransform(transform);
            DrawPixmap(p, m_EffectImage.value(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect.toRect());
            p.restore();
        }
        //>@?ê÷?Dot
        CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value(Element_Dot);
        if(tmpEle)
        {
            DrawPixmap(p, m_EffectImage.value(tmpEle->mRcPointer.mCurPointer), tmpEle->mRect.toRect());
        }
    }
    else if(m_Type == DIGITAL)
    {
        //>@?ê÷?±???
        DrawPixmap(p, m_EffectImage.value(BGRC), rect());
        //>@?ê÷? ±??
        for(int i=Element_Year0;i<=Element_Second1;i++)
        {
            CLOCK_ELEMENTINFO* tmpEle = m_ClockElement.value((CLOCKE_ELEMENT)i);
            if(tmpEle == 0)
                continue;
            m_PixmapPointer.mLastPointer = m_EffectImage.value(tmpEle->mRcPointer.mLastPointer);
            m_PixmapPointer.mCurPointer = m_EffectImage.value(tmpEle->mRcPointer.mCurPointer);
            if(tmpEle->mAnimateFlag == false)
            {
                Paint2D_None(p, tmpEle->mRect.toRect());
            }
            else
            {
                switch(m_AreaAnimate.mEffectType)
                {
                case EffectTypeRoll:
                    Paint2D_Roll(p, tmpEle->mRect.toRect());
                    break;
                case EffectTypeFlip:
                    Paint2D_Flip(p, tmpEle->mRect.toRect());
                    break;
                default:
                    Paint2D_None(p, tmpEle->mRect.toRect());
                    break;
                }
            }
        }
    }
    return;
}

void TimeInstrument::updateEffect(CLOCK_INFO pTime)
{
    if(m_EffectValid == false)
        return;
    if(pTime.isValid() == false)
    {
        IDE_DEBUG("DateTime is invalid,please check it!");
        return;
    }
    bool flag = false;
    //>@???? ??÷ ±÷”??????μ????¨
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
            //IDE_TRACE_INT(tmpEle->mRcPointer.mLastPointer);
            //IDE_TRACE_INT(tmpEle->mRcPointer.mCurPointer);
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

    //>@?è?? ±÷”???÷
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
                //>@”???????μ±?”60±???1 ±???÷μ?÷?’???±?
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
                //>@”???????μ±?”60±???1 ±???÷μ?÷?’???±?
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
                //>@”???????μ±?”60±???1 ±???÷μ?÷?’???±?
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
    //>@÷?––STEP0÷–μ?????
    OperateStep0();
}


void TimeInstrument::StartTimeline(int pStart, int pEnd, int pInterval, QTimeLine::CurveShape pShape)
{
    m_Animator.stop();
    m_Animator.setFrameRange(pStart, pEnd);
    m_Animator.setDuration(pInterval);
    m_Animator.setCurveShape(pShape);
    m_Animator.start();
}

bool TimeInstrument::LoadPixmap(int pKey, QString pPixmapName)
{
    if(pPixmapName.isEmpty())
        return false;
    FILEPATH_TYPE tmpType = getRcFileType(pPixmapName);
    if(tmpType == PATH_TYPENONE || tmpType == PATH_TYPENETADDR)
        return false;
    QImage tmpImage;
    if(tmpImage.load(pPixmapName) == false)
        return false;
    m_EffectImage.insert(pKey, QPixmap::fromImage(tmpImage));
    return true;
}

bool TimeInstrument::LoadPixmap(int pKey, QString pPixmapName, QSize pSize)
{
    if(pPixmapName.isEmpty())
        return false;
    FILEPATH_TYPE tmpType = getRcFileType(pPixmapName);
    if(tmpType == PATH_TYPENONE || tmpType == PATH_TYPENETADDR)
        return false;
    if(!pSize.isValid())
        return false;
    QImage tmpImage;
    if(tmpImage.load(pPixmapName) == false)
        return false;
    m_EffectImage.insert(pKey, QPixmap::fromImage(tmpImage.scaled(pSize, Qt::IgnoreAspectRatio)));
    return true;
}

bool TimeInstrument::DrawPixmap(QPainter &p, QPixmap pPixmap, QRect pTargetRect)
{
    if(pTargetRect.isValid() == false)
    {
        //IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p.drawPixmap(pTargetRect, pPixmap);
        return true;
    }
    //IDE_TRACE();
    return false;
}

bool TimeInstrument::DrawPixmap(QPainter &p, QPixmap pPixmap, QRect pTargetRect, QRect pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false)
    {
        //IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p.drawPixmap(pTargetRect.left(), pTargetRect.top(), pTargetRect.width(), pTargetRect.height(),
                     pPixmap,
                     pSourceRect.left(), pSourceRect.top(), pSourceRect.width(), pSourceRect.height());
        return true;
    }
    return false;
}


void TimeInstrument::Paint2D_None(QPainter &p, QRect targetRect)
{
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
}

//>@–?“??ê÷?μ???????–????¨
void TimeInstrument::Paint2D_Flip(QPainter &p, QRect targetRect)
{
    qreal w = targetRect.width();
    qreal h = targetRect.height();
    qreal hw = w / 2.0;
    qreal hh = h / 2.0;
    // behind is the new pixmap
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
    if(m_Animator.state() != QTimeLine::Running)
        return;
    qreal FrameCount = m_AreaAnimate.mFrameEnd - m_AreaAnimate.mFrameStart;
    if(FrameCount <= 0)
        return;
    qreal halfcount =  FrameCount / 2.0;
    QTransform transform;
    qreal index = m_Animator.currentFrame();

    p.save();
    if (index <= halfcount)
    {
        // the top part of the old pixmap is flipping
        qreal angle;
        if(m_AreaAnimate.mOrientation == Positive)
        {
            angle = -180.0 * index / FrameCount;
        }
        else
        {
            angle = 180.0 * index / FrameCount;
        }
        transform.translate(targetRect.left()+hw, targetRect.top()+hh);
        if(m_AreaAnimate.mDirection == Horizontal)
            transform.rotate(angle, Qt::YAxis);
        else
            transform.rotate(angle, Qt::XAxis);
        p.setTransform(transform);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(-hw, -hh, w, h), QRect(0,0,w,h));

        // the bottom part is still the old pixmap
        p.resetTransform();
        if(m_AreaAnimate.mDirection == Horizontal)
            if(m_AreaAnimate.mOrientation == Positive)
                p.setClipRect(hw, 0, hw, targetRect.height());
            else
                p.setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate.mOrientation == Positive)
                p.setClipRect(0, hh, targetRect.width(), hh);
            else
                p.setClipRect(0, 0, targetRect.width(), hh);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(0,0,w,h), QRect(0,0,w,h));
    }
    else
    {
        transform.translate(targetRect.left(), targetRect.top());
        p.setTransform(transform);
        if(m_AreaAnimate.mDirection == Horizontal)
            if(m_AreaAnimate.mOrientation == Positive)
                p.setClipRect(hw, 0, hw, targetRect.height());
            else
                p.setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate.mOrientation == Positive)
                p.setClipRect(0, hh, targetRect.width(), hh);
            else
                p.setClipRect(0, 0, targetRect.width(), hh);

        // the bottom part is still the old pixmap
        DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(0,0,w,h), QRect(0,0,w,h));
        p.resetTransform();
        // the bottom part of the new pixmap is flipping
        qreal angle;
        if(m_AreaAnimate.mOrientation == Positive)
        {
            angle = 180.0 - 180.0 * index / FrameCount;
        }
        else
        {
            angle = - 180.0 * (FrameCount-index) / FrameCount;
        }
        transform.translate(targetRect.left()+hw, targetRect.top()+hh);
        if(m_AreaAnimate.mDirection == Horizontal)
            transform.rotate(angle, Qt::YAxis);
        else
            transform.rotate(angle, Qt::XAxis);
        p.setTransform(transform);
        DrawPixmap(p, m_PixmapPointer.mCurPointer, QRect(-hw, -hh, w, h), QRect(0,0,w,h));
    }
    p.restore();
}

void TimeInstrument::Paint2D_Roll(QPainter &p, QRect targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal tmpDiff;
    if(m_Animator.state() != QTimeLine::Running)
        return;
    qreal FrameCount = m_AreaAnimate.mFrameEnd - m_AreaAnimate.mFrameStart - 1;
    if(FrameCount <= 0)
    {
        //IDE_TRACE();
        return;
    }
    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@ ????′??±í‘?μ?“?μ?μ±??–?“??ê÷?μ???”ú°￡
    p.save();
    QTransform transform1, transform2;
    if(m_AreaAnimate.mDirection == Vertical)
    {
        tmpDiff = hh / FrameCount;
        if(m_AreaAnimate.mOrientation == Positive)
        {
            y = (qreal)(tmpDiff*index);
            transform1.translate(targetRect.left(), targetRect.top()+y);
            p.setTransform(transform1);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(0,0,hw,hh-y), QRect(0,0,hw,hh-y));
        }
        else
        {
            y = (qreal)(tmpDiff*index);
            transform1.translate(targetRect.left(), targetRect.top()-y);
            p.setTransform(transform1);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(0,y,hw,hh-y), QRect(0,y,hw,hh-y));
        }
        p.resetTransform();
        if(m_AreaAnimate.mOrientation == Positive)
        {
            y = hh - y;
            transform2.translate(targetRect.left(), targetRect.top()-y);
            p.setTransform(transform2);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, QRect(0,y,hw,hh-y), QRect(0,y,hw,hh-y));
        }
        else
        {
            y = hh - y;
            transform2.translate(targetRect.left(), targetRect.top()+y);
            p.setTransform(transform2);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, QRect(0,0,hw,hh-y), QRect(0,0,hw,hh-y));
        }
    }
    else
    {
        tmpDiff = (qreal)(hw) / FrameCount;
        if(m_AreaAnimate.mOrientation == Positive)
        {
            x = (qreal)(tmpDiff*index);
            transform1.translate(targetRect.left()+x, targetRect.top());
            p.setTransform(transform1);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(0,0,hw-x,hh), QRect(0,0,hw-x,hh));
        }
        else
        {
            x = (qreal)(tmpDiff*index);
            transform1.translate(targetRect.left()-x, targetRect.top());
            p.setTransform(transform1);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(x,0,hw-x,hh), QRect(x,0,hw-x,hh));
        }
        p.resetTransform();
        if(m_AreaAnimate.mOrientation == Positive)
        {
            x = hw - x;
            transform2.translate(targetRect.left()-x, targetRect.top());
            p.setTransform(transform2);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, QRect(x,0,hw-x,hh), QRect(x,0,hw-x,hh));
        }
        else
        {
            x = hw - x;
            transform2.translate(targetRect.left()+x, targetRect.top());
            p.setTransform(transform2);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, QRect(0,0,hw-x,hh), QRect(0,0,hw-x,hh));
        }
    }
    p.restore();
}

SpeedInstrument::SpeedInstrument(QWidget *parent) :
    QWidget(parent)
{
    m_DragFlag = false;
}

void SpeedInstrument::init()                    //?àù??????ê?áèí?
{
    setValue(0);
}

void SpeedInstrument::mousePressEvent(QMouseEvent *e)
{
    m_DragFlag = true;
}

void SpeedInstrument::mouseMoveEvent(QMouseEvent *e)
{
    if(m_DragFlag)
    {
        if(mValue < 288)
            setValue(mValue+1);
    }
}

void SpeedInstrument::mouseReleaseEvent(QMouseEvent *e)
{
    m_DragFlag = false;
}

void SpeedInstrument::focusOutEvent(QFocusEvent *e)
{
    m_DragFlag = false;
}

void SpeedInstrument::paintEvent(QPaintEvent *)
{
    //??÷?÷?’?μ?????μ?
    static const QPoint hourHand[8] =
    {
        QPoint(4, -20),
        QPoint(4, -60),
        QPoint(1, -70),
        QPoint(1, -80),
        QPoint(-1, -80),
        QPoint(-1, -70),
        QPoint(-4, -60),
        QPoint(-4,-20)
    };
    // ??÷?—’?′
    QColor pointer(200,0,0);
    QColor hourColor(255, 255, 255);
    QColor minuteColor(255,122,29);
    QColor textColor(255, 255, 255);

    int side = qMin(width(), height());                //?°?ó–°÷μ
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//????????
    painter.setPen(Qt::black);
    //painter.setBrush(Qt::black);

    painter.translate(width() / 2, height() / 2);       //?′???÷÷?“??ì????????‘?μ?

    QConicalGradient conicalGradient(0,0,-54);
    //?????à“???QConicalGradient?‘?? μ??￡¨?? ??÷±???÷––???±í???? ?????
    conicalGradient.setColorAt(0,Qt::darkRed);
    conicalGradient.setColorAt(0.2,Qt::red);
    conicalGradient.setColorAt(0.4,Qt::magenta);
    conicalGradient.setColorAt(0.6,Qt::darkGray);
    conicalGradient.setColorAt(0.8,Qt::black);//??÷???±?μ?—’?′??????±???
    painter.setBrush(QBrush(conicalGradient));

    QRectF rect(-(side/2-6), -(side/2-6), side-12, side-12);
    int startAngle = -54 * 16;
    int spanAngle = (180+110) * 16;
    painter.drawChord(rect, startAngle, spanAngle);

//    painter.fillRect(rect(), QBrush(Qt::black));       //??÷?±???—’?′
    painter.scale(side / 300.0, side / 300.0);          //??à????¨??–°
    //??÷?÷?’?± –?”??ó??—’?′
    painter.setPen(Qt::NoPen);
    painter.setBrush(pointer);
    //??÷? ±????
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(hourColor);
    for (int i = 0; i < 9; ++i)
    {
        painter.drawLine(-53, 73, -62, 85);
        painter.rotate(36.0);
    }
    painter.restore();
    //??÷??÷????
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(minuteColor);
    for (int j = 0; j < 8; ++j)
    {
        painter.drawLine(-73, 53, -85, 62);
        painter.rotate(36.0);
    }
    painter.restore();
    //??÷?????÷μ
    painter.setPen(textColor);
    painter.drawText(-61, 57,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(0));//?”￡?x,y￡??? ?,????30,?ì??30
    painter.drawText(-90, 15,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(10));
    painter.drawText(-90, -35,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(20));
    painter.drawText(-62, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(30));
    painter.drawText(-15, -90,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(40));
    painter.drawText(32, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(50));
    painter.drawText(62, -31,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(60));
    painter.drawText(60, 20,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(70));
    painter.drawText(35, 55,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(80));
    //??‘?
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::white,2,Qt::SolidLine));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawEllipse(-20,-20,40,40);
    painter.restore();
    //??÷?’?
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.rotate(180+36+mValue*288/80);
    painter.drawConvexPolygon(hourHand, 8);
    painter.restore();
    //?ó???ê‘??°
    painter.save();
    painter.setPen(QPen(Qt::red,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,-54*16,72*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::yellow,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,18*16,144*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::white,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,162*16,72*16);
    painter.restore();
    //??‘??? ??÷
    QFont font;
    font.setPointSize(12);
    setFont(font);

    painter.drawText(-13, -11,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(mValue));
 }

void SpeedInstrument::setValue(int newvalue)
{
    mValue= newvalue;
    update();
}

PieInstrument::PieInstrument(QWidget *parent)
    : QWidget(parent)
{
}

PieInstrument::~PieInstrument()
{

}

void PieInstrument::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    // ??????± 
    painter.setPen(Qt::NoPen);
    // ??÷???????
    painter.setRenderHint(QPainter::Antialiasing);
    //******************************************** ‘?±??? *****************************************//
    if(1)
    {
        // ????‘??ê
        QRectF rect_top(10.0, 20.0, 280.0, 160.0);
        // μ???‘??ê
        QRectF rect_bottom(10.0, 60.0, 280.0, 160.0);
        // ÷–????–?
        QRectF rect_midd(10.0, 100.0, 280.0, 40.0);
        // ??–??? ?????
        int startAngle = 230 * 16;
        // ??–?????????
        int spanAngle = 60 * 16;
        painter.setBrush(QColor(97,35,35,255));
        // ?ê÷?μ???‘??ê
        painter.drawEllipse(rect_bottom);
        // ?ê÷?÷–????–?
        painter.drawRect(rect_midd);
        painter.setBrush(QColor(Qt::darkYellow));
        // ?ê÷?μ?????–?
        painter.drawPie(rect_bottom, startAngle, spanAngle);
        // ??–?μ??“”??°μ???μ?
        double pi = 3.1415926;
        double dx1 = rect_top.width() * 0.5 * cos(230 * pi / 180);
        double dy1 = rect_top.height() * 0.5 * sin(230 * pi / 180);
        double dx2 = rect_top.width() * 0.5 * cos(290 * pi / 180);
        double dy2 = rect_top.height() * 0.5 * sin(290 * pi / 180);
        // ????μ?μ???±í÷μ
        QPointF posBackCenter = QPointF(rect_top.center());
        double dX1 = posBackCenter.x() + dx1 + 0.5;
        double dY1 = posBackCenter.y() - dy1 + 0.5;
        double dX2 = posBackCenter.x() + dx2 + 0.5;
        double dY2 = posBackCenter.y() - dy2 + 0.5;
        // ??????μ?
        QPointF topLeft = QPointF(dX1, dY1);
        QPointF bottomRight = QPointF(dX2, dY2) + QPointF(0,40);
        QPointF bottomLeft = topLeft + QPointF(0,40);
        painter.setBrush(QColor(Qt::darkYellow));
        // ?ê÷??¨?”??–?μ???”ú
        QPolygonF path;
        path << topLeft << QPointF(dX2,dY2) << bottomRight << bottomLeft;
        painter.drawPolygon(path);
        // ?ê÷?????‘??ê
        painter.setBrush(QColor(156,56,56,255));
        painter.drawEllipse(rect_top);
        // ?ê÷???????–?
        painter.setBrush(QColor(Qt::yellow));
        painter.drawPie(rect_top, startAngle, spanAngle);
    }
    QWidget::paintEvent(e);
}


#ifdef SURRPORT_ACTIVEX
AxWidget::AxWidget(QWidget* parent) :
    QAxWidget(parent)
{
}

AxWidget::~AxWidget()
{

}

void AxWidget::PlaySwf(QString pFile)
{
    //connect(this,SIGNAL(FSCommand(QString,QString)),this,SLOT(flashAction(QString,QString)));
    setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
    dynamicCall("LoadMovie(long,string)", 0, pFile.toLatin1().data());
}

void AxWidget::OpenWebPagInIE(QString pUrl)
{
    setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}")); //??÷???????IE
    dynamicCall("Navigate(const QString&)", pUrl.toLatin1().data()); //μ?”??? ?
}

void AxWidget::OpenXls(QString pFile)
{
    setProperty("Visible", true);
    setControl(QString("Excel.Application"));
    dynamicCall("Open (const QString&)", pFile);
}

void AxWidget::OpenDoc(QString pFile)
{
    setProperty("Visible", true);
    setControl(QString("Word.Document"));
    dynamicCall("Open (const QString&)", pFile);
}
#endif

