#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    connect(ui->lineedit_usrname, SIGNAL(sFocusIn()), this, SLOT(slot_LoginFoucsIn()));
    connect(ui->lineedit_usrname, SIGNAL(sFocusOut()), this, SLOT(slot_LoginFoucsOut()));
    ui->lineedit_usrname->SetFlag(LineEditForm::FLAG_ICON, QPixmap(":/icon/rc/usr.png"));
    ui->lineedit_usrname->SetStatusTip(GBKToUTF8("用户名"));

    connect(ui->lineedit_passwd, SIGNAL(sFocusIn()), this, SLOT(slot_LoginFoucsIn()));
    connect(ui->lineedit_passwd, SIGNAL(sFocusOut()), this, SLOT(slot_LoginFoucsOut()));
    ui->lineedit_passwd->SetFlag(LineEditForm::FLAG_ICON, QPixmap(":/icon/rc/lock.png"));
    ui->lineedit_passwd->SetFlag(LineEditForm::FLAG_PASSWD, true);
    ui->lineedit_passwd->SetStatusTip(GBKToUTF8("密码"));

    connect(ui->lineedit_reg_phone, SIGNAL(sFocusIn()), this, SLOT(slot_RegisterFoucsIn()));
    connect(ui->lineedit_reg_phone, SIGNAL(sFocusOut()), this, SLOT(slot_RegisterFoucsOut()));
    ui->lineedit_reg_phone->SetFlag(LineEditForm::FLAG_ICON, QPixmap(":/icon/rc/usr.png"));
    ui->lineedit_reg_phone->SetStatusTip(GBKToUTF8("密保手机号"));

    connect(ui->lineedit_reg_passwd, SIGNAL(sFocusIn()), this, SLOT(slot_RegisterFoucsIn()));
    connect(ui->lineedit_reg_passwd, SIGNAL(sFocusOut()), this, SLOT(slot_RegisterFoucsOut()));
    ui->lineedit_reg_passwd->SetFlag(LineEditForm::FLAG_ICON, QPixmap(":/icon/rc/lock.png"));
    ui->lineedit_reg_passwd->SetStatusTip(GBKToUTF8("密保问题"));

    connect(ui->lineedit_reg_verify, SIGNAL(sFocusIn()), this, SLOT(slot_RegisterFoucsIn()));
    connect(ui->lineedit_reg_verify, SIGNAL(sFocusOut()), this, SLOT(slot_RegisterFoucsOut()));
    ui->lineedit_reg_verify->SetFlag(LineEditForm::FLAG_ALIGN, Qt::AlignCenter);
    ui->lineedit_reg_verify->SetStatusTip(GBKToUTF8("获取密码"));

    iniParse tmpParser(D_FSRCPATH + "etc.ini");
    QString tmpUserName, tmpPasswd;
    tmpParser.getProperty("usrname", tmpUserName);
    tmpParser.getProperty("passwd", tmpPasswd);
    ui->lineedit_usrname->SetText((tmpUserName.toLatin1()));

    QString tmpRember;
    tmpParser.getProperty("remberpasswd", tmpRember);
    if(!tmpRember.compare("true", Qt::CaseInsensitive))
    {
        ui->check_remberpasswd->setChecked(true);
        ui->lineedit_passwd->SetText((tmpPasswd.toLatin1()));
    }

    ui->stackedWidget->setCurrentIndex(0);

    ui->btn_reg_confirm->setEnabled(true);

    mSqlParser = new SQL_PARSER;
    if(!mSqlParser->open(qApp->applicationDirPath(), QString("/Resource/IC_DB.db")))
    {
        QMessageBox::information(0, GBKToUTF8("数据库操作"), GBKToUTF8("打开数据库失败"));
        qApp->exit();
        return;
    }

    QString tmpDirStr = D_FSRCPATH + QString("image/");
    QDir tmpDir;
    tmpDir.mkdir(tmpDirStr);

    QTimer::singleShot(1, this, SLOT(slot_InitImage()));
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::slot_InitImage()  //从网上下载图片
{
    QString tmpDirStr = D_FSRCPATH + QString("image/");
    QDir tmpDir(tmpDirStr);
    QStringList tmpList = tmpDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if(!tmpList.isEmpty())
    {
        ui->wid_image->Show(QPixmap(tmpDirStr + tmpList.first()));
        for(int i=1;i<tmpList.count();i++)
        {
            ui->wid_image->Add(QPixmap(tmpDirStr + tmpList.at(i)));
        }
    }
}

void LoginForm::mousePressEvent(QMouseEvent *e)
{
    m_startPoint = e->globalPos();
    m_windowsPoint = this->pos();
    m_bIsPressed = true;
    QApplication::setOverrideCursor(Qt::SizeAllCursor);
    QWidget::mousePressEvent(e);
}

void LoginForm::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bIsPressed && e->buttons() == Qt::LeftButton)
    {
        QPoint curPos = e->globalPos();
        int ndist = (curPos - m_startPoint).manhattanLength();
        if (ndist >= QApplication::startDragDistance())
        {
            QPoint newPos(m_windowsPoint.x() + (curPos.x() - m_startPoint.x()), m_windowsPoint.y() + (curPos.y() - m_startPoint.y()));
            if (newPos.y() <= 0)
            {
                newPos.setY(0);   //往上移动的时候不能超越屏幕边缘
            }
            this->move(newPos);
        }
    }
    QWidget::mouseMoveEvent(e);
}

void LoginForm::mouseReleaseEvent(QMouseEvent *e)
{
    QApplication::restoreOverrideCursor();
    m_bIsPressed = false;
    QWidget::mouseReleaseEvent(e);
}

void LoginForm::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::white);
    painter.drawRect(QRect(D_SHADOW_LEN, D_SHADOW_LEN, this->width()-D_SHADOW_LEN*2, this->height()-D_SHADOW_LEN*2));

    QColor color(0, 0, 0, 100);
    for(int i=0; i<D_SHADOW_LEN; i++)
    {
        painter.save();
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(QRect(D_SHADOW_LEN-i, D_SHADOW_LEN-i, this->width()-(D_SHADOW_LEN-i)*2, this->height()-(D_SHADOW_LEN-i)*2), D_SHADOW_RADIUS, D_SHADOW_RADIUS);
        painter.restore();
    }
}

void LoginForm::on_btn_forgetpasswd_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ClearLoginError();
}

void LoginForm::on_btn_reg_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ClearRegisterError();
}

void LoginForm::on_btn_reg_confirm_clicked()
{
    QString tmpDbPasswd;
    QString tmpPhone = ui->lineedit_reg_phone->Text();
    if(tmpPhone.isEmpty())
    {
        ui->label_getpasswd_info->setText("Empty");
        return;
    }
    QSqlQuery tmpPasswdQuery = mSqlParser->Exec(QString("select passwd from person_admin where phone=\"%1\"").arg(tmpPhone));
    if(tmpPasswdQuery.next())
    {
        QSqlRecord tmpRecord = tmpPasswdQuery.record();
        tmpDbPasswd = tmpRecord.value("passwd").toString();
    }
    ui->lineedit_reg_verify->SetText(tmpDbPasswd);
}

void LoginForm::on_btn_close_clicked()
{
    mSqlParser->deleteLater();
    mSqlParser = 0;
    reject();
}

void LoginForm::on_btn_min_clicked()
{
    this->showMinimized();
}

void LoginForm::ClearLoginError()
{
    ui->label_logintips->clear();
    ui->lineedit_usrname->SetStatus(1);
    ui->lineedit_passwd->SetStatus(1);
}

void LoginForm::ClearRegisterError()
{
    ui->lineedit_reg_phone->SetStatus(1);
    ui->lineedit_reg_passwd->SetStatus(1);
    ui->lineedit_reg_verify->SetStatus(1);
}

void LoginForm::slot_LoginFoucsIn()
{
    ClearLoginError();
}

void LoginForm::slot_LoginFoucsOut()
{

}

void LoginForm::slot_RegisterFoucsIn()
{
    LineEditForm *tmpLineEdit = qobject_cast<LineEditForm *>(sender());
    if(tmpLineEdit)
    {
        QString tmpName = tmpLineEdit->objectName();
        if(!tmpName.compare("lineedit_reg_phone", Qt::CaseInsensitive))
        {
            ui->lineedit_reg_phone->SetStatus(1);
        }
        else if(!tmpName.compare("lineedit_reg_passwd", Qt::CaseInsensitive))
        {
            ui->lineedit_reg_passwd->SetStatus(1);
        }
        else if(!tmpName.compare("lineedit_reg_verify", Qt::CaseInsensitive))
        {
            ui->lineedit_reg_verify->SetStatus(1);
        }
    }
}

void LoginForm::slot_RegisterFoucsOut()
{
    LineEditForm *tmpLineEdit = qobject_cast<LineEditForm *>(sender());
    if(tmpLineEdit)
    {
        QString tmpName = tmpLineEdit->objectName();
        if(!tmpName.compare("lineedit_reg_phone", Qt::CaseInsensitive))
        {
            QString tmpText = ui->lineedit_reg_phone->Text();
            if(tmpText.isEmpty() || tmpText.count() != 11 || !isDigitStr(tmpText))
            {
                ui->lineedit_reg_phone->SetStatus(0);
            }
        }
        else if(!tmpName.compare("lineedit_reg_passwd", Qt::CaseInsensitive))
        {
            QString tmpText = ui->lineedit_reg_phone->Text();
            if(tmpText.isEmpty())
            {
                ui->lineedit_reg_passwd->SetStatus(0);
            }
        }
        else if(!tmpName.compare("lineedit_reg_verify", Qt::CaseInsensitive))
        {
            QString tmpText = ui->lineedit_reg_verify->Text();
            if(tmpText.isEmpty())
            {
                ui->lineedit_reg_verify->SetStatus(0);
            }
        }
    }
}

void LoginForm::on_btn_login_clicked()
{
    QString tmpUsrName = ui->lineedit_usrname->Text();
    QString tmpPasswd = ui->lineedit_passwd->Text();
    bool tmpFlag = true;
    if(tmpUsrName.isEmpty())
    {
        ui->lineedit_usrname->SetStatus(0); //设置Lineedit为红框
        ui->label_logintips->setText(GBKToUTF8("请填写用户名！"));
        tmpFlag = false;
    }
    if(tmpPasswd.isEmpty())
    {
        ui->lineedit_passwd->SetStatus(0);
        ui->label_logintips->setText(GBKToUTF8("请填写密码！"));
        tmpFlag = false;
    }
    if(!tmpFlag)
        return;
    //>@保存用户名到etc.ini文件
    iniParse tmpParser(D_FSRCPATH + "etc.ini");
    tmpParser.setProperty("usrname", QString(tmpUsrName.toLatin1()));
    //>@根据是否记住密码，保存密码到etc.ini文件
    if(ui->check_remberpasswd->isChecked())
    {
        tmpParser.setProperty("passwd", QString(tmpPasswd.toLatin1()));
    }

    QString tmpDbPasswd;
    QSqlQuery tmpPasswdQuery = mSqlParser->Exec(QString("select passwd from person_admin where name=\"%1\"").arg(tmpUsrName));
    if(tmpPasswdQuery.next())
    {
        QSqlRecord tmpRecord = tmpPasswdQuery.record();
        tmpDbPasswd = tmpRecord.value("passwd").toString();
    }
    if(!tmpDbPasswd.compare(tmpPasswd))
    {
        ui->label_logintips->setText(GBKToUTF8("登陆成功！"));
        mSqlParser->deleteLater();
        mSqlParser = 0;

        mUsrName = tmpUsrName;
        accept();
    }
    else
    {
        ui->label_logintips->setText(GBKToUTF8("密码错误！"));
    }
}

void LoginForm::on_check_remberpasswd_clicked(bool checked)
{
    iniParse tmpParser(D_FSRCPATH + "etc.ini");
    tmpParser.setProperty("remberpasswd", checked?"true":"false");
    if(!checked)
        tmpParser.setProperty("passwd", "");
}

