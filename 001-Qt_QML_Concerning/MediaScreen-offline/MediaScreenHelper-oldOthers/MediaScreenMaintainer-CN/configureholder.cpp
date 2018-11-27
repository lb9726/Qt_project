#include "configureholder.h"
#include "define.h"
#include <QCoreApplication>
#include <QDate>
#include <QCryptographicHash>

ConfigureHolder::ConfigureHolder(QObject *parent) : QObject(parent)
{
    apkPath = "";
}


bool ConfigureHolder::createXml(QString dir)
{
    QDomDocument cfgXml;
    QDomElement rootElement = cfgXml.createElement("root");
    cfgXml.appendChild(rootElement);
    QDate currentDate = QDate::currentDate();
    rootElement.setAttribute("releasedate",currentDate.toString("yyyy/MM/dd"));

    QString res="www.shbst.com";
    QByteArray string=res.toLatin1();
    QCryptographicHash *hash=new QCryptographicHash(QCryptographicHash::Sha1);
    hash->addData(string);
    QByteArray sha1=hash->result();

    QDomElement sha1Element = cfgXml.createElement("security");
    sha1Element.appendChild(cfgXml.createTextNode(QString(sha1)));
    rootElement.appendChild(sha1Element);

    QString strPath = dir + "\\firmware\\" + "security.xml";
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

void ConfigureHolder::orderFile(QString dir)
{
    QString usbdir  = dir+"\\firmware\\";
    usbPath = usbdir;
    IDE_TRACE_STR(apkPath);
    QFileInfo apkInfo(apkPath);
    if(!apkInfo.exists()){
        IDE_TRACE();
        return;
    }
    QString dstName = apkInfo.fileName();
    IDE_TRACE_STR(usbdir+dstName);
    copyReourceFile(apkPath, usbdir+dstName);
}

void ConfigureHolder::setApkPath(QString apkPath)
{
    this->apkPath = apkPath;
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
