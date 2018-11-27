#include "configureholder.h"
#include "define.h"
#include <QCoreApplication>
#include <QDate>
#include <QRegExp>
#include <QChar>
#include <QDir>

QString yysArrays[3][3] = {QString("China Mobile"), QString("China Unicom"), QString(""),
                          QString("M1"), QString("SingTel"), QString("Starhub"),
                           QString(""), QString(""), QString("")};

ConfigureHolder::ConfigureHolder(QObject *parent) : QObject(parent)
{
    clearParameters();
}

ConfigureHolder::~ConfigureHolder()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
}

void ConfigureHolder::InitDoc()
{
    QDomDocument Uiinfo;
    QDomElement root = Uiinfo.createElement("root");
    Uiinfo.appendChild(root);
}

bool ConfigureHolder::createXml(QString dir)
{
    Q_UNUSED(dir);
    QDomDocument cfgXml;
    QDomElement rootElement = cfgXml.createElement("root");
    cfgXml.appendChild(rootElement);
    QDate currentDate = QDate::currentDate();
    rootElement.setAttribute("releasedate", currentDate.toString("yyyy/MM/dd"));
    QtSleep(10);
    if(isReset) //isReset is true;
    {
        QDomElement resetElement = cfgXml.createElement("reset");
        resetElement.appendChild(cfgXml.createTextNode("true"));
        rootElement.appendChild(resetElement);
    }
    else
    {
        QDomElement resourceElement = cfgXml.createElement("resource");

        if(isMediaContentEnabled)
        {
            if(videoOrPicture)
            {
                QFileInfo videoInfo(videoPath);
                QString dstName = "0."+videoInfo.suffix();

                QDomElement videoElement = cfgXml.createElement("video");
                videoElement.appendChild(cfgXml.createTextNode(dstName));
                resourceElement.appendChild(videoElement);
            }
            else if (audioSelect)
            {
                QFileInfo videoInfo(videoPath);
                QString dstName = "0."+videoInfo.suffix();

                QDomElement videoElement = cfgXml.createElement("audio");
                videoElement.appendChild(cfgXml.createTextNode(dstName));
                resourceElement.appendChild(videoElement);
            }
            else
            {
                QDomElement pictureElement = cfgXml.createElement("picture");
                pictureElement.setAttribute("interval",getPicInterval());

                for(int i = 0; i<pictureList.count(); ++i)
                {
                    QString pa = pictureList.at(i);
                    QFileInfo imageInfo(pa);
                    QString dstName = QString("%1.%2").arg(i).arg(imageInfo.suffix());
                    QDomElement element = cfgXml.createElement("item");
                    element.setAttribute("index", i);
                    element.appendChild(cfgXml.createTextNode(dstName));
                    pictureElement.appendChild(element);
                }
                resourceElement.appendChild(pictureElement);
            }
        }
        QDomElement parameterElement = cfgXml.createElement("parameter");

        if(bBrightnessVolume || 1 == mBrightOrVolumeChanged)
        {
            QDomElement volumeElement = cfgXml.createElement("volume");
            volumeElement.appendChild(cfgXml.createTextNode(QString("%1").arg(vVolume)));
            parameterElement.appendChild(volumeElement);

            QDomElement brightnessElement = cfgXml.createElement("brightness");
            brightnessElement.appendChild(cfgXml.createTextNode(QString("%1").arg(vBrightness)));
            parameterElement.appendChild(brightnessElement);
        }
        //        qDebug()<<"timeHasSelect = "<<timeHasSelect;
        //        if (1 == timeHasSelect)
        //        {
        if(isDateTime)
        {
            QDomElement timeElement = cfgXml.createElement("time");
            QDomElement tformatElement = cfgXml.createElement("format");
            tformatElement.appendChild(cfgXml.createTextNode(vTimeFormat));
            timeElement.appendChild(tformatElement);
            parameterElement.appendChild(timeElement);

            QDomElement dateElement = cfgXml.createElement("date");
            QDomElement dformatElement = cfgXml.createElement("format");
            dformatElement.appendChild(cfgXml.createTextNode(vDateFormat));
            dateElement.appendChild(dformatElement);
            parameterElement.appendChild(dateElement);

            QDomElement timeareaElement = cfgXml.createElement("timearea");
            timeareaElement.appendChild(cfgXml.createTextNode("true"));
            parameterElement.appendChild(timeareaElement);
        }
        else
        {
            QDomElement timeareaElement = cfgXml.createElement("timearea");
            timeareaElement.appendChild(cfgXml.createTextNode("false"));
            parameterElement.appendChild(timeareaElement);
        }
        //        }
        QtSleep(10);
        if(bStandby)
        {
            QDomElement standbyElement = cfgXml.createElement("standby");

            QDomElement stageoneElement = cfgXml.createElement("stageone");
            QDomElement time1Element = cfgXml.createElement("time");

            time1Element.appendChild(cfgXml.createTextNode(QString("%1").arg(vStage1Interval)));
            QDomElement brightness1Element = cfgXml.createElement("brightness");

            brightness1Element.appendChild(cfgXml.createTextNode(QString("%1").arg(vStage1Brightness)));
            stageoneElement.appendChild(time1Element);
            stageoneElement.appendChild(brightness1Element);

            QDomElement stagetwoElement = cfgXml.createElement("stagetwo");
            QDomElement time2Element = cfgXml.createElement("time");
            time2Element.appendChild(cfgXml.createTextNode(QString("%1").arg(vStage2Interval)));
            QDomElement brightness2Element = cfgXml.createElement("brightness");
            brightness2Element.appendChild(cfgXml.createTextNode(QString("%1").arg(vStage2Brightness)));
            stagetwoElement.appendChild(time2Element);
            stagetwoElement.appendChild(brightness2Element);

            standbyElement.appendChild(stageoneElement);
            standbyElement.appendChild(stagetwoElement);
            parameterElement.appendChild(standbyElement);
        }
        //        qDebug()<<"audioHasSelect = "<< audioHasSelect;
        //        if (1 == audioHasSelect)
        //        {
        if (audioSwither)
        {
            QDomElement audioswitherElement = cfgXml.createElement("audioswitch");
            audioswitherElement.appendChild(cfgXml.createTextNode("on"));
            parameterElement.appendChild(audioswitherElement);
        }
        else if (audioSwitherOff)
        {
            QDomElement audioswitherElement = cfgXml.createElement("audioswitch");
            audioswitherElement.appendChild(cfgXml.createTextNode("off"));
            parameterElement.appendChild(audioswitherElement);
        }
        //        }

        if(isfullscreen)
        {
            QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
            fullscreenElement.appendChild(cfgXml.createTextNode("true"));
            parameterElement.appendChild(fullscreenElement);
        }
        else
        {
            QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
            fullscreenElement.appendChild(cfgXml.createTextNode("false"));
            parameterElement.appendChild(fullscreenElement);
        }
        //        qDebug()<<"scrollHasSelect = "<< scrollHasSelect;
        //        if (1 == scrollHasSelect)
        //        {
        int ret = judgeString(this->vScrollText);
        qDebug()<<"xml vScrollText = "<< ret;
        if(isScrollText)
        {
            QDomElement scrollingareaElement = cfgXml.createElement("scrollingarea");
            scrollingareaElement.appendChild(cfgXml.createTextNode("true"));
            parameterElement.appendChild(scrollingareaElement);

            if (2 == ret)// 不为空也不全为空格时
            {
                QDomElement scrollingtextElement = cfgXml.createElement("scrollingtext");
                scrollingtextElement.appendChild(cfgXml.createTextNode(vScrollText));
                resourceElement.appendChild(scrollingtextElement);
            }
        }
        else
        {
            QDomElement scrollingareaElement = cfgXml.createElement("scrollingarea");
            scrollingareaElement.appendChild(cfgXml.createTextNode("false"));
            parameterElement.appendChild(scrollingareaElement);
        }
        //        }

        //        qDebug()<<"titleHasSelect = "<< titleHasSelect;
        //        if (1 == titleHasSelect)
        //        {
        int tret = judgeString(this->vTitle);
        qDebug()<<" xml vTitle = "<< tret;
        if(isTitle)
        {
            QDomElement titleareaElement = cfgXml.createElement("titlearea");
            titleareaElement.appendChild(cfgXml.createTextNode("true"));
            parameterElement.appendChild(titleareaElement);
            if (2 == tret) // 不为空也不全为空格时
            {
                QDomElement titleElement = cfgXml.createElement("title");
                titleElement.appendChild(cfgXml.createTextNode(vTitle));
                resourceElement.appendChild(titleElement);
            }
        }
        else
        {
            QDomElement titleareaElement = cfgXml.createElement("titlearea");

            titleareaElement.appendChild(cfgXml.createTextNode("false"));
            parameterElement.appendChild(titleareaElement);
        }
        //        }
        QtSleep(10);
        if (apnOn)
        {
            QDomElement apnElement = cfgXml.createElement("APN");

            QDomElement yysElement = cfgXml.createElement("network");// 运营商
            apnElement.appendChild(yysElement);
            yysElement.appendChild(cfgXml.createTextNode(apnYysText));

            QDomElement jrdElement = cfgXml.createElement("apn"); // 接入点
            apnElement.appendChild(jrdElement);
            jrdElement.appendChild(cfgXml.createTextNode(apnJrdText));

            QDomElement mccElement = cfgXml.createElement("mcc");
            apnElement.appendChild(mccElement);
            mccElement.appendChild(cfgXml.createTextNode(apnMccText));

            QDomElement mncElement = cfgXml.createElement("mnc");
            apnElement.appendChild(mncElement);
            mncElement.appendChild(cfgXml.createTextNode(apnMncText));

            QDomElement nameElement = cfgXml.createElement("user");
            apnElement.appendChild(nameElement);
            nameElement.appendChild(cfgXml.createTextNode(apnUNameText));

            QDomElement passwdElement = cfgXml.createElement("password");
            apnElement.appendChild(passwdElement);
            passwdElement.appendChild(cfgXml.createTextNode(apnPasswdText));

            parameterElement.appendChild(apnElement);
        }

        if (!isAutoSettingFlag) // 手动
        {
            QDomElement ipElement = cfgXml.createElement("IP");

            QDomElement typeElement = cfgXml.createElement("type");
            typeElement.appendChild(cfgXml.createTextNode("1"));
            ipElement.appendChild(typeElement);

            QDomElement ipAddressElement = cfgXml.createElement("ip");
            ipAddressElement.appendChild(cfgXml.createTextNode(IpAddressText));
            ipElement.appendChild(ipAddressElement);

            QDomElement subnetMaskElement = cfgXml.createElement("netmask");
            subnetMaskElement.appendChild(cfgXml.createTextNode(subnetText));
            ipElement.appendChild(subnetMaskElement);

            QDomElement defaultGateWayElement = cfgXml.createElement("route");
            defaultGateWayElement.appendChild(cfgXml.createTextNode(defaultGateWay));
            ipElement.appendChild(defaultGateWayElement);

            QDomElement dnsServiceElement = cfgXml.createElement("dns");
            dnsServiceElement.appendChild(cfgXml.createTextNode(dnsService));
            ipElement.appendChild(dnsServiceElement);

            parameterElement.appendChild(ipElement);
        }
        else  // 自动
        {
            QDomElement ipElement = cfgXml.createElement("IP");
            QDomElement typeElement = cfgXml.createElement("type");
            typeElement.appendChild(cfgXml.createTextNode("0"));
            ipElement.appendChild(typeElement);
            parameterElement.appendChild(ipElement);
        }
        if (msysTimeOn)
        {
            QDomElement systimeElement = cfgXml.createElement("systemtime");
            systimeElement.appendChild(cfgXml.createTextNode(sysTimeText));
            parameterElement.appendChild(systimeElement);
        }

        rootElement.appendChild(parameterElement);

        if(resourceElement.hasChildNodes())
            rootElement.appendChild(resourceElement);
    }
#if defined(Q_OS_WIN)
    QString strPath = dir + "\\update\\" + "mediascreen.xml";
#elif defined(Q_OS_LINUX)
    QString strPath = dir + "/update/" + "mediascreen.xml";
#endif
    QString tmpDstString = GetFilePath(strPath);
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE();
        return false;
    }
    QtSleep(10);
    QFile xmlFile(strPath);
    if(!xmlFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream out(&xmlFile);
    out.setCodec("UTF-8");
    cfgXml.save(out, 4, QDomNode::EncodingFromTextStream);
    xmlFile.close();
    return true;
}

bool ConfigureHolder::delDir(const QString path)
{
    qDebug()<<"is call "<<__PRETTY_FUNCTION__<<"path = "<<path;
    if (path.isEmpty())
    {
        return false;
    }
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug()<<"path is not exists";
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)
    { //遍历文件信息
        if (file.isFile())
        { // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else
        { // 递归删除
            delDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

void ConfigureHolder::startDelDir()
{
    delDir(usbPath);
}

void ConfigureHolder::orderFile(QString dir)
{
    qDebug()<<__PRETTY_FUNCTION__<<"dir = "<<dir;
    if(!isMediaContentEnabled)
    {
        return;
    }
    QString usbdir = dir;
#if defined(Q_OS_WIN)
    if (dir.endsWith("\\"))
    {
        usbdir  = dir + "update\\multimedia\\";
    }
    else
    {
        usbdir  = dir + "\\update\\multimedia\\";
    }
#elif defined(Q_OS_LINUX)
    if (dir.endsWith("/"))
    {
        usbdir  = dir + "update/multimedia/";
    }
    else
    {
        usbdir  = dir + "/update/multimedia/";
    }
#endif

    if(videoOrPicture || audioSelect)
    {
        IDE_TRACE_STR(videoPath);
        QFileInfo videoInfo(videoPath);
        if(!videoInfo.exists())
        {
            IDE_TRACE();
            return;
        }
        QString dstName = "0."+videoInfo.suffix();
        IDE_TRACE_STR(usbdir+dstName);
        copyReourceFile(videoPath, usbdir+dstName);
    }
    else
    {
        //qDebug()<<pictureList;
        for(int i = 0; i < pictureList.count(); ++i)
        {
            QString pa = pictureList.at(i);
            QFileInfo imageInfo(pa);
            QString dstName = QString("%1.%2").arg(i).arg(imageInfo.suffix());
            copyReourceFile(pa, usbdir + dstName);
        }
    }
}

bool ConfigureHolder::copyReourceFile(QString src, QString dst)
{
    if(0 == src.compare(dst, Qt::CaseInsensitive))
    {
        return true;
    }

    QFileInfo tmpSrcFileInfo(src);
    if(!tmpSrcFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString = GetFilePath(dst);
    if(tmpDstString.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE_STR(tmpDstString);
        return false;
    }

    bool flag = false;
    qint64 tmpSize = tmpSrcFileInfo.size();

    if(tmpSize > D_FILE_10M)
    {
        flag = false;
        IDE_TRACE();
        QFile tmpSrcFile(src);
        QFile tmpDstFile(dst);
        if(!tmpSrcFile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE();
            return false;
        }
        if(!tmpDstFile.open(QIODevice::WriteOnly))
        {
            IDE_TRACE();
            return false;
        }

        qint64 count = 0;
        qreal copyProgress = 0.0;
        while(!tmpSrcFile.atEnd())
        {
            QByteArray ba = tmpSrcFile.read(D_FILE_PER_BUFFER);
            tmpDstFile.write(ba);
            count +=ba.size();
            pro = count/(qreal)tmpSize;
            if(pro - copyProgress >= 0.01)
            {
                emit copyFileProgress(pro*100);
                // qDebug()<<"pro*100=" << pro*100;
            }
            if(pro == 1.0)
            {
                emit copyFileProgress(100);
                pro = 0.0;
                // qDebug()<<"copyFileProgress "<<pro*100;
            }
            QCoreApplication::processEvents();
        }
        tmpSrcFile.close();
        tmpDstFile.close();
        flag = true;
        emit copyFileProgress(0);
    }
    else
    {
        flag = QFile::copy(src, dst);
        //IDE_TRACE();
    }
    return flag;
}

QString ConfigureHolder::toWindwosPath(QString pFilePath)
{
    if (pFilePath.isEmpty())
    {
        return QString();
    }
    return pFilePath.replace("/", "\\");
}

QString ConfigureHolder::GetFilePath(QString pFilePath)
{
    if (pFilePath.isEmpty())
    {
        return QString();
    }
    pFilePath.replace("\\", "/");
    if(pFilePath.endsWith('/'))
    {
        return QString();
    }
    int index = pFilePath.lastIndexOf('/');
    if(index < 0)
    {
        return QString();
    }
    return pFilePath.mid(0, index + 1);
}

void ConfigureHolder::clearParameters()
{
    isReset = false;

    isMediaContentEnabled = false;
    isfullscreen = false;
    videoOrPicture = false;
    videoPath = "";
    pictureList.clear();

    bConfigureParameter = false;
    bBrightnessVolume = false;
    mBrightOrVolumeChanged = 0;
    bScrollText = false;
    bTitle = false;
    bDateTime = false;
    bStandby = false;

    //isBrightnessVolume = false;
    vVolume = 60;
    vBrightness = 80;

    //isScrollText = false;
    vScrollText="";

    //isTitle = false;
    vTitle="";

    //isDateTime = false;
    vTimeFormat = "12";
    vDateFormat = "yyyy.MM.dd";

    //isStandby = false;
    vStage1Interval = 90;
    vStage1Brightness = 15;
    vStage2Interval = 1200;
    vStage2Brightness = 0;
    vPicInterval = 3;
    scrollHasSelect = 0;
    titleHasSelect = 0;
    audioHasSelect = 0;
    timeHasSelect = 0;

    sysTimeText = QString();
    apnCountryIndex = -1;
    apnYysIndex = -1;
    apnYysText = QString();
    apnJrdText = QString();
    apnMccText = QString();
    apnMncText = QString();
    apnUNameText = QString();
    apnPasswdText = QString();
    isAutoSettingFlag = false;
    IpAddressText = QString();
    subnetText = QString();
    defaultGateWay = QString();
    dnsService = QString();
}

void ConfigureHolder::setDelDirPath(const QString path)
{
    QString mPath = path;
#if defined(Q_OS_WIN)
    if (!mPath.endsWith("\\"))
    {
        mPath = mPath + "\\update";
    }
    else
    {
        mPath = mPath + "\\update";
    }
#elif defined(Q_OS_LINUX)
    if (!mPath.endsWith("/"))
    {
        mPath = mPath + "/update";
    }
    else
    {
        mPath = mPath + "update";
    }
#endif
    qDebug()<<" mPath = "<< mPath;
    usbPath = mPath;
}

bool ConfigureHolder::resetScreen(bool isReset)
{
    this->isReset = isReset;
    return true;
}

void ConfigureHolder::sendCheckedSignal()
{
    IDE_TRACE_STR("emit resetDefaultClicked");
    emit resetDefaultClicked();
}

void ConfigureHolder::sendCanceledSignal()
{
    emit resetCanceled();
}

void ConfigureHolder::sendShowPreview()
{
    emit showPreviewSignal();
}

void ConfigureHolder::sendPreviewCheckedFalse()
{
    emit previewCheckedFalse();
}

void ConfigureHolder::sendStartPicAnimation()
{
    emit startPicAnimation();
}

void ConfigureHolder::sendIsFullScreenSignal(bool isflag)
{
    emit isFullScreenChanges(isflag);
}

void ConfigureHolder::sendCheckFullScreenSignal(bool isflag)
{
    Q_UNUSED(isflag);
    emit checkFullScreen();
}

void ConfigureHolder::sendHorizontalAutoPlay(bool isfull)
{
    if (isfull)
    {
        emit horizontalFullscreenAudioAutoPlay();
    }
    else
    {
        emit horizontalAudioAutoPlay();
    }
}

void ConfigureHolder::sendVerticalAutoPlay(bool isfull)
{
    if (isfull)
    {
        emit verticalFullscreenAudioAutoPlay();
    }
    else
    {
        emit verticalAudioAutoPlay();
    }
}

void ConfigureHolder::sendStopAudioPlay()
{
    emit stopAllAudioPlay();
}

void ConfigureHolder::sendPlayAfterClose(bool isfull)
{
    if (isfull)
    {
        emit startPlayAfterCloseFullScreen();
    }
    else
    {
        emit startPlayAfterCloseNormal();
    }
}

void ConfigureHolder::setBrightOrVolumeChanged(int para)
{
    qDebug()<<"para = "<< para;
    mBrightOrVolumeChanged = para;
}

bool ConfigureHolder::updateMultiMedia(bool isMediaContentEnabled, bool videoOrPicture, bool isAudio, bool isfullscreen, QString paths)
{
    this->isMediaContentEnabled = isMediaContentEnabled;
    this->videoOrPicture = videoOrPicture;
    this->audioSelect = isAudio;
    this->isfullscreen = isfullscreen;

    if(videoOrPicture || isAudio)
    {
        //IDE_TRACE_STR(paths);
#if defined(Q_OS_WIN)
        videoPath = toWindwosPath(paths);
#elif defined(Q_OS_LINUX)
        if (!videoPath.startsWith("/"))
        {
            videoPath = "/" + paths;
        }
#endif
        //IDE_TRACE_STR(videoPath);
    }
    else
    {
        QStringList tmpList = paths.split("*");
        pictureList.clear();
        for (int i = 0; i < tmpList.size(); ++i)
        {
            QString img = tmpList.at(i);
            if(!img.isEmpty())
            {
                #if defined(Q_OS_WIN)
                    pictureList.append(toWindwosPath(img));
                #elif defined(Q_OS_LINUX)
                    if (!img.startsWith("/"))
                    {
                        img = "/" + img;
                    }
                    pictureList.append(img);
                #endif
            }
        }
    }

    return true;
}

QVariant ConfigureHolder::retPicturePath(QString paths)
{
    QStringList tmpList = paths.split("*");
    pictureList2.clear();
    for (int i = 0; i < tmpList.size(); ++i)
    {
        QString img = tmpList.at(i);
        if(!img.isEmpty())
        {
            #if defined(Q_OS_WIN)
                pictureList2.append(img);
            #elif defined(Q_OS_LINUX)
                if (!img.startsWith("/"))
                {
                    img = "/" + img;
                }
                pictureList2.append(img);
            #endif
        }
    }
    qDebug()<<"picturelist2 = "<<pictureList2;
    return pictureList2;
}

bool ConfigureHolder::updateParameterBasic(bool bConfigureParameter, bool bBrightnessVolume, bool bScrollText, bool bTitle, bool bDateTime, bool bStandby, bool isScrollText, bool isTitle, bool isDateTime, bool audioSel)
{
    this->bConfigureParameter = bConfigureParameter;
    this->bBrightnessVolume = bBrightnessVolume;
    this->bScrollText = bScrollText;
    this->bTitle = bTitle;
    this->bDateTime = bDateTime;
    this->bStandby = bStandby;
    this->isScrollText =  isScrollText;
    this->isTitle = isTitle;
    this->isDateTime = isDateTime;
    this->audioSwither = audioSel;
    return true;
}

bool ConfigureHolder::updateParameterBasic1(bool bConfigureParameter, bool bBrightnessVolume, bool bScrollText, bool bTitle, bool isScrollText, bool isTitle, int scrollFlag, int titleFlag)
{
    this->bConfigureParameter = bConfigureParameter;
    this->bBrightnessVolume = bBrightnessVolume;
    this->bScrollText = bScrollText;
    this->bTitle = bTitle;
    this->isScrollText = isScrollText;
    this->isTitle = isTitle;
    this->scrollHasSelect = scrollFlag;
    this->titleHasSelect = titleFlag;
    return true;
}

bool ConfigureHolder::updateArea1ParaAudio(bool audioOn, bool audioOff)
{
    this->audioSwither = audioOn;
    this->audioSwitherOff = audioOff;
    return true;
}

bool ConfigureHolder::updateParameterBasic2(bool bConfigureParameter, bool bDateTime, bool bStandby, bool isDateTime, bool audioSel, bool audioOff, int timeFlag, int audioFlag)
{
    Q_UNUSED(audioOff);
    Q_UNUSED(audioSel);
    this->bConfigureParameter = bConfigureParameter;
    this->bDateTime = bDateTime;
    this->bStandby = bStandby;
    this->isDateTime = isDateTime;
//    this->audioSwither = audioSel;
//    this->audioSwitherOff = audioOff;
    this->timeHasSelect = timeFlag;
    this->audioHasSelect = audioFlag;
    return true;
}

bool ConfigureHolder::updateBrightnessVolume(qint32 vBrightness, qint32 vVolume)
{
    this->vBrightness = vBrightness;
    this->vVolume = vVolume;
    IDE_TRACE_INT(vBrightness);
    IDE_TRACE_INT(vVolume);
    return true;
}

int ConfigureHolder::judgeString(QString str)
{
    int ret = -1;
    if (str.isEmpty())
    {
        ret = 0;
        qDebug()<<"ret = "<< ret <<" isEmpty";
        return ret;
    }
    else
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (QChar(' ') != str.at(i)) // 不全为空格
            {
                ret = 2;
                qDebug()<<"ret = "<< ret <<" no all Space";
                return ret;
            }
            else
            {
                if (i == str.length() - 1)  // 全部为空格
                {
                    ret = 1; // 全部为空格
                    qDebug()<<"ret = "<< ret <<"all Space";
                    return ret;
                }
            }
        }
    }
    return ret;
}

bool ConfigureHolder::updateScrollText(QString vScrollText)
{
    this->vScrollText = vScrollText;
    return true;
}

bool ConfigureHolder::updateTitle(QString vTitle)
{
    this->vTitle = vTitle;
    return true;
}

bool ConfigureHolder::updateDateTime(QString vTimeFormat, QString vDateFormat)
{
    this->vTimeFormat = vTimeFormat;
    this->vDateFormat = vDateFormat;
    return true;
}

bool ConfigureHolder::updateStandby(quint32 vStage1Interval, quint8 vStage1Brightness, quint32 vStage2Interval, quint8 vStage2Brightness)
{
    this->vStage1Interval = vStage1Interval;
    this->vStage1Brightness = vStage1Brightness;
    this->vStage2Interval = vStage2Interval;
    this->vStage2Brightness = vStage2Brightness;
    return true;
}

bool ConfigureHolder::updatePictureInterval(qint32 vPictureInterval)
{
    this->vPicInterval = vPictureInterval;
    IDE_TRACE_INT(vPicInterval);
    return true;
}

int ConfigureHolder::getPicInterval()
{
    IDE_TRACE_INT(vPicInterval);
    return vPicInterval;
}

void ConfigureHolder::setSystimeText(QString ptext, bool sysTimeOn)
{
    msysTimeOn = sysTimeOn;
    sysTimeText = ptext;
    qDebug()<<"systimeText = "<<sysTimeText;
}

void ConfigureHolder::setCountryIndex(int pCountryIndex, int pYysIndex)
{
    apnCountryIndex = pCountryIndex;
    apnYysIndex = pYysIndex;
    qDebug()<<"apnCountryIndex = "<<apnCountryIndex<<"apnYysIndex = "<<apnYysIndex;
}

void ConfigureHolder::setApnFlag(bool pApnOn)
{
    apnOn = pApnOn;
}

void ConfigureHolder::setApnParameter(QString papnYysText, QString papnJrdText, QString papnMccText, QString papnMncText, QString papnUNameText, QString papnPasswdText)
{
    if (2 == apnCountryIndex) // 如果用户选择了others选项
    {
        apnYysText = papnYysText; // 用户输入的文本赋值给apnYysText，否则使用数组中的字符串
    }
    else
    {
       apnYysText = yysArrays[apnCountryIndex][apnYysIndex];
    }
    apnJrdText = papnJrdText;
    apnMccText = papnMccText;
    apnMncText = papnMncText;
    apnUNameText = papnUNameText;
    apnPasswdText = papnPasswdText;
    qDebug()<<"apnYysText = "<<apnYysText<<"apnJrdText = "<<apnJrdText;
    qDebug()<<"apnMccText = "<<apnMccText<<"apnMncText = "<<apnMncText;
    qDebug()<<"apnUNameText = "<<apnUNameText<<"apnPasswdText = "<<apnPasswdText;
}

void ConfigureHolder::setIpParameter(bool pIsAutoSettingFlag, QString pIpAddressText, QString psubnetText, QString pdefaultGateWay, QString pdnsService)
{
    isAutoSettingFlag = pIsAutoSettingFlag;
    if (!isAutoSettingFlag)
    {
        IpAddressText = pIpAddressText;
        subnetText = psubnetText;
        defaultGateWay = pdefaultGateWay;
        dnsService = pdnsService;
        qDebug()<<"isAutoSettingFlag = "<<isAutoSettingFlag;
        qDebug()<<"IpAddressText = "<<IpAddressText<<"subnetText = "<<subnetText;
        qDebug()<<"defaultGateWay = "<<defaultGateWay<<"dnsService = "<<dnsService;
    }
}

void ConfigureHolder::setArea2ParameterAllFlag(bool pHidetimeFlag, bool pHideApnFlag, bool pIpSettingAutoFlag)
{
    isDateTime = pHidetimeFlag;
    apnOn = pHideApnFlag;
    isAutoSettingFlag = pIpSettingAutoFlag;
}

