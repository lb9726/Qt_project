#include "sqlbasic.h"

SQLBasic::SQLBasic(QObject *parent) :
    QIODevice(parent)
{
    mSqlTBModel = 0;
    mQuery = 0;

    mThread = new QThread;
    moveToThread(mThread);
    mThread->start(QThread::LowPriority);
}

SQLBasic::~SQLBasic()
{
    disconnect(this, SIGNAL(sInfo(QString)), 0, 0);
    close();
}

bool SQLBasic::open(OpenMode mode)
{
    close();
#if 0
    QStringList driverList = QSqlDatabase::drivers();
    foreach(QString driver, driverList)
        IDE_DEBUG(driver);
#endif
    if(!init())   //>@同一ConnectName的连接只能连接一次。
    {
        IDE_TRACE();
        return false;
    }
    bool ret = mSqlDatabase.open();
    if(!ret)
    {
        IDE_DEBUG(mSqlDatabase.lastError().databaseText());
        return false;
    }
    mSqlTBModel = new QSqlTableModel(this, mSqlDatabase);
    mQuery = new QSqlQuery(mSqlDatabase);
    return true;
}

bool SQLBasic::open(QString pPath, QString pName, OpenMode mode)
{
    //>@如果是打开之前使用的数据库，则直接返回打开情况，如果没有打开使用open(OpenMode mode)打开即可。
    if(!pPath.compare(mPath, Qt::CaseInsensitive) && !pName.compare(mName, Qt::CaseInsensitive))
        return isOpen();
    mPath = pPath;
    mName = pName;
    return open(mode);
}

void SQLBasic::close()
{
    if(mSqlDatabase.isOpen())
        mSqlDatabase.close();
    if(mSqlTBModel)
        mSqlTBModel->deleteLater();
    mSqlTBModel = 0;
    if(mQuery)
    {
        mQuery->clear();
        delete mQuery;
    }
    mQuery = 0;
}

QSqlQuery SQLBasic::Exec(QString cmd)
{
    QSqlQuery query(mSqlDatabase);
    if(!mSqlDatabase.isOpen())
        return query;
    if(!query.exec(cmd))
    {
        mError = query.lastError();
        emit sInfo(mError.text());
    }
    return query;
}

bool SQLBasic::SelectTB(QString tableName)
{
    if(!mSqlDatabase.isOpen())
        return false;
    mSqlTBModel->setTable(tableName);
    if(mSqlTBModel->select())
        while(mSqlTBModel->canFetchMore())
            mSqlTBModel->fetchMore();
    return true;
}

bool SQLBasic::QueryRecord(QString pTableName, QString pFilter)
{
    if(!SelectTB(pTableName))
        return false;
    mSqlTBModel->setFilter(pFilter);
    return mSqlTBModel->select();
}

QSqlRecord SQLBasic::GetRecord(QString pTableName, QString pFilter)
{
    if(!SelectTB(pTableName))
        return QSqlRecord();
    mSqlTBModel->setFilter(pFilter);
    if(mSqlTBModel->select())
    {
        if (mSqlTBModel->rowCount() == 1)
        {
            return mSqlTBModel->record(0);
        }
    }
    return QSqlRecord();
}

bool SQLBasic::SaveTB(QSqlRecord pRecord)
{
    mSqlTBModel->setRecord(0, pRecord);
    return SaveTB();
}

bool SQLBasic::SaveTB()
{
    if(!mSqlDatabase.isOpen())
        return false;
    return mSqlTBModel->submitAll();
}

bool SQLBasic::RebackTB()
{
    if(!mSqlDatabase.isOpen())
        return false;
    mSqlTBModel->revertAll();
    return true;
}

bool SQLBasic::AddRow(QString tableName, QList<QVariant>fieldVauleList)
{
    if(!SelectTB(tableName))
        return false;
    int rowCnt = mSqlTBModel->rowCount();
    IDE_TRACE();
    if(!mSqlTBModel->insertRow(rowCnt))
    {
        IDE_TRACE();
        return false;
    }
    bool ret = false;
    for(int i=0;i<fieldVauleList.count();i++)
    {
        IDE_DEBUG(fieldVauleList.at(i).toString());
        ret = mSqlTBModel->setData(mSqlTBModel->index(rowCnt, i), fieldVauleList.at(i));
        if(!ret)
            break;
    }
    if(ret)
        return SaveTB();
    else
    RebackTB();
    return false;
}

bool SQLBasic::DelRow(QString tableName, QString pFilter)
{
    if(!SelectTB(tableName))
        return false;
    mSqlTBModel->setFilter(pFilter);
    if(mSqlTBModel->select())
    {
        if(mSqlTBModel->rowCount() == 1)
        {
            mSqlTBModel->removeRows(0, 1); // 如果要删除所有满足条件的记录则把1改成model.rowCount()
            return SaveTB();
        }
    }
    return false;
}

bool SQLBasic::DelRow(QString tableName, QString pField, QString pFiledValue)
{
    if(!SelectTB(tableName))
        return false;
    mSqlTBModel->setFilter(QString("%1='%2'").arg(pField).arg(pFiledValue));
    if(mSqlTBModel->select())
    {
        if(mSqlTBModel->rowCount() == 1)
        {
            mSqlTBModel->removeRows(0, 1); // 如果要删除所有满足条件的记录则把1改成model.rowCount()
            return SaveTB();
        }
    }
    return false;
}

bool SQLBasic::DelRow(QString tableName, int row)
{
    if(!SelectTB(tableName))
        return false;
    if(row > mSqlTBModel->rowCount())
        return false;
    if(mSqlTBModel->removeRow(row))
        return SaveTB();
    RebackTB();
    return false;
}

bool SQLBasic::ModifyRow(QString tableName, QString pField, QString pFiledValue, QList<QVariant>fieldVauleList)
{
    if(!SelectTB(tableName))
        return false;
    mSqlTBModel->setFilter(QString("%1='%2'").arg(pField).arg(pFiledValue));
    if(mSqlTBModel->select())
    {
        if(mSqlTBModel->rowCount() == 1)
        {
            bool ret = false;
            for(int i=0;i<fieldVauleList.count();i++)
            {
                IDE_DEBUG(fieldVauleList.at(i).toString());
                ret = mSqlTBModel->setData(mSqlTBModel->index(0, i), fieldVauleList.at(i));
                if(!ret)
                return false;
//                    break;
            }
            if(ret)
                return SaveTB();
            else
                return RebackTB();
        }
        else
        {
            return AddRow(tableName, fieldVauleList);
        }
    }
    return false;
}

bool SQLBasic::ModifyRow(QString tableName, QString pFilter, QList<QVariant>fieldVauleList)
{
    if(!SelectTB(tableName))
        return false;
    mSqlTBModel->setFilter(pFilter);
    if(mSqlTBModel->select())
    {
        if(mSqlTBModel->rowCount() == 1)
        {
            bool ret = false;
            for(int i=0;i<fieldVauleList.count();i++)
            {
                IDE_DEBUG(fieldVauleList.at(i).toString());
                ret = mSqlTBModel->setData(mSqlTBModel->index(0, i), fieldVauleList.at(i));  //修改行
                if(!ret)
                    break;
            }
            if(ret)
                return SaveTB();
            return RebackTB();
        }
        else
        {
            return AddRow(tableName, fieldVauleList);  //重新加入电梯信息
        }
    }
    return false;
}

bool SQLBasic::ModifyRow(QString tableName, QString pKeyField, QString pKeyFiledValue, QString pDstField, QString pDstFiledValue)
{
    if(!SelectTB(tableName))
        return false;
    mSqlTBModel->setFilter(QString("%1='%2'").arg(pKeyField).arg(pKeyFiledValue));
    if(mSqlTBModel->select())
    {
        if(mSqlTBModel->rowCount() == 1)
        {
            QSqlRecord tmpRecord = mSqlTBModel->record();
            tmpRecord.setValue(pDstField, pDstFiledValue);
            return SaveTB();
        }
    }
    return false;
}

bool SQLBasic::ModifyRow(QString tableName, int column, int row, QVariant vaule)
{
    if(!SelectTB(tableName))
        return false;
    if(row > mSqlTBModel->rowCount())
        return false;
    if(mSqlTBModel->setData(mSqlTBModel->index(row, column), vaule))
        return SaveTB();
    RebackTB();
    return false;
}

bool SQLBasic::ModifyRow(QString tableName, QString columnName, int row, QVariant vaule)
{
    int column = mSqlTBModel->record().indexOf(columnName);
    return ModifyRow(tableName, column, row, vaule);
}

QVariant::Type SQLBasic::GetTbFieldType(QString tbName, QString fieldName)
{
    if(!mSqlDatabase.isOpen())
        return QVariant::Invalid;
    QSqlTableModel model(this, mSqlDatabase);
    model.setTable(tbName);
    if(!model.select())
        return QVariant::Invalid;
    while(model.canFetchMore())
        model.fetchMore();
    return model.record().field(fieldName).type();
}

bool SQLBasic::ClearTb(QString tbName)
{
#ifdef SQLITE
    QString cmd = QString("DROP TABLE %1").arg(tbName);
#else
    QString cmd = QString("TRUNCATE [%1]").arg(tbName);
#endif
    if(Exec(cmd).isActive())
        return true;
    return false;
}

bool SQLBasic::UploadFile(QString pTable, int pType, QString pFilePath)
{
    QFile tmpFile(pFilePath);
    if(tmpFile.exists() && tmpFile.open(QIODevice::ReadOnly))
    {
        QByteArray tdata = tmpFile.readAll();
        tmpFile.close();
        if(tdata.isEmpty())
            return false;
        QByteArray data = qCompress(tdata, 9);
        QString tmpCmd = QString("INSERT INTO %1 (id,filetype,filename,filecontent) VALUES(NULL,:filetype,:filename,:filecontent)").arg(pTable);
        mQuery->prepare(tmpCmd);
        mQuery->bindValue(":filetype", pType);
        mQuery->bindValue(":filename", tmpFile.fileName());
        mQuery->bindValue(":filecontent", data);
        return mQuery->exec();
    }
    return false;
}

bool SQLBasic::DownloadFile(QString pTable, int pType, QString pFilePath)
{
    QFile tmpFile(pFilePath);
    if(tmpFile.open(QIODevice::WriteOnly))
    {
        QString tmpCmd = QString("SELECT filecontent FROM %1 WHERE filetype='%2' AND filename='%3'").arg(pTable).arg(pType).arg(tmpFile.fileName());
        mQuery->prepare(tmpCmd);
        if(!mQuery->exec())
        {
            tmpFile.close();
            return false;
        }
        if(!mQuery->next())
        {
            tmpFile.close();
            return false;
        }
        QByteArray tdata = mQuery->value(0).toByteArray();
        QByteArray data = qUncompress(tdata);
        tmpFile.write(data);
        tmpFile.close();
        return true;
    }
    return false;
}

