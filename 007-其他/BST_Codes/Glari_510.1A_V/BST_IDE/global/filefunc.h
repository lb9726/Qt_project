#ifndef FILEFUNC_H
#define FILEFUNC_H

#include "define.h"

#define        D_FILEPACKAGE_SIZE   100000

#define        D_SCANINFO_PRG              1
#define        D_SCANINFO_MSG              2
#define        D_SCANINFO_MSGPRG           3

#define        D_FILEINFO_PRG              11
#define        D_FILEINFO_MSG              12
#define        D_FILEINFO_MSGPRG           13

#define        D_DIRINFO_PRG               21
#define        D_DIRINFO_MSG               22
#define        D_DIRINFO_MSGPRG            23

#define        D_SUBFOLDER_LAYER           5

QString  BiteorMega(int peso);  //>@从bit计算MB

QStringList SplitString(QString pString, QString pKey);

bool Match(QString pData, QString pPattern);

QString GetSelfSysVersion();
QString GetSelfAppVersion(QString pFile);

typedef enum{
    PATH_TYPENONE = 0,
    PATH_TYPERC,
    PATH_TYPENETADDR,
    PATH_TYPEABS,
    PATH_TYPEQRC
} FILEPATH_TYPE;

FILEPATH_TYPE GetFilePathType(QString pFile);

//----------------------------------------------------------------------------------

QString GetFileName(QString pFilePath);
QString GetFilePath(QString pFilePath);
QString GetFileSuffix(QString pFileName);

QString GetParentPath(QString pDirPath);
QString GetDirName(QString pDirPath);

bool CreatPath(QString pPath);
bool CreatFile(QString pPath);

qint64 GetPathSize(QString pDir);

QFileInfoList ListFolder(QString pDir, bool pSubDir, qint64 &pTotalSize, int *pControl = 0, pShowInfo *pShowFunc = 0);
QFileInfoList ListFolder(QFileInfo pDirInfo, bool pSubDir, qint64 &pTotalSize, int *pControl = 0, pShowInfo *pShowFunc = 0);

QStringList GetFileList(QFileInfoList pFileList, bool pContainSize = false, bool pContainVer = false);

QFileInfoList SearchFiles(QDir pDir, QString pFile,  QString pSuffix, QString pTxt, Qt::CaseSensitivity pSensitive = Qt::CaseInsensitive, bool pSubfolder = false);

bool CutFile(QString srcFile, QString dstFile, int *pControl = 0, pShowInfo *pShowFunc = 0);
bool CopyFile(QString srcFile, QString dstFile, int *pControl = 0, pShowInfo *pShowFunc = 0);
bool DelFile(QString pPath, int *pControl = 0, pShowInfo *pShowFunc = 0);

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder = true, int *pControl = 0, pShowInfo *pShowFunc = 0);
bool MoveFolder(QString pSrc, QString pDst, int *pControl = 0, pShowInfo *pShowFunc = 0);
bool DelFolder(QString pDir, int *pControl = 0, pShowInfo *pShowFunc = 0);
bool ClearFolder(QString pDir, int *pControl = 0, pShowInfo *pShowFunc = 0);

class FileOperate : public QThread
{
    Q_OBJECT
public:
    enum State{
        S_NONE = 0,
        S_ERROR,
        S_FILE_COPY,
        S_FILE_CUT,
        S_FILE_DEL,
        S_FOLDER_COPY,
        S_FOLDER_MOVE,
        S_FOLDER_DEL,
        S_FOLDER_CLR
    };
    FileOperate(QObject *parent = 0);

    void run();

    void Start(State pType);
    void Stop();
    bool isBusy();
    bool WaitForFinished(quint32 pTimeout = 30000);

    void CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, pShowInfo *pShowFunc = 0);
    void MoveFolder(QString pSrc, QString pDst, pShowInfo *pShowFunc = 0);
    void DelFolder(QString pDir, pShowInfo *pShowFunc = 0);
    void ClearFolder(QString pDir, pShowInfo *pShowFunc = 0);
    void CutFile(QString srcFile, QString dstFile, pShowInfo *pShowFunc = 0);
    void CopyFile(QString srcFile, QString dstFile, pShowInfo *pShowFunc = 0);
    void DelFile(QString pPath);

signals:
    void sChanged(State pState);

public:
    State                      m_State;
    pShowInfo                 *m_ShowFunc;
    int                        m_StopFlag;  //>@为0时表示终止

    QString                    m_Src, m_Dst;
    bool                       m_SubFolder;
};


#endif // FILETASK_H
