#include "logfun.h"

#ifdef SURRPORT_LOG
Log4Qt::Logger* g_Logger = 0;
#endif

QHash<QString, LOGTIP_INFO*> g_LogTipGroup;

const QString logTipManager::LogTipFileName = "TIPS.xml";
const QString logTipManager::LogTipFileType = "BST TIPS File";

logTipManager::logTipManager(QString fileName) :
    xmlParse(fileName)
{
    m_Valid = false;
    QFile file(fileName);
    bool flag = file.exists();
    if(flag == false)
    {
        IDE_TRACE();
        return ;
    }
    flag = Open();
    if(flag == false)
    {
       IDE_TRACE();
       return ;
    }
    //>@Check filetype
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        IDE_TRACE();
        return;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(LogTipFileType, Qt::CaseInsensitive) != 0)
    {
        IDE_TRACE_STR(tmpString);
        return;
    }
    m_Valid = true;

    getNewLogTipCategory();
}

bool logTipManager::getNewLogTipCategory()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Tip");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }
    g_LogTipGroup.clear();
    QDomElement tmpElement;
    QDomNode tmpNode;
    QDomNodeList newCategoryNode = GetChildNodes();
    for(int i=0;i < newCategoryNode.count();++i)
    {
        tmpNode = newCategoryNode.at(i);
        tmpElement = tmpNode.toElement();
        QString tmpTipName = tmpElement.tagName();
        MSG_LEVEL tmpLevel = GetMsgLevel(tmpElement.attribute("Para"));
        if(tmpLevel == LEVEL_INVALID)
            continue;
        LOGTIP_INFO *tmpLogTip = new LOGTIP_INFO;
        tmpLogTip->level = tmpLevel;
        tmpLogTip->content = tmpElement.text();
        g_LogTipGroup.insert(tmpTipName, tmpLogTip);
        //IDE_TRACE_STR(tmpTipName);
    }
    return true;
}


logManager::logManager(QString fileName) :
    xmlParse(fileName)
{
    if(m_FileName.endsWith(".log") == false)
        m_FileName.append(".log");
}

bool logManager::NewLog()
{
    bool flag = New();
    if(flag == false)
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    m_Valid = true;

    appendChild(createProcessingInstruction("xml", XmlHead));

    QDomElement rootElement;
    rootElement = createElement("root");
    appendChild(rootElement);

    QDomElement tmpElement = createElement("FileType");
        tmpElement.appendChild(createTextNode("Analysis Log File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Log");
        tmpElement.appendChild(createElement("History"));
        tmpElement.appendChild(createElement("Error"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool logManager::OpenLog()
{
    bool flag = Open();
    if(flag == false)
    {
       m_Valid = false;
       return false;
    }
    m_Valid = true;
    //>@Check FileType
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare("Analysis Log File", Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool logManager::CloseLog()
{
    SaveLog();
    return true;
}

bool logManager::SaveLog()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}


//>@查找日志配置文件，如果查找不到或者配置文件不合法
bool InitLoggerInfo()
{
    //>@读取所支持的日志tips
    QString tmpLogTipFile = D_FSRCPATH + D_BOOTDIR + logTipManager::LogTipFileName;
    logTipManager *tmpLogManager = new logTipManager(tmpLogTipFile);
    if(tmpLogManager->m_Valid == false)
    {
        IDE_TRACE_STR(tmpLogTipFile);
    }
    delete tmpLogManager;
#if 1  //>@根据自身路径设置日志存放路径
    //>@配置文件中的日志存放地址。
    QString tmpLogConfFile = D_FSRCPATH + D_BOOTDIR + D_LOGBKCONF;
    QFile tmpLogFile(tmpLogConfFile);
    if(tmpLogFile.exists() == false || tmpLogFile.open(QIODevice::ReadOnly) == false)
    {
        IDE_TRACE_STR(tmpLogConfFile);
        return false;
    }
    QString line, tmpLogFilePath, tmpLogFileName;
    int tmpMaxLogFileNum = 50;
    QByteArray tmpLogFileBuffer = tmpLogFile.readAll();
    line = QString("log4j.appender.E.MaxBackupIndex=");
    int tmpMaxLogFileNumIndex = tmpLogFileBuffer.indexOf(line);
    if(tmpMaxLogFileNumIndex)
    {
        tmpLogFile.seek(tmpMaxLogFileNumIndex);
        line = tmpLogFile.readLine();
        if(line.contains(QString("log4j.appender.E.MaxBackupIndex")))
        {
            int index = line.indexOf("=");
            QString tmpMaxString = line.mid(index+1);
            bool ok = false;
            int tmpNum = tmpMaxString.toInt(&ok, 10);
            if(ok)
               tmpMaxLogFileNum = tmpNum;
        }
    }
    if(tmpLogFileBuffer.contains(QByteArray("log4j.appender.E.File=")))
    {
        tmpLogFilePath = D_FSRCPATH + D_LOGDIR;

        QString tmpName = qApp->applicationName();
        if(tmpName.isEmpty())
            return false;
        int tmpIndex = tmpName.indexOf(".");
        if(tmpIndex)
            tmpLogFileName = tmpName.mid(0, tmpIndex);
        else
            tmpLogFileName = tmpName;
        tmpLogFileBuffer.replace(QByteArray("log4j.appender.E.File="), QString("log4j.appender.E.File=%1").arg(tmpLogFilePath+tmpLogFileName).toLatin1());
    }
    tmpLogFile.close();
    tmpLogConfFile = D_FSRCPATH + D_BOOTDIR + D_LOGCONF;
    QFile tmpNewLogFile(tmpLogConfFile);
    if(tmpNewLogFile.exists())
        tmpNewLogFile.remove();
    if(tmpNewLogFile.open(QIODevice::WriteOnly) == false)
    {
        IDE_TRACE_STR(tmpLogConfFile);
        return false;
    }
    tmpNewLogFile.write(tmpLogFileBuffer);
    tmpNewLogFile.close();
#else  //>@获取日志配置文件中的存放路径
    //>@配置文件中的日志存放地址。
    QString tmpLogConfFile = D_FSRCPATH + D_BOOTDIR + logManager::LogConfFileName;
    QFile tmpLogFile(tmpLogConfFile);
    if(tmpLogFile.exists() == false || tmpLogFile.open(QIODevice::ReadWrite) == false)
    {
        IDE_TRACE_STR(tmpLogConfFile);
        return false;
    }
    QString line, tmpLogFilePath, tmpLogFileName;
    int tmpMaxLogFileNum = 50;
    QTextStream in(&tmpLogFile);
    in.seek(0);
    while (in.atEnd() == false)
    {
        line = in.readLine();
        if(line.contains(QString("log4j.appender.E.MaxBackupIndex")))
        {
            int index = line.indexOf("=");
            QString tmpMaxString = line.mid(index+1);
            bool ok = false;
            int tmpNum = tmpMaxString.toInt(&ok, 10);
            if(ok)
               tmpMaxLogFileNum = tmpNum;
        }
        else if(line.contains(QString("log4j.appender.E.File")))
        {
            int index = line.indexOf("=");
            QString tmpLogPath = line.mid(index+1);
            //>@获取文件路径和文件名
            int index1 = tmpLogPath.lastIndexOf("/");
            int index2 = tmpLogPath.lastIndexOf("."); //>@默认为任何文件均带有后缀名，则均有.。
            if(index1 < 0)
            {
                IDE_TRACE_STR(tmpLogPath);
                return false;
            }
            if(index2 < 0)
            {
                IDE_TRACE_STR(tmpLogPath);
                return false;
            }
            if(index1 >= index2)
            {
                IDE_TRACE_STR(tmpLogPath);
                return false;
            }
            tmpLogFilePath = tmpLogPath.mid(0, index1 + 1);
            tmpLogFileName = tmpLogPath.mid(index1 + 1);
            break;
        }
    }
    tmpLogFile.close();
#endif

    //>@使用QtCreate启动应用程序时会将BST_IDE目录作为程序运行目录，而不是BST_IDE.exe所在文件夹作为运行目录，
    //>@但程序单独执行时是以BST_IDE.exe所在文件夹作为运行目录的。
    //>@针对不同运行方式，下一句要进行修改。
    //>@初始化日志文件目录
    IDE_TRACE_INT(tmpMaxLogFileNum);
    IDE_TRACE_STR(tmpLogFilePath);
    IDE_TRACE_STR(tmpLogFileName);
    InitLogPath(tmpLogFilePath, tmpLogFileName, tmpMaxLogFileNum);
#ifdef SURRPORT_LOG
    g_Logger = Log4Qt::Logger::rootLogger();
    g_Logger->removeAllAppenders();
    Log4Qt::PropertyConfigurator::configure(tmpLogConfFile);
#endif
    return true;
}

bool InitLogPath(QString pLogPath, QString pLogName, int pMaxLogFileNum)
{
    if(pLogName.isEmpty())
        return false;
    if(pLogPath.endsWith("/") == false)
        pLogPath.append("/");
    QDir tmpDir(pLogPath);
    if(tmpDir.exists() == false)
        tmpDir.mkpath(pLogPath);
    QString pFilePath = QString("%1%2").arg(pLogPath).arg(pLogName);
    QString tmppath;
    int i;
    for(i=1;i<=pMaxLogFileNum;i++)
    {
        tmppath = QString("%1.%2").arg(pFilePath).arg(i);
        QFile file(tmppath);
        if(file.exists() == true)
            continue;
        else
            break;
    }
    QString srcFile;
    QString dstFile;
    if(i == pMaxLogFileNum+1)  //>@日志文件已满
    {
        for(int j=1;j<=pMaxLogFileNum;j++)
        {
            srcFile = QString("%1.%2").arg(pFilePath).arg(j);
            dstFile = QString("%1.%2").arg(pFilePath).arg(j-1);
            CopyFile(srcFile,dstFile);
        }
    }
    srcFile = pFilePath;
    dstFile = QString("%1.%2").arg(pFilePath).arg(i);
    CopyFile(srcFile,dstFile);
    return true;
}

void Logger(MSG_LEVEL pLevel, QString pMsg)
{
#ifdef SURRPORT_LOG
    if(g_Logger == 0)
        return;
    if(pLevel == LEVEL_WARN)
        g_Logger->warn(pMsg);
    else if(pLevel == LEVEL_ERROR)
        g_Logger->error(pMsg);
    else
        g_Logger->info(pMsg);
#endif
}

void Logger(QString Key, QStringList tmplist)
{
    QString tmpcontent;
    MSG_LEVEL tmpLevel;
    LOGTIP_INFO *tmpLogTip = g_LogTipGroup.value(Key);
    if(tmpLogTip == 0)
    {
        tmpLevel = LEVEL_INFO;
        tmpcontent = Key;
    }
    else
    {
        tmpLevel = tmpLogTip->level;
        tmpcontent = tmpLogTip->content;
    }
    int count = tmplist.count();
    QString tmpString;
    if(count == 1)
        tmpString = QString(tmpcontent).arg(tmplist.at(0));
    else if(count == 2)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1));
    else if(count == 3)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2));
    else if(count == 4)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2)).arg(tmplist.at(3));
    else if(count == 5)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2)).arg(tmplist.at(3)).arg(tmplist.at(4));
    else
        tmpString = tmpcontent;
    Logger(tmpLevel, tmpString);
}

