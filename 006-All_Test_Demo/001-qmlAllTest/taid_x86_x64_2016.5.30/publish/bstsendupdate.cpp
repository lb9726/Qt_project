#include "bstsendupdate.h"
#include  <QFile>
#include <QTimer>
#include "../include/JlCompress.h"

BstSendUpdate::BstSendUpdate( QObject* parent )
    : QObject( parent )
{
    m_editor = (UiEditor*)parent;
    m_clientMap = new QMap<QString, Client*>();
    //绑定udp接受终端广播
    int port = 30021;
    m_terListSocket = new QUdpSocket( this );
    bool result = m_terListSocket->bind( port );
    
    if ( !result )
    {
        qDebug() << "terListSocket bind err";
        return ;
    }
    
    connect( m_terListSocket, SIGNAL( readyRead() ),this, SLOT( dataReceived() ) );

    //    connect(this,SIGNAL(currentProgressNum(QString,int)),this,SLOT(testCurrentProgressNum(QString,int)));
    //    connect(this,SIGNAL(updateError(QString,QString)),this,SLOT(testUpdateError(QString,QString)));
    //    connect(this,SIGNAL(updateOk(QString)),this,SLOT(testUpdateOk(QString)));
}
BstSendUpdate::~BstSendUpdate()
{
    if ( m_clientMap != NULL )
    {
        QMap<QString, Client*>::const_iterator i  = m_clientMap->constBegin();
        
        while ( i != m_clientMap->constEnd() )
        {
            delete i.value();
            i++;
        }
        
        delete m_clientMap;
    }
}

void BstSendUpdate::putUpdateFile( QString filePath, QString terminalId )
{
    QFile file( filePath );
    
    if ( !file.exists() )
    {
        emit updateError( terminalId, "file not exist" );
        return;
    }
    
    if ( !m_clientMap->contains( terminalId ) )
    {
        emit updateError( terminalId, "terminal not exist" );
        return;
    }
    
    if ( !( *m_clientMap )[terminalId]->getOnlineStatus() )
    {
        emit updateError( terminalId, "terminal not online" );
        return;
    }
    
    ( *m_clientMap )[terminalId]->putFile( filePath, "//update.zip" );
}

void BstSendUpdate::batchUpdate( QStringList deviceList)
{
    QString resourcePath = QCoreApplication::applicationDirPath();
    if(!resourcePath.endsWith("/"))
    {
        resourcePath.append("/");
    }
    resourcePath.append("taidResource.zip");
    qDebug()<<resourcePath;
    QFile file(resourcePath);

    if(!file.exists())
    {
        emit updateError("", "Can't get resource zip file.");
        return;
    }

    if(file.size() > 500*1024*1024)
    {
        emit updateError("", "Resource file can't large than 500MB.");
        return;
    }

    m_allUpdateCount = deviceList.count();
    for(int i = 0; i<deviceList.count(); ++i)
    {
        m_curUpdateCount = i;
        emit updateCurDevice(m_curUpdateCount);
        putUpdateFile(resourcePath,deviceList.at(i));
    }
    file.remove();
    //emit updateFinish();
}

QStringList BstSendUpdate::getTerminalList()
{
    QStringList terminaList;
    QMap<QString, Client*>::const_iterator i  = m_clientMap->constBegin();
    while ( i != m_clientMap->constEnd())
    {
        if ( i.value()->getOnlineStatus())
        {
            terminaList.push_back( i.key());
        }
        i++;
    }
    //qDebug()<<terminaList;
    return terminaList;
}

bool BstSendUpdate::zipResource()
{
    return false;
}

void BstSendUpdate::dataReceived()
{
    while ( m_terListSocket->hasPendingDatagrams())
    {
        QByteArray dgram;
        dgram.resize( m_terListSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        
        m_terListSocket->readDatagram( dgram.data(), dgram.size(),
                                       &sender, &senderPort );
        QString terminalid = dgram;
        //qDebug()<<"sender"<<sender.toString();
        //qDebug()<<"Heart Beat";
        if ( !m_clientMap->contains( terminalid ) )
        {
            Client* client = new Client( QHostAddress::LocalHost, 0, sender, 69 );
            client->sentTerminalId( terminalid );
            connect( client, SIGNAL( currentProgressNum( QString, int ) )
                     , this, SIGNAL( currentProgressNum( QString, int ) ) );
            connect( client, SIGNAL( updateOk( QString ) ), this, SIGNAL( updateOk( QString ) ) );
            m_clientMap->insert( terminalid, client );            
        }
        else
        {
            //终端ip地址改变
            if ( ( *m_clientMap )[terminalid]->getRemoteAddr() != sender)
            {
                delete ( *m_clientMap )[terminalid];
                m_clientMap->remove( terminalid );
                Client* client = new Client( QHostAddress::LocalHost, 0, sender, 69 );
                client->sentTerminalId( terminalid );
                connect( client, SIGNAL( currentProgressNum( QString, int ) )
                         , this, SIGNAL( currentProgressNum( QString, int ) ) );
                connect( client, SIGNAL( updateOk( QString ) ), this, SIGNAL( updateOk( QString ) ) );
                m_clientMap->insert( terminalid, client );
            }
            else
            {
                ( *m_clientMap )[terminalid]->setOnline();
            }
            
        }
    }
}

void BstSendUpdate::testCurrentProgressNum(QString terminalId,int progressNum)
{
    m_curprogress = progressNum;
}

void BstSendUpdate::testUpdateError(QString terminalId,QString error)
{
    //emit updateError("error");
}

void BstSendUpdate::testUpdateOk(QString terminalId)
{
    //qDebug() << "testUpdateOk== terminalId:"+terminalId;
}

void BstSendUpdate::testGetTerminalList()
{
    QStringList list= getTerminalList();
    for(int i = 0; i<list.count(); ++i)
    {
        //qDebug()<<"terminallist:"+QString::number(i)+" "+list[i];
        putUpdateFile("/home/huyuan/food.zip",list[i]);
        //putUpdateFile("/home/huyuan/food.zip",list[i]);

    }
    if(list.count() > 0)
    {
        testTimer->stop();
    }
}
