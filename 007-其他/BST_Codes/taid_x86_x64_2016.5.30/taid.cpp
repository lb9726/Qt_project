#include "taid.h"
#include <QQmlContext>
#include "./util/uieditor.h"

Taid::Taid(QObject *parent) : QObject(parent)
{
    rootObject = NULL;
    mainQml = QUrl("qrc:/qml/main.qml");
}

Taid::Taid(QQmlApplicationEngine*eng) : QObject(eng)
{
    rootObject = NULL;
    engine = eng;
    mainQml = QUrl("qrc:/qml/main.qml");
}

Taid::~Taid(){
    if(m_UiEditor)
        delete m_UiEditor;
    if(rootObject)
        rootObject->deleteLater();
}

void Taid::Init(){
    m_UiEditor = new UiEditor();
    m_TempleteLoad =  new TempleteLoad(m_UiEditor);

    engine->rootContext()->setContextProperty("Templete",m_TempleteLoad);
    engine->rootContext()->setContextProperty("GTaid",this);
    engine->rootContext()->setContextProperty("GUiEditor",m_UiEditor);
    connect(engine,&QQmlApplicationEngine::objectCreated,this,&Taid::qmlObjectCreated);

    m_BstSendUpdate =  new BstSendUpdate();
    connect(m_BstSendUpdate,&BstSendUpdate::currentProgressNum,this,&Taid::slt_currentProgress);
    connect(m_BstSendUpdate,&BstSendUpdate::updateError,this,&Taid::slt_updateError);
    connect(m_BstSendUpdate,&BstSendUpdate::updateOk,this,&Taid::slt_updateOk);
    connect(m_BstSendUpdate,&BstSendUpdate::updateFinish,this,&Taid::slt_updateFinish);
    connect(m_BstSendUpdate,&BstSendUpdate::updateCurDevice,this,&Taid::slt_updateCurDevice);
    engine->rootContext()->setContextProperty("Publisher",m_BstSendUpdate);
}

void Taid::slt_currentProgress(QString terminalId, int progressNum)
{
    QVariant retVal;
    terminalId = terminalId;
    QMetaObject::invokeMethod(rootObject, "publishProgress",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, m_BstSendUpdate->m_curUpdateCount),
                              Q_ARG(QVariant, progressNum));
}

void Taid::slt_updateError(QString terminalId, QString err)
{
    QVariant retVal;
    terminalId = terminalId;
    QMetaObject::invokeMethod(rootObject, "publishError",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, err));
}

void Taid::slt_updateOk(QString terminalId)
{
    //qDebug()<<"ok@"<<terminalId;
    slt_updateFinish();
}

void Taid::qmlObjectCreated(QObject * object, const QUrl & url){
    if(object && mainQml == url){
        rootObject = object;
        m_UiEditor->setRootQmlObj(rootObject);
    }    
}

void Taid::slt_updateFinish(){
    QVariant retVal;
    qDebug()<<"slt_updateFinish";
    QMetaObject::invokeMethod(rootObject, "publishFinish",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, "finish"));
}

void Taid::slt_updateCurDevice(int deviceOrder){

}
