#ifndef UIEDITOR_H
#define UIEDITOR_H
#include <QObject>
#include "./util/uiparser.h"
#include "./util/define.h"

//#include "compressclient.h"

class Taid;
class CompressClient;
class UiEditor : public QObject
{
    Q_OBJECT
public:
    explicit UiEditor(QObject *rootObject = 0);
    ~UiEditor();
    bool InitCom(QString pPath);
    bool AddCom(UI_TYPE pComType, QDomElement &pComElement);
    bool RemoveUI(UI_TYPE pComType);
    void ReleaseUIs();
    void init();
    void setRootQmlObj(QObject *rootObject);
signals:
    void sgn_loadui(QString file);
    void sgn_functionTest();


public slots:
    void orderResouce();
    void orderFunction();
    void orderArrow();
    void orderMedia();
    void orderPicture();
    void orderVideo();
    bool removeElement(QString comName);
    QStringList pullProperty(QString QString);
    void pushProperty(QString comName, QStringList list);
    QStringList pullDesktopProperty();
    QStringList pullFloorProperty();
    QStringList pullArrowProperty();
    QStringList pullFunctionProperty();
    QStringList pullMediaProperty();
    QStringList pullVideoProperty();
    QStringList pullTextProperty();
    QStringList pullTitleProperty();
    QStringList pullClockProperty();
    QStringList pullTimeProperty();
    QStringList pullDateProperty();
    QStringList pullFlrBtnProperty();
    QStringList pullCallBtnProperty();
    QStringList pullKeyBoardProperty();
    bool pushDesktopProperty(QStringList list);
    bool pushFloorProperty(QStringList list);
    bool pushArrowProperty(QStringList list);
    bool pushFunctionProperty(QStringList list);
    bool pushMediaProperty(QStringList list);
    bool pushTextProperty(QStringList list);
    bool pushVideoProperty(QStringList list);
    bool pushTitleProperty(QStringList list);
    bool pushClockProperty(QStringList list);
    bool pushTimeProperty(QStringList list);
    bool pushDateProperty(QStringList list);
    bool pushFlrBtnProperty(QStringList list);
    bool pushCallBtnProperty(QStringList list);
    bool pushKeyBoardProperty(QStringList list);

public slots:
    void archiveCurTheme();
    void resizeRect(QString elementType, QRect rect);
    void savePara();

    void functionTest();
    void slt_functionTest();

    bool loadUiFile(QString file);
    void selectedUiFile(QString file);
    void setCurDirection(bool direction);
    QSize getReslution(bool);
    QString getDeviceModel();
    QString getProjectDir();


    //completed
    void loadUICom();
    bool removeUIElement(QString comName);
    bool addNewCom(UI_TYPE comType);
    bool delCom(UI_TYPE comName);
    bool addRecPara(UI_TYPE uiType, DIRECTION direct, QRect rect);
    bool preperUIElement(UI_TYPE uiType, QString preperType);

    //QML use Slot.
    bool createUICom(QString comName, QRect rectH, QRect rectV);
    QStringList pullResource(QString comName);
    void pushResource(QString comName, QStringList list);
    bool pushCallBtnResource(QStringList list);

    QStringList scanKeyBoardImg(QString basePath);
    QStringList pullFlrBtnResource(int startFlr, int endFlr, QString pPath="");
    bool pushFlrBtnResource(QSize size, bool isHori, QStringList fileList);

    QStringList scanClockResource(bool, QString pPath = "");
    bool createDateArea(QRect rect, QStringList resList, QString dateStyle);
    bool createTimeArea(QRect rect, QStringList resList, bool withSecond);

    QStringList pullMediaResource(int mediaType);
    bool pushExeResource(QString exportPath, QString filePath);
    bool publishFile(QString exportPath);


public:
    QString mProjectDir;
    DIRECTION                               mDirection;
    UiParser                                *mUiManager;
    QHash<UI_TYPE, UiComParser*>            mUiList;
    int m_layout;
    void ResourceManage();
    QObject* rootQmlObject;
    QString qmlFilePrefix;
    CompressClient* compressClient;


    QStringList pullFloorResource();
    QStringList pullDesktopResource();
    QStringList pullArrowResource();
    QStringList pullMediaResource();
    QStringList pullTextResource();
    QStringList pullFunctionResource();
    QStringList pullCallBtnResource();
    QStringList pullKeyBoardResource();
    QStringList pullClockResource();
    QStringList pullFlrBtnResource();
    QStringList pullPictureResource();
    QStringList pullVideoResource();


    bool pushDesktopResource(QStringList);
    bool pushFloorResource(QStringList);
    bool pushArrowResource(QStringList);
    bool pushFunctionResource(QStringList);
    bool pushMediaResource(QStringList);
    bool pushPictureResource(QStringList);
    bool pushVideoResource(QStringList);
    bool pushTextResource(QStringList);
    bool pushKeyBoardResource(QStringList);
    bool pushClockResource(QStringList list);
    bool pushFlrBtnResource(QStringList list);

    void convertAndTransmit(QString src, QString dst, UI_TYPE uiType, bool aspectRatio = false);
    QSize scaleToHeight(QSize sSize, QSize dSize);
    QString shiftFilePrefix(QString filePath);
    void createSubCallBtn(int btnStatu, float spacing, QString press, QString release);
    void convertImageTo(QString src, QString dst, QSize size, bool aspectRatio=false);


    QString scanKeyImg(QString pPath, QString pDir);
    int scanKeyImg(QString pDir);

    void createFlrBtnKey(QDomElement subUi, int keyIndex, QString fImg, QString bImg, QRect rect);
    void createKeyBtnKey(QDomElement subUi, int keyIndex, QString fImg, QString bImg, QRect rect);
    QString scanClockImg(QString pPath, QString bName);

    bool qCopyDirectory(const QDir &fromDir, const QDir &toDir, bool bCoverIfFileExists = true);
    void clearElementRcList(UI_TYPE ui_type);
};

#endif // UIEDITOR_H
