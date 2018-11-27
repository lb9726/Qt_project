#include "configureholder.h"
#include "define.h"
#include <QCoreApplication>
#include <QDate>

ConfigureHolder::ConfigureHolder(QObject *parent) : QObject(parent)
{
    clearParameters();
    jobsMap.clear();
    appropriateJobNumber = 2;
    GetAppropriateJobNumber();
    isMultiThreadCopying = false;
    fileTotalBytes = 0;
    copyedBytes = 0;
    perJobCopyedBytes = new qint64[appropriateJobNumber];
}

void ConfigureHolder::InitDoc(){
    QDomDocument Uiinfo;
    QDomElement root = Uiinfo.createElement("root");
    Uiinfo.appendChild(root);
}

bool ConfigureHolder::createXml(QString dir)
{
    QDomDocument cfgXml;
    QDomElement rootElement = cfgXml.createElement("root");
    cfgXml.appendChild(rootElement);
    QDate currentDate = QDate::currentDate();
    rootElement.setAttribute("releasedate",currentDate.toString("yyyy/MM/dd"));
    //isReset = true;
    if(isReset){
        QDomElement resetElement = cfgXml.createElement("reset");
        resetElement.appendChild(cfgXml.createTextNode("true"));
        rootElement.appendChild(resetElement);
    }else {
        QDomElement resourceElement = cfgXml.createElement("resource");
        //if(isMediaContentEnabled||bScrollText||bTitle){

        if(isMediaContentEnabled){
            if(videoOrPicture){
                QFileInfo videoInfo(videoPath);
                QString dstName = "0."+videoInfo.suffix();

                QDomElement videoElement = cfgXml.createElement("video");
                videoElement.appendChild(cfgXml.createTextNode(dstName));
                resourceElement.appendChild(videoElement);
            }else{
                QDomElement pictureElement = cfgXml.createElement("picture");
                pictureElement.setAttribute("interval",3);

                for(int i=0;i<pictureList.count();i++){
                    QString pa = pictureList.at(i);
                    QFileInfo imageInfo(pa);
                    QString dstName = QString("%1.%2").arg(i).arg(imageInfo.suffix());
                    QDomElement element = cfgXml.createElement("item");
                    element.setAttribute("index",i);
                    element.appendChild(cfgXml.createTextNode(dstName));
                    pictureElement.appendChild(element);
                }
                resourceElement.appendChild(pictureElement);
            }
        }

        if(bConfigureParameter&&(bBrightnessVolume||bDateTime||bStandby||bScrollText||bTitle)){
            QDomElement parameterElement = cfgXml.createElement("parameter");

            if(bBrightnessVolume){
                QDomElement volumeElement = cfgXml.createElement("volume");
                volumeElement.appendChild(cfgXml.createTextNode(QString("%1").arg(vVolume)));
                parameterElement.appendChild(volumeElement);

                QDomElement brightnessElement = cfgXml.createElement("brightness");
                brightnessElement.appendChild(cfgXml.createTextNode(QString("%1").arg(vBrightness)));
                parameterElement.appendChild(brightnessElement);
            }
            if(bDateTime){
                if(isDateTime){
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

                }else{
                    QDomElement timeareaElement = cfgXml.createElement("timearea");
                    timeareaElement.appendChild(cfgXml.createTextNode("false"));
                    parameterElement.appendChild(timeareaElement);
                }
            }
            if(bStandby){
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
            if(isMediaContentEnabled){
                if(isfullscreen){
                    QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
                    fullscreenElement.appendChild(cfgXml.createTextNode("true"));
                    parameterElement.appendChild(fullscreenElement);
                }else{
                    QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
                    fullscreenElement.appendChild(cfgXml.createTextNode("false"));
                    parameterElement.appendChild(fullscreenElement);
                }
            }
            if(bScrollText){
                if(isScrollText){
                    QDomElement scrollingareaElement = cfgXml.createElement("scrollingarea");
                    scrollingareaElement.appendChild(cfgXml.createTextNode("true"));
                    parameterElement.appendChild(scrollingareaElement);

                    QDomElement scrollingtextElement = cfgXml.createElement("scrollingtext");
                    scrollingtextElement.appendChild(cfgXml.createTextNode(vScrollText));
                    resourceElement.appendChild(scrollingtextElement);

                }else{
                    QDomElement scrollingareaElement = cfgXml.createElement("scrollingarea");
                    scrollingareaElement.appendChild(cfgXml.createTextNode("false"));
                    parameterElement.appendChild(scrollingareaElement);
                }
            }
            if(bTitle){
                if(isTitle){
                    QDomElement titleareaElement = cfgXml.createElement("titlearea");

                    titleareaElement.appendChild(cfgXml.createTextNode("true"));
                    parameterElement.appendChild(titleareaElement);


                    QDomElement titleElement = cfgXml.createElement("title");
                    titleElement.appendChild(cfgXml.createTextNode(vTitle));
                    resourceElement.appendChild(titleElement);
                }else{
                    QDomElement titleareaElement = cfgXml.createElement("titlearea");

                    titleareaElement.appendChild(cfgXml.createTextNode("false"));
                    parameterElement.appendChild(titleareaElement);
                }
            }
            rootElement.appendChild(parameterElement);
        }else{
            if(isMediaContentEnabled){
                if(isfullscreen){
                    QDomElement parameterElement = cfgXml.createElement("parameter");
                    QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
                    fullscreenElement.appendChild(cfgXml.createTextNode("true"));
                    parameterElement.appendChild(fullscreenElement);
                    rootElement.appendChild(parameterElement);
                }else{
                    QDomElement parameterElement = cfgXml.createElement("parameter");
                    QDomElement fullscreenElement = cfgXml.createElement("fullscreen");
                    fullscreenElement.appendChild(cfgXml.createTextNode("false"));
                    parameterElement.appendChild(fullscreenElement);
                    rootElement.appendChild(parameterElement);
                }
            }
        }
        if(resourceElement.hasChildNodes())
            rootElement.appendChild(resourceElement);
    }

    QString strPath = dir + "\\update\\" + "mediascreen.xml";
    QString tmpDstString = GetFilePath(strPath);
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE();
        return false;
    }

    QFile xmlFile( strPath );
    if(!xmlFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QTextStream out(&xmlFile);
    out.setCodec("UTF-8");
    cfgXml.save(out, 4, QDomNode::EncodingFromTextStream);
    xmlFile.close();
    return true;
}

void ConfigureHolder::orderFile(QString dir)
{
    if(!isMediaContentEnabled){
        return;
    }

    QString usbdir  = dir+"\\update\\multimedia\\";
    usbPath = usbdir;
    if(videoOrPicture){
        IDE_TRACE_STR(videoPath);
        QFileInfo videoInfo(videoPath);
        if(!videoInfo.exists()){
            IDE_TRACE();
            return;
        }
        QString dstName = "0."+videoInfo.suffix();
        IDE_TRACE_STR(usbdir+dstName);
        copyReourceFile(videoPath, usbdir+dstName);
    }else{
        //qDebug()<<pictureList;
        for(int i=0;i<pictureList.count();i++){
            QString pa = pictureList.at(i);
            QFileInfo imageInfo(pa);
            QString dstName = QString("%1.%2").arg(i).arg(imageInfo.suffix());
            copyReourceFile(pa, usbdir+dstName);
        }
    }
}

bool ConfigureHolder::copyReourceFile(QString src, QString dst)
{
    if(src.compare(dst, Qt::CaseInsensitive) == 0)
        return true;

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
    /*    if(tmpSize > D_FILE_512M){
        flag = copyFileMulti(src,dst);
        IDE_TRACE();
    }
    else*/ if(tmpSize > D_FILE_10M)
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
            qreal pro = count/(qreal)tmpSize;
            if(pro - copyProgress >=0.01){
                emit copyFileProgress(pro*100);
                ///qDebug()<<pro*100;
            }
            if(pro == 1.0){
                emit copyFileProgress(100);
                //qDebug()<<"copyFileProgress "<<pro*100;
            }

            QCoreApplication::processEvents();
        }
        tmpSrcFile.close();
        tmpDstFile.close();
        flag = true;
    }
    else
    {
        flag = QFile::copy(src, dst);
        //IDE_TRACE();
    }
    return flag;
}

QString ConfigureHolder::toWindwosPath(QString pFilePath){
    if (pFilePath.isEmpty())
        return QString();

    return pFilePath.replace("/", "\\");
}

QString ConfigureHolder::GetFilePath(QString pFilePath)
{

    if (pFilePath.isEmpty())
        return QString();
    pFilePath.replace("\\", "/");
    if(pFilePath.endsWith('/')){
        return QString();
    }
    int index = pFilePath.lastIndexOf('/');
    if(index < 0)
        return QString();
    return pFilePath.mid(0, index + 1);
}

bool ConfigureHolder::copyFileMulti(QString src,QString dst)
{
    QFile srcfile(src);
    QFile dstfile(dst);

    if(!srcfile.open(QIODevice::ReadOnly)){
        IDE_TRACE();
        return false;
    }

    if(!dstfile.open(QIODevice::WriteOnly)){
        IDE_TRACE();
        return false;
    }

    QFileInfo fileInfo(src);
    fileTotalBytes = fileInfo.size();
    if(fileTotalBytes == 0){
        return false;
    }
    dstfile.resize(fileInfo.size());

    perJobLoad =  fileInfo.size()/appropriateJobNumber;
    lastJobLoad = fileInfo.size()/appropriateJobNumber + fileInfo.size()%appropriateJobNumber;
    jobsMap.clear();

    for(quint32 i=0;i<appropriateJobNumber-1;i++){
        CopyJob* copyJob = new CopyJob();
        copyJob->setJob(i+1,src,dst,perJobLoad*i,perJobLoad);
        jobsMap.insert(i+1,copyJob);
        connect(copyJob,&CopyJob::copyedbytes,this,&ConfigureHolder::copyProgressMult);
        connect(copyJob,&CopyJob::jobFinished,this,&ConfigureHolder::jobFinished);
    }
    CopyJob* lastJob = new CopyJob();
    lastJob->setJob(appropriateJobNumber,src,dst,perJobLoad*(appropriateJobNumber-1),lastJobLoad);
    jobsMap.insert(appropriateJobNumber,lastJob);
    connect(lastJob,&CopyJob::copyedbytes,this,&ConfigureHolder::copyProgressMult);
    connect(lastJob,&CopyJob::jobFinished,this,&ConfigureHolder::jobFinished);
    startJobs();
    return true;
}

void ConfigureHolder::startJobs(){
    CopyJob* job;
    copyedBytes = 0;
    copyPercent = 0.0;
    QMap<int,CopyJob*>::const_iterator i = jobsMap.constBegin();
    while (i != jobsMap.constEnd()) {
        job = i.value();
        job->start();
        ++i;
    }
}

void ConfigureHolder::copyProgressMult(int id ,qint64 copyBytes){
    qint64 tmp = 0;
    perJobCopyedBytes[id-1] = copyBytes;
    for(quint32 i=0;i<appropriateJobNumber;i++){
        tmp += perJobCopyedBytes[i];
    }
    copyedBytes = tmp;
    //qDebug()<<"fileTotalBytes:"<<fileTotalBytes <<"copyBytes: "<<copyedBytes;
    qreal pro = (qreal)copyedBytes/fileTotalBytes;
    if((pro - copyPercent) >=0.01){
        int progress = pro*100;
        copyPercent = pro;
        emit copyFileProgress(progress);
    }
    if(pro == 1.0){
        emit copyFileProgress(100);
    }
}

void ConfigureHolder::jobFinished(int id){
    CopyJob* job = jobsMap.value(id);
    job->deleteLater();
    jobsMap.remove(id);
    if(jobsMap.isEmpty()){
        IDE_TRACE_STR("copy finished.");
    }
}

void ConfigureHolder::GetAppropriateJobNumber(){
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    appropriateJobNumber = sysInfo.dwNumberOfProcessors;
}

void ConfigureHolder::clearParameters(){
    isReset = false;

    isMediaContentEnabled = false;
    isfullscreen = false;
    videoOrPicture = false;
    videoPath = "";
    pictureList.clear();

    bConfigureParameter = false;
    bBrightnessVolume = false;
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
    vDateFormat = "yyyy.mm.dd";

    //isStandby = false;
    vStage1Interval = 90;
    vStage1Brightness = 15;
    vStage2Interval = 1200;
    vStage2Brightness = 0;
}

bool ConfigureHolder::resetScreen(bool isReset)
{
    this->isReset = isReset;
    return true;
}

bool ConfigureHolder::updateMultiMedia(bool isMediaContentEnabled, bool videoOrPicture, bool isfullscreen, QString paths)
{
    this->isMediaContentEnabled = isMediaContentEnabled;
    this->videoOrPicture = videoOrPicture;
    this->isfullscreen = isfullscreen;

    if(videoOrPicture){
        //IDE_TRACE_STR(paths);
        videoPath = toWindwosPath(paths);
        //IDE_TRACE_STR(videoPath);
    }else{
        QStringList tmpList = paths.split("*");
        pictureList.clear();
        for (int i = 0; i < tmpList.size(); ++i){
            QString img = tmpList.at(i);
            if(!img.isEmpty()){
                pictureList.append(toWindwosPath(img));
            }
        }
    }

    return true;
}

bool ConfigureHolder::updateParameterBasic(bool bConfigureParameter, bool bBrightnessVolume, bool bScrollText, bool bTitle, bool bDateTime, bool bStandby, bool isScrollText, bool isTitle, bool isDateTime)
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
    return true;
}

bool ConfigureHolder::updateBrightnessVolume(qint32 vBrightness, qint32 vVolume)
{
    this->vBrightness = vBrightness;
    this->vVolume = vVolume;
    return true;
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

