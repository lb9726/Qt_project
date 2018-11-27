#ifndef SQLQUERY_H
#define SQLQUERY_H

#include "define.h"

#ifdef SURRPORT_SQL
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>
#include <QModelIndex>
#endif

#define D_FILESERVERPORT 9000

typedef enum{
    Administrator = 0,  //>@π‹¿Ì‘±
    Researcher,
    Salesman,
    Producer,
    Material
} SQL_AUTHORITY;

class SqlQuery : public QObject
{
    Q_OBJECT
public:
    SqlQuery(QObject *parent = 0);
    ~SqlQuery();

    bool Connect(QString pDatabaseIpAdr, QString pFileserverIpAdr, qint16 pPort, QString pDatabaseName,
                 QString pUsrName, QString pPasswd);
    bool Disconnect();

    bool OpenDatabase(QString pDatabase, QString pServer, QString pUID, QString pPWD);
    bool CloseDatabase();

    bool AddRow(QString pTableName, int pRow, QList<QVariant> pPara);
    bool DelRow(QString pTableName, int pRow);
    bool UpdateRow(QString pTableName, int pRow, QList<QVariant> pPara);

    bool ExecSqlQuery(QString pQuery);
    QList<QVariant> QueryDatas(QString pTableName, QString pColumns, QString pCondition);

    bool DownLoadFile(QString pServerFilePath, QString pLocalFilePath, quint32 pTimeout);

public slots:
    void slot_ReadFileOperateOutput();

signals:
    void sGetFile(bool flag);
    void sProgress(quint8 pVaule);

public:
    QSqlDatabase        m_SqlDatabase;
    QSqlQueryModel     *m_SqlQueryModel;
    SQL_AUTHORITY       m_Authority;
    QString             m_LastErrorString;

    QProcess           *m_FileProcess;
    QString             m_FileServerAddr;
    qint16              m_FileServerPort;
};

#endif // SQLQUERY_H
