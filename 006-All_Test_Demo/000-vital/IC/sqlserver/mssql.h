#ifndef MSSQL_H
#define MSSQL_H

#include "sqlbasic.h"

//>@=========================关键字============================================
#define D_DeviceID                              GBKToUnicode("设备")
#define D_MainBoardID                           GBKToUnicode("主板")
#define D_InterfaceBoardID                      GBKToUnicode("接口板")
#define D_OrderID                               GBKToUnicode("订单号")

#define D_ID                                    GBKToUnicode("ID")
#define D_SubmitTime                            GBKToUnicode("提交时间")
#define D_BarCode                               GBKToUnicode("二维码")
#define D_Describe                              GBKToUnicode("描述")
#define D_FileType                              GBKToUnicode("文档类型")
#define D_StoreNetDisk                          GBKToUnicode("存档网盘")
//>@=========================产品表============================================
#define D_OrderTb                               GBKToUnicode("产品列表")
    #define D_OrderTb_SystemType                    GBKToUnicode("系统定制")
//>@=========================设备表============================================
#define D_DeviceTb                              GBKToUnicode("设备列表")
#define D_MainBoardTb                           GBKToUnicode("主板列表")
#define D_InterfaceBoardTb                      GBKToUnicode("接口板列表")

    #define D_DeviceTb_MainBoardBarcode             GBKToUnicode("主板二维码")
    #define D_DeviceTb_InterfaceBoardBarcode        GBKToUnicode("接口板二维码")
//>@==========================日志表===========================================
#define D_DeviceLogTb                           GBKToUnicode("设备日志")
#define D_MainBoardLogTb                        GBKToUnicode("主板日志")
#define D_InterfaceBoardLogTb                   GBKToUnicode("接口板日志")
//>@==========================板表===========================================
#define D_MainBoardTypeTb                       GBKToUnicode("主板型号")
#define D_InterfaceBoardTypeTb                  GBKToUnicode("接口板型号")
    #define D_BoardType                             GBKToUnicode("型号")
    #define D_WholesaleType                         GBKToUnicode("批番号")
//>@==========================文件表===========================================
#define D_FileTypeTb                            D_FileType

#define D_MainBoardFileTb                       GBKToUnicode("主板文件")
#define D_InterfaceBoardFileTb                  GBKToUnicode("接口板文件")
    #define D_BoardFile_FileName              GBKToUnicode("名称")
    #define D_BoardFile_Version               GBKToUnicode("版本")
    #define D_BoardFile_HardwareType          GBKToUnicode("主板型号")
    #define D_BoardFile_SrcFile               GBKToUnicode("源文件")
    #define D_BoardFile_Author                GBKToUnicode("设计者")
//>@==========================主题===========================================
#define D_UITb                                  GBKToUnicode("主题")
#define D_UIRecordTb                            GBKToUnicode("主题记录")
//>@=====================================================================
#define D_NetDiskAdrTb                          GBKToUnicode("网盘地址")
    #define D_NetDiskAdrTb_Name                     GBKToUnicode("名称")
    #define D_NetDiskAdrTb_Adr                      GBKToUnicode("地址")
//>@==========================用户表==========================================
#define D_RegisteredUserTb                     GBKToUnicode("注册用户")
#define D_UserPermissionTb                     GBKToUnicode("注册权限列表")
    #define D_UserName                          GBKToUnicode("姓名")
    #define D_UserPermisson                     GBKToUnicode("权限")
    #define D_UserCode                          GBKToUnicode("编号")
    #define D_UserPasswd                        GBKToUnicode("密码")
    #define D_UserAddress                       GBKToUnicode("地址")
    #define D_UserPhone                         GBKToUnicode("联系方式")
//>@=====================================================================
//>@Key word
#define D_Tb_ID                     GBKToUTF8("ID")
#define D_Keyword_ID                GBKToUTF8("ID")
#define D_Keyword_FileName          GBKToUTF8("文件名")
#define D_Keyword_FileType          GBKToUTF8("文件名")

#define D_Keyword_SrcFile           GBKToUTF8("源文件")
#define D_Keyword_HardwareType      GBKToUTF8("所属型号")
#define D_Keyword_SaveDir           GBKToUTF8("归档路径")
#define D_Keyword_Time              GBKToUTF8("提交时间")

//>@Key word--FileType
#define D_SystemImage               GBKToUTF8("系统镜像")
#define D_App                       GBKToUTF8("应用程序")
#define D_BurnFile                  GBKToUTF8("烧录程序")
#define D_BurnDoc                   GBKToUTF8("烧录文档")
#define D_AssemblyDoc               GBKToUTF8("装配文档")
#define D_TestDoc                   GBKToUTF8("测试文档")
#define D_NetDisk0                  GBKToUTF8("测试文档")
#define D_NetDisk1                  GBKToUTF8("测试文档")
//>@//>@Key word--scanPoint
#define D_EnterOrderID              GBKToUTF8("请输入订单号")
#define D_ScanMainBoardID           GBKToUTF8("请扫描主板二维码")
#define D_ScanInterfaceBoardID      GBKToUTF8("请扫描接口板二维码")
#define D_EnterOrderID              GBKToUTF8("请输入订单号")
#define D_EnterOrderID              GBKToUTF8("请输入订单号")

class mssql : public SQLBasic
{
    Q_OBJECT
public:
    explicit mssql(QObject *parent = 0);
    ~mssql();

    bool init();

    //>@提交某一行，如果没有指定主键值（或者找不到主键值）则新增一行
    bool SubmitTbRow(QString tbName, QHash<QString, QString>fieldVauleHash, QString pkVaule = QString());
    bool DelTbRow(QString tbName, QString pkVaule);

    //>@返回所有表名
    QStringList GetAllTbs();
    //>@获取某个表中某个字段的长度
    int GetFieldLenth(QString tbName, QString fieldName);
    //>@获取所有主键
    QStringList GetTbPks(QString tbName);
    //>@获取第一个主键
    QString GetTbPk(QString tbName);
    //>@获取唯一键《第几组唯一键（1,2,3），唯一键字段》
    QMultiHash<QString, QString> GetTbUks0(QString tbName);
    //>@获取第一个唯一键
    QStringList GetTbUks1(QString tbName);
    //>@获取唯一键的值<主键值(1,2,3)，各个唯一键字段的值（第一个就是表中第二个字段的值）>  pNotes是否在得到的值前添加[字段名]
    QMultiHash<QString, QString> GetTbUksVaules(QString tbName, bool pNotes=true);
    //>@获取唯一键（如果没有返回第二列的）的字段名
    QString GetTbIkey(QString tbName);
    //>@获取所有字段
    QStringList GetTbFields(QString tbName);
    //>@获取非空字段
    QStringList GetTbUnNullFields(QString tbName);

    //>@获取主键列中的最大值
    QString GetTbMaxPkVaule(QString tbName);
    //>@获取所有父表（字段(外键)，父(外键)表名）
    QHash<QString, QString> GetTbParTb(QString tbName);
    //>@获取所有将自己的某个字段作为外键的表明()
    QHash<QString, QString> GetTbCldTbs(QString tbName);
    //>@
    QString GetTbCldTb(QString tbName, QString field);
    //>@获取外键表
    void GetTbAllCldTbs(QString tbName, QStringList &ret);
    //>@获取一张表查询到另一张表的路径（源表#外键#子表->源表#外键#子表->源表#外键#目标表）
    bool GetTbCldTbLinks(QString srcTb, QString dstTb, QStringList &ret);
    //>@获取主键和第一列的值（主键值，第一列值）
    QHash<QString, QString> GetTbPkIk(QString tbName);

    //>@查找某个表中的第一组唯一键的下层结构，（层数-表名:唯一键列表（,分割）#外键->外键对应的子表）
    void GetTbFieldsLinks(QString tbName, QStringList fieldList, QStringList &retList, int level=-1);
    //>@获取某个字段的最终叶子节点路径（层数-表名:唯一键列表）也即是上个函数的前面一部分
    QStringList GetTbFieldLinks(QString tbName, QString field);
    //>@获取某个字段的叶子组合值（外键对应的组合键中的字段名，字段值）
    QHash<QString, QString> GetTbFieldVaules(QString tbName, QString field);

    //>@
    QSqlQuery ScanTb0(QString tbName, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb1(QString tbName, QString pkVaule, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb2(QString tbName, QStringList srcFieldList, QStringList srcVauleList, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb3(QString tbName, QStringList srcFieldList, QList<QStringList> srcVauleslist, QStringList retFieldList, QString orderField = QString());

    //>@Advance fun

    //>@通过订单找到主板以及接口板的型号  返回值为ID，引用参数为ID对应的第二列值（ 第一个为主板的型号，第二个为接口板的型号）
    QStringList GetBoardTypes(QString pCodeID, QString &pMainBoardType, QString &pInterfaceBoardType);
    //>@通过二维码查找订单
    QString GetOrder(QString pCodeType, QString pCodeID);
    //>@通过板的型号查找对应的文件
    QSqlQuery GetMainBoardFiles(QString pBoardTypeLink);
    QSqlQuery GetInterfaceBoardFiles(QString pBoardTypeLink);

    //>@获取某张表中某一行对应的文件的网盘路径
    QString getNetDiskAdr(QString tbName, QString pkVaule);
    QString GetMainBoardFileDir0(QString fileType, QString hardwareType) {return QString();}
    QString GetMainBoardFileDir1(QString fileID) {return QString();}
    QString GetInterfaceFileDir0(QString fileType, QString hardwareType) {return QString();}
    QString GetInterfaceFileDir1(QString fileID) {return QString();}

    QStringList GetAllID(QString tbName);
    QString DeviceIDToOrderID(QString deviceID);
    QString MainBoardIDToOrderID(QString mainBoardID);
    QString InterfaceBoardIDToOrderID(QString interfaceBoardID);

    bool AddRecordToMainBoardTb(QString orderID, QString barcode);
    bool AddRecordToMainBoardLogTb(QString barcode, QString action);

    bool AddRecordToInterfaceBoardTb(QString orderID, QString barcode);
    bool AddRecordToInterfaceBoardLogTb(QString barcode, QString action);

    //>@添加一个产品记录
    bool AddRecordToDeviceTb(QString barcode, QString mainBoardbarcode, QString interfaceBoardbarcode);
    bool AddRecordToDeviceLogTb(QString barcode, QString action);
};

#endif // MSSQL_H
