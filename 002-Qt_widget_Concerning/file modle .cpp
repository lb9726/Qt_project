#include "widget.h"
#include "ui_widget.h"
#include <QElapsedTimer>
#include <QTextCodec>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QStringListModel>
//#include "copyfilethread.h"
#define MAX_DIR_NUMBER 20
//最大的盘符数量
#define RELEASE_VERSION ("1.0.0")

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    VideoBtnClickedFlag(true),
    PictureBtnClickedFlag(false),
    RadioBoxFlag_Twelveformat(true),
    RadioBoxFlag_Twentyfourformat(false),
    yearmouthday(true),
    mouthdayyear(false),
    Istrue(false)
{
    ui->setupUi(this);
    windowFeature();
    cfilethread = new CopyfileThread;
    connect(cfilethread, SIGNAL(copyStationSig(int,QString)),this, SLOT(copyStation(int,QString)));

    this->Init();
}

Widget::~Widget()
{
    cfilethread->copyStop();
    cfilethread->wait();
    delete ui;
}


void Widget::windowFeature()
{
    QString version = QString("MedieaScreenHelper v%1").arg(RELEASE_VERSION);
    this->setWindowTitle(version);
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(873, 462);
}

void Widget::Init()
{
    this->ui->listView->hide();
    this->ui->pushbutton_selectpicture->hide();
    this->ui->plainTextEdit->hide();
    this->ui->label_prompt->hide();
    connect(this->ui->pushButton_makeudisk,&QPushButton::clicked,this,&Widget::formatDir);

    connect(this->ui->pushButton_selectvideo, &QPushButton::clicked, this, &Widget::OpenfileDaliog);
    connect(this->ui->pushbutton_selectpicture, &QPushButton::clicked, this, &Widget::OpenmultifileDaliog);

    connect(this->ui->radioButton_video,&QRadioButton::clicked, this, &Widget::Justselectvideo);
    connect(this->ui->radioButton_picture,&QRadioButton::clicked, this, &Widget::Justselectpicture);
    connect(this->ui->radioButton_12,&QRadioButton::clicked, this, &Widget::SetTimeformatclicked_twelve);
    connect(this->ui->radioButton_24,&QRadioButton::clicked, this, &Widget::SetTimeformatclicked_twentyfour);
    connect(this->ui->radioButton_year,&QRadioButton::clicked, this, &Widget::Setyearmouthday);
    connect(this->ui->radioButton_mouth,&QRadioButton::clicked, this, &Widget::Setmouthdayyear);

    connect(this->ui->horizontalSlider_volume,SIGNAL(valueChanged(int)),this,SLOT(SetspinBoxvolume()));
    connect(this->ui->spinBox_volume,SIGNAL(valueChanged(int)),this,SLOT(SethorizontalSlidervolume()));
    connect(this->ui->horizontalSlider_bright,SIGNAL(valueChanged(int)),this,SLOT(SetspinBoxbright()));
    connect(this->ui->spinBox_bright,SIGNAL(valueChanged(int)),this,SLOT(SethorizontalSliderbright()));

    connect(this->ui->horizontalSlider_1_bright,SIGNAL(valueChanged(int)),this,SLOT(SetspinBoxbright_1()));
    connect(this->ui->spinBox_1_bright,SIGNAL(valueChanged(int)),this,SLOT(SethorizontalSliderbright_1()));
    connect(this->ui->horizontalSlider_2_bright,SIGNAL(valueChanged(int)),this,SLOT(SetspinBoxbright_2()));
    connect(this->ui->spinBox_2_bright,SIGNAL(valueChanged(int)),this,SLOT(SethorizontalSliderbright_2()));
    connect(this->ui->pushButton_copy,SIGNAL(clicked(bool)),this,SLOT(on_copyBtnclicked()));

    //ui->comboBox_USB->clear();
    //this->usb_disk_info.clear();
    this->ScanHD();
}


void Widget::formatDir(){
    int ret = QMessageBox::warning(this,"Formate Warning","This operate will erase all data on the disk. Please backup your files.",QMessageBox::Ok,QMessageBox::Cancel);
    if(ret == QMessageBox::Ok){
        ret = QMessageBox::warning(this,"Formate Warning","This operate will erase all data on the disk. Please backup your files.",QMessageBox::Ok,QMessageBox::Cancel);
        if(ret == QMessageBox::Ok){
            formatUdisk(ui->comboBox_USB->currentText());
        } else if(ret == QMessageBox::Cancel){
            qDebug()<<"Cancel button";
        }else{
            qDebug()<<"unkonw button";
        }
    } else if(ret == QMessageBox::Cancel){
        qDebug()<<"Cancel button";
    }else{
        qDebug()<<"unkonw button";
    }
    //formatUdisk(ui->comboBox_USB->currentText());
}
void Widget::formatUdisk(QString diskDir)
{
    QMessageBox::warning(this,"Formate Warning","This operate will erase all data on the disk. Please backup your files.",QMessageBox::Ok,QMessageBox::Cancel);
    QString bashFile = "formatUdisk.bat";
    if(!isValideDriver(diskDir))
    {
        QMessageBox::information(this,"Tip Information","Invalid Driver Name.");
    }
    QFile file(bashFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(this,"Warning Information","Open/Generate formate bash file fail.");
    }
    else
    {
        //QString cmd = QString("FORMAT %1 /FS:FAT32 /q /x /y").arg(diskDir);/X为卸载盘符
        QString cmd = QString("FORMAT %1 /FS:FAT32 /q /y").arg(diskDir);
        QByteArray ba;
        ba.append(cmd);
        file.write(ba);
        file.close();
        //QElapsedTimer timer;
        //timer.start();
        QByteArray bashPath;
        bashPath.append(bashFile);
        WinExec(bashPath,SW_HIDE);//SW_NORMAL  SW_HIDE
        //qDebug() << "The format operation took" << timer.elapsed() << "milliseconds";
    }
}
//the Disk Driver must bee the '[A-Z]:' style
bool Widget::isValideDriver(QString diskDir){
    bool isDriver = false;
    if(diskDir.length()!=2)
        return false;
    if(diskDir.at(0)<'A'||diskDir.at(0)>'Z')
        return false;
    if(diskDir.at(1) != ':')
        return false;
    QFileInfoList driverList =  QDir::drives();
    for(int i=0; i < driverList.count(); i++)
    {
        QFileInfo file_info = driverList.at(i);
        QString dir = file_info.absolutePath();
        if(dir.startsWith(diskDir)){
            isDriver = true;
            break;
        }
    }
    return isDriver;
}


bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result){
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    QString DirPath;
    MSG* msg = (MSG*)message;

    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        //qDebug()<< "winEvent in MgFrame : WM_DEVICECHANGE" ;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv -> dbcv_flags == 0)
                {
                    //插入U盘，获取盘符
                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->AddDisk(DirPath);
                    qDebug()<<"this->USBDisk Path ="<<DirPath;

                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv -> dbcv_flags == 0)
                {
                    qDebug() << "USB_Removed";

                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->DeleteDisk(DirPath);
                    qDebug()<<"this-delete is ="<<DirPath;
                }
            }
            break;
        }
    }
    return false;
}

void Widget::ScanHD()
{
    //qDebug()<<"startup scan";
    char diskPath[6] = { 0 };
    DWORD allDisk = GetLogicalDrives(); //返回一个32位整数，将他转换成二进制后，表示磁盘,最低位为A盘
    if (allDisk!=0)
    {
        for (int i=0;i<MAX_DIR_NUMBER;i++)     //假定最多有12个磁盘从A开始计数
        {
            if ((allDisk & 1)==1)
            {
                sprintf( diskPath, "%c", 'A'+i );
                strcat( diskPath, ":" );
                LPCWSTR RootPathName =(LPCWSTR)QString(diskPath).utf16();

                if (GetDriveType(RootPathName) ==  DRIVE_REMOVABLE)
                {
                    if( GetVolumeInformation(RootPathName,0,0,0,0,0,0,0) ) //判断驱动是否准备就绪
                    {
                        //qDebug()<<"usb Driver is allready: "<<diskPath;
                        QString  path(diskPath);
                        this->AddDisk(path);
                    }
                }
            }
            allDisk = allDisk>>1;
        }
    }
}

void Widget::AddDisk(QString &DiskName)
{
    int iter = 0;
    if(SearchDisk(DiskName,iter))
    {
        return ;
    }
    USB_DISK_INFO usbtemp;
    usbtemp.DirPath = DiskName;
    GetMemorySize(usbtemp);//获取设备内存信息
    this->usb_disk_info.push_back(usbtemp);
    this->AddComboBoxItem(DiskName);
}

void Widget::DeleteDisk(QString &DiskName)
{
    int iter = 0;
    if(SearchDisk(DiskName,iter))
    {
        this->usb_disk_info.remove(iter);
        this->DeleteComboBoxItem(DiskName);
    }
}

bool Widget::SearchDisk(QString &DiskName, int &iter)
{
    for(int i = 0;i < this->usb_disk_info.size();i++)
    {
        if(usb_disk_info.at(i).DirPath.compare(DiskName) == 0)
        {
            iter = i;
            return true;
        }
    }
    return false;
}

void Widget::GetMemorySize(USB_DISK_INFO &disk)
{
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    LPCWSTR lpcwstrDriver=(LPCWSTR)disk.DirPath.utf16();
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
        return ;
    }
    disk.FreeMemory  = (float)liTotalFreeBytes.QuadPart/1024/1024/1024; //剩余内存
    disk.TotalMemory = (float)liTotalBytes.QuadPart/1024/1024/1024;     //总内存
}

char Widget::FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
void Widget::AddComboBoxItem(QString &item)
{
    QStringList ComboboxList = QStringList()<<item;
    //ComboboxList<<item;
    ui->comboBox_USB->addItems(ComboboxList);
}
void Widget::DeleteComboBoxItem(QString &item)
{
    int i = ui->comboBox_USB->findText(item);
    ui->comboBox_USB->removeItem(i);
}
void Widget::on_comboBox_USB_currentIndexChanged(QString e)
{
    //qDebug()<<" this is test changed ="<<e;
    int iter;
    if(this->SearchDisk(e,iter))
    {
        this->CurrentUseDisk = this->usb_disk_info.at(iter);
    }
}
USB_DISK_INFO Widget::GetCheckedDisk()
{
    return this->CurrentUseDisk;
}

void Widget::OpenfileDaliog()
{
    //选择文件
    QString filename = QFileDialog::getOpenFileName(this, tr("Selete one video file"),".",
                                                    tr("file format(*.avi *.mp4 *.rmvb *.mkv)"));
    qDebug()<<"\n" << filename;
    m_videoname.clear();
    m_videoname = filename;
    QStringList list;
    list.clear();
    list<<m_videoname;
    cfilethread->SetFileList(list);
    this->ui->lineEdit_2->setText(filename);
}
//open picture Daliog
void Widget::OpenmultifileDaliog()
{
    QStringList files = QFileDialog::getOpenFileNames(this,"Select one or more files to open",
                                                      ".","Images (*.png *.jpg)");
    m_listview.clear();
    m_listview = files;
    cfilethread->SetFileList(m_listview);
    QStringListModel *model = new QStringListModel(files);
    this->ui->listView->setModel(model);
}

bool Widget::Filecopy(QString sourcefile, QString tofile)
{
  //  tofile.replace("\\", "/");
    if (sourcefile == tofile)
        return false;
    if (!QFile::copy(sourcefile, tofile))
    {
        return false;
    }
    return true;
}

void Widget::Justselectvideo()
{
    // select video buttuon
    VideoBtnClickedFlag = true;
    PictureBtnClickedFlag = false;
    this->ui->label_prompt->hide();
    this->ui->listView->hide();
    this->ui->pushbutton_selectpicture->hide();
    this->ui->lineEdit_2->show();
    this->ui->pushButton_selectvideo->show();
}

void Widget::Justselectpicture()
{
    // select picture button
    VideoBtnClickedFlag = false;
    PictureBtnClickedFlag = true;
    this->ui->label_prompt->show();
    this->ui->listView->show();
    this->ui->pushbutton_selectpicture->show();
    this->ui->lineEdit_2->hide();
    this->ui->pushButton_selectvideo->hide();
}

void Widget::SethorizontalSlidervolume()
{
    this->ui->horizontalSlider_volume->setValue((int)this->ui->spinBox_volume->value()*1);
   // qDebug()<<"\n" << "SethorizontalSlidervolume";
}
void Widget::SetspinBoxvolume()
{
    this->ui->spinBox_volume->setValue((int)this->ui->horizontalSlider_volume->value()/1);
}

void Widget::SethorizontalSliderbright()
{
    this->ui->horizontalSlider_bright->setValue((int)this->ui->spinBox_bright->value()*1);
}

void Widget::SetspinBoxbright()
{
    this->ui->spinBox_bright->setValue((int)this->ui->horizontalSlider_bright->value()/1);
}

void Widget::SethorizontalSliderbright_1()
{
    this->ui->horizontalSlider_1_bright->setValue((int)this->ui->spinBox_1_bright->value()*1);
}

void Widget::SetspinBoxbright_1()
{
    this->ui->spinBox_1_bright->setValue((int)this->ui->horizontalSlider_1_bright->value()/1);
}

void Widget::SethorizontalSliderbright_2()
{
    this->ui->horizontalSlider_2_bright->setValue((int)this->ui->spinBox_2_bright->value()*1);
}

void Widget::SetspinBoxbright_2()
{
    this->ui->spinBox_2_bright->setValue((int)this->ui->horizontalSlider_2_bright->value()/1);
}

// create xml files
void Widget::Createxml()
{
    QDomDocument Uiinfo;
    QDomProcessingInstruction strHeader;
    strHeader = Uiinfo.createProcessingInstruction("xml", "version=\'1.0\' encoding=\'UTF-8\'");
    Uiinfo.appendChild(strHeader);

    QDomElement root = Uiinfo.createElement("root");
    Uiinfo.appendChild(root);
    root.setAttribute("releasedate", GetCurTime());

    if (IsChecked_CheckBox(this->ui->checkBox_Reset))
    {
        QDomElement res = Uiinfo.createElement("reset");
        root.appendChild(res);
        QDomText no_text = Uiinfo.createTextNode("true");
        res.appendChild(no_text);
    }
    QDomElement resou = Uiinfo.createElement("resource");
    root.appendChild(resou);
    QDomElement title = Uiinfo.createElement("title");
    resou.appendChild(title);
    QDomText content = Uiinfo.createTextNode(GetLineEdit_text(this->ui->lineEdit));
    title.appendChild(content);

    QDomElement scrolltext = Uiinfo.createElement("scrolltext");
    resou.appendChild(scrolltext);
    QDomText scrtext = Uiinfo.createTextNode(GetTextEdit_text());
    scrolltext.appendChild(scrtext);
    if (VideoBtnClickedFlag)
    {
        QDomElement multmedia = Uiinfo.createElement("video");
        resou.appendChild(multmedia);

        QDomText videotext = Uiinfo.createTextNode(GetLineEdit_text(this->ui->lineEdit_2).split("/").last());
        multmedia.appendChild(videotext);
    }
    else if (PictureBtnClickedFlag)
    {
        QDomElement multmedia = Uiinfo.createElement("picture");
        resou.appendChild(multmedia);

        multmedia.setAttribute("interval", "3");
        int num = GetListview_counts();
        int i = 0;
        while(num)
        {
            QDomElement Item = Uiinfo.createElement("Item");
            QString temp;
            temp.clear();
            temp = QString::number(i);
            Item.setAttribute("index", temp);
            QString str = m_listview.at(i);
            QDomText videotext = Uiinfo.createTextNode(str.split("/").last());
            Item.appendChild(videotext);
            multmedia.appendChild(Item);
            i++;
            num --;
        }
    }

    QDomElement parameter = Uiinfo.createElement("parameter");
    root.appendChild(parameter);

    QDomElement volume = Uiinfo.createElement("volume");
    parameter.appendChild(volume);
    QDomText vol = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_volume));
    volume.appendChild(vol);

    QDomElement bright = Uiinfo.createElement("brightness");
    parameter.appendChild(bright);
    QDomText bri = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_bright));
    bright.appendChild(bri);
    if(IsChecked_CheckBox(this->ui->checkBox_fullscreen))
    {
        QDomElement fullscreen = Uiinfo.createElement("fullscreen");
        parameter.appendChild(fullscreen);
        QDomText fullscreentext = Uiinfo.createTextNode("true");
        fullscreen.appendChild(fullscreentext);
    }

    if (IsChecked_CheckBox(this->ui->checkBox_hideScroText))
    {
        QDomElement scrollarea = Uiinfo.createElement("scrollarea");
        parameter.appendChild(scrollarea);
        QDomText scrollareatext = Uiinfo.createTextNode("true");
        scrollarea.appendChild(scrollareatext);
    }

    if (IsChecked_CheckBox(this->ui->checkBox_hideTime))
    {
        QDomElement timearea = Uiinfo.createElement("timearea");
        parameter.appendChild(timearea);
        QDomText timeareatext = Uiinfo.createTextNode("true");
        timearea.appendChild(timeareatext);
    }

    if (IsChecked_CheckBox(this->ui->checkBox_hideTitle))
    {
        QDomElement titlearea = Uiinfo.createElement("titlearea");
        parameter.appendChild(titlearea);
        QDomText titleareatext = Uiinfo.createTextNode("true");
        titlearea.appendChild(titleareatext);
    }

    QDomElement standby = Uiinfo.createElement("standby");
    parameter.appendChild(standby);
    QDomElement stageone = Uiinfo.createElement("stageone");
    standby.appendChild(stageone);

    QDomElement stageonetime = Uiinfo.createElement("time");
    stageone.appendChild(stageonetime);
    QDomText stageonetimetext = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_1_time));
    stageonetime.appendChild(stageonetimetext);

    QDomElement stageonebright = Uiinfo.createElement("brightness");
    stageone.appendChild(stageonebright);
    QDomText stageonebrighttext = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_1_bright));
    stageonebright.appendChild(stageonebrighttext);

    QDomElement stagetwo = Uiinfo.createElement("stagetwo");
    standby.appendChild(stagetwo);
    QDomElement stagetwotime = Uiinfo.createElement("time");
    stagetwo.appendChild(stagetwotime);
    QDomText stagetwotimetext = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_2_time));
    stagetwotime.appendChild(stagetwotimetext);

    QDomElement stagetwobright = Uiinfo.createElement("brightness");
    stagetwo.appendChild(stagetwobright);
    QDomText stagetwobrighttext = Uiinfo.createTextNode(GetspinBox_text(this->ui->spinBox_2_bright));
    stagetwobright.appendChild(stagetwobrighttext);

    QDomElement Timeformat = Uiinfo.createElement("time");
    parameter.appendChild(Timeformat);
    QDomElement tformat = Uiinfo.createElement("format");
    Timeformat.appendChild(tformat);
    QDomText timetext;
    if (RadioBoxFlag_Twelveformat)
    {
         timetext = Uiinfo.createTextNode("12");
    }
    else if (RadioBoxFlag_Twentyfourformat)
    {
        timetext = Uiinfo.createTextNode("24");
    }
    tformat.appendChild(timetext);

    QDomElement dataformat = Uiinfo.createElement("data");
    parameter.appendChild(dataformat);
    QDomElement dformat = Uiinfo.createElement("format");
    dataformat.appendChild(dformat);
    QDomText datatext;
    if (yearmouthday)
    {
        datatext = Uiinfo.createTextNode("yyyy/mm/dd");
    }
    else if(mouthdayyear)
    {
        datatext = Uiinfo.createTextNode("mm/dd/yyyy");
    }
    dformat.appendChild(datatext);

//    QString strPath = "C:/Users/yonghao/Desktop/libo/conf.xml";
    QString strPath = GetCheckedDisk().DirPath + "cfg.xml";
    qDebug()<<strPath;
    QFile file( strPath );
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out.setCodec("UTF-8");
    Uiinfo.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();
}

QString Widget::GetLineEdit_text(QLineEdit *line)
{
    return line->text();
}

int Widget::GetListview_counts()
{
    int i = 0;
    QStringList::Iterator it = m_listview.begin();
    while(it != m_listview.end())
    {
        qDebug() << (*it) << '\n';
        QString filename = m_listview.at(i);
        qDebug() << filename << '\n';
        QString lastname = filename.split("/").last();
//        QString newname = "C:/Users/yonghao/Desktop/libo/"+ lastname;
        QString newname = GetCheckedDisk().DirPath + lastname;
        qDebug() << lastname << '\n';
        qDebug() << newname << '\n';
        i++;
        ++it;
    }
    qDebug() << i << '\n';
    return i;
}

QString Widget::GetTextEdit_text()
{
    qDebug() << this->ui->textEdit->toPlainText() <<'\n';
    return this->ui->textEdit->toPlainText();
}

QString Widget::GetspinBox_text(QSpinBox *box)
{
    qDebug() << box->text() <<'\n';
    return box->text();
}

bool Widget::IsChecked_CheckBox(QCheckBox *Cbox)
{
    return Cbox->checkState();
}

bool Widget::GetvideoClicked()
{
    return VideoBtnClickedFlag;
}

bool Widget::GetpictureClicked()
{
    return PictureBtnClickedFlag;
}

QStringList Widget::GetPictureList()
{
    return m_listview;
}

QString Widget::getVideoList()
{
    return m_videoname;
}

void Widget::copyStation(int flag, QString qsText = NULL)
{
    switch(flag)
    {
        case COPY_START: progDlg->setWindowTitle("start copy"); break;

        case COPY_STOP: qDebug()<<"stop is coming\n";
        progDlg->setWindowTitle("copy end...");
//        ui->pushButton_copy->setEnabled(true);
        progDlg->setHidden(true);
        break;

        case COPY_FILE_NAME:
        progDlg->setWindowTitle(qsText);
        break;

        case COPY_TOTAL_RATE:
        progDlg->setValue(qsText.toInt());
        break;

        case COPY_SINGLE_RATE:
        progDlg->setValue(qsText.toInt());
        break;

        case COPY_ERROR_MEM_FULL:
        progDlg->setWindowTitle("the storage is fulled!");
        break;
    }
}

void Widget::on_copyBtnclicked()
{
    if (JudgeCreateXml())
    {
        QMessageBox::critical(NULL, "critical", "text area is empty can't create config file");
        return;
    }
//    if(this->ui->lineEdit_2->text().isEmpty() && 0 == GetListview_counts())
//    {
//        QMessageBox::critical(NULL, "critical", "copy files is Empty");
//    }
    else
    {
        Createxml();
        progDlg  = new QProgressDialog("Operation in progress.","cancel", 0, 100);
        progDlg->setModal(true);
        if (NULL == progDlg)
        {
            qDebug()<< "progDlg is NULL ,end programe!";
            return;
        }
        connect(progDlg, SIGNAL(canceled()), this, SLOT(CloseProgress()));
        progDlg->setMinimumDuration(1);
        QStringList list;
        list.clear();
        if (GetvideoClicked())
        {
            list<<getVideoList();
        }
        else if (GetpictureClicked())
        {
            list = GetPictureList();
        }
        cfilethread->setDesDirPath(GetCheckedDisk().DirPath); //set copyfile Despath
        qDebug()<<"list = "<<list;
        cfilethread->copyStart(list);
    }
}

void Widget::CloseProgress()
{
    qDebug()<<"canceled is clicked\n";
    progDlg->setHidden(true);
    progDlg->close();
//    delete progDlg;
    cfilethread->copyStop();
    cfilethread->wait();
}

QString Widget::GetCurTime()
{
    QDateTime Time = QDateTime::currentDateTime();
    QString tim = Time.toString("yyyy/MM/dd");
    qDebug()<<"tim = "<< tim<<'\n';
    return tim;
}

void Widget::SetTimeformatclicked_twelve()
{
    RadioBoxFlag_Twelveformat = true;
    RadioBoxFlag_Twentyfourformat = false;
}

void Widget::SetTimeformatclicked_twentyfour()
{
    RadioBoxFlag_Twelveformat = false;
    RadioBoxFlag_Twentyfourformat = true;
}

void Widget::Setyearmouthday()
{
    yearmouthday = true;
    mouthdayyear = false;
}

void Widget::Setmouthdayyear()
{
    yearmouthday = false;
    mouthdayyear = true;
}

bool Widget::JudgeCreateXml()
{
    if ((this->ui->lineEdit_2->text().isEmpty() && 0 == GetListview_counts())
        || this->ui->lineEdit->text().isEmpty()
        || this->ui->textEdit->toPlainText().isEmpty()) {
        Istrue = true;
    }
    else
    {
        Istrue = false;
    }
    return Istrue;
}
