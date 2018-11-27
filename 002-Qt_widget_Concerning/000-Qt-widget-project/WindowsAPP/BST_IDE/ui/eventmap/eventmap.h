#ifndef EVENTMAP_H
#define EVENTMAP_H

#include "global.h"

#define D_EVENT_SPLIT       QString("_")
#define D_ACTIONS_SPLIT     QString(";")

class LogicThemeManager;

class MAP_ITEM
{
public:
    MAP_ITEM() {mType = COM_INVALID;}
    MAP_ITEM(QString pString)
    {
        mType = COM_INVALID;
        QStringList tmpList = pString.split(D_EVENT_SPLIT);
        if(tmpList.count() == 2)
        {
            mType = getComponetType(tmpList.first());
            mValue = tmpList.last();
        }
    }
    bool operator==(const MAP_ITEM &s)
    {
        if(this->mType != s.mType)
            return false;
        if(this->mValue != s.mValue)
            return false;
        return true;
    }
    bool operator!=(const MAP_ITEM &s)  { return !operator==(s); }

    COM_TYPE   mType;
    QString    mValue;
};

class MapInfo
{
public:
    void AddAction(QString pString)
    {
        MAP_ITEM tmpMapItem = MAP_ITEM(pString);
        for(int i=0;i<mActions.count();i++)
        {
            if(tmpMapItem == mActions.at(i))
                return;
        }
        mActions.append(tmpMapItem);
    }
    MAP_ITEM GetAction(COM_TYPE pType)
    {
        for(int i=0;i<mActions.count();i++)
        {
            if(pType == mActions.at(i).mType)
                return mActions.at(i);
        }
        return MAP_ITEM();
    }
    MAP_ITEM            mEvent;
    QList<MAP_ITEM>     mActions;
} ;

class ActionInfo
{
public:
    ActionInfo();
    ActionInfo(QString pActions);

public:
    bool MapToAction(QString pActions);
    bool IsNull();

public:
    quint32         mDesktop;
    quint32         mArrow, mFloor, mFunction;
    quint32         mVideo, mAudio, mImage, mText;
    quint32         mLogo, mNamePlate, mWeather;

    QString         mExtraFloor;
};

/*
    <Floor_13>Floor_A.png<Floor_13> 表示映射为一张图片文件名不能含有_符号，图片存放在floor目录中。注意，此处与将主题中的floor设置为Intergeter还不一样，那种方式是指所有的楼层都是以集成方式显示
    <Floor_13>Floor_B12<Floor_13>
*/

class EventMap : public QObject
{
    Q_OBJECT
public:
    explicit EventMap(QObject *parent = 0);
    ~EventMap();

    bool Initialize(QString pPath);
    void Release();

    void SetEnable(bool pFlag) {mEnable = pFlag;}

    QList<MapInfo*> GetMapList(MAP_EVENT pTypeFilter);

    bool OpenMap(QString pPath);
    void CloseMap();
    bool SaveMap();

    bool AddMap(QString pEvent, QString pActions);
    bool DelMap(QString pEvent);

    void EmitActions(ActionInfo pAction);

    bool SetEvent(QDomElement &pElement, xmlParse *pParse);
    bool GetEvent(QDomElement &pElement, xmlParse *pParse);

    //>@-----------------------
    //>@ 31 ~28
    //>@  类型(16个)
    //>@-----------------------
    quint32 MapToEvent(MAP_EVENT pEventType, QString pTitle);

signals:
    void sDesktopAction(AREA_OPERATE, QVariant);
    void sArrowAction(AREA_OPERATE, QVariant);
    void sFloorAction(AREA_OPERATE, QVariant);
    void sFunctionAction(AREA_OPERATE, QVariant);
    void sMediaAction(AREA_OPERATE, QVariant);
    void sVideoAction(AREA_OPERATE, QVariant);
    void sAudioAction(AREA_OPERATE, QVariant);
    void sImageAction(AREA_OPERATE, QVariant);
    void sTextAction(AREA_OPERATE, QVariant);
    void sLogoAction(AREA_OPERATE, QVariant);
    void sWeatherAction(AREA_OPERATE, QVariant);
    void sNamePlateAction(AREA_OPERATE, QVariant);
    void sClockAction(AREA_OPERATE, QVariant);

    void sButtonAction(AREA_OPERATE, QVariant);

    //>@显示器反馈给电梯
    void sBtnCallEvent(QVariant pBtnList);  //>@楼层呼叫信号

protected:
    void timerEvent(QTimerEvent *event); //>@循环时间事件

public slots:
    void slot_AlarmEvent(QDateTime pTime);  //>@时钟事件
    void slot_ArrowEvent(quint32 pState);
    void slot_FloorEvent(quint32 pNum);
    void slot_FunctionEvent(quint32 pNum);
    void slot_AudioEvent(quint32 pNum);
    void slot_MediaEvent(quint32 pNum);

    void slot_ButtonEvent(QVariant pBtnList, BUTTONSTATE pState);

public:
    bool                        mEnable;
    mapManager                 *mMapManager;
    QHash<quint32, ActionInfo>  mMapHash;
#ifdef DISPLAY
    QPointer<LogicThemeManager> mThemeManager;
#endif
};

#endif // EVENTMAP
