#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include "global.h"
#include <QVariant>
#include <QtPlugin>
#include <QAction>
#include <QWidget>

class MainForm;
class LogicThemeManager;
class EventMap;

typedef void(*EmitReport)(LogicThemeManager* pThemeManager, AREA_OPERATE pOperate, QVariant pPara);  //函数指针类型定义

class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    virtual QString Name() = 0;// 插件名称
    virtual QString RcPath() = 0;// 插件显示在主框架中的图标文件路径
    virtual QList<QAction*>* Actions() = 0;// 插件提供的对外操作接口集
    virtual MainForm* Widget() = 0;// 插件的主界面

    virtual void InitUI(EventMap *pEventMap, QString pRcPath = QString()) = 0;

    //>@设置回调函数,类参数，把Qt主程序里的函数指针和其参数传到该串口通讯类中
    virtual void SetReportCallback(LogicThemeManager *pThemeManager, EmitReport pFunc) = 0;

    virtual void ReportOperate(AREA_OPERATE pOperate, QVariant pPara) = 0;  //>@向主程序报告插件状态，调用此函数，即执行回调函数
    virtual void ExecOperate(AREA_OPERATE pOperate, QVariant pPara) = 0;   //>@主程序驱动插件实现动作

public:
    EmitReport              m_EmitReport;   //信号释放函数的指针, 用于指向回调函数
    LogicThemeManager      *m_ThemeManager;   //Qt中类实例的指针，指向Qt主程序中的类实例，作回调函数的实参，以便在Qt主程序中进行信号释放
};

Q_DECLARE_INTERFACE(PluginInterface, "PluginInterface");

#endif // BASEPLUGIN_H
