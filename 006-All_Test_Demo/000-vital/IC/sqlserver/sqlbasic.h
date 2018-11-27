#ifndef SQLBASIC_H
#define SQLBASIC_H

#include "define.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlField>
#include <QSqlIndex>
#include <QStringList>
#include <QMap>
#include <QVariant>

#include <QtNetwork>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
Q_DECLARE_METATYPE(QHostAddress)

class SQLBasic : public QIODevice
{
    Q_OBJECT
public:
    explicit SQLBasic(QObject *parent = 0);
    ~SQLBasic();

    virtual bool open(OpenMode mode=ReadWrite);
    virtual bool open(QString pPath, QString pName, OpenMode mode=ReadWrite);
    void close();

    qint64 readData(char *data, qint64 maxlen) {return 0;}
    qint64 writeData(const char *data, qint64 len) {return 0;}

    virtual bool init() {return false;}

    bool isOpen() {return mSqlDatabase.isOpen();}

    bool OpenSQLDatabase(QHostAddress ipAdr, QString sqlName, int pType=0);
    void CloseSQLDatabase();

    bool Connect(QHostAddress ipAdr, QString sqlName, int pType=0){}
    bool Disconnect(){}

    QSqlQuery Exec(QString cmd);
    void sendInfo(QString err)   {emit sInfo(err);}

    bool QueryRecord(QString pTableName, QString pFilter);

    QSqlRecord GetRecord(QString pTableName, QString pFilter);  //>@获取可以修改的Record
    bool SelectTB(QString tableName);
    bool SaveTB(QSqlRecord pRecord);
    bool SaveTB();
    bool RebackTB();

    bool AddRow(QString tableName, QList<QVariant>fieldVauleList);
    bool DelRow(QString tableName, QString pFilter);
    bool DelRow(QString tableName, QString pField, QString pFiledValue);
    bool DelRow(QString tableName, int row);
    bool ModifyRow(QString tableName, QString pField, QString pFiledValue, QList<QVariant>fieldVauleList);
    bool ModifyRow(QString tableName, QString pKeyField, QString pKeyFiledValue, QString pDstField, QString pDstFiledValue);
    bool ModifyRow(QString tableName, int column, int row, QVariant vaule);
    bool ModifyRow(QString tableName, QString columnName, int row, QVariant vaule);
    bool ModifyRow(QString tableName, QString pFilter, QList<QVariant>fieldVauleList);

    bool UploadFile(QString pTable, int pType, QString pFilePath);
    bool DownloadFile(QString pTable, int pType, QString pFilePath);

    //>@获取字段类型（int nchar（string））
    QVariant::Type GetTbFieldType(QString tbName, QString fieldName);

    //>@清空表
    bool ClearTb(QString tbName);//>@!!!All clean

    QString GetError() {return mError.text();}

signals:
    void sInfo(QString err);

public:
    QThread                     *mThread;
    QString                      mPath, mName;

    QSqlError                    mError;

    QSqlDatabase                 mSqlDatabase;
    QSqlTableModel              *mSqlTBModel;
    QSqlQuery                   *mQuery;
};

#endif // SQLBASIC_H
