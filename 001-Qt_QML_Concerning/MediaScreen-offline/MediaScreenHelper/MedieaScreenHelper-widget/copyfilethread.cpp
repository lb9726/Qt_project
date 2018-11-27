#include "copyfilethread.h"
//#include "widget.h"

CopyfileThread::CopyfileThread()
{
//    wid = new Widget;
}

int CopyfileThread::copyStart( QStringList list)
{
    //fileList.clear();
    QString soupath = Filepath_to_Dirpath(list.at(0));//将用户选择的路径换成文件夹路径
    qDebug()<<"list = "<<list <<"\n";
    GetAllFilesSize(soupath); //遍历文件夹下所有文件大小,存入数组
    qDebug()<<"soupath = " << soupath<<"\n";
    GetFiletotalSizes(list, fileArr); //获取用户需要拷贝的文件的总大小
    bStop = false;
    emit copyStationSig(COPY_START, NULL);
    this->start();
    qDebug()<<"will send Stop in copyStart()\n";
    emit copyStationSig(COPY_STOP, NULL);
    return SUCCESS;
}
// 获取QStringList 的个数
int CopyfileThread::GetListCounts(QStringList list)
{
    QStringList::iterator it = list.begin();
    int i = 0;
    while(it != list.end())
    {
        ++i;
        ++it;
    }
    return i;
}

//调试打印输出
void CopyfileThread::printf_lists(QStringList list)
{
    int i = GetListCounts(list);
    int j;
    for(j = 0; j < i; j++)
    {
        qDebug()<<list.at(i)<<'\n';
    }
}

//将文件的路径转换成文件夹路径
QString CopyfileThread::Filepath_to_Dirpath(QString filepath)
{
    QStringList list = filepath.split("/");
    QString path;
    path.clear();
    int i = GetListCounts(list);
    for (int j = 0; j < i-1; j++)   //去掉最后一个文件名，组成文件夹的路径
    {
        path.append(list.at(j));
        path.append("/");
    }
//    path.append("/");
    return path;
}

// 获取用户选择文件夹下的所有文件的大小，调用QFileInfo的size方法实现
void CopyfileThread::GetAllFilesSize(QString Sourcepath)
{
    // loading filelist
    QString fileName;
    QDir fileDir;
    QFileInfoList fileInfoList;
    QFileInfo fileInfo;
    int i = 0;
    m_filenumber = 0;
    fileDir.setPath(Sourcepath);                // 传入源文件的文件夹路径
    qDebug()<<Sourcepath<<"GetAllFilesSize \n";
    fileDir.setFilter(QDir::Files);             // 只选择文件其他的过滤掉
    fileInfoList = fileDir.entryInfoList();     // 获取文件信息列表

    do{
        fileInfo = fileInfoList.at(i);
        fileArr[i].f_name = Sourcepath + fileInfo.fileName();
        fileArr[i].f_size = fileInfo.size()/1024;  // 保存文件夹下所有文件的大小
        qDebug()<<fileArr[i].f_name<<'\n';
        qDebug()<<fileArr[i].f_size<<'\n';
 //       fileName = srcPath + "/" + fileInfo.fileName();
        i++;
    }while(i < fileInfoList.size());
    m_filenumber = i;
    qDebug() << "m_filenumber = "<< m_filenumber << '\n';
}

// 获取用户选择的拷贝的文件大小
unsigned long CopyfileThread::GetFiletotalSizes(QStringList list, FILEINFO file_Arr[])
{
    int i = GetListCounts(list);
    int j, k;
    fileTotalSize = 0;
    for(j = 0; j < m_filenumber; j++)
    {
        for (k = 0; k < i; k++)
        {
            if (file_Arr[j].f_name == list.at(k))
            {
                fileTotalSize += file_Arr[j].f_size;
            }
        }
    }
    qDebug() << "filetotalsize = "<<fileTotalSize << '\n';
    return fileTotalSize;
}

// 文件拷贝
int CopyfileThread::fileCopy(QString fileName)
{
    QFileInfo fileInfo;
    QString desFileName;
    QByteArray byteArray;
    unsigned long fileSize;
    QString qsStaText;
    unsigned long count = 0;
    unsigned long freeSpace = 4*1024*1024;
    qDebug()<<"filecopy's fileName = "<<fileName;
//    desPath = "C:/Users/yonghao/Desktop/libo";
    desFileName = desPath + "/" + fileName.split("/").last();
    qDebug()<<desFileName<<"\n";
    fileInfo.setFile(fileName);
    fileSize = fileInfo.size()/1024;
    qDebug()<<"fileSize="<<fileSize<<"\n";
    if(fileSize > freeSpace) return ERROR_MEM_FULL;

    fileInfo.setFile(desFileName);
    if(fileInfo.isFile())                       // 存在则删除
    {
        qDebug()<<desFileName<< "the file is exist!";
        QFile::remove(desFileName);
    }

    // 创建目标文件
    QFile desFile(desFileName);
    desFile.open(QIODevice::WriteOnly);

    // 打开原文件
    QFile srcFile(fileName);
    srcFile.open(QIODevice::ReadOnly);

    while(!srcFile.atEnd())
    {
        count++;
        qDebug()<<"count = "<<count<< '\n';
        qDebug()<<"fileTotalSize = "<<fileTotalSize<< '\n';
        byteArray = srcFile.read(1024);
        desFile.write(byteArray);

        qsStaText = QString::number(100*count/fileSize);
        emit copyStationSig(COPY_SINGLE_RATE, qsStaText);

        qsStaText = QString::number(100*(curSize + count)/fileTotalSize);
        emit copyStationSig(COPY_TOTAL_RATE, qsStaText);
        qDebug()<<"bStop = "<<bStop<< '\n';
        if(bStop)
        {
            desFile.close();
            srcFile.close();
            QFile::remove(desFileName);
            return SUCCESS;
        }
    }
    desFile.close();
    srcFile.close();

    curSize += fileSize;

    return SUCCESS;
}

//线程拷贝
void CopyfileThread::run()
{
    int i;
    QString fileName;
    qDebug()<<"run() is runing!";
    qDebug()<<"fileList.count="<<fileList.count()<<'\n';
//    qDebug()<<"fileName = " << fileName<<'\n';
    for(i = 0; i < fileList.count(); ++i)
    {
        fileName = fileList.at(i);
        qDebug()<<"fileList.count="<<fileList.count()<<'\n';
        qDebug()<<"fileName = " << fileName<<'\n';
        emit copyStationSig(COPY_FILE_NAME, fileName.split("/").last());
        if(ERROR_MEM_FULL == fileCopy(fileName))
        {
            bStop = true;
            emit copyStationSig(COPY_ERROR_MEM_FULL, NULL);
            this->exit();
            return;
        }

        if(bStop) break;
    }
    qDebug()<<"will send Stop in run()\n";
    emit copyStationSig(COPY_STOP, fileName);
}

void CopyfileThread::copyStop()
{
    bStop = true;
}

void CopyfileThread::setSrcDirPath(QString path)
{
    srcPath = path;
}
void CopyfileThread::setDesDirPath(QString path)
{
    desPath = path;
}

void CopyfileThread::SetFileList(QStringList list)
{
    fileList = list;
}
