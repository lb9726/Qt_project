#include "globalfunction.h"

const QString xmlParse::XmlHead = "version=\"1.0\" encoding=\"UTF-8\"";
const QString xmlParse::XmlSuffix = ".xml";

bool FindSoftware(QString pName)
{
    if(pName.isEmpty())
        return false;
    pName.replace("/", "\\");
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
    QStringList groupsList=reg.childGroups();
    foreach(QString group,groupsList)
    {
        reg.beginGroup(group);
        QString softWareName=reg.value("DisplayName").toString();
        if(softWareName.contains(pName, Qt::CaseInsensitive))
        {
            return true;
        }
        reg.endGroup();
    }
    return false;
}

QString GetSoftwarePath(QString pName)
{
    if(pName.isEmpty())
        return QString();
    pName.replace("/", "\\");
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths",QSettings::NativeFormat);
    QStringList groupsList=reg.childGroups();
    foreach(QString group,groupsList)
    {
        if(group.contains(pName, Qt::CaseInsensitive))
        {
            reg.beginGroup(group);
            return reg.value(".").toString();  //>@
            //reg.endGroup();
        }
    }
    return QString();
}

//>@设置开机启动
bool SetSoftwareBootrun(QString pPath, bool pEnable)
{
    if(pPath.isEmpty())
        return false;
    QSettings reg("HKEY_LOCAL_MACHINE//SOFTWARE//Microsoft""//Windows//CurrentVersion//Run", QSettings::NativeFormat);
    QString tmpName = getFileName(pPath);
    if(pEnable)
        reg.setValue(tmpName, pPath);
    else
        reg.remove(tmpName);
    return true;
}

//>@打印当前主机信息
QString GetHostPCInfo()
{
    QString tmpString;
#ifdef WINDOWS
    QSysInfo sysInfo;
    QSysInfo::WinVersion tmpVer = sysInfo.windowsVersion();
    switch(tmpVer)
    {
    case QSysInfo::WV_4_0:
        tmpString = QString("OperateSystem  :Win_NT\n");
        break;
    case QSysInfo::WV_5_0:
        tmpString = QString("OperateSystem  :Win_2000");
        break;
    case QSysInfo::WV_5_1:
        tmpString = QString("OperateSystem  :Win_XP\n");
        break;
    case QSysInfo::WV_5_2:
        tmpString = QString("OperateSystem  :Win_2003\n");
        break;
    case QSysInfo::WV_6_0:
        tmpString = QString("OperateSystem  :Win_VISTA\n");
        break;
    case QSysInfo::WV_6_1:
        tmpString = QString("OperateSystem  :Win_WINDOWS7\n");
        break;
    default:break;
    }

    SYSTEM_INFO info;
    GetSystemInfo(&info);
    tmpString.append(QString("CPUs    :%1\nCPU type:%2")
                        .arg(info.dwNumberOfProcessors)
                        .arg(info.dwProcessorType));
#endif
    return tmpString;
}

void ExecProcess(QString pCommand, QString &pStdOut, QString &pStdErr, int msecs)
{
    QProcess tmpProcess;
    tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
    tmpProcess.start(pCommand);
    IDE_TRACE_STR(pCommand);
    tmpProcess.waitForFinished(msecs);
    pStdOut = tmpProcess.readAllStandardOutput();
    pStdErr = tmpProcess.readAllStandardError();
    tmpProcess.kill();
    IDE_TRACE_STR(pStdOut);
    IDE_TRACE_STR(pStdErr);
}

bool isDevExist(QString pPath)
{
    QProcess tmpProcess;
    QString tmpQuery = QString("ls %1").arg(pPath);  //>@对于ls命令，如果能找到此文件，则会在readAllStandardOutput中输出，并在文件后附带一个“\n”,如果文件不存在，则会在readAllStandardError中输出。
    tmpProcess.start(tmpQuery);
    tmpProcess.waitForFinished(1000);
    QString tmpRet = tmpProcess.readAllStandardOutput();
    if(!tmpRet.isEmpty())
        return true;
    return false;
}

bool isDevRW(QString pPath)
{
    QProcess tmpProcess;
    QString tmpQuery = QString("cat /proc/mounts | grep %1 | grep rw").arg(pPath);
    tmpProcess.start(tmpQuery);
    IDE_TRACE_STR(tmpQuery);
    tmpProcess.waitForFinished(1000);
    QString tmpRet = tmpProcess.readAllStandardOutput();
    if(tmpRet.isEmpty())  //>@如果不为空，则表示当前路径已经被挂载为目标类型。
        return false;
    return true;
}

bool ReMount(QString pPath, int pType)
{
    QString tmpType;
    if(pType == 1)
    {
        tmpType = "ro";
    }
    else if(pType == 2)
    {
        tmpType = "rw";
    }
    else
        return false;
    QProcess tmpProcess;
    QString tmpQuery = QString("cat /proc/mounts | grep %1 | grep %2").arg(pPath).arg(tmpType);
    tmpProcess.start(tmpQuery);
    IDE_TRACE_STR(tmpQuery);
    tmpProcess.waitForFinished(1000);
    QString tmpRet = tmpProcess.readAllStandardOutput();
    if(tmpRet.isEmpty())  //>@如果不为空，则表示当前路径已经被挂载为目标类型。
    {
        /*查看ro挂载的分区,如果发现有ro，就重新mount
        umount /dev/sda1
        mount /dev/sda1 /boot
        如果发现有提示“device is busy”，找到是什么进程使得他busy
        fuser -m /boot 将会显示使用这个模块的pid
        fuser -mk /boot 将会直接kill那个pid
        然后重新mount即可。*/

        QString tmpMount = QString("mount -o %1,remount %2").arg(tmpType).arg(pPath);
        tmpProcess.start(tmpMount);
        IDE_TRACE_STR(tmpMount);
        tmpProcess.waitForFinished(1000);

        //>@重新查询
        tmpProcess.start(tmpQuery);
        IDE_TRACE_STR(tmpQuery);
        tmpProcess.waitForFinished(1000);
        tmpRet = tmpProcess.readAllStandardOutput();
        if(tmpRet.isEmpty())
            return false;
    }
    return true;
}

quint64 GetDiskFreeSpace(QString driver)
{
#ifdef WINDOWS
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        IDE_DEBUG("ERROR: Call to GetDiskFreeSpaceEx() failed.");
        return 0;
    }
    return (quint64) liTotalFreeBytes.QuadPart/1024/1024;  //>@MByte
#endif
    return 0;
}

QString GetDiskLabel(QString pDiskPath)
{
    if(pDiskPath.isEmpty())
        return QString();
#ifdef WINDOWS
    pDiskPath.replace('/', '\\');  //>@windows下必须是\.
    SHFILEINFO sfi;
#ifdef UNICODE
    SHGetFileInfo((WCHAR*)pDiskPath.utf16(), 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME);
#else
    SHGetFileInfo(pDiskPath.toLocal8Bit(), 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME);
#endif
    return QString::fromUtf16((ushort*)sfi.szDisplayName);
#endif
    return QString();
}

QMultiHash<int, DRIVER_INFO> GetDiskList()
{
    QMultiHash<int, DRIVER_INFO> tmpInfoList;
#ifdef WINDOWS
    TCHAR pBuf[255];
    memset(pBuf, '/0', 255);
    GetLogicalDriveStrings(sizeof(pBuf)/sizeof(TCHAR), pBuf);

    //获取各光盘盘符
    DWORD dwDevIndex =0;
    TCHAR* lpSingleDevice = (TCHAR*)malloc(sizeof(TCHAR)*MAX_DRIVER_LEN);
    if (!lpSingleDevice)
        return tmpInfoList;

    TCHAR pbuff[BUFSIZE];
    memset(lpSingleDevice, 0x0, MAX_DRIVER_LEN);
    memset(pbuff, 0x0, BUFSIZE);

    DWORD dwBufLen = GetLogicalDriveStrings(BUFSIZE - 1, pbuff);//列举盘符
    for (DWORD i = 0; i < dwBufLen; i++)
    {
        if (pbuff[i] != 0x00)//分离各个盘符
        {
            lpSingleDevice[dwDevIndex] = pbuff[i];
            dwDevIndex++;
            continue;
        }

        lpSingleDevice[dwDevIndex]  = 0x0;
        dwDevIndex = 0;
        if(*lpSingleDevice == 'A')  //A盘 不需要
            continue;
        SHFILEINFO sfi;
        SHGetFileInfo(lpSingleDevice, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME);
        UINT tmpType = GetDriveType(lpSingleDevice);
        DRIVER_INFO tmpInfo;
#ifdef UNICODE
        tmpInfo.mPath = QString::fromUtf16((ushort*)lpSingleDevice);
        tmpInfo.mLabel = QString::fromUtf16((ushort*)sfi.szDisplayName);
#else
        tmpInfo.mPath = QString::fromLocal8Bit(lpSingleDevice);
        tmpInfo.mLabel = QString::fromLocal8Bit(sfi.szDisplayName);
#endif
        if(sfi.hIcon)
        {
            QPixmap tmpPix;
#ifndef QT5
            tmpPix = QPixmap::fromWinHICON(sfi.hIcon);
#endif
            if(!tmpPix.isNull())
                tmpInfo.mIcon = QIcon(tmpPix);
        }
        IDE_TRACE_STR(tmpInfo.mLabel);
        tmpInfoList.insert(tmpType, tmpInfo); //>@DRIVE_REMOVABLE
    }
    free(lpSingleDevice);
#endif
    return tmpInfoList;
}

bool LoginNetDisk(QString pNetAddr, QString pLoaclDisk, QString pName, QString pPasswd)
{
#if 1
    QProcess tmpProcess;
    QString tmpCmd = QString("net use %1 /del").arg(pNetAddr);
    tmpProcess.start(tmpCmd);
    tmpProcess.waitForFinished(5000);
    QString tmpRet = tmpProcess.readAllStandardError() + tmpProcess.readAllStandardOutput();
    if(tmpRet.isEmpty())
        return false;
    tmpCmd = QString("net use %1 %2 \"%3\" /user:\"%4\"").arg(pLoaclDisk).arg(pNetAddr).arg(pPasswd).arg(pName);
    tmpProcess.start(tmpCmd);
    tmpProcess.waitForFinished();
    tmpRet = tmpProcess.readAllStandardError() + tmpProcess.readAllStandardOutput();
    if(tmpRet.isEmpty())
        return false;
#else
    NETRESOURCE net_Resource;
    net_Resource.dwDisplayType = RESOURCEDISPLAYTYPE_DIRECTORY;
    net_Resource.dwScope = RESOURCE_CONNECTED;
    net_Resource.dwType = RESOURCETYPE_ANY;
    net_Resource.dwUsage = 0;
    net_Resource.lpComment = ToMfcStr("");
    net_Resource.lpLocalName = ToMfcStr(pLoaclDisk);  //映射成本地驱动器z:
    net_Resource.lpProvider= NULL;
    net_Resource.lpRemoteName = ToMfcStr(pNetAddr);   // \\servername\共享资源名
    DWORD dwFlags = CONNECT_UPDATE_PROFILE;
    DWORD dw = WNetAddConnection2(&net_Resource, ToMfcStr(pPasswd), ToMfcStr(pName), dwFlags);
    switch(dw)
    {
        case ERROR_SUCCESS:
            ShellExecute(NULL, ToMfcStr("open"), ToMfcStr("z:"), NULL, NULL, SW_SHOWNORMAL);
            break;
        case ERROR_ACCESS_DENIED:
            wprintf(ToMfcStr("没有权访问！"));
            break;
        case ERROR_ALREADY_ASSIGNED:
            ShellExecute(NULL, ToMfcStr("open"), ToMfcStr("z:"), NULL, NULL, SW_SHOWNORMAL);
            break;
        case ERROR_INVALID_ADDRESS:
            wprintf(ToMfcStr("IP地址无效"));
            break;
        case ERROR_NO_NETWORK:
            wprintf(ToMfcStr("网络不可达!"));
            break;
        default:
            break;
    }
#endif
    return true;
}

#ifdef WINDOWS
//启动cmd执行 netsh命令，并等待命令结束
void ExcuteCommand(QString pCommandParam)
{
    //初始化shellexe信息
    SHELLEXECUTEINFO   ExeInfo;
    ZeroMemory(&ExeInfo, sizeof(SHELLEXECUTEINFO));
    ExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ExeInfo.lpFile = (LPCWSTR)(QString("cmd.exe").utf16());
    ExeInfo.lpParameters = (LPCWSTR)(QString("/c %1").arg(pCommandParam).utf16());
    ExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ExeInfo.nShow = SW_HIDE;
    ExeInfo.hwnd = NULL;
    ExeInfo.lpVerb = NULL;
    ExeInfo.lpDirectory = NULL;
    ExeInfo.hInstApp = NULL;

    //执行命令
    //ShellExecuteEx(&ExeInfo);

    //等待进程结束
    //WaitForSingleObject(ExeInfo.hProcess, INFINITE);
}
#endif

bool SetDHCP(QString pFile, QString pIP)
{
    QFile file(pFile);
    if(!file.exists() || !file.open(QFile::ReadOnly))
    {
        return false;
    }
    QStringList tmpList = pIP.split('.');
    if(tmpList.count() != 4)
        return false;

    QTextStream userFileStream(&file);
    QStringList strlis;
    while(!userFileStream.atEnd())
    {
        QString lineIni=userFileStream.readLine();
        strlis<<lineIni;
    }
    file.close();
    file.remove();
    QFile file1(pFile);
    if(!file1.open(QFile::WriteOnly))
    {
        return false;
    }

    QTextStream newInputStream(&file1);
    for (int i = 0;i < strlis.size();i++)
    {
        QString tmpstrlis = strlis.at(i);
        if(tmpstrlis.startsWith(QString("start"), Qt::CaseInsensitive))
        {
            QString tmpValue = QString("start");
            tmpList.replace(3, QString("10"));
            newInputStream<<tmpValue<<" "<<tmpList.join(".")<<"\n";
        }
        else if(tmpstrlis.startsWith(QString("end"), Qt::CaseInsensitive))
        {
            QString tmpValue = QString("end");
            tmpList.replace(3, QString("250"));
            newInputStream<<tmpValue<<" "<<tmpList.join(".")<<"\n";
        }
        else
        {
            newInputStream<<strlis.at(i)<<"\n";
        }
    }
    file1.close();
    return true;
}

/*
netsh interface ip set address "连接名称" dhcp   //dhcp方式
netsh interface ip set address "连接名称" static IP地址 子网掩码 网关   //静态IP方式

netsh interface ip set dnsservers "连接名称" dhcp      //自动获取方式
netsh interface ip set dnsservers "连接名称" static IP地址  //设置主 DNS
netsh interface ip add dnsservers "连接名称"  IP地址      //设置从属 DNS
*/
bool SetNetwork(QString pNetName, bool pDHCP, QHostAddress pIP, QHostAddress pNetMask, QHostAddress pGateWay, QHostAddress pDNS)
{
    if(pNetName.isEmpty())
        return false;
    if(pDHCP)
    {
#ifdef WINDOWS
        QString tmpCMD = QString("netsh interface ip set address \"%1\" dhcp").arg(pNetName);
        IDE_TRACE_STR(tmpCMD);
        ExcuteCommand(tmpCMD);
        tmpCMD = QString("netsh interface ip set dnsservers \"%1\" dhcp").arg(pNetName);
        IDE_TRACE_STR(tmpCMD);
        ExcuteCommand(tmpCMD);
#else

#endif
        return true;
    }
    if(!pIP.isNull())
    {
#ifdef WINDOWS
        QString tmpCMD = QString("netsh interface ip set address \"%1\" static %2")
                                .arg(pNetName)
                                .arg(pIP.toString());
        if(!pNetMask.isNull())
            tmpCMD.append(QString(" %1").arg(pNetMask.toString()));
        if(!pGateWay.isNull())
            tmpCMD.append(QString(" %1").arg(pGateWay.toString()));
        IDE_TRACE_STR(tmpCMD);
        ExcuteCommand(tmpCMD);
#endif
    }
    if(!pDNS.isNull())
    {
#ifdef WINDOWS
        QString tmpCMD = QString("netsh interface ip set dnsservers \"%1\" static %2")
                                .arg(pNetName)
                                .arg(pDNS.toString());
        IDE_TRACE_STR(tmpCMD);
        ExcuteCommand(tmpCMD);
#endif
    }
    return true;
}

int CalcPixelFromPoint(int pPoint)
{
    return (DPI * pPoint / 72);
}
int CalcPointFromPixel(int pPixel)
{
    return (pPixel * 72 / DPI);
}

qreal RegularAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

//>@计算相对于270度的差值
qreal CalDiffBeginAngle(qreal angle)
{
    qreal tmpAngle = RegularAngle(angle);
    if(tmpAngle >= 0 && tmpAngle < PI/2.0)
        return PI/2.0 + tmpAngle;
    else if(tmpAngle >= PI/2.0 && tmpAngle < 3.0*PI/2.0)
        return 3.0*PI/2.0 - tmpAngle;
    else if(tmpAngle >= 3.0*PI/2.0 && tmpAngle < 2.0*PI)
        return tmpAngle - 3.0*PI/2.0;
    return 0;
}

//>@"Arial", pPrior表示宽度优先或者高度优先
int CalcFontSizeFromSize(QString pFontType, QSize pSize, PRIOR pPrior)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pPrior == PRIOR_WIDTH)
        {
            if(pixelsWide>=pSize.width())
                break;
        }
        else if(pPrior == PRIOR_HEIGHT)
        {
            if(pixelsHigh >= pSize.height())
                break;
        }
        ++fontsize;
    }
    return fontsize;
}

int CalcFontSizeFromRect(QString pFontType, QSize pSize)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pixelsWide>=pSize.width() || pixelsHigh >= pSize.height())
            break;
        ++fontsize;
    }
    return fontsize;
}

QSize CalcSizeFromFontSize(QString pFontType, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width('8'), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QChar pChar, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pChar), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QString pText, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pText), fm.height());
}

QPointF CalcIntersectPos(QRectF pRect, QLineF pLine)
{
    QPointF p1 = pRect.topLeft();
    QList<QPointF>  tmpRectPoints;
    tmpRectPoints << pRect.topRight() << pRect.bottomRight() << pRect.bottomLeft();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for(int i=0; i<tmpRectPoints.count(); ++i)
    {
        p2 = tmpRectPoints.at(i);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(pLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            return intersectPoint;
        p1 = p2;
    }
    return QPointF();
}

void WriteText(QPainter *pPainter, QString pText, QRect pRect, QFont pFont, QColor pColor)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    pPainter->save();
    pPainter->setFont(pFont);
    pPainter->setPen(pColor);
    pPainter->drawText(pRect, pText);
    pPainter->restore();
}

//>@所有文字写在一行
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, PRIOR pPrior)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    int fontsize = 1;
    int fontwidth = pRect.width()/num;
    int fontheight = pRect.height();
    fontsize = CalcFontSizeFromSize(pFontType, QSize(fontwidth, fontheight), pPrior);
    QFont font(pFontType, fontsize, QFont::DemiBold, false);
    font.setUnderline(false);
    WriteText(pPainter, pText, pRect, font, pColor);
}

/*
    QFontMetricsF fm(tmpFontInfo.mFont);
    textRect =  QRectF(fm.boundingRect(rect.toRect(), Qt::AlignLeft, tmpContent));
    p.drawText(rect, tmpContent);
*/

//>@根据字体大小自动排列文字并进行换行
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, int pFontSize)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isValid() == false)
        return;
    QSize tmpSize = CalcSizeFromFontSize(pFontType, pFontSize);
    QFont font(pFontType, pFontSize, QFont::DemiBold, false);
    font.setUnderline(false);
    pPainter->save();
    pPainter->setFont(font);
    pPainter->setPen(pColor);
    quint32 tmpRowCharCount = (qreal)(pRect.width()) / (qreal)(tmpSize.width());
    if(tmpRowCharCount <= 0) //>@防止死循环bug
        tmpRowCharCount = 1;
    quint32 tmpColumnCount = (qreal)(pText.count()) / (qreal)(tmpRowCharCount) + 1;
    for(int i=0;i<tmpColumnCount;i++)
    {
        int y = pRect.y()+i*(tmpSize.height()+2);
        if(y >= pRect.y() + pRect.height() - tmpSize.height())
            break;
        pPainter->drawText(pRect.x(),
                           y,
                           pText.mid(tmpRowCharCount*i, tmpRowCharCount));
    }
    pPainter->restore();
}

qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

bool isFileExist(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.exists();
    return flag;
}

bool delFile(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.remove();
    return flag;
}

QStringList SplitString(QString pString, QString pKey)
{
    QStringList tmpStringList;
    tmpStringList.clear();
    if(pString.isEmpty())
        return tmpStringList;
    int index1 = 0, index2 = 0;
    int count = pString.length();
    while(index1<count)
    {
        index2 = pString.indexOf(pKey, index1);
        if(index2 < 0)
        {
            tmpStringList << pString.mid(index1);
            return tmpStringList;
        }
        tmpStringList << pString.mid(index1, index2-index1);
        index1 = index2 + 1;
    }
    return tmpStringList;
}

QString  BiteorMega(int peso)
{
    QString humanread;
    double canno = peso / 1024;
    int sale = canno;
    if (peso > 0)
    {
        if (canno < 1)
        {
           sale = 1;
        }
    }

   if (sale < 1025)
   {
       humanread = QString("%1.KB").arg(sale);
       return humanread;
   }

   float megad = sale / 1024;

   if (megad < 1025)
   {
       humanread = QString("%1.MB").arg(megad);
       return humanread;
   }
   else
   {
       humanread = QString("1.GB+");
   }
   return humanread;
}

bool is_file(QString fullFileName)
{
    if (!fullFileName.size() > 0) {
      return false;
    }
    QDir diro(fullFileName);
       if (diro.exists()) {
         return false;
       }
    QFile f( fullFileName );
    if ( f.exists(fullFileName) ) {
    return true;
    } else {
    return false;
    }
}

bool StringToInt(QString pString,int &pInt)
{
    bool ok;
    int dec = pString.toInt(&ok, 10);  //>@10进制
    if(ok)
        pInt = dec;
    return ok;
}

QString getFileIconName(QString pFileName)
{
    QString tmpIconString;
    if (pFileName.isEmpty())
        return tmpIconString;
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index);
    else
        return tmpIconString;
    if(!QString::compare(fileSuffix, ".xml", Qt::CaseInsensitive))
    {
        tmpIconString = QString(":/");
    }
    return tmpIconString;
}

UI_TYPE getUiType(QString pName)
{
    if(pName.isEmpty())
        return UI_UNKNOW;
    else if(pName.compare(QString("enum"), Qt::CaseInsensitive) == 0)
        return UI_ENUM;
    else if(pName.compare(QString("stack"), Qt::CaseInsensitive) == 0)
        return UI_STACK;
    else if(pName.compare(QString("string"), Qt::CaseInsensitive) == 0)
        return UI_STRING;
    else if(pName.compare(QString("enetaddr"), Qt::CaseInsensitive) == 0)
        return UI_ENETADDR;
    else if(pName.compare(QString("num"), Qt::CaseInsensitive) == 0)
        return UI_NUM;
    else if(pName.compare(QString("check"), Qt::CaseInsensitive) == 0)
        return UI_CHECK;
    else if(pName.compare(QString("button"), Qt::CaseInsensitive) == 0)
        return UI_BUTTON;
    else if(pName.compare(QString("bool"), Qt::CaseInsensitive) == 0)
        return UI_BOOL;
    else if(pName.compare(QString("matrix"), Qt::CaseInsensitive) == 0)
        return UI_MATRIX;
    else if(pName.compare(QString("link"), Qt::CaseInsensitive) == 0)
        return UI_LINK;
    else if(pName.compare(QString("label"), Qt::CaseInsensitive) == 0)
        return UI_LABEL;
    else if(pName.compare(QString("map"), Qt::CaseInsensitive) == 0)
        return UI_MAP;
    else if(pName.compare(QString("file"), Qt::CaseInsensitive) == 0)
        return UI_FILE;
    return UI_UNKNOW;
}

QString getUiString(UI_TYPE pType)
{
    if(UI_ENUM == pType)
        return QString("enum");
    else if(UI_STACK == pType)
        return QString("stack");
    else if(UI_STRING == pType)
        return QString("string");
    else if(UI_ENETADDR == pType)
        return QString("enetaddr");
    else if(UI_NUM == pType)
        return QString("num");
    else if(UI_CHECK == pType)
        return QString("check");
    else if(UI_BUTTON == pType)
        return QString("button");
    else if(UI_BOOL == pType)
        return QString("bool");
    else if(UI_MATRIX == pType)
        return QString("matrix");
    else if(UI_LINK == pType)
        return QString("link");
    else if(UI_LABEL == pType)
        return QString("label");
    else if(UI_MAP == pType)
        return QString("map");
    else if(UI_FILE == pType)
        return QString("file");
    return QString();
}

FILE_TYPE getFileType(QString FileName)
{
    FILE_TYPE tmpFileType = TYPE_INVALID;
    if (FileName.isEmpty())
        return tmpFileType;
    int index = FileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = FileName.mid(index);
    else
        return TYPE_INVALID;
    if(!QString::compare(fileSuffix, ".bstpro", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_PRO;
    }
    else if(!QString::compare(fileSuffix, ".ui", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_UI;
    }
    else if(!QString::compare(fileSuffix, ".mdp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MDP;
    }
    else if(!QString::compare(fileSuffix, ".stc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STC;
    }
    else if(!QString::compare(fileSuffix, ".sto", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STO;
    }
    else if(!QString::compare(fileSuffix, ".rc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_RC;
    }
    else if(!QString::compare(fileSuffix, ".editor", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_EDITOR;
    }
    else if(!QString::compare(fileSuffix, ".devinfo", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_DEVINFO;
    }
    else if(!QString::compare(fileSuffix, ".map", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MAP;
    }
    else if(fileSuffix.contains(".avi", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp4", Qt::CaseInsensitive) ||
            fileSuffix.contains(".flv", Qt::CaseInsensitive) ||
            fileSuffix.contains(".rmvb", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_VIDEOEDITOR;
    }
    else if(fileSuffix.contains(".png", Qt::CaseInsensitive) ||
            fileSuffix.contains(".jpg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".svg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".gif", Qt::CaseInsensitive) ||
            fileSuffix.contains(".bmp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_IMAGEEDITOR;
    }
    else if(fileSuffix.contains(".wav", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp3", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_AUDIOEDITOR;
    }
    return tmpFileType;
}

FILE_TYPE getFileType(FILE_INFO *pFileInfo)
{
    if(pFileInfo->CategoryType.compare(QString("Solution"), Qt::CaseInsensitive)==0)
    {
        return TYPE_PRO;
    }
    else if(pFileInfo->CategoryType.compare(QString("File"), Qt::CaseInsensitive)==0)
    {
        return getFileType(pFileInfo->ItemType);
    }
    return TYPE_INVALID;
}

QString getFileSuffix(FILE_TYPE pFileType)
{
    QString tmpString;
    switch(pFileType)
    {
        case TYPE_START:
        {
            break;
        }
        case TYPE_MANUFACTURE:
        {
            tmpString.append("mft");
            break;
        }
        case TYPE_VIDEOEDITOR:
        {
            tmpString.append("veditor");
            break;
        }
        case TYPE_IMAGEEDITOR:
        {
            tmpString.append("ieditor");
            break;
        }
        case TYPE_AUDIOEDITOR:
        {
            tmpString.append("aeditor");
            break;
        }
        case TYPE_PRO:
        {
            tmpString.append("bstpro");
            break;
        }
        case TYPE_UI:
        {
            tmpString.append("ui");
            break;
        }
        case TYPE_MDP:
        {
            tmpString.append("mdp");
            break;
        }
        case TYPE_STC:
        {
            tmpString.append("stc");
            break;
        }
        case TYPE_MAP:
        {
            tmpString.append("map");
            break;
        }
        case TYPE_DEVINFO:
        {
            tmpString.append("devinfo");
            break;
        }
        case TYPE_STO:
        {
            tmpString.append("sto");
            break;
        }
        case TYPE_LOG:
        {
            tmpString.append("log");
            break;
        }
        case TYPE_RC:
        {
            tmpString.append("rc");
            break;
        }
        default:
            break;
    }
    return tmpString;
}

QString getNewFileName(FILE_TYPE pType)
{
    return QString("New.") + getFileSuffix(pType);
}

FILEPATH_TYPE getRcFileType(QString pFileName)
{
    if(pFileName.isEmpty())
    {
        return PATH_TYPENONE;
    }
    //>@1、查看是否为qrc中的文件
    if(pFileName.startsWith(":"))
    {
        return PATH_TYPEQRC;
    }
    //>@2、查看是否为网络流媒体文件
    if(pFileName.startsWith("http://") ||
       pFileName.startsWith("https://") ||
       pFileName.startsWith("rtmp://") ||
       pFileName.startsWith("rtsp://") ||
       pFileName.startsWith("mms://"))
    {
        return PATH_TYPENETADDR;
    }
    //>@3、查看是否为绝对/相对路径下的文件
    if(isFileExist(pFileName))
    {
        return PATH_TYPEABS;
    }
    return PATH_TYPENONE;
}

RC_TYPE getRcType(QString FileName)
{
    if (FileName.isEmpty())
        return RC_INVALID;
    if(FileName.endsWith(".bmp",Qt::CaseInsensitive))
    {
        return RC_BMP;
    }
    else if(FileName.endsWith(".jpg",Qt::CaseInsensitive))
    {
        return RC_JPG;
    }
    else if(FileName.endsWith(".png",Qt::CaseInsensitive))
    {
        return RC_PNG;
    }
    else if(FileName.endsWith(".gif",Qt::CaseInsensitive))
    {
        return RC_GIF;
    }
    else if(FileName.endsWith(".svg",Qt::CaseInsensitive))
    {
        return RC_SVG;
    }
    else if(FileName.endsWith(".qml",Qt::CaseInsensitive))
    {
        return RC_QML;
    }
    else if(FileName.endsWith(".avi",Qt::CaseInsensitive))
    {
        return RC_AVI;
    }
    else if(FileName.endsWith(".wav",Qt::CaseInsensitive))
    {
        return RC_WAV;
    }
    else if(FileName.endsWith(".mp3",Qt::CaseInsensitive))
    {
        return RC_MP3;
    }
    else if(FileName.endsWith(".mp4",Qt::CaseInsensitive))
    {
        return RC_MP4;
    }
    else if(FileName.endsWith(".flv",Qt::CaseInsensitive))
    {
        return RC_FLV;
    }
    else if(FileName.endsWith(".rmvb",Qt::CaseInsensitive))
    {
        return RC_RMVB;
    }
    else if(FileName.endsWith(".font",Qt::CaseInsensitive))
    {
        return RC_FONT;
    }
    else if(FileName.endsWith(".3ds",Qt::CaseInsensitive))
    {
        return RC_3DS;
    }
    else if(FileName.endsWith(".html",Qt::CaseInsensitive))
    {
        return RC_HTML;
    }
    else if(FileName.endsWith(".xml",Qt::CaseInsensitive))
    {
        return RC_XML;
    }
    return RC_INVALID;
}

//>@将pSrcFile文件规整到某个文件夹下，返回规整后的文件名
//>@如果pRcName不为空，则按照此名进行规整文件的名字
//>@  e:/abc.png  ->  d:/abc文件夹下
QString regularFile(QString pRcName, QString pSrcFile, QString pDstPath, QSize pSize)
{
    if(!pSize.isValid())
        return QString();
    FILEPATH_TYPE tmpType = getRcFileType(pSrcFile);
    if(tmpType != PATH_TYPERC && tmpType != PATH_TYPEABS)  //>@只转换相对和绝对路径下的文件，QRC以及网络文件都不可以转换
        return pSrcFile;
    if(!pDstPath.endsWith('/'))
        pDstPath.append('/');
    //>@根据文件类型规整资源
    QString tmpDstFilePath;
    QString tmpDstFileName;
    RC_TYPE tmpRcType = getRcType(pSrcFile);
    switch(tmpRcType)
    {
        case RC_BMP:
        case RC_JPG:
        case RC_PNG:
        {
            if(!pRcName.isEmpty())
            {
                //>@修改目标文件名为RcName，后缀为png
                tmpDstFileName = pRcName + QString(".png");
            }
            else
            {
                QString tmpFileName = getFileName(pSrcFile);
                int index = tmpFileName.lastIndexOf(".");
                if(index < 0)
                    return QString();
                tmpDstFileName = tmpFileName.mid(0, index) + QString(".png");
            }
            tmpDstFilePath = pDstPath + tmpDstFileName;
            if(ConvertImage(pSrcFile, tmpDstFilePath, pSize) == false)
            {
                return QString();
            }
            break;
        }
        case RC_GIF:
        {
            if(!pRcName.isEmpty())
            {
                //>@修改目标文件名为RcName，后缀为gif
                tmpDstFileName = pRcName + QString(".gif");
            }
            else
            {
                QString tmpFileName = getFileName(pSrcFile);
                int index = tmpFileName.lastIndexOf(".");
                if(index < 0)
                    return QString();
                tmpDstFileName = tmpFileName.mid(0, index) + QString(".gif");
            }
            tmpDstFilePath = pDstPath + tmpDstFileName;
            if(CopyFile(pSrcFile, tmpDstFilePath) == false)
            {
                return QString();
            }
            break;
        }
        case RC_SVG:
        {
            //>@固定图像资源以svg文件名命名
            if(!pRcName.isEmpty())
            {
                //>@修改目标文件名为RcName，后缀为svg
                tmpDstFileName = pRcName + QString(".svg");
            }
            else
            {
                QString tmpFileName = getFileName(pSrcFile);
                int index = tmpFileName.lastIndexOf(".");
                if(index < 0)
                    return QString();
                tmpDstFileName = tmpFileName.mid(0, index) + QString(".svg");
            }
            tmpDstFilePath = pDstPath + tmpDstFileName;
            if(CopyFile(pSrcFile, tmpDstFilePath) == false)
            {
                return QString();
            }
            break;
        }
        case RC_QML:
        {
            //>@转换QML文件以及所使用的图像资源，固定图像资源以QML文件名命名
            QString tmpFileName = getFileName(pSrcFile);
            tmpDstFilePath = pDstPath + tmpFileName;
            //>@获取qml对应的文件夹名
            int index1 = pSrcFile.lastIndexOf('.');
            if(index1 < 0)
                return QString();
            QString tmpSrcDir = pSrcFile.mid(0, index1);
            //>@获取qml对应的目标文件夹名
            int index2 = tmpDstFilePath.lastIndexOf('.');
            if(index2 < 0)
                return QString();
            QString tmpDstDir = tmpDstFilePath.mid(0, index2);
            //>@
            if(CopyFolder(tmpSrcDir, tmpDstDir, true) == false)
                return QString();
            //>@最后拷贝QML文件
            if(CopyFile(pSrcFile, tmpDstFilePath) == false)
            {
                return QString();
            }
            break;
        }
        case RC_FONT:
        case RC_HTML:
        case RC_XML:
        case RC_3DS:
        {
            tmpDstFilePath = pDstPath + getFileName(pSrcFile);
            if(CopyFile(pSrcFile, tmpDstFilePath) == false)
            {
                return QString();
            }
            break;
        }
        case RC_MP3:
        case RC_WAV:
        {
            if(!pRcName.isEmpty())
            {
                tmpDstFileName = pRcName + QString(".wav");
            }
            else
            {
                QString tmpFileName = getFileName(pSrcFile);
                int index = tmpFileName.lastIndexOf(".");
                if(index < 0)
                    return QString();
                tmpDstFileName = tmpFileName.mid(0, index) + QString(".wav");
            }
            tmpDstFilePath = pDstPath + tmpDstFileName;
            if(ConvertAudio(pSrcFile, tmpDstFilePath) == false)
            {
                return QString();
            }
            break;
        }
        case RC_MP4:
        case RC_AVI:
        case RC_RMVB:
        case RC_FLV:
        {
            if(!pRcName.isEmpty())
            {
                tmpDstFileName = pRcName + QString(".mp4");
            }
            else
            {
                QString tmpFileName = getFileName(pSrcFile);
                int index = tmpFileName.lastIndexOf(".");
                if(index < 0)
                    return QString();
                tmpDstFileName = tmpFileName.mid(0, index) + QString(".mp4");
            }
            tmpDstFilePath = pDstPath + tmpDstFileName;
            if(ConvertVideo(pSrcFile, tmpDstFilePath, pSize) == false)
            {
                return QString();
            }
            break;
        }
        default:
            return QString();  //>@对于QRC以及网址等文件不进行转换
    }
    return tmpDstFilePath;
}

int GetKeyType(QString pKey)
{
    if(pKey.length() == 1)
        return (int)(pKey.toLatin1().at(0));
    else if(pKey.compare("Esc") == 0)
        return (QTASCII)?(Qt::Key_Escape):(0x1b);
    else if(pKey.compare("Tab") == 0)
        return (QTASCII)?(Qt::Key_Tab):(0x0b);
    else if(pKey.compare("Backspace") == 0)
        return (QTASCII)?(Qt::Key_Backspace):(0x08);
    else if(pKey.compare("Enter") == 0)
        return (QTASCII)?(Qt::Key_Enter):(0x0d);
    else if(pKey.compare("Del") == 0)
        return (QTASCII)?(Qt::Key_Delete):(0x7f);
    else if(pKey.compare("Home") == 0)
        return (QTASCII)?(Qt::Key_Home):(0x02);
    else if(pKey.compare("End") == 0)
        return (QTASCII)?(Qt::Key_End):(0x03);
    else if(pKey.compare("Left") == 0)
        return Qt::Key_Left;
    else if(pKey.compare("Up") == 0)
        return Qt::Key_Up;
    else if(pKey.compare("Right") == 0)
        return Qt::Key_Right;
    else if(pKey.compare("Down") == 0)
        return Qt::Key_Down;
    else if(pKey.compare("Shift") == 0)
        return (QTASCII)?(Qt::Key_Shift):(0x0f);
    else if(pKey.compare("Ctrl") == 0)
        return (QTASCII)?(Qt::Key_Control):(0x11);
    else if(pKey.compare("Alt") == 0)
        return (QTASCII)?(Qt::Key_Alt):(0x10);
    else if(pKey.compare("Caps") == 0)
        return Qt::Key_CapsLock;
    else if(pKey.compare("Space") == 0)
        return Qt::Key_Space;
    else if(pKey.compare("Disablity") == 0)
        return RCDISABILITY;
    return 0;
}

MDPConfigParaInfo::MDPConfigParaInfo(const QDomElement &pElement)
{
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    //>@获取权限
    QString tmpString = pElement.attribute("Permission");
    if(!tmpString.compare("RO", Qt::CaseInsensitive))
        mPermission = RO;
    else if(!tmpString.compare("EDIT", Qt::CaseInsensitive))
        mPermission = EDIT;
    else if(!tmpString.compare("RW", Qt::CaseInsensitive))
        mPermission = RW;
    mParaType = getUiType(pElement.attribute("Para"));
    mParas = pElement.attribute("List");
    if(mParaType != UI_STACK)
    {
        mValue = pElement.text();
    }
    else
    {
        mValue = pElement.attribute("Active");
        QDomNodeList tmpList = pElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            SubConfigParaInfo *tmpParaInfo = new SubConfigParaInfo;
            tmpParaInfo->Name = tmpElement.tagName();
            QDomNodeList tmpSubList = tmpElement.childNodes();
            for(int i=0;i<tmpSubList.count();i++)
            {
                QDomElement tmpSubElement = tmpSubList.at(i).toElement();
                if(tmpSubElement.isNull())
                    continue;
                tmpParaInfo->ParaInfoList.append(new MDPConfigParaInfo(tmpSubElement));
            }
            mSubInfos.append(tmpParaInfo);
        }
    }
}

MDPConfigParaInfo::~MDPConfigParaInfo()
{
    if(mSubInfos.count() <= 0)
        return;
    for(int i=mSubInfos.count()-1;i>=0;i--)
    {
        SubConfigParaInfo* tmpParaInfo = mSubInfos.takeAt(i);
        if(tmpParaInfo)
        {
            if(tmpParaInfo->ParaInfoList.count() <= 0)
                continue;
            for(int j=tmpParaInfo->ParaInfoList.count()-1;j>=0;j--)
            {
                MDPConfigParaInfo *tmpInfo = tmpParaInfo->ParaInfoList.takeAt(j);
                if(tmpInfo)
                    delete tmpInfo;
            }
            delete tmpParaInfo;
        }
    }
}

bool MDPConfigParaInfo::IsNull()
{
    if((mName.isEmpty())||(mParaType == UI_UNKNOW))
        return true;
    return false;
}

MDPConfigInfo::MDPConfigInfo(const QDomElement &pElement)
{
    mDrag = false;
    mZValue = 0;
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    mRc = pElement.attribute("img");
    QDomNodeList nodeList = pElement.childNodes();
    for(int i=0;i<nodeList.count();i++)
    {
        QDomElement cldElement = nodeList.at(i).toElement();
        if(cldElement.isNull())
            continue;
        QString cldName = cldElement.tagName();
        if(!cldName.compare("Drag", Qt::CaseInsensitive))
        {
            mDrag =  (!cldElement.text().compare("True", Qt::CaseInsensitive))?true:false;
        }
        else if(!cldName.compare("geometry", Qt::CaseInsensitive))
        {
            QString text = cldElement.text();
            QStringList list = text.split(",", QString::SkipEmptyParts);
            if(list.count()!=4)
                continue;
            mRectF = QRectF(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble(),list.at(3).toDouble()).normalized();
        }
        else if(!cldName.compare("ZValue", Qt::CaseInsensitive))
        {
            mZValue = cldElement.text().toInt();
        }
        else if(!cldName.compare("Parameter", Qt::CaseInsensitive))
        {
            QDomNodeList cldParaNodeList = cldElement.childNodes();
            for(int i=0;i<cldParaNodeList.count();i++)
            {
                QDomElement cldParaElement = cldParaNodeList.at(i).toElement();
                if(cldParaElement.isNull())
                    continue;
                mParaInfoList.append(new MDPConfigParaInfo(cldParaElement));
            }
        }
    }
}
MDPConfigInfo::~MDPConfigInfo()
{
    mName = QString();
    mDrag = false;
    mRc = QString();
    mRectF = QRectF();
    mZValue = 0;
    if(mParaInfoList.count() > 0)
    {
        for(int i=mParaInfoList.count();i>=0;i--)
        {
            MDPConfigParaInfo *tmpParaInfo = mParaInfoList.takeAt(i);
            if(tmpParaInfo)
                delete tmpParaInfo;
        }
    }
}
bool MDPConfigInfo::IsNull()
{
    if((mName.isEmpty())||
       (mRc.isEmpty())||
       (!mRectF.isValid()))
        return true;
    return false;
}

bool ParseIP(QString pIP, QHostAddress &pIPAddr, quint16 &pPort, QString &pCode)
{
    QStringList tmpList = pIP.split(':');
    if(tmpList.isEmpty())
        return false;
    QHostAddress tmpAddr(tmpList.at(0));
    if(tmpAddr.isNull())
        return false;
    if(tmpList.count() >= 3)  //>@10.200.0.17:20001:N30100211210001
    {
        bool ok=false;
        quint16 tmpPort = tmpList.at(1).toUShort(&ok, 10);
        if(!ok)
            tmpPort = 0;
        pPort = tmpPort;
        pCode = tmpList.at(2);
    }
    else if(tmpList.count() == 2)  //>@10.200.0.17:N30100211210001
    {
        pPort = 0;
        pCode = tmpList.at(1);
    }
    pIPAddr = tmpAddr;
    return true;
}

QListWidgetItem *AddItemToListWidget(QListWidget *pList, QString pText)
{
    if(!pList || pText.isEmpty())
        return (QListWidgetItem *)0;
    for(int i=pList->count()-1;i>=0;i--)
    {
        QListWidgetItem *tmpOldItem = pList->item(i);
        if(tmpOldItem)
        {
            if(!tmpOldItem->text().compare(pText))
            {
                pList->takeItem(i);
                delete tmpOldItem;
            }
        }
    }
    QListWidgetItem *tmpItem = new QListWidgetItem(pText);
    pList->addItem(tmpItem);
    return tmpItem;
}

COM_TYPE getComponetType(QString ComponentName)
{
    COM_TYPE tmpComType = COM_INVALID;
    if (ComponentName.isNull())
        return tmpComType;
    else if (ComponentName.isEmpty() ||
             ComponentName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComponentName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_DESKTOP;

    }
    else if(ComponentName.compare(QString("SetBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_SETBAR;
    }
    else if(ComponentName.compare(QString("MessageBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_MESSAGEBAR;
    }
    else if(ComponentName.compare(QString("ProgressBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_PROGRESSBAR;
    }
    else if(!ComponentName.compare(QString("Animal"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ANIMAL;
    }
    else if(!ComponentName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOOR;
    }
    else if(!ComponentName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ARROW;
    }
    else if(!ComponentName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCTION;
    }
    else if(!ComponentName.compare(QString("FlrSign"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRSIGN;
    }
    else if(!ComponentName.compare(QString("RenCode"), Qt::CaseInsensitive))
    {
        tmpComType = COM_RENCODE;
    }
    else if(!ComponentName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpComType = COM_VIDEO;
    }
    else if(!ComponentName.compare(QString("Media"), Qt::CaseInsensitive))
    {
        tmpComType = COM_MEDIA;
    }
    else if(!ComponentName.compare(QString("Music"), Qt::CaseInsensitive))
    {
        tmpComType = COM_MUSIC;
    }
    else if(!ComponentName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpComType = COM_IMAGE;
    }
    else if(!ComponentName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEBVIEW;
    }
    else if(!ComponentName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpComType = COM_QML;
    }
    else if(!ComponentName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENGL;
    }
    else if(!ComponentName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpComType = COM_AUDIO;
    }
    else if(!ComponentName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpComType = COM_PHONE;
    }
    else if(!ComponentName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TEXT;
    }
    else if(!ComponentName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_LOGO;
    }
    else if(!ComponentName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEATHER;
    }
    else if(!ComponentName.compare(QString("Clock"), Qt::CaseInsensitive))
    {
        tmpComType = COM_CLOCK;
    }
    else if(!ComponentName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpComType = COM_NAMEPLATE;
    }
    else if(!ComponentName.compare(QString("FlrLCD"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRLCD;
    }
    else if(!ComponentName.compare(QString("FlrBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRBTN;
    }
    else if(!ComponentName.compare(QString("FlrBtnTxt"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRBTNTXT;
    }
    else if(!ComponentName.compare(QString("FlrBtnDesc"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRBTNDESC;
    }
    else if(!ComponentName.compare(QString("FlrBtnAnim"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLRBTNANIM;
    }
    else if(!ComponentName.compare(QString("KeyboardBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_KEYBOARDBTN;
    }
    else if(!ComponentName.compare(QString("CallBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_CALLBTN;
    }
    else if(!ComponentName.compare(QString("Talkback"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TALKBACKBTN;
    }
    else if(!ComponentName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_EMERGENCYBTN;
    }
    else if(!ComponentName.compare(QString("TalkbackBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TALKBACKBTN;
    }
    else if(!ComponentName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENBTN;
    }
    else if(!ComponentName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_CLOSEBTN;
    }
    else if(!ComponentName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_SETBTN;
    }
    else if(!ComponentName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_DISABILITYBTN;
    }
    else if(!ComponentName.compare(QString("Status"), Qt::CaseInsensitive))
    {
        tmpComType = COM_STATUS;
    }
    return tmpComType;
}

QString getComponetName(COM_TYPE ComponentType)
{
    switch(ComponentType)
    {
    case COM_DESKTOP:
        return QString("Desktop");
    case COM_SETBAR:
        return QString("SetBar");
    case COM_MESSAGEBAR:
        return QString("MessageBar");
    case COM_PROGRESSBAR:
        return QString("ProgressBar");
    case COM_ANIMAL:
        return QString("Animal");
    case COM_FLOOR:
        return QString("Floor");
    case COM_ARROW:
        return QString("Arrow");
    case COM_FUNCTION:
        return QString("Function");
    case COM_RENCODE:
        return QString("RenCode");
    case COM_FLRSIGN:
        return QString("FlrSign");
    case COM_MEDIA:
        return QString("Media");
    case COM_VIDEO:
        return QString("Video");
    case COM_MUSIC:
        return QString("Music");
    case COM_IMAGE:
        return QString("Image");
    case COM_WEBVIEW:
        return QString("WebView");
    case COM_QML:
        return QString("QML");
    case COM_OPENGL:
        return QString("OpenGL");
    case COM_AUDIO:
        return QString("Audio");
    case COM_PHONE:
        return QString("Phone");
    case COM_TEXT:
        return QString("Text");
    case COM_LOGO:
        return QString("Logo");
    case COM_WEATHER:
        return QString("Weather");
    case COM_CLOCK:
        return QString("Clock");
    case COM_NAMEPLATE:
        return QString("NamePlate");
    case COM_FLRLCD:
        return QString("FlrLCD");
    case COM_FLRBTN:
        return QString("FlrBtn");
    case COM_FLRBTNTXT:
        return QString("FlrBtnTxt");
    case COM_FLRBTNDESC:
        return QString("FlrBtnDesc");
    case COM_FLRBTNANIM:
        return QString("FlrBtnAnim");
    case COM_KEYBOARDBTN:
        return QString("KeyboardBtn");
    case COM_CALLBTN:
        return QString("CallBtn");
    case COM_PHONEBTN:
        return QString("PhoneBtn");
    case COM_EMERGENCYBTN:
        return QString("EmergencyBtn");
    case COM_TALKBACKBTN:
        return QString("TalkbackBtn");
    case COM_OPENBTN:
        return QString("OpenBtn");
    case COM_CLOSEBTN:
        return QString("CloseBtn");
    case COM_SETBTN:
        return QString("SetBtn");
    case COM_DISABILITYBTN:
        return QString("DisabilityBtn");
    case COM_STATUS:
        return QString("Status");
    default:
        return QString();
    }
    return QString();
}

QString getComponetPath(COM_TYPE pType)
{
    if(pType == COM_DESKTOP)
        return QString("/");
    QString tmpName = getComponetName(pType);
    if(!tmpName.isEmpty())
        return tmpName.prepend('/');
    return tmpName;
}

COM_TYPE getParentComType(COM_TYPE pType)
{
    if(pType > COM_AREA && pType < COM_COMPONENT)
    {
        return COM_DESKTOP;
    }
    return COM_INVALID;
}

COM_TYPE getParentComType(QString ComPath)
{
    if(ComPath.isEmpty())
        return COM_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return COM_INVALID;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) //>@desktop and rect
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return COM_INVALID;
        }
        return COM_DESKTOP;
    }
    else
    {
        tmpString = ComPath.mid(index1+1, index2-index1-1);
    }
    return getComponetType(tmpString);
}

QString getComPath(QString ComName)
{
    QString tmpString;
    if (ComName.isNull())
        return tmpString;
    else if (ComName.isEmpty() ||
             ComName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        return QString("/");
    }
    else if(!ComName.compare(QString("SetBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/SetBar");
    }
    else if(!ComName.compare(QString("MessageBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MessageBar");
    }
    else if(!ComName.compare(QString("ProgressBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ProgressBar");
    }
    else if(!ComName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Floor");
    }
    else if(!ComName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Arrow");
    }
    else if(!ComName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Function");
    }
    else if(!ComName.compare(QString("Media"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Media");
    }
    else if(!ComName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Video");
    }
    else if(!ComName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Image");
    }
    else if(!ComName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpString = QString("/WebView");
    }
    else if(!ComName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpString = QString("/QML");
    }
    else if(!ComName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpString = QString("/OpenGL");
    }
    else if(!ComName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Audio");
    }
    else if(!ComName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Phone");
    }
    else if(!ComName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Text");
    }
    else if(!ComName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Logo");
    }
    else if(!ComName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Weather");
    }
    else if(!ComName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Time");
    }
    else if(!ComName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpString = QString("/NamePlate");
    }
    else if(!ComName.compare(QString("FlrBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FlrBtn");
    }
    else if(!ComName.compare(QString("PhoneBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/PhoneBtn");
    }
    else if(!ComName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/EmergencyBtn");
    }
    else if(!ComName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/OpenBtn");
    }
    else if(!ComName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/CloseBtn");
    }
    else if(!ComName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/SetBtn");
    }
    else if(!ComName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/DisabilityBtn");
    }
    return tmpString;
}

QString getParentComPath(QString ComPath)
{
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
            return QString("");     //>@Empty表示当前控件为Desktop
        else
            return QString("/");
    }
    else
    {
        tmpString = ComPath.mid(index1, index2-index1);
    }
    return tmpString;
}

QString getCurComName(QString ComPath)
{
    if(ComPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(ComPath.compare(QString("Desktop"), Qt::CaseInsensitive) == 0)
            return QString("Desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("Desktop");     //>@Empty表示Desktop
        else
            return tmpString;
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return tmpString;
}

COM_TYPE getCurComType(QString ComPath)
{
    QString tmpString;
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return getComponetType(ComPath);
    }
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return COM_DESKTOP;     //>@Empty表示Desktop
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return getComponetType(tmpString);
}

QString getFileName(QString pFilePath)
{
#if 1
    QFileInfo tmpFileInfo(pFilePath);
    return tmpFileInfo.fileName();
#else
    if (pFilePath.isEmpty())
        return QString();
    QString tmpFileName;
    int index = pFilePath.lastIndexOf("/");
    if(index >= 0)
        tmpFileName =  pFilePath.mid(index + 1);
    else
        tmpFileName = pFilePath;
    return tmpFileName;
#endif
}

QString getFileDirectory(QString pFilePath)
{
#if 0
    QFileInfo tmpFileInfo(pFilePath);
    if(tmpFileInfo.isFile())
    {
        return tmpFileInfo.dir().path();
    }
    return QString();
#else
    if (pFilePath.isEmpty())
        return QString();
    pFilePath.replace("\\", "/");
    if(pFilePath.endsWith('/'))
        return QString();
    int index = pFilePath.lastIndexOf('/');
    if(index < 0)
        return QString();
    return  pFilePath.mid(0, index + 1);
#endif
}

//>@ e:/xx/xx/
QString getParentDirectory(QString pDirPath)
{
#if 0
    QFileInfo tmpFileInfo(pDirPath);
    if(tmpFileInfo.isDir())
    {
        QDir tmpDir = tmpFileInfo.dir();
        if(tmpDir.cdUp())
            return tmpDir.dirName();
    }
    return QString();
#else
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(0, index2 + 1);
#endif
}

QString GetDirectoryName(QString pDirPath)
{
#if 0
    QFileInfo tmpFileInfo(pDirPath);
    if(tmpFileInfo.isDir())
    {
        return tmpFileInfo.dir().dirName();
    }
    return QString();
#else
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(index2 + 1, index1 - index2 - 1);
#endif
}

QRect GetRectFromString(QString pRectString)
{
    if(pRectString.isEmpty())
        return QRect();
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW<=0 || tmpH<=0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

QRectF GetRotateRect(QSizeF pParentSize, QRectF pRect, ROTATE_ANGLE pAngle)
{
    if(pParentSize.isValid() == false || pRect.isValid() == false)
        return QRectF();
    qreal ParWidth = pParentSize.width();
    qreal ParHeight = pParentSize.height();
    QRectF tmpRect;
    int value;
    switch(pAngle)
    {
        case ROTATE_90: //>@均为顺时针旋转
        {
            //>@获取左下角顶点相对于父窗口左下角顶点的距离
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = pRect.left();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        case ROTATE_180:
        {
            //>@获取右下角顶点为起点
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.width());
            tmpRect.setHeight(pRect.height());
            break;
        }
        case ROTATE_270:
        {
            //>@获取右上角顶点为起点
            value = pRect.top();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        default:
            return QRectF();
    }
    return tmpRect;
}

CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle)
{
    CONSTRAINT tmpConstraint;
    switch(pAngle)
    {
        case ROTATE_90: //>@均为顺时针旋转
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_180:
        {
            tmpConstraint.mDirection = (DIRECTION)(pConstraint.mDirection);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_270:
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)(pConstraint.mOrientation);
            break;
        }
        default:
            return pConstraint;
    }
    return tmpConstraint;
}

qint64 GetPathSize(QString pDir)
{
    qint64 tmpSize = 0;
    QFileInfo tmpFileInfo(pDir);
    if(tmpFileInfo.isDir())
    {
        if(!pDir.endsWith("/"))
            pDir.append("/");
        QDir tmpDir(pDir);
        QStringList folders = tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpPath = QString("%1%2").arg(pDir).arg(folders[i]);
            tmpSize += GetPathSize(tmpPath);
        }
        QFileInfoList files = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < files.size(); ++i)
        {
            tmpSize += files.at(i).size();
        }
    }
    else
    {
        tmpSize = tmpFileInfo.size();
    }
    return tmpSize;
}

//>@返回文件的绝对路径
QStringList findFiles(const QDir &dir,
                      const QString &fileName,
                      const QString &txt,
                      Qt::CaseSensitivity sensitive)
{
    QString tmpDirPath = dir.path();
    if(!tmpDirPath.endsWith('/'))
        tmpDirPath.append('/');
    QStringList foundFiles;
    QStringList files;
    if(fileName.isEmpty())
        files = dir.entryList(QDir::Files | QDir::NoSymLinks);
    else
        files = dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    if(txt.isEmpty())
    {
        for(int i=0;i<files.count();i++)
        {
            foundFiles.append(tmpDirPath + files.at(i));
        }
    }
    else
    {
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(dir.absoluteFilePath(files[i]));
            if (file.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream in(&file);
                while (!in.atEnd())
                {
                    line = in.readLine();
                    if (line.contains(txt, sensitive))
                    {
                        foundFiles.append(tmpDirPath + files[i]);
                        break;
                    }
                }
            }
        }
    }
    return foundFiles;
}

QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder)
{
    QFileInfo tmpFileInfo(DirName);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!DirName.endsWith('/'))
        DirName.append('/');
    QDir dir = QDir(DirName);
    QStringList findFileList;
    if(subfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << findFilesInFloder(DirName + folders[i], fileName, true);
            }
        }
    }
    findFileList << findFiles(dir, fileName, QString(""), Qt::CaseSensitive);
    return findFileList;
}

//>@查找某种类型的文件
QStringList SearchFiles(QString pDirPath, QString pSuffix, bool pSubfolder)
{
    QFileInfo tmpFileInfo(pDirPath);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    QDir dir = QDir(pDirPath);
    QStringList findFileList;
    if(pSubfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << SearchFiles(pDirPath + folders[i], pSuffix, pSubfolder);
            }
        }
    }

    QStringList tmpFileList = dir.entryList(QStringList(QString("*.%1").arg(pSuffix)), QDir::Files | QDir::NoSymLinks);
    for(int i=0;i<tmpFileList.count();i++)
    {
        findFileList.append(pDirPath + tmpFileList.at(i));
    }
    return findFileList;
}

QStringList ErgodicDirectory(QString d)
{
    QFileInfo tmpFileInfo(d);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!d.endsWith('/'))
        d.append('/');

    QStringList tmpFileList;
    QDir dir(d);
    if (dir.exists())
    {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;
        for (int l = 0; l < list.size(); l++)
        {
            fi = list.at(l);
            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                tmpFileList << ErgodicDirectory(fi.absoluteFilePath());
            }
            else if(fi.isFile())
            {
                tmpFileList.append(fi.absoluteFilePath());
            }
        }
    }
    return tmpFileList;
}

bool CutFile(QString srcFile, QString dstFile)
{
    if(!CopyFile(srcFile, dstFile))
        return false;
    return DelFile(srcFile);
}

//>@注意：srcFile与dstFile必须都是文件， 如果遇到同名文件则覆盖
bool CopyFile(QString srcFile, QString dstFile, QString pPrefix)
{
    //>@如果路径相同，则不进行拷贝
    if(srcFile.compare(dstFile, Qt::CaseInsensitive) == 0)
        return true;
    QFileInfo tmpFileInfo(srcFile);
    if(!tmpFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString = getFileDirectory(dstFile);
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE();
        return false;
    }
    if(!pPrefix.isEmpty())
    {
        QString tmpName = getFileName(dstFile);
        if(!tmpName.startsWith(pPrefix))
        {
            if(!tmpDstString.endsWith('/'))
                tmpDstString.append('/');
            dstFile = tmpDstString + pPrefix + tmpName;
        }
    }
    if(!DelFile(dstFile))
    {
        IDE_TRACE();
        return false;
    }
    bool flag = QFile::copy(srcFile, dstFile);
#if (defined(UBUNTU) || defined(LINUX))
    if(flag)
    {
        system("sync");
    }
#endif
    return flag;
}

bool DelFile(QString pPath)
{
    QFileInfo tmpFileInfo(pPath);
    if(!tmpFileInfo.exists())
        return true;
    if(!tmpFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QFile file(pPath);
    if(!file.setPermissions(QFile::WriteOther))
    {
        IDE_TRACE();
    }
    if(!file.remove())
    {
        IDE_TRACE();
        return false;
    }
#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif
    return true;
}

bool CreatFile(QString pPath)
{
    QFile file(pPath);
    if(file.exists())
        return true;
    CreatPath(getFileDirectory(pPath));
    if(!file.open(QFile::WriteOnly))
    {
        IDE_TRACE();
        return false;
    }
    file.close();
#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif
    return true;
}

bool CreatPath(QString pPath)
{
    QDir dir(pPath);
    if(!dir.exists())
    {
        if(!dir.mkpath(pPath))
        {
            dir.cdUp();
            IDE_TRACE();
            return false;
        }
        dir.cdUp();
    }
    return true;
}

//>@包括子文件夹下的文件。
QStringList getAllInFloders(bool pSubDir, QString pDir, bool pSize)
{
    if(pDir.isEmpty())
        return QStringList();
    pDir.replace("\\", "/");
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    QDir tmpDir = QDir(pDir);
    if(!tmpDir.exists())
        return QStringList();

    QStringList getAllList;
    //>@先处理文件夹
    if(pSubDir)
    {
        QStringList tmpFolders = tmpDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < tmpFolders.count(); ++i)
        {
            QString tmpSubPath = pDir + tmpFolders.at(i);
            getAllList.append(getAllInFloders(pSubDir, tmpSubPath, pSize));
        }
    }
    //>@再处理文件
    QStringList tmpFiles = tmpDir.entryList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpFiles.count();i++)
    {
        QString tmpFile = pDir + tmpFiles.at(i);
        if(pSize)
        {
            QFileInfo tmpFileInfo(tmpFile);
            tmpFile.append(":");
            tmpFile.append(QString::number(tmpFileInfo.size()));
        }
        getAllList.append(tmpFile);
    }
    return getAllList;
}

//>@仅获取子文件夹。
QStringList getAllSubfolderInFloders(QString pDir)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

QStringList getAllFileInFloders(QString pDir)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

//>@从某一个文件夹中获取所有文件路径，包括子目录下的
QStringList getAllFilesAndSize(QString pDir)
{
    QDir dir = QDir(pDir);
    if(!dir.exists())
        return QStringList();
    if(!pDir.endsWith('/'))
        pDir.append('/');
    IDE_TRACE_STR(pDir);
    QStringList getAllList;
    QFileInfoList tmpList = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i=0;i<tmpList.count();i++)
    {
        QFileInfo tmpFileInfo = tmpList.at(i);
        if(tmpFileInfo.isDir())
        {
            QString tmpPath = tmpFileInfo.filePath();
            getAllList.append(getAllFilesAndSize(tmpPath));
        }
        else if(tmpFileInfo.isFile())
        {
            getAllList.append(tmpFileInfo.filePath() + QString(":%1").arg(tmpFileInfo.size()));
        }
    }
    return getAllList;
}

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, bool pCopyFile, bool pIdel)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    if(!CreatPath(pDstDir))
    {
        IDE_TRACE_STR(pDstDir);
        return false;
    }
    QDir tmpSrcDir(pSrcDir);
    if(pSubfolder)
    {
        QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
            QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
            CopyFolder(tmpSrcPath, tmpDstPath, pSubfolder, pCopyFile, pIdel);
        }
    }
    if(pCopyFile)
    {
        QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
        for(int i = 0; i < files.size(); ++i)
        {
            QString tmpDstFilePath = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
            if(pIdel)
                QtSleep(10);
            CopyFile(files[i], tmpDstFilePath);
        }
    }
    return true;
}

//>@清空文件夹内文件
bool ClearFolder(QString pDir)
{
    if(!DelFolder(pDir))
        return false;
    return CreatPath(pDir);
}

bool DelFolder(QString pDir, bool pIdel)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir(pDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {
            QString tmpdir = pDir + fileInfo.fileName();
            if(!DelFolder(tmpdir))
            {
                return false;
            }
        }
        else if(fileInfo.isFile())
        {
            if(pIdel)
                QtSleep(10);
            if(!DelFile(fileInfo.filePath()))
            {
                return false;
            }
        }
    }
    if(!dir.cdUp())            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
    {
        IDE_TRACE();
        return false;
    }
    if(dir.exists(pDir))
    {
        if(!dir.rmdir(pDir))
        {
            IDE_TRACE();
            return false;
        }
    }
    return true;
}

bool MoveFolder(QString pSrcDir, QString pDstDir)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    QDir tmpDir(pSrcDir);
    if(!tmpDir.exists())
        return false;
    if(!CreatPath(pDstDir))
        return false;
    //>@先移动子文件夹
    QDir tmpSrcDir(pSrcDir);
    QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
        QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
        MoveFolder(tmpSrcPath, tmpDstPath);
    }
    //>@再移动文件
    QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        QString tmpString = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
        CutFile(files[i], tmpString);
    }
    return true;
}

FileOperate::FileOperate(QObject *parent) :
    QThread(parent)
{
    m_OperateType = -1;
}

void FileOperate::Wait(int pTimeout)
{
    quint32 i=0;
    while(((i+=100) < pTimeout) && isRunning())
    {
        QtSleep(10);
        wait(90);
    }
}

void FileOperate::Start(int pType)
{
    if(isRunning())
    {
        quit();
        wait(1000);
    }
    m_OperateType = pType;
    start(LowestPriority);
}

bool FileOperate::CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, bool pCopyFile, bool pIdel, quint32 pTimeout)
{
    m_Src = pSrcDir;
    m_Dst = pDstDir;
    m_SubFolder = pSubfolder;
    m_CopyFile = pCopyFile;
    m_Idel = pIdel;
    Start(1);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::MoveFolder(QString pSrc, QString pDst, quint32 pTimeout)
{
    m_Src = pSrc;
    m_Dst = pDst;
    Start(2);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::DelFolder(QString pDir, bool pIdel, quint32 pTimeout)
{
    m_Src = pDir;
    m_Idel = pIdel;
    Start(3);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::ClearFolder(QString pDir, quint32 pTimeout)
{
    m_Src = pDir;
    Start(4);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::CutFile(QString srcFile, QString dstFile, quint32 pTimeout)
{
    m_Src = srcFile;
    m_Dst = dstFile;
    Start(5);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::CopyFile(QString srcFile, QString dstFile, QString pPrefix, quint32 pTimeout)
{
    m_Src = srcFile;
    m_Dst = dstFile;
    m_Prefix = pPrefix;
    Start(6);
    if(pTimeout)
        Wait(pTimeout);
}

bool FileOperate::DelFile(QString pPath, quint32 pTimeout)
{
    m_Src = pPath;
    Start(7);
    if(pTimeout)
        Wait(pTimeout);
}

void FileOperate::run()
{
    switch(m_OperateType)
    {
        case 1:  //>@CopyFolder
        {
            ::CopyFolder(m_Src, m_Dst, m_SubFolder, m_CopyFile, m_Idel);
            break;
        }
        case 2:   //>@MoveFolder
        {
            ::MoveFolder(m_Src, m_Dst);
            break;
        }
        case 3:   //>@DelFolder
        {
            ::DelFolder(m_Src, m_Idel);
            break;
        }
        case 4:   //>@ClearFolder
        {
            ::ClearFolder(m_Src);
            break;
        }
        case 5:   //>@CutFile
        {
            ::CutFile(m_Src, m_Dst);
            break;
        }
        case 6:   //>@CopyFile
        {
            ::CopyFile(m_Src, m_Dst, m_Prefix);
            break;
        }
        case 7:   //>@DelFile
        {
            ::DelFile(m_Src);
            break;
        }
        default:
        {
            break;
        }
    }
}


//>@pValidRect:为0表示全部拷贝  pResolution:目标图片分辨率  先裁剪原图，在转化为目标分辨率。
//>@ConvertImage(QString("d:/a.jpg"), QRect(0,0,640,480), QString("d:/b.bmp"), QSize(320,240));
bool ConvertImage(QString pSrcFileName,
                  QRect pValidRect,
                  QString pDstFileName,
                  QSize pResolution)
{
    if(pSrcFileName.isEmpty() || pSrcFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty() || pDstFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    QImage tmpImage;
    tmpImage.load(pSrcFileName);
    QRect tmpRect = tmpImage.rect();
    if(pValidRect.left() != tmpRect.left() ||
       pValidRect.right() != tmpRect.right() ||
       pValidRect.top() != tmpRect.top() ||
       pValidRect.bottom() != tmpRect.bottom())
    {
        QImage newImage = tmpImage.copy(pValidRect);
        QImage dstImage = newImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    else
    {
        QImage dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    return flag;
}

QString getFileSuffix(QString pFileName)
{
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index+1);
    else
        return fileSuffix;
    if(fileSuffix.length() < 2)
        return QString();
    return fileSuffix;
}

//>@根据参数判断图片缩放大小
bool ConvertImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    if(tmpFile.remove() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(tmpNewImage.save(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QImage GetScaledImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        return QImage();
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        return QImage();
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    return tmpNewImage;
}

QSize GetScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QImage tmpImage= GetScaledImage(pFileName, pSize, pPrior);
    if(tmpImage.isNull())
    {
        return QSize();
    }
    return tmpImage.size();
}

//>@如果pResolution无效，则不缩放
bool ConvertImage(QString pSrcFileName,
                  QString pDstFileName,
                  QSize pResolution)
{
    QFile tmpSrcFile(pSrcFileName);
    if(tmpSrcFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage;
    if(!tmpImage.load(pSrcFileName))
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString suffix = getFileSuffix(pDstFileName);
    if(suffix.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString;
    QFile tmpDstFile(pDstFileName);
    if(tmpDstFile.exists())
    {
        QFile::Permissions tmpPermissions = tmpDstFile.permissions(pDstFileName);
        if(!(tmpPermissions & QFile::WriteUser))
            tmpDstFile.setPermissions(tmpPermissions | QFile::WriteUser);
        tmpDstFile.remove();
    }
    else
    {
        tmpDstString = getFileDirectory(pDstFileName);
        QDir tmpDir;
        tmpDir.mkpath(tmpDstString);
    }

    QImage dstImage;
    //>@检查目标图片分辨率是否合理，如果大小无效或者大于1920x1920，则保持原大小不变。
    if(pResolution.isEmpty() || pResolution.isValid()==false ||
       pResolution.width() > 1920 ||
       pResolution.height() > 1920)
    {
        dstImage = tmpImage;
    }
    else
    {
        dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if(!dstImage.save(pDstFileName, suffix.toLatin1().data()))
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    if((pLenth==0)||(pString.isEmpty()))
        return tmpString;
    int tmpDex = pString.indexOf(pKey,pCaps);
    if(tmpDex==-1)
        return tmpString;
    if((tmpDex+pKey.length()+pStart,pLenth) > pString.length())
        return tmpString;

    tmpString = pString.mid(tmpDex+pKey.length()+pStart,pLenth);
    return tmpString;
}

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    int tmpPreDex = pString.indexOf(pPre,pCaps);
    if(tmpPreDex==-1)
        return tmpString;
    int tmpAfterDex = pString.indexOf(pAfter,pCaps);
    if(tmpAfterDex==-1)
        return tmpString;

    tmpString = pString.mid(tmpPreDex+pPre.length()+pPreindent,
                            tmpAfterDex-tmpPreDex-pPre.length()-pAfterindent);
    return tmpString;
}

/************************************************************************
//>@
************************************************************************/
MULTIMEDIA_CONVERT::MULTIMEDIA_CONVERT(QObject *parent) :
    QObject(parent)
{
    m_Process = 0;
    mMutex = new QMutex(QMutex::Recursive);
}

MULTIMEDIA_CONVERT::~MULTIMEDIA_CONVERT()
{
    KillProcess();
}

bool MULTIMEDIA_CONVERT::GetFileInfo(QString pSrc, int pTimeout)
{
    QFile file(pSrc);
    if(!file.exists())
    {
        IDE_TRACE();
        return false;
    }
    m_FileInfo.mPath = pSrc;
    m_FileInfo.mName = getFileName(m_FileInfo.mPath);
    m_FileInfo.mSize = file.size();
    QString cmd = QString("%1 -i %2").arg((QString)D_FFMPEG_PATH).arg(pSrc);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertAudio(QString pSrc, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
    {
        IDE_TRACE();
        return false;
    }
//    ./ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -y test.wav
    QString cmd = QString("%1 -y -i %2 -ac 1 -ar 44100 -y %3").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QSize pResolution, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pResolution.isValid())
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -s 640x480 -r 29.97 -y test.wav
    QString resolution = SizeToStr(pResolution);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -r %4 -y %5").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(resolution).
                         arg(pFrame).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QRect pRect, QSize pSize, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pSize.isValid())
        return false;
    if(!GetFileInfo(pSrc, pTimeout/10))
        return false;
    QtSleep(100);
    QSize srcResolution = m_FileInfo.mResolution;
    quint16 left =0,top=0,right=0,bottom=0;
    left = pRect.left();
    top = pRect.top();
    right = srcResolution.width() - pRect.width() - left;
    bottom = srcResolution.height() - pRect.height() - top;
    if(((left + right)>=srcResolution.width())||((top + bottom)>=srcResolution.height()))
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -croptop 100 -cropbottom 100 -cropleft 100 -cropright 100 -s 640x480 -r 29.97 -y test.wav
    QString tempDst = QString("%1.%2").arg(pSrc).arg(getFileSuffix(pSrc));
    QString cutCmd = QString("%1 -y -i %2 -croptop %3 -cropbottom %4 -cropleft %5 -cropright %6 -r %7 -y %8").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(top).arg(bottom).arg(left).arg(right).
                         arg(pFrame).
                         arg(tempDst);
    if(!StartProcess(cutCmd, pTimeout*3/10))
       return false;
    if(!PreHandel(tempDst, pDst))
        return false;
    QString resolution = SizeToStr(pSize);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -y %4").
                         arg((QString)D_FFMPEG_PATH).
                         arg(tempDst).
                         arg(resolution).
                         arg(pDst);
    if(!StartProcess(cmd, pTimeout*3/5))
       return false;
    QFile tempFile(tempDst);
    if(tempFile.exists())
    {
        tempFile.setPermissions(QFile::WriteOther);
        if(!tempFile.remove())
        {
            IDE_TRACE();
            return false;
        }
    }
    return true;
}
//>@----------------------------------------------------------------
void MULTIMEDIA_CONVERT::slot_CatchStandardOutput()
{
    quint8 tmpCnt = 0;
    QString tmpString,tmpInfo,tmpKeyInfo,tmpKey0,tmpKey1;
    QStringList tmpStrList;
    tmpStrList.clear();
    if(m_Process != 0)
    {
        if(m_Process->state())
            tmpInfo = m_Process->readAllStandardOutput();
    }
    //qDebug()<<"---"<<tmpInfo;
//>@Type
    tmpKey0 = QString("Input #");
    tmpKey1 = QString(", from '");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mType = GetSrcType(tmpKeyInfo);
    }
//>@Lenth
    tmpKey0 = QString("Duration:");
    tmpKeyInfo = GetStrBehindKey(tmpInfo,tmpKey0,1,8,Qt::CaseSensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mLenth = GetLenth(tmpKeyInfo);
    }
//>@Resolution
    tmpKey0 = QString("Stream #0.0");
    tmpKey1 = QString("[PAR");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mResolution = GetResolution(tmpKeyInfo);
    }
//>@Progress
    tmpKey0 = QString("time=");
    tmpKey1 = QString("bitrate=");
    tmpStrList = tmpInfo.split(tmpKey0, QString::KeepEmptyParts, Qt::CaseInsensitive);
    tmpCnt = tmpStrList.count();
    for(int i=1;i<tmpCnt;i++)
    {
        tmpString = QString(" time=%1").arg(tmpStrList.at(i));
        tmpKeyInfo = GetStrBetweenPreAndAfter(tmpString,tmpKey0,tmpKey1,0,1,Qt::CaseSensitive);
        if(!tmpKeyInfo.isEmpty())
        {
            emit sProgress(GetProgress(tmpKeyInfo,m_FileInfo.mLenth));
        }
    }
}

void MULTIMEDIA_CONVERT::slot_Finished(int pID, QProcess::ExitStatus pState)
{
    switch(pState)
    {
        case 0:
            mTaskState = Success;
            break;
        case 1:
            mTaskState = Failed;
            break;
    }
}

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect, QPixmap pBgPixmap)
{
    QRect tmpRect;
    if(pBgPixmap.isNull() == false)
        tmpRect = pBgPixmap.rect();
    else
        tmpRect = pRect.toRect();
    QImage tmpImage(tmpRect.width(), tmpRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@创建透明图层
    QPainter painter(&tmpImage);
    if(pBgPixmap.isNull() == false)
        painter.drawPixmap(0,0,pBgPixmap);
    MULTIMEDIA_CONVERT tmpMediaCtrl;
    if(tmpMediaCtrl.GetFileInfo(pMediaName, 500))
    {
        qreal tmpDiffHeight = pRect.height() / 6.0;
        qreal tmpDiffWidth = tmpDiffHeight * 2.0 /3.0;

        QString tmpString;
        if(tmpMediaCtrl.m_FileInfo.mType == RC_INVALID)
        {
            tmpString= QString("[MediaType] : INVALID");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::red, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP3)
        {
            tmpString= QString("[MediaType] : MP3");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_WAV)
        {
            tmpString= QString("[MediaType] : WAV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP4)
        {
            tmpString= QString("[MediaType] : MP4");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_AVI)
        {
            tmpString= QString("[MediaType] : AVI");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_RMVB)
        {
            tmpString= QString("[MediaType] : RMVB");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_FLV)
        {
            tmpString= QString("[MediaType] : FLV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        tmpString = QString("[Path] : %1").arg(tmpMediaCtrl.m_FileInfo.mPath);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Name] : %1").arg(tmpMediaCtrl.m_FileInfo.mName);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*2, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalSize] : %1").arg(tmpMediaCtrl.m_FileInfo.mSize);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*3, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Resolution] : %1x%2").arg(tmpMediaCtrl.m_FileInfo.mResolution.width()).arg(tmpMediaCtrl.m_FileInfo.mResolution.height());
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*4, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalLength] : %1").arg(tmpMediaCtrl.m_FileInfo.mLenth);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*5, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
    }
    painter.end();
    return QPixmap::fromImage(tmpImage);
}

bool ConvertAudio(QString pSrcFileName, QString pDstFileName)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".wav", Qt::CaseInsensitive) == false)
        pDstFileName.append(".wav");
    if(tmpString.compare("wav", Qt::CaseInsensitive) == 0)
    {
        return CopyFile(pSrcFileName, pDstFileName);
    }
    else if(tmpString.compare("mp3", Qt::CaseInsensitive) == 0)
    {
        MULTIMEDIA_CONVERT tmpAudioConvert;
        return tmpAudioConvert.ConvertAudio(pSrcFileName, pDstFileName, 100000);
    }
    else
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool ConvertVideo(QString pSrcFileName, QString pDstFileName, QSize pResolution)
{
    return ConvertVideo(pSrcFileName, QRect(), pDstFileName, pResolution);
}

bool ConvertVideo(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".mp4", Qt::CaseInsensitive) == false)
        pDstFileName.append(".mp4");
    MULTIMEDIA_CONVERT tmpVideoConvert;
    return tmpVideoConvert.ConvertVideo(pSrcFileName, pResolution, 10, pDstFileName, 1000000);
}

/****************************************************************************************************
            格式化输入输出函数
***************************************************************************************************/
/************************************************************
_scanf目前可支持：
仅支持字符，字符串，整形数的格式化输入；
一次性输入MAX_SCANF_COUNT个数据；
fmt中没有分隔符时默认使用空格和制表符；
分隔符可使用任意单字节数据；
以换行作为输入的结束符；
exp：
_scanf((pInputFun)myFun,"%d,%s,%c",&a,&b,&c) 输入123,jinwenjie,A\n
************************************************************/

#ifdef WINDOWS

size_t _Scanf(pInputCharFun InputFun, const char* fmt,...)
{
    //解析fmt
    if(fmt == NULL)
        return 0;
    size_t count=0;
    char tmpBuf[MAX_SCANF_COUNT][2]={{0}};  //最大支持一次读入10个数据类型
    size_t __tmp = strlen(fmt);
    for(size_t tmp=0;tmp<__tmp;tmp++)
    {
        if(fmt[tmp] == '%')
        {
            //防止溢出fmt范围
            if(tmp+1 >= __tmp)
                return 0;
            if((fmt[tmp+1]=='c')||(fmt[tmp+1]=='s'||(fmt[tmp+1]=='d')))  //目前仅支持%d,%s,%c
            {
                tmpBuf[count][0]	= fmt[++tmp];
                //printf("%c\t",tmpBuf[count][0]);
                //防止溢出fmt范围
                if(tmp+1 >= __tmp+1)
                    return 0;
                if((fmt[tmp+1]=='%')) //分隔符支持除%以外的所有字符，但只允许是单字符
                {
                    tmpBuf[count][1]	= 0;
                }
                else if(fmt[tmp+1]=='\0')  //fmt结束
                {
                    tmpBuf[count][1]	= 1;
                }
                else
                {
                    tmpBuf[count][1]	= fmt[tmp+1];
                }
                count++;
                if(count == MAX_SCANF_COUNT)
                    break;
            }
            else
                return 0;//格式不合法
        }
    }
    //此处count值为fmt中有多个少数据类型
    //printf(" total : %d\r\n",count);

    char key;
    char * p_arg_c = 0;
    char * p_arg_s = 0;
    int * p_arg_d = 0;
    va_list argbuf;
    va_start(argbuf, fmt);
    size_t i = 0;  //用于指示实际存入的数据数
    bool InReading = false;	//指示读取一个数据类型是否结束，true表还在读取
    size_t StrCount=0; //读取的字符串长度
    while(1)
    {
        if( i == count) //如果已经读取了指定的数据类型个数，则直接退出
        {
            goto exit;
        }
        key = InputFun();
        //如果没有分隔符默认以空格和\t进行分割，否则就以指定字符进行分割。注意如果有连续多个分隔符存在则默认看作一个分隔符
        if((((tmpBuf[i][1]==0)||(tmpBuf[i][1]==1))?((key != ' ')&&(key != '\t')):(key != tmpBuf[i][1]))&&(key != '\r')&&(key != '\n'))   //注意如果以其他字符作为输入终止符，则需要更改key != '\r' key != '\n'
        {
            switch(tmpBuf[i][0])
            {
            case 'c':  //只读入一个字符
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //只取字母，否则认为不合法。如果有特殊需要要修改此处
                {
                    p_arg_c= va_arg(argbuf, char*);
                    *p_arg_c = key;
                    InReading = true;
                }
                else
                {
                    goto exit;
                }
                break;
            case 's':
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //只取字母，否则认为不合法。如果有特殊需要要修改此处
                {
                    if(InReading == false)
                    {
                        p_arg_s = va_arg(argbuf, char*);
                        *p_arg_s = key;
                        StrCount++;
                        InReading = true;
                    }
                    else
                    {
                        *(p_arg_s+StrCount++) = key;   //此处没有加判断是否溢出的判断
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            case 'd':
                if(key <= '9' && key >= '0')
                {
                    if(InReading == false)
                    {
                        p_arg_d = va_arg(argbuf, int*);
                        *p_arg_d = key - '0';
                        InReading = true;
                    }
                    else
                    {
                        *p_arg_d *= 10;
                        *p_arg_d += key - '0';
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            }
        }
        else
        {
            if(InReading == true)  //确认成功读取了一个数据结构时才自加1.
                i++;
            InReading = false;  //当遇到分隔符时，表示数据结构取值结束
            if(StrCount>0)  //如果是字符串结束，则在最后补上\0
            {
                *(p_arg_s+StrCount++) = '\0';
                StrCount = 0;
            }
            if(key == '\n')	//以换行符作为输入结束
            {
                goto exit;
            }
        }
    }
exit:
    va_end(argbuf);
    return i;
}


size_t _Printf(pOutputStrFun OutputStrFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    len = OutputStrFun(tmpBuf, strlen(tmpBuf));
    delete []tmpBuf;
    return len;
}

size_t _Printf(pOutputCharFun OutputCharFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    for( len=0;len<strlen(tmpBuf);len++ )
    {
        if(OutputCharFun(tmpBuf[len]) == false)
            break;
    }
    return len;
}
#endif

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
void SetFuzzy(QGraphicsItem *pItem, qreal pBlurRadius)
{
    if(!pItem) return;
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
    effect->setBlurRadius(pBlurRadius);
    pItem->setGraphicsEffect(effect);
}
void SetColor(QGraphicsItem *pItem, QColor pColor)
{
    if(!pItem) return;
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(pColor);
    pItem->setGraphicsEffect(effect);
}
void SetShadow(QGraphicsItem *pItem, QColor pColor, qreal pBlurRadius, QPointF pPointF)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setColor(pColor);
    effect->setBlurRadius(pBlurRadius);
    effect->setOffset(pPointF);
    pItem->setGraphicsEffect(effect);
}
void SetOpacity(QGraphicsItem *pItem, qreal pOpacity)
{
    if(!pItem) return;
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacity(pOpacity);
    pItem->setGraphicsEffect(effect);
}
void ClearFuzzy(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsBlurEffect* effect = VAR_CAST<QGraphicsBlurEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearColor(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsColorizeEffect* effect = VAR_CAST<QGraphicsColorizeEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearShadow(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect* effect = VAR_CAST<QGraphicsDropShadowEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearOpacity(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsOpacityEffect* effect = VAR_CAST<QGraphicsOpacityEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}

void Effect_Fuzzy(QGraphicsItem *Item,qreal n)
{
    if(!Item) return;
    QGraphicsBlurEffect *uEffect = new QGraphicsBlurEffect();
    uEffect->setBlurRadius(n);
    Item->setGraphicsEffect(uEffect);
}

void Effect_color(QGraphicsItem *Item,int r, int g, int b)
{
    if(!Item) return;
    if(r==-1 || g==-1 || b==-1)
    {
        Item->setGraphicsEffect(0);
    }
    else
    {
        QGraphicsColorizeEffect *uEffect = new QGraphicsColorizeEffect();
        uEffect->setColor(QColor(r,g,b));
        Item->setGraphicsEffect(uEffect);
    }
}

void Effect_Shadow(QGraphicsItem *Item,qreal dx, qreal dy)
{
    if(!Item) return;
    QGraphicsDropShadowEffect *uEffect = new QGraphicsDropShadowEffect();
    uEffect->setOffset(dx,dy);
    Item->setGraphicsEffect(uEffect);
}

void Effect_Transparent(QGraphicsItem *Item,qreal opacity)
{
    if(!Item) return;
    QGraphicsOpacityEffect *uEffect = new QGraphicsOpacityEffect();
    uEffect->setOpacity(opacity);
    Item->setGraphicsEffect(uEffect);
}
#endif

GraphicsSceneBase::GraphicsSceneBase(QObject *parent, QRect pRect) :
    QGraphicsScene(parent)
{
    m_Rect = pRect;
    setSceneRect(pRect);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent)
{
    QRectF rect = this->geometry();
    m_Scene = new GraphicsSceneBase(0, QRect(0,0,rect.width(),180));
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent, QRect pRect):
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(pRect);

    m_Scene = new GraphicsSceneBase(0, pRect);
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::RubberBandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

xmlParse::xmlParse() :
    QObject(), QDomDocument()
{
    m_ForceSave = true;
    m_Valid = false;
    m_IsChanged = true;
}

xmlParse::xmlParse(QString fileName) :
    QObject(), QDomDocument()
{
    m_ForceSave = true;
    m_Valid = false;
    m_IsChanged = true;
#if 1
    QFileInfo tmpFileInfo(fileName);
    m_FileName = tmpFileInfo.fileName();
    m_FileDir = tmpFileInfo.dir().path();
    if(m_FileDir.contains('\\'))
    {
        if(!m_FileDir.endsWith('\\'))
            m_FileDir.append('\\');
    }
    else
    {
        if(!m_FileDir.endsWith('/'))
            m_FileDir.append('/');
    }
#else
    int index = fileName.lastIndexOf('/');
    if(index < 0)
    {
        m_FileName = fileName;
        m_FileDir = "";
    }
    else
    {
        m_FileName = fileName.mid(index+1);
        m_FileDir = fileName.mid(0,index+1);
        if(m_FileDir.endsWith('/') == false)
            m_FileDir.append('/');
    }
#endif
}

xmlParse::~xmlParse()
{
    if(m_IsChanged && m_ForceSave)
    {
        IDE_TRACE();
        SaveAs(m_FileDir + m_FileName);
    }
    //clear();
}

bool xmlParse::GetItemElement(QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

bool xmlParse::GetItemElement(QDomElement itemDomElement, QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = itemDomElement;
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

QString xmlParse::GetItemElementValue()
{
    QDomNode tmpNode = itemElement.firstChild();
    if(tmpNode.isNull() == false)
       return itemElement.text();
    return QString("");
}

QString xmlParse::GetItemElementValue(QString itemPath)
{
    if(GetItemElement(itemPath))
    {
        return GetItemElementValue();
    }
    return QString("");
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QDomElement itemDomElement)
{
    QList<QDomAttr> tmpAttrs;
    QDomNamedNodeMap tmpNodesMap = itemDomElement.attributes();
    for(int i=0;i<tmpNodesMap.count();i++)
        tmpAttrs.append(tmpNodesMap.item(i).toAttr());
    return tmpAttrs;
}

QList<QDomAttr> xmlParse::GetItemElementAttrs()
{
    return GetItemElementAttrs(itemElement);
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QString itemPath)
{
    QList<QDomAttr> tmpAttrs;
    if(GetItemElement(itemPath))
        return GetItemElementAttrs();
    return tmpAttrs;
}

bool xmlParse::SearchItemElement(QString itemName)
{
    QDomElement docElem = documentElement();
    QDomNodeList nodes=docElem.elementsByTagName(itemName);
    if(nodes.size()>0)
    {
        QDomElement tmpElement = nodes.at(0).toElement();
        if(tmpElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        itemElement = tmpElement;
        return true;
    }
    return false;
}

bool xmlParse::CreateItemElement(QString itemPath)
{
    //>@提取待创建的元素路径，并检验路径是否存在，如果不存在则创建
    bool nodeFlag = true;
    QString tmpString;
    QDomElement tmpElement, tmpElement_bk, newElement;
    QDomNode newNode;

    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
        indexbk = index;
        tmpElement_bk = tmpElement;
    }
    if(!nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
    }
    itemElement = tmpElement;
    m_IsChanged = true;
    return true;
}

bool xmlParse::CreateItemElement(QDomElement pParElement, QString pName, QString pValue)
{
    if(pParElement.isNull())
        return false;
    QDomElement newElement = createElement(pName);
    QDomText newTitleText = createTextNode(pValue);
    newElement.appendChild(newTitleText);
    pParElement.appendChild(newElement);
    return true;
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QString& pContent)
{
    if(itemDomElement.isNull())
        return false;
    QTextStream out(&pContent);
    itemDomElement.save(out, 0);
    return true;
}

bool xmlParse::SaveItemElement(QString& pContent)
{
    return SaveItemElement(itemElement, pContent);
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice)
{
    if(itemDomElement.isNull() || !pDevice)
        return false;
    if(!pDevice->isOpen())
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(pDevice);
    itemDomElement.save(out, 0);
    return true;
}

bool xmlParse::SaveItemElement(QIODevice *pDevice)
{
    return SaveItemElement(itemElement, pDevice);
}

bool xmlParse::ModifyItemElement(QString itemPath, QString value)
{
    bool flag = GetItemElement(itemPath);
    if(flag == false)
    {
        flag = CreateItemElement(itemPath);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
    }
    return ModifyItemElement(itemElement, value);
}

bool xmlParse::ModifyItemElement(QDomElement itemDomElement, QString value)
{
    if(itemDomElement.isElement() == false)
    {
        IDE_TRACE();
        return false;
    }
    //>@如果值为空，则表示清空当前text
    //>@如果已经有文字节点了，则替换，否则添加即可
    QDomNodeList tmpList = itemDomElement.childNodes();
    if(!tmpList.isEmpty())
    {
        for(int i=tmpList.count()-1;i>=0;i--)
        {
            QDomNode tmpNode = tmpList.at(i);
            if(tmpNode.isNull())
                continue;
            if(tmpNode.nodeType() == QDomNode::TextNode)
            {
                m_IsChanged = true;
                itemDomElement.removeChild(tmpNode);
                break;
            }
        }
    }
    if(!value.isEmpty())
    {
        QDomText newTitleText = createTextNode(value);
        QDomNode tmpNewNode = itemDomElement.appendChild(newTitleText);
        m_IsChanged = true;
        if(tmpNewNode.isNull())
        {
            IDE_TRACE();
            //return false;
        }
    }
    return true;
}

bool xmlParse::SetText(QDomElement itemDomElement, QString value)
{
    return ModifyItemElement(itemDomElement, value);
}

bool xmlParse::DeleteItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
        return true;
    QDomNode tmpParentNode = itemDomElement.parentNode();
    if(tmpParentNode.isNull())
    {
        IDE_TRACE();
        return false;
    }
    QDomNode newNode = tmpParentNode.removeChild(itemDomElement);
    if(newNode.isNull())
    {
        IDE_TRACE();
        return false;
    }
    newNode.clear();
    m_IsChanged = true;
    return true;
}

bool xmlParse::ClearItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
        return false;
    QDomNodeList tmpList = itemDomElement.childNodes();
    if(tmpList.count())
    {
        for(int i=tmpList.count()-1;i>=0;i--)
        {
            QDomNode newNode = itemDomElement.removeChild(tmpList.at(i));
            if(newNode.isNull())
                continue;
            newNode.clear();
        }
    }
    m_IsChanged = true;
    return true;
}

bool xmlParse::write(QIODevice *device)
{
    const int IndentSize = 4;
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(device);
    save(out, IndentSize);
    return true;
}

bool xmlParse::read(QIODevice *device)
{
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = setContent(device, false, &errorStr, &errorLine, &errorColumn);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool xmlParse::Open()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;

    if(!m_FileDir.endsWith("/"))
        m_FileDir.append("/");
    QFile file(m_FileDir + m_FileName);
    bool flag = file.exists();
    if(flag == true)
    {
        flag = file.open(QFile::ReadOnly | QFile::Text);
        if (flag == false)
        {
            IDE_TRACE();
            return false;
        }
        flag = read(&file);
        if(flag == false)
        {
            file.close();
            IDE_TRACE();
            return false;
        }
    }
    else
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool xmlParse::New()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;

    QDir dir(m_FileDir);
    dir.mkpath(m_FileDir);

    QFile file(m_FileDir + m_FileName);       //<@此处只要新建一个空文件即可。
    bool flag = file.exists();
    if(flag == true)
    {
        IDE_TRACE();
        return false;
    }
    flag = file.open(QFile::WriteOnly | QFile::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool xmlParse::Save()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;
    if(m_IsChanged)
    {
        //将当前文件备份
        QString tmpString = m_FileDir +m_FileName + QString("_bk");
        bool flag = SaveAs(tmpString);
        IDE_TRACE_STR(tmpString);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
        if(!CutFile(tmpString, m_FileDir+m_FileName))
        {
            IDE_TRACE();
            return false;
        }
        m_IsChanged = false;
#if (defined(UBUNTU) || defined(LINUX))
        system("sync");
#endif
        QtSleep(10);
    }
    return true;
}

bool xmlParse::Rename(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
        return false;
#if 1
    QFile file(m_FileDir + m_FileName);
    file.setPermissions(QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    bool flag = file.exists();
    if(flag == false)  //>@如果文件不存在，则重命名看作成功。
    {
        return true;
    }
    flag = file.rename(m_FileDir + fileName);
    return flag;
#else
    QFile file(m_FileDir + m_FileName);
    file.copy(m_FileDir + fileName);
    file.remove();
#endif
}

bool xmlParse::SaveAs(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
        return false;
    if(isNull())
    {
        IDE_TRACE();
        return false;
    }
    QFile f(fileName);
    bool flag = f.open(QIODevice::WriteOnly | QIODevice::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    flag = write(&f);
    if(flag == false)
    {
        IDE_TRACE();
        f.close();
        return false;
    }
    f.close();
    m_IsChanged = false;
    return true;
}

iniParse::iniParse(QString pfileName) :
    QSettings(pfileName, QSettings::IniFormat)
{
}

bool iniParse::getProperty(QString pkey, QString &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    pValue = tmpVariant.toString();
    return true;
}

bool  iniParse::getProperty(QString pkey, int &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    bool ok = false;
    int tmpValue = tmpVariant.toInt(&ok);
    if(ok == false)
        return false;
    pValue = tmpValue;
    return true;
}


ThreadTick::ThreadTick(quint32 pTickFreq, QObject *parent) :
    QThread(parent)
{
    m_TickFreq = pTickFreq;
    m_Enable = false;
}

ThreadTick::~ThreadTick()
{
    m_TickFreq = 0;
    m_Enable = false;
}

void ThreadTick::Wait(quint32 pTimeout)
{
    quint32 i=0;
    while(m_Enable && m_TickFreq && ((i+=100) < pTimeout))
    {
        msleep(100);
    }
}

void ThreadTick::run()
{
    while(m_Enable && m_TickFreq)
    {
        emit sTick();
        Wait(m_TickFreq);
    }
}

bool ThreadTick::Start()
{
    m_Enable = true;
    if(!isRunning())
        start();
    return true;
}

bool ThreadTick::Stop()
{
    m_Enable = false;
    return true;
}

void ThreadTick::slot_Start()
{
    Start();
}

void ThreadTick::slot_Stop()
{
    Stop();
}



