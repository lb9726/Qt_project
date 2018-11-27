#include "uiclass.h"

LinkButton::LinkButton(QWidget *parent) : QPushButton(parent)
{
    m_State = 0;
    Init();
}

LinkButton::LinkButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    m_State = 0;
    Init();
}

void LinkButton::Init()
{
    setStyleSheet("border:none;");
}

bool LinkButton::event(QEvent *e)
{
    if(e->type() == QEvent::HoverEnter)
    {
        m_State = 1;
        update();
        return true;
    }
    else if(e->type() == QEvent::HoverLeave)
    {
        m_State = 0;
        update();
        return true;
    }
    return QPushButton::event(e);
}

void LinkButton::mousePressEvent(QMouseEvent *e)
{
    m_State = 2;
    QPushButton::mousePressEvent(e);
}

void LinkButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_State = 1;
    QPushButton::mouseReleaseEvent(e);
}

void LinkButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QRect tmpRect = rect();
    if(m_State == 0)
    {
        QFont tmpFont = p.font();
        tmpFont.setUnderline(false);
        p.setFont(tmpFont);
        p.setPen(QPen(QColor(40, 40, 250, 200), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else if(m_State >= 1)
    {
        QFont tmpFont = p.font();
        tmpFont.setUnderline(true);
        p.setFont(tmpFont);
        p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    p.drawText(tmpRect, Qt::AlignCenter, text());
    if(m_Flag.contains(FLAG_FRAME) && m_State == 2)
    {
        p.setPen(QPen(QColor(255, 170, 0), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawRoundedRect(tmpRect.adjusted(1, 1, -2, -2), 5, 5);
    }
}

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent)
{
    Init();
}

LineEdit::LineEdit(const QString &pText, QWidget* parent) : QLineEdit(parent)
{
    Init();
    SetText(pText);
}

void LineEdit::Init()
{
    mPasswdEcho = false;
    mText.clear();
    setStyleSheet("border:none; color:ligthgrey;");
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_TextChanged(QString)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(slot_SelectionChanged()));
    connect(this, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(slot_CursorPositionChanged(int,int)));
}

void LineEdit::slot_SelectionChanged()
{
    if(mText.isEmpty())
    {
       setCursorPosition(0);
       //setSelection(0, 0);
    }
}

void LineEdit::slot_CursorPositionChanged(int pOld,int pNew)
{
	Q_UNUSED(pOld);
	Q_UNUSED(pNew);
    if(mText.isEmpty())
    {
        setCursorPosition(0);
    }
}

void LineEdit::slot_TextChanged(QString pText)
{
    if(pText.isEmpty())
    {
        SetText(QString());
    }
    else
    {
        if(mText.isEmpty())
        {
            int pos = cursorPosition();
            clear();
            SetText(pText.mid(pos-1, 1));
        }
    }
}

void LineEdit::SetStatusTip(QString pText)
{
    setStatusTip(pText);
    if(text().isEmpty())
    {
        SetText(QString());
    }
}

void LineEdit::SetText(QString pText)
{
    mText = pText;
    if(pText.isEmpty() || pText.isNull())
    {
        setEchoMode(Normal);
        clear();
        setStyleSheet("border:none; color:lightgray;");
        setText(statusTip());
        setCursorPosition(0);
    }
    else
    {
        if(mPasswdEcho)
            setEchoMode(Password);
        else
            setEchoMode(Normal);
        setStyleSheet("border:none; color:black;");
        setText(pText);
    }
}

void LineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    deselect();
    //setFocusPolicy(Qt::NoFocus);
    emit sFocusOut();
}

void LineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    if(mText.isEmpty())
    {
        QTimer::singleShot(0, this, SLOT(slot_ResetCursorPos()));
    }
    //setFocusPolicy(Qt::StrongFocus);
    emit sFocusIn();
}

void LineEdit::slot_ResetCursorPos()
{
    setCursorPosition(0);
    update();
}

#include "ui_lineeditform.h"

LineEditForm::LineEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineEditForm)
{
    ui->setupUi(this);
    mFlag = 1;
    mHasFocus = false;
    ui->label->hide();
    connect(ui->lineEdit, SIGNAL(sFocusOut()), this, SLOT(slot_FocusOut()));
    connect(ui->lineEdit, SIGNAL(sFocusIn()), this, SLOT(slot_FocusIn()));
}

LineEditForm::~LineEditForm()
{
    delete ui;
}

void LineEditForm::SetFlag(quint32 pFlag, QVariant pPara)
{
    if(pFlag == FLAG_ICON)
    {
        ui->label->show();
        QPixmap icon = pPara.value<QPixmap>();
        ui->label->setPixmap(icon);
    }
    else if(pFlag == FLAG_ALIGN)
    {
        ui->lineEdit->setAlignment((Qt::Alignment)(pPara.toInt()));
    }
    else if(pFlag == FLAG_PASSWD)
    {
        ui->lineEdit->mPasswdEcho = pPara.toBool();
    }
    UiClass::SetFlag(pFlag, pPara);
}

void LineEditForm::SetText(QString pText)
{
    ui->lineEdit->SetText(pText);
}

QString LineEditForm::Text()
{
    if(!ui->lineEdit->mText.isEmpty())
        return ui->lineEdit->text();
    return QString();
}

void LineEditForm::SetStatusTip(QString pText)
{
    ui->lineEdit->SetStatusTip(pText);
}

void LineEditForm::SetStatus(int pFlag)
{
    mFlag = pFlag;
    update();
}

void LineEditForm::slot_FocusOut()
{
    mHasFocus = false;
    emit sFocusOut();
    update();
}

void LineEditForm::slot_FocusIn()
{
    mHasFocus = true;
    emit sFocusIn();
    update();
}

void LineEditForm::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setRenderHint(QPainter::Antialiasing);

    QRect tmpRect = rect();
    if(mHasFocus)
    {
        p.setPen(QPen(QColor(40, 40, 250, 200), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        tmpRect = QRect(tmpRect.x()+1, tmpRect.y()+1, tmpRect.width()-2, tmpRect.height()-2);
    }
    else
    {
        if(mFlag)
            p.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        else
            p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        tmpRect = QRect(tmpRect.x()+1, tmpRect.y()+1, tmpRect.width()-2, tmpRect.height()-2);
    }
    p.drawRoundedRect(tmpRect, 4, 4, Qt::AbsoluteSize);
}

ImageForm::ImageForm(QWidget *parent) :
    QWidget(parent)
{
    m_ImgIndex = -1;
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(slot_Timeout()));
}

ImageForm::~ImageForm()
{

}

void ImageForm::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    if(m_ImgIndex >= 0)
    {
        p.drawPixmap(rect(), m_ImgList.at(m_ImgIndex));
    }
}

void ImageForm::Add(QPixmap pPixmap, bool pInsert)
{
    if(m_ImgList.isEmpty())
    {
        m_Timer.stop();
    }
    else
    {
        if(!m_Timer.isActive())
            m_Timer.start(3000);
    }
    if(pInsert)
    {
        m_ImgList.insert(m_ImgIndex+1, pPixmap);
    }
    else
    {
        m_ImgList.append(pPixmap);
    }
}

void ImageForm::Show(QPixmap pPixmap, bool pInsert)
{
    if(m_ImgList.isEmpty())
    {
        m_Timer.stop();
    }
    else
    {
        m_Timer.start(3000);
    }
    if(pInsert)
    {
        m_ImgIndex++;
        m_ImgList.insert(m_ImgIndex, pPixmap);
    }
    else
    {
        m_ImgList.append(pPixmap);
        m_ImgIndex = m_ImgList.count() - 1;
    }
    update();
}

void ImageForm::slot_Timeout()
{
    if(m_ImgList.isEmpty())
    {
        m_ImgIndex = -1;
    }
    else
    {
        m_ImgIndex++;
        if(m_ImgIndex >= m_ImgList.count())
            m_ImgIndex = 0;
        update();
    }
}
