#include "mssql.h"

mssql::mssql(QObject *parent) :
    SQLBasic(parent)
{
}

mssql::~mssql()
{
}

bool mssql::init()
{
    QString tmpString = QString("DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2")
                                .arg(mPath)
                                .arg(mName);
    mSqlDatabase = QSqlDatabase::addDatabase("QODBC");
    mSqlDatabase.setConnectOptions("SQL_ATTR_PACKET_SIZE=1024;SQL_ATTR_CONNECTION_TIMEOUT=1,SQL_ATTR_LOGIN_TIMEOUT=1");
    mSqlDatabase.setDatabaseName(tmpString);
    return true;
}


bool mssql::SubmitTbRow(QString tbName, QHash<QString, QString>fieldVauleHash, QString pkVaule)
{
    QString cmd,fields,vaules,fieldsVaules;
    QStringList fieldList = fieldVauleHash.keys();
    for(int i=0;i<fieldList.count();i++)
    {
        QString field = fieldList.at(i);
        QString vaule = fieldVauleHash.value(field);
        fields.append(QString("%1, ").arg(field));
        vaules.append(QString("'%1', ").arg(vaule));
        fieldsVaules.append(QString("%1 = '%2', ").arg(field).arg(vaule));
    }
    if(fields.isEmpty())
        return false;
    fields.remove(fields.length()-2, 2);
    vaules.remove(vaules.length()-2, 2);
    fieldsVaules.remove(fieldsVaules.length()-2, 2);
    if(pkVaule.isEmpty())
    {
        cmd = QString("insert %1 (%2) values(%3)").
                arg(tbName).
                arg(fields).
                arg(vaules);
    }
    else
    {
        QString pk = GetTbPk(tbName);
        cmd = QString("update %1 set %2 where %3 = '%4'").
                arg(tbName).
                arg(fieldsVaules).
                arg(pk).
                arg(pkVaule);
    }
    if(Exec(cmd).isActive())
        return true;
    return false;
}

bool mssql::DelTbRow(QString tbName, QString pkVaule)
{
    QString pKey = GetTbPk(tbName);
    if(pKey.isEmpty())
        return true;
    QString cmd = QString("delete from %1 where %2='%3'").arg(tbName).arg(pKey).arg(pkVaule);
    if(Exec(cmd).isActive())
        return true;
    return false;
}

QStringList mssql::GetAllTbs()
{
    QStringList tbList;
    QString cmd = QString("select o.name from sysobjects o Where o.XType='U'");
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return tbList;
    while(query.next())
    {
        QString tbName = query.value(0).toString();
        if(tbName.compare("sysdiagrams", Qt::CaseInsensitive) == 0)
            continue;
        tbList.append(tbName);
    }
    return tbList;
}

int mssql::GetFieldLenth(QString tbName, QString fieldName)
{
    int ret = -1;
    QString cmd = QString("select cn.length "
                          "from SysColumns cn "
                          "where cn.id = object_id('%1') and cn.name='%2'").arg(tbName).arg(fieldName);

    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return ret;
    bool flag = false;
    while(query.next())
        ret = query.value(0).toInt(&flag);
    if(flag)return ret;
    return -1;
}

QStringList mssql::GetTbPks(QString tbName)
{
    QStringList ret;
    QString cmd = QString("select c.name "
                          "from sysindexes i "
                          "join sysindexkeys k on i.id = k.id and i.indid = k.indid "
                          "join sysobjects o on i.id = o.id "
                          "join syscolumns c on i.id=c.id and k.colid = c.colid "
                          "where o.xtype = 'U' "
                          "and exists(select * from sysobjects where xtype = 'PK' and name = i.name) and o.name='%1' "
                          "order by k.colid").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())
        return ret;
    while(query.next())
    {
        for(int i=0;i<query.record().count();i++)
        {
//            IDE_DEBUG(query.value(i).toString().trimmed());
            ret.append(query.value(i).toString().trimmed());
        }
    }
    return ret;
}

QString mssql::GetTbPk(QString tbName)
{
    QStringList pkList = GetTbPks(tbName);
    if(pkList.isEmpty())return QString();
    return pkList.at(0);
}

QMultiHash<QString, QString> mssql::GetTbUks0(QString tbName)
{
    QMultiHash<QString, QString> ret;
    QString cmd = QString("select k.indid, c.name "
                          "from sysindexes i "
                          "join sysindexkeys k on i.id = k.id and i.indid = k.indid "
                          "join sysobjects o on i.id = o.id "
                          "join syscolumns c on i.id=c.id and k.colid = c.colid "
                          "where o.xtype = 'U' "
                          "and exists(select * from sysobjects where xtype = 'UQ' and name = i.name) and o.name='%1' "
                          "order by k.indid").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return ret;
    while(query.next())
    {
        QString indexID = query.value(0).toString().trimmed();
        QString field = query.value(1).toString().trimmed();
        ret.insert(indexID, field);
    }
    return ret;
}

QStringList mssql::GetTbUks1(QString tbName)
{
    QMultiHash<QString, QString> uksHash = GetTbUks0(tbName);
    if(uksHash.isEmpty())
    {
        QStringList fieldList = GetTbFields(tbName);
        return (fieldList.count()>=2)?QStringList()<<fieldList.at(1):QStringList();
    }
    QStringList keyList = uksHash.uniqueKeys();
    return uksHash.values(keyList.first());
}

QMultiHash<QString, QString> mssql::GetTbUksVaules(QString tbName, bool pNotes)
{
    QMultiHash<QString, QString> ret;
    QMultiHash<QString, QString> ukMultiHash = GetTbUks0(tbName);
    QStringList UkList;
    if(!ukMultiHash.isEmpty())
        UkList = ukMultiHash.values(ukMultiHash.uniqueKeys().first());
    if(UkList.isEmpty())
    {
        QStringList fieldList = GetTbFields(tbName);
        if(fieldList.count()<2)
            return ret;
        UkList.append(fieldList.at(1));
    }
    QString retFields;
    for(int i=0;i<UkList.count();i++)
        retFields.append(QString("%1, ").arg(UkList.at(i)));
    retFields.remove(retFields.length()-2, 2);
    QString cmd = QString("select %1, %2 from %3").
                    arg(GetTbPk(tbName)).
                    arg(retFields).
                    arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())
        return ret;
    while(query.next())
    {
        for(int i=1;i<query.record().count();i++)
        {
            QString tmpValue;
            if(pNotes)
                tmpValue = QString("[%2]%1").arg(query.value(i).toString().trimmed()).arg(UkList.at(i-1));
            else
                tmpValue = QString("%1").arg(query.value(i).toString().trimmed());
            ret.insert(query.value(0).toString().trimmed(), tmpValue);
        }
    }
    return ret;
}

QString mssql::GetTbIkey(QString tbName)
{
    QStringList fieldList = GetTbFields(tbName);
    if(fieldList.count()<2)
        return QString();
    return fieldList.at(1);
}

QStringList mssql::GetTbFields(QString tbName)
{
    QStringList fieldList;
    if(tbName.isEmpty())return fieldList;
    QString cmd = QString("select cn.name "
                      "from SysColumns cn "
                      "where cn.id = object_id(N'%1')").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())
        return fieldList;
    while(query.next())
        fieldList.append(query.value(0).toString().trimmed());
    return fieldList;
}

QStringList mssql::GetTbUnNullFields(QString tbName)
{
    QStringList ret;
    QString cmd = QString("Select cn.name "
                          "from syscolumns cn , sysobjects b "
                          "where cn.id=b.id and b.name='%1' and isnullable=0").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return ret;
    while(query.next())
    {
        for(int i=0;i<query.record().count();i++)
            ret.append(query.value(i).toString());
    }
    return ret;
}

QString mssql::GetTbMaxPkVaule(QString tbName)
{
    QString ret;
    QString pk = GetTbPk(tbName);
    QString cmd = QString("select max(%1) from %2").arg(pk).arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return ret;
    while(query.next())
        ret = query.value(0).toString().trimmed();
    return ret;
}

QHash<QString, QString> mssql::GetTbParTb(QString tbName)//parTb
{
    QHash<QString, QString> retHash;
    QString cmd = QString("select ftable.name, cn.name "
                          "from sysforeignkeys "
                          "join sysobjects fk on sysforeignkeys.constid = fk.id "
                          "join sysobjects ftable on sysforeignkeys.fkeyid = ftable.id "
                          "join sysobjects rtable on sysforeignkeys.rkeyid = rtable.id "
                          "join syscolumns cn on sysforeignkeys.fkeyid = cn.id and sysforeignkeys.fkey = cn.colid "
                          "where sysforeignkeys.rkeyid = object_id(N'%1')").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return retHash;
    while(query.next())
        retHash.insert(query.value(0).toString().trimmed(), query.value(1).toString().trimmed());
    return retHash;
}

QHash<QString, QString> mssql::GetTbCldTbs(QString tbName)
{
    QHash<QString, QString> retHash;
    QString cmd = QString("select cn.name, rtable.name "
                          "from sysforeignkeys "
                          "join sysobjects fk on sysforeignkeys.constid = fk.id "
                          "join sysobjects ftable on sysforeignkeys.fkeyid = ftable.id "
                          "join sysobjects rtable on sysforeignkeys.rkeyid = rtable.id "
                          "join syscolumns cn on sysforeignkeys.fkeyid = cn.id and sysforeignkeys.fkey = cn.colid "
                          "where sysforeignkeys.fkeyid = object_id(N'%1')").arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())return retHash;
    while(query.next())
        retHash.insert(query.value(0).toString().trimmed(), query.value(1).toString().trimmed());
    return retHash;
}

QString mssql::GetTbCldTb(QString tbName, QString field)
{
    QHash<QString, QString> linkHash = GetTbCldTbs(tbName);
    return linkHash.value(field);
}

void mssql::GetTbAllCldTbs(QString tbName, QStringList &ret)
{
    QHash<QString, QString> rkPkHash;
    rkPkHash = GetTbCldTbs(tbName);
    QStringList rkList = rkPkHash.keys();
    for(int i=0;i<rkList.count();i++)
    {
        QString rk = rkList.at(i);
        QString cldTb = rkPkHash.value(rk);
//        IDE_DEBUG(tbName+"#"+rk+"#"+cldTb);
        ret.append(tbName+"#"+rk+"#"+cldTb);
        GetTbAllCldTbs(cldTb, ret);
    }
    return;
}

bool mssql::GetTbCldTbLinks(QString srcTb, QString dstTb, QStringList &ret)
{
    static bool flag = false;
    QHash<QString, QString> rkPkHash;
    rkPkHash = GetTbCldTbs(srcTb);
    QStringList rkList = rkPkHash.keys();
    for(int i=0;i<rkList.count();i++)
    {
        QString rk = rkList.at(i);
        QString cldTb = rkPkHash.value(rk);
        IDE_DEBUG(srcTb+"#"+rk+"#"+cldTb);
        ret.append(srcTb+"#"+rk+"#"+cldTb);
        if(cldTb.compare(dstTb, Qt::CaseInsensitive) == 0)
        {
            flag = true;
            return true;
        }
        if(GetTbCldTbLinks(cldTb, dstTb, ret))
            return true;
    }
    return flag;
}

QHash<QString, QString> mssql::GetTbPkIk(QString tbName)
{
    QHash<QString, QString> retHash;
    if(tbName.isEmpty())
        return retHash;
    QStringList fieldList = GetTbFields(tbName);
    if(fieldList.count()<2)
        return retHash;
    QString pk = GetTbPk(tbName);
    QString ik = fieldList.at(1);
    QString cmd = QString("select %1, %2 from %3").arg(pk).arg(ik).arg(tbName);
    QSqlQuery query = Exec(cmd);
    if(!query.isActive())
        return retHash;
    while(query.next())
        retHash.insert(query.value(0).toString().trimmed(), query.value(1).toString().trimmed());
    return retHash;
}

void mssql::GetTbFieldsLinks(QString tbName, QStringList fieldList, QStringList &retList, int level)
{
    level++;
    QHash<QString, QString> rkPkHash = GetTbCldTbs(tbName);
    QStringList rkList = rkPkHash.keys();
    QString rk, cldTb, fields;
    QStringList ukList;
    for(int i=0;i<rkList.count();i++)
    {
        fields.clear();
        rk = rkList.at(i);
        if(!fieldList.isEmpty()&&!fieldList.contains(rk))
            goto step0;
        for(int j=0;j<fieldList.count();j++)
            fields.append(QString("%1,").arg(fieldList.at(j)));
        if(!fields.isEmpty())
            fields.remove(fields.length()-1, 1);
        cldTb = rkPkHash.value(rk);
        ukList = GetTbUks1(cldTb);
        retList.append(QString("%1-%2:%3#%4->%5").arg(level).arg(tbName).arg(fields).arg(rk).arg(cldTb));
        GetTbFieldsLinks(cldTb, ukList, retList, level);
    }
step0:
    return;
}

QStringList mssql::GetTbFieldLinks(QString tbName, QString field)
{
    QStringList ret, linkList;
    GetTbFieldsLinks(tbName, QStringList()<<field, linkList);
    ret = linkList;
    QString tbList;
    for(int i=0;i<linkList.count();i++)
    {
        QString link = linkList.at(i);
        QString tb = link.split(":", QString::SkipEmptyParts).at(0);
        tb = tb.split("-", QString::SkipEmptyParts).at(1);
        if(!tbList.contains(tb))
        {
            tbList.append(tb);
        }
    }
    for(int i=0;i<linkList.count();i++)
    {
        QString link = linkList.at(i);
        QString cldTb = link.split("->", QString::SkipEmptyParts).at(1);
        int level = link.split("-", QString::SkipEmptyParts).at(0).toInt();
        qDebug()<<cldTb<<":"<<level;
        if(!tbList.contains(cldTb))
        {
            IDE_TRACE_INT(level);
            QStringList ukList = GetTbUks1(cldTb);
            QString fields;
            for(int j=0;j<ukList.count();j++)
                fields.append(QString("%1,").arg(ukList.at(j)));
            if(!fields.isEmpty())
                fields.remove(fields.length()-1, 1);
            ret.append(QString("%1-%2:%3").arg(level++).arg(cldTb).arg(fields));
        }
    }
    return ret;
}

QHash<QString, QString>mssql::GetTbFieldVaules(QString tbName, QString field)
{
    QHash<QString, QString> ret;
    QStringList linkList = GetTbFieldLinks(tbName, field);
    for(int i=0;i<linkList.count();i++)
    {
        IDE_DEBUG(linkList.at(i));
    }
    return ret;
}

QSqlQuery mssql::ScanTb0(QString tbName, QStringList retFieldList, QString orderField)
{
    if(retFieldList.isEmpty())
        return QSqlQuery();
    if(orderField.isEmpty())
        orderField = retFieldList.at(0);
    QString retFields;
    for(int i=0;i<retFieldList.count();i++)
        retFields.append(retFieldList.at(i)).append(", ");
    retFields.remove(retFields.length()-2, 2);
    QString cmd = QString("Select %1 from [%2] order by %3 desc").
            arg(retFields).
            arg(tbName).
            arg(orderField);
    return Exec(cmd);
}

QSqlQuery mssql::ScanTb1(QString tbName, QString pkVaule, QStringList retFieldList, QString orderField)
{
    if(retFieldList.isEmpty())return QSqlQuery();
    if(orderField.isEmpty())orderField = retFieldList.at(0);
    QString pk = GetTbPk(tbName);
    if(pk.isEmpty())
        return QSqlQuery();
    return ScanTb2(tbName,
                   QStringList()<<pk,
                   QStringList()<<pkVaule,
                   retFieldList,
                   orderField);
}

QSqlQuery mssql::ScanTb2(QString tbName, QStringList srcFieldList, QStringList srcVauleList, QStringList retFieldList, QString orderField)
{
    if(srcFieldList.isEmpty()|| srcFieldList.count()!=srcVauleList.count()|| retFieldList.isEmpty())
        return QSqlQuery();
    if(orderField.isEmpty())
        orderField = retFieldList.at(0);
    QString constraint;
    for(int i=0;i<srcFieldList.count();i++)
        constraint.append(QString("%1='%2' and ").
                          arg(srcFieldList.at(i)).
                          arg(srcVauleList.at(i)));
    constraint.remove(constraint.length()-5, 5);
    QString retFields;
    for(int i=0;i<retFieldList.count();i++)
        retFields.append(retFieldList.at(i)).append(", ");
    retFields.remove(retFields.length()-2, 2);
    QString cmd = QString("Select %1 from [%2] where %3 order by %4 desc").
                            arg(retFields).
                            arg(tbName).
                            arg(constraint).
                            arg(orderField);
    return Exec(cmd);
}

QSqlQuery mssql::ScanTb3(QString tbName, QStringList srcFieldList, QList<QStringList> srcVauleslist, QStringList retFieldList, QString orderField)
{
    if(tbName.isEmpty()||
        srcFieldList.isEmpty()||
        srcFieldList.count()!=srcVauleslist.count()||
        retFieldList.isEmpty())
        return QSqlQuery();
    if(orderField.isEmpty())orderField = retFieldList.at(0);
    QString fields;
    for(int i=0;i<retFieldList.count();i++)
    {
        QString field = retFieldList.at(i);
        fields.append(field).append(", ");
    }
    fields.remove(fields.length()-2, 2);
    QString multiVaules;
    for(int i=0;i<srcFieldList.count();i++)
    {
        QString field = srcFieldList.at(i);
        QStringList vauleList = srcVauleslist.at(i);
        if(field.isEmpty()||vauleList.isEmpty())
            continue;
        QString vaules;
        for(int j=0;j<vauleList.count();j++)
        {
            QString vaule = vauleList.at(j);
            if(vaule.isEmpty())
                continue;
            vaules.append((QString("%1 %2 or ").arg(field).arg(vaule)));
        }
        vaules.remove(vaules.length()-4, 4);
        if(vaules.isEmpty())
            continue;
        vaules = QString("(%1) ").arg(vaules);
        multiVaules.append(QString("%1and ").arg(vaules));
    }
    multiVaules.remove(multiVaules.length()-5, 5);
    QString cmd;
    if(multiVaules.isEmpty())
    {
        cmd = QString("Select %1 from %2 order by %3 desc").
                arg(fields).
                arg(tbName).
                arg(orderField);
    }
    else
    {
        cmd = QString("Select %1 from %2 where %3 order by %4 desc").
                arg(fields).
                arg(tbName).
                arg(multiVaules).
                arg(orderField);
    }
    cmd.replace("#", "%");
    return Exec(cmd);
}

QStringList mssql::GetBoardTypes(QString pCodeID, QString &pMainBoardType, QString &pInterfaceBoardType)
{
    QSqlQuery query = ScanTb2(D_OrderTb,
                              QStringList()<<D_BarCode,
                              QStringList()<<pCodeID,
                              QStringList()<<D_MainBoardID<<D_InterfaceBoardID);
    if(!(query.isActive() && query.first()))
        return QStringList();
    QStringList tmpTypeLinkList;
    tmpTypeLinkList.append(query.value(0).toString().trimmed());
    tmpTypeLinkList.append(query.value(1).toString().trimmed());
    QSqlQuery query1 = ScanTb2(D_MainBoardTypeTb,
                               QStringList()<<D_ID,
                               QStringList()<<tmpTypeLinkList.at(0),
                               QStringList()<<D_BoardType<<D_WholesaleType);
    if(query1.isActive() && query1.first())
        pMainBoardType = QString("%1_%2").arg(query1.value(0).toString().trimmed()).arg(query1.value(1).toString().trimmed());
    QSqlQuery query2 = ScanTb2(D_InterfaceBoardTypeTb,
                               QStringList()<<D_ID,
                               QStringList()<<tmpTypeLinkList.at(1),
                               QStringList()<<D_BoardType<<D_WholesaleType);
    if(query2.isActive() && query2.first())
        pInterfaceBoardType = QString("%1_%2").arg(query2.value(0).toString()).arg(query2.value(1).toString());
    return tmpTypeLinkList;
}

QString mssql::GetOrder(QString pCodeType, QString pCodeID)
{
    if(pCodeType.isEmpty() || pCodeID.isEmpty())
        return QString();
    if(pCodeType.compare(D_OrderID) == 0)
        return pCodeID;
    //>@如果是设备
    if(pCodeType.compare(D_DeviceID) == 0)
    {
        QSqlQuery query = ScanTb2(D_DeviceTb,
                                  QStringList()<<D_BarCode,
                                  QStringList()<<pCodeID,
                                  QStringList()<<D_DeviceTb_MainBoardBarcode<<D_DeviceTb_InterfaceBoardBarcode);
        if(!(query.isActive() && query.first()))
            return QString();
        QString tmpString0 = query.value(0).toString();
        QString tmpString1 = query.value(1).toString();
        if(!tmpString0.isEmpty())  //>@如果有主板，则通过主板寻找订单号
        {
            pCodeType = D_MainBoardID;
            pCodeID = tmpString0;
        }
        else if(!tmpString1.isEmpty())
        {
            pCodeType = D_InterfaceBoardID;
            pCodeID = tmpString1;
        }
        else
            return QString();
    }
    QString tmpOrderCode;
    if(pCodeType.compare(D_MainBoardID) == 0)
    {
        QSqlQuery query = ScanTb2(D_MainBoardTb,
                                  QStringList()<<D_BarCode,
                                  QStringList()<<pCodeID,
                                  QStringList()<<D_OrderID);
        if(!(query.isActive() && query.first()))
            return QString();
        tmpOrderCode = query.value(0).toString();
    }
    else if(pCodeType.compare(GBKToUTF8("接口板")) == 0)
    {
        QSqlQuery query = ScanTb2(D_InterfaceBoardTb,
                                  QStringList()<<D_BarCode,
                                  QStringList()<<pCodeID,
                                  QStringList()<<D_OrderID);
        if(!(query.isActive() && query.first()))
            return QString();
        tmpOrderCode = query.value(0).toString();
    }
    return tmpOrderCode;
}

QSqlQuery mssql::GetMainBoardFiles(QString pBoardTypeLink)
{
    if(pBoardTypeLink.isEmpty())
        return QSqlQuery();
    return ScanTb2(D_MainBoardFileTb,
                              QStringList()<<D_ID,
                              QStringList()<<pBoardTypeLink,
                              QStringList()<<D_FileType<<D_BoardFile_Version);
}

QSqlQuery mssql::GetInterfaceBoardFiles(QString pBoardTypeLink)
{
    if(pBoardTypeLink.isEmpty())
        return QSqlQuery();
    return ScanTb2(D_InterfaceBoardFileTb,
                              QStringList()<<D_ID,
                              QStringList()<<pBoardTypeLink,
                              QStringList()<<D_FileType<<D_BoardFile_Version);
}

QString mssql::getNetDiskAdr(QString tbName, QString pkVaule)
{
    QString ret;
    QStringList retList;
    if(GetTbCldTbLinks(tbName, D_NetDiskAdrTb, retList))
    {
        QString cte, select, from, where;
        for(int i=0;i<retList.count();i++)
        {
            QStringList list =  retList.at(i).split("#", QString::SkipEmptyParts);
            QString tb = list.at(0);
            QString rk = list.at(1);
            QString cldTb = list.at(2);
            QString cteName = QString("cte%1").arg(i);
            QString cteNikName = QString("nik%1").arg(i);
            QString cteNextNikName = QString("nik%1").arg(i+1);
            cte.append(QString("%1 as(select * from %2), ").arg(cteName).arg(tb));
            select.append(QString("%1.%2, ").arg(cteNikName).arg(rk));
            from.append(QString("%1 %2, ").arg(cteName).arg(cteNikName));
            if(i == 0)
            {
                where.append(QString("%1.%2 = '%3' and ").
                             arg(cteNikName).
                             arg(GetTbPk(tb)).
                             arg(pkVaule));
            }
            where.append(QString("%1.%2 = %3.%4 and ").
                         arg(cteNikName).
                         arg(rk).
                         arg(cteNextNikName).
                         arg(GetTbPk(cldTb)));
            if(i == retList.count()-1)
            {
                cteName = QString("cte%1").arg(i+1);
                cteNikName = QString("nik%1").arg(i+1);
                cte.append(QString("%1 as(select * from %2), ").arg(cteName).arg(cldTb));
                select.append(QString("%1.%2, ").arg(cteNikName).arg(D_NetDiskAdrTb_Adr));
                from.append(QString("%1 %2, ").arg(cteName).arg(cteNikName));
            }
        }
        cte.remove(cte.length()-2, 2);
        select.remove(select.length()-2, 2);
        from.remove(from.length()-2, 2);
        where.remove(where.length()-4, 4);
        QString cmd = QString("with %1 select %2 from %3 where %4").
                arg(cte).
                arg(select).
                arg(from).
                arg(where);
        IDE_TRACE_STR(cmd);
        QSqlQuery query = Exec(cmd);
        if(!query.isActive())
            return QString();
        while(query.next())
            ret = query.value(query.record().count()-1).toString().trimmed();
    }
    return ret;
}

QStringList mssql::GetAllID(QString tbName)
{
    QStringList ret;
    QStringList fieldList = GetTbFields(tbName);
    if(fieldList.count()<2)
        return ret;
    QString IDField = fieldList.at(1);
    QSqlQuery query = ScanTb0(tbName,
                                       QStringList()<<IDField,
                                       IDField);
    while(query.next())
        ret.append(query.value(0).toString().trimmed());
    return ret;
}

QString mssql::DeviceIDToOrderID(QString deviceID)
{
    QString ret;
    QSqlQuery query = ScanTb2(D_DeviceTb,
                               QStringList()<<D_BarCode,
                               QStringList()<<deviceID,
                               QStringList()<<D_DeviceTb_MainBoardBarcode<<D_DeviceTb_InterfaceBoardBarcode,
                               D_BarCode);
    if(!query.isActive())
        return QString();
    QString mainBoardID;
    while(query.next())
        mainBoardID = query.value(0).toString();
    query = ScanTb2(D_MainBoardTb,
                             QStringList()<<D_BarCode,
                             QStringList()<<mainBoardID,
                             QStringList()<<D_OrderID,
                             D_OrderID);
    if(!query.isActive())
        return QString();
    while(query.next())
        ret = query.value(0).toString();
    return ret;
}

QString mssql::MainBoardIDToOrderID(QString mainBoardID)
{
    QString ret;
    QSqlQuery query = ScanTb2(D_MainBoardTb,
                                       QStringList()<<D_BarCode,
                                       QStringList()<<mainBoardID,
                                       QStringList()<<D_OrderID,
                                       D_OrderID);
    if(!query.isActive())
        return QString();
    while(query.next())
        ret = query.value(0).toString();
    return ret;
}

QString mssql::InterfaceBoardIDToOrderID(QString interfaceBoardID)
{
    QString ret;
    QSqlQuery query = ScanTb2(D_InterfaceBoardTb,
                                       QStringList()<<D_BarCode,
                                       QStringList()<<interfaceBoardID,
                                       QStringList()<<D_OrderID,
                                       D_OrderID);
    if(!query.isActive())
        return QString();
    while(query.next())
        ret = query.value(0).toString();
    return ret;
}

bool mssql::AddRecordToMainBoardTb(QString orderID, QString barcode)
{
    QSqlQuery query = ScanTb2(D_OrderTb,
                               QStringList()<<D_OrderID,
                               QStringList()<<orderID,
                               QStringList()<<D_ID,
                               D_OrderID);
    QString orderTbPk;
    if(!query.isActive())
        return false;
    while(query.next())
        orderTbPk = query.value(0).toString();
    IDE_DEBUG(orderTbPk);
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_OrderID, orderTbPk);
    fieldVauleHash.insert(D_BarCode, barcode);
    return SubmitTbRow(D_MainBoardTb, fieldVauleHash);
}

bool mssql::AddRecordToMainBoardLogTb(QString barcode, QString action)
{
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_BarCode, barcode);
    QString curTime = QDateTime::currentDateTime().toString();
    fieldVauleHash.insert(D_SubmitTime, curTime);
    fieldVauleHash.insert(D_Describe, action);
    return SubmitTbRow(D_MainBoardLogTb, fieldVauleHash);
}

bool mssql::AddRecordToInterfaceBoardTb(QString orderID, QString barcode)
{
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_OrderID, orderID);
    fieldVauleHash.insert(D_BarCode, barcode);
    return SubmitTbRow(D_InterfaceBoardTb, fieldVauleHash);
}

bool mssql::AddRecordToInterfaceBoardLogTb(QString barcode, QString action)
{
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_BarCode, barcode);
    QString curTime = QDateTime::currentDateTime().toString();
    fieldVauleHash.insert(D_SubmitTime, curTime);
    fieldVauleHash.insert(D_Describe, action);
    return SubmitTbRow(D_InterfaceBoardLogTb, fieldVauleHash);
}

bool mssql::AddRecordToDeviceTb(QString barcode, QString mainBoardbarcode, QString interfaceBoardbarcode)
{
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_BarCode, barcode);
    fieldVauleHash.insert(D_DeviceTb_MainBoardBarcode, mainBoardbarcode);
    fieldVauleHash.insert(D_DeviceTb_InterfaceBoardBarcode, interfaceBoardbarcode);
    return SubmitTbRow(D_DeviceTb, fieldVauleHash);
}

bool mssql::AddRecordToDeviceLogTb(QString barcode, QString action)
{
    QHash<QString, QString>fieldVauleHash;
    fieldVauleHash.insert(D_BarCode, barcode);
    QString curTime = QDateTime::currentDateTime().toString();
    fieldVauleHash.insert(D_SubmitTime, curTime);
    fieldVauleHash.insert(D_Describe, action);
    return SubmitTbRow(D_DeviceLogTb, fieldVauleHash);
}

