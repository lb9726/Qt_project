#include "exeupdate.h"

ExeUpdate::ExeUpdate(QObject *parent) : QObject(parent)
{

}

bool ExeUpdate::checkUpdate()
{
    //通过DCT判断是否存在程序更新
    return true;
}

bool ExeUpdate::updateFile(QString filePath, int type)
{
    return true;
}

