#include "sqlite.h"

sqlite::sqlite(QObject *parent) :
    SQLBasic(parent)
{
}

sqlite::~sqlite()
{
}

bool sqlite::open(QString pPath, QString pName, OpenMode mode)
{
    pPath.replace("\\", "/");
    if(!pPath.endsWith("/"))
        pPath.append("/");
    return SQLBasic::open(pPath, pName, mode);
}

bool sqlite::init()
{
    mSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    //mSqlDatabase.setConnectOptions("SQL_ATTR_PACKET_SIZE=1024;SQL_ATTR_CONNECTION_TIMEOUT=1,SQL_ATTR_LOGIN_TIMEOUT=1");
    mSqlDatabase.setDatabaseName(mPath + mName);
    return true;
}

