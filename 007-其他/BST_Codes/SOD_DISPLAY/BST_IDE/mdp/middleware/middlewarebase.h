#ifndef MIDDLEWAREBASE_H
#define MIDDLEWAREBASE_H

#include "devfun.h"

/*
    1、根据参数生成配置脚本文件
    2、根据参数拷贝数据库中的文件夹
    3、提供可用的应用程序列表
*/

class MiddlewareDevice : public IODeviceBase
{
    Q_OBJECT
public:
    MiddlewareDevice(QObject *parent = 0);
    ~MiddlewareDevice();

    bool InitDevice(MW_Type pType, QDomElement pElement);
    QString GetSystemPath()  {return D_FSRCPATH+D_SYSTEMDIR+D_MIDDLEWAREDIR+mDevName+QString("/");}
    virtual bool DownloadLibrary();  //>@下载库
    virtual bool GenerateScript();   //>@生成相关配置文件

    //>@获取本节点中的重要信息
    virtual QString GetInformation() { return QString("%1").arg(mDevName); }
};

#endif // MIDDLEWAREBASE_H

