#include "appclient.h"
#include "define.h"

#define D_STX  ((char)0x80)
#define D_ETX  ((char)0x81)
#define D_ESC  ((char)0x1B)

#define D_STX_ESC  ((char)0xE7)
#define D_STX_ETX  ((char)0xE8)
#define D_ESC_ESC  ((char)0x00)


AppClient::AppClient(QObject *parent) : QObject(parent)
{
    m_FrameBuffer.clear();
    m_HBCount = 0;
    m_tcpPort = 10001;
    isConnected  = false;
}

void AppClient::startWork(){
    m_HBTimeoutTimer = new QTimer(this);
    m_HBTimeoutTimer->setInterval(5000);//心跳周期
    connect(m_HBTimeoutTimer,SIGNAL(timeout()),this,SLOT(hbTimerOut()));
    initTcpServer(m_tcpPort);
}

void AppClient::initTcpServer(int port)
{
    ///IDE_TRACE();
    m_pTcpServer = new QTcpServer();
    if(!m_pTcpServer->listen(QHostAddress::Any,port)){
        IDE_TRACE_STR("Test TCP Server Listen Error!");
    }
    connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
    m_pTcpSocket = NULL;
}

void AppClient::newConnectSlot()
{
    //IDE_TRACE();
    if(isConnected){
        return;
    }
    if(m_pTcpSocket){
        m_pTcpSocket->close();
    }
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(readyMessage()));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
    isConnected = true;
    //IDE_TRACE();
}

void AppClient::clientDisconnected(){
    IDE_TRACE();
    isConnected = false;
    m_pTcpSocket = NULL;
}

void AppClient::readyMessage()
{
    QByteArray ba = m_pTcpSocket->readAll();
    ParseBuffer(ba);
}

quint32 AppClient::ParseBuffer(QByteArray& buffer)
{
    if(buffer.isEmpty())
    {
        buffer.clear();
        return 0;
    }
    quint32 tmpCount = buffer.count();
    char tmpValue = 0;
    char tmpValueNext = 0;
    quint32 i = 0;

    while(i < tmpCount)
    {
        tmpValue = buffer.at(i++);
        if(D_STX == tmpValue){
            m_FrameBuffer.clear();
        }else if(D_ETX == tmpValue){
            ParseFrame();
        }else{
            if(tmpValue != D_ESC){
                m_FrameBuffer.append(tmpValue);
            }else{
                tmpValueNext = buffer.at(i++);
                if(tmpValueNext == D_STX_ESC)
                    m_FrameBuffer.append(D_STX);
                else if(tmpValueNext == D_STX_ETX)
                    m_FrameBuffer.append(D_ETX);
                else if(tmpValueNext == D_ESC_ESC)
                    m_FrameBuffer.append(D_ESC);
            }
        }

    }
    buffer.clear();
    return 1;
}

quint32 AppClient::ParseFrame()
{
    if(m_FrameBuffer.count() <= 0)
    {
        m_FrameBuffer.clear();
        IDE_TRACE();
        return -1;
    }
    m_FrameBuffer.remove(m_FrameBuffer.count()-1,1);
//    qDebug()<< m_FrameBuffer;
    bool ok = false;
    QVariantMap cmdObj = JsonParser.parse(m_FrameBuffer,&ok).toMap();
    if(!ok){
        IDE_TRACE();
        return -2;
    }
    QString type  = cmdObj["type"].toString();
    ///IDE_TRACE_STR(type);
    if(type.isEmpty()){
        return -3;
    }

    if(!QString::compare(type,"login",Qt::CaseInsensitive))
    {
        if(cmdObj.contains("info"))
        {
            QVariantMap map = cmdObj["info"].toMap();
            login(map);
        }
    }
    else if(!QString::compare(type,"heartbeat",Qt::CaseInsensitive))
    {
        heatBeatFrame();
    }
    else if(!QString::compare(type,"getallparameter",Qt::CaseInsensitive))
    {
        QVariantMap map = cmdObj["info"].toMap();
        getallparameter(map);
    }
    else if(!QString::compare(type,"setparameter",Qt::CaseInsensitive))
    {
        if(cmdObj.contains("info"))
        {
            QVariantMap para = cmdObj["info"].toMap();
            setparameter(para);
        }
    }
    else if(!QString::compare(type,"appupdateFinished",Qt::CaseInsensitive))
    {
        if(cmdObj.contains("info"))
        {
            QVariantMap para = cmdObj["info"].toMap();
            appupdateFinished(para);
        }
    }else {

    }
    return 0;
}

quint32 AppClient::login(QVariantMap &map){
    if(map.contains("ssid")&&map.contains("password")){        
        QString ssid = map["ssid"].toString();
        QString password = map["password"].toString();
        IDE_TRACE_STR(ssid);
        IDE_TRACE_STR(password);
        emit sigLogin(ssid,password);        
    }
    return -1;
}

void AppClient::sltloginResult(bool res){
    QVariantMap loginObj;
    loginObj.insert("type", "login");
    QVariantMap info;
    if(res){
        info.insert("result", true);
        //m_HBTimeoutTimer->start();

    }else{
        info.insert("result", false);
    }
    loginObj.insert("info", info);
    ///qDebug()<<"loginObj: "<<loginObj;
    sendJson(loginObj);
    emit  sigAppConnected();
}

quint32 AppClient::getallparameter(QVariantMap &map){
    emit sigSyncParametere();
    return -1;
}

quint32 AppClient::setparameter(QVariantMap &map){
    QMapIterator<QString,QVariant> it(map);
    while (it.hasNext()) {
        it.next();
        emit sigParameterSetting(it.key(),it.value());
    }
    return -1;
}

quint32 AppClient::appupdateFinished(QVariantMap &map){
    emit sigUpdateFinished();
    return -1;
}

quint32 AppClient::heatBeatFrame(){
    sendHB();
    //m_HBTimeoutTimer->start();
    return 0;
}

void AppClient::hbTimerOut(){
    m_pTcpSocket->close();
    emit sigAppDisconnected();

}

void AppClient::sendHB(){
    //QByteArray json;
    bool ok;
    QVariantMap hb;
    hb.insert("type", "heartbeat");
    QByteArray json = JsonSerializer.serialize(hb, &ok);
    if (ok) {
        sendFrame(json);
    }
}

void AppClient::sendJson(QVariantMap &jsonMap){
    bool ok;
    QByteArray json = JsonSerializer.serialize(jsonMap, &ok);
    if (ok) {
        //qDebug()<<"sendJson : "<<json;
        sendFrame(json);
    } else {
        IDE_TRACE();
    }
}

void AppClient::sendFrame(QByteArray &ba){
    QByteArray buffer;
    quint32 tmpCount = ba.count();
    char tmpValue = 0;
    quint32 i = 0;
    char crc= 0x00;
    buffer.append(D_STX);
    while(i < tmpCount)
    {
        tmpValue = ba.at(i++);
        crc ^= tmpValue;
        if(D_STX == tmpValue){
            buffer.append(D_ESC);
            buffer.append(D_STX_ESC);
        }else if(D_ETX == tmpValue){
            buffer.append(D_ESC);
            buffer.append(D_STX_ETX);
        }else if(D_ESC == tmpValue){
            buffer.append(D_ESC);
            buffer.append(D_ESC_ESC);
        } else{
            buffer.append(tmpValue);
        }
    }
    crc &= 0x7F;
    if(D_STX == crc){
        buffer.append(D_ESC);
        buffer.append(D_STX_ESC);
    }else if(D_ETX == crc){
        buffer.append(D_ESC);
        buffer.append(D_STX_ETX);
    }else if(D_ESC == crc){
        buffer.append(D_ESC);
        buffer.append(D_ESC_ESC);
    } else{
        buffer.append(crc);
    }
    buffer.append(D_ETX);
    sendRawMessage(buffer);
}

void AppClient::sendRawMessage(QByteArray &data)
{
    if(isConnected && m_pTcpSocket){
        m_pTcpSocket->write(data);
    }else{
        IDE_TRACE();
    }
}
