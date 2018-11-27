/***************************************************************************
 *   Copyright (C) 2004 by Flameeyes                                       *
 *   dgp85@users.sourceforge.net                                           *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "client.h"
#include "clientread.h"
#include "clientwrite.h"
#include "utils.h"
#include "Winsock2.h"
#include <qsocketnotifier.h>

//#include <netinet/in.h>
#include <QByteArray>
#include <QTimer>

/*!
\brief Finds the write instance which has the provided TransferInfo data
\param ti TransferInfo data to find the write instance of
\return A pointer to the write session, or NULL if not foundable
*/
ClientWriteSession* Client::findWSession( const TransferInfo& ti )
{
    for ( QList<ClientWriteSession*>::iterator it = writes.begin(); it != writes.end(); it++ )
        if ( ( *it )->transferInfo() == ti )
        {
            return ( *it );
        }
        
    return NULL;
}

/*!
\brief Finds the read instance which has the provided TransferInfo data
\param ti TransferInfo data to find the read instance of
\return A pointer to the read session, or NULL if not foundable
*/
ClientReadSession* Client::findRSession( const TransferInfo& ti )
{
    for ( QList<ClientReadSession*>::iterator it = reads.begin(); it != reads.end(); it++ )
        if ( ( *it )->transferInfo() == ti )
        {
            return ( *it );
        }
        
    return NULL;
}

/*!
\brief Default constructor
\param addr Address of the server to connect to
\param port Port of the server to connect to (default 69)

\note You should create one client per server you want to connect to
*/
Client::Client( const QHostAddress& localAddr,  uint16_t localPort, const QHostAddress& remoteAddr, uint16_t remotePort )
    : QObject(),
      m_localAddr( localAddr ),
      m_remoteAddr( remoteAddr ),
      m_localPort( localPort ),
      m_remotePort( remotePort ),
      m_online( true ),
      m_onlineCount( 0 )
{
    //sd.setBlocking(false);
    bool ok=sd.bind( m_localPort );
    if(!ok){
      qDebug()<<"bind err";
      return;
    }

    //sn = new QSocketNotifier(sd.socket(), QSocketNotifier::Read, this);
    QObject::connect( &sd, SIGNAL( readyRead() ), this, SLOT( dataReceived() ) );
    QTimer* onTimer = new QTimer( this );
    connect( onTimer, SIGNAL( timeout() ), this, SLOT( updateTeminalStatu() ) );
    onTimer->start(1000);
}

Client::~Client()
{
}

//! Slot called when data is received from the socket
void Client::dataReceived()
{
    while ( sd.hasPendingDatagrams() )
    {
        QByteArray dgram;
        dgram.resize( sd.pendingDatagramSize() );
        QHostAddress sender;
        quint16 senderPort;
        
        sd.readDatagram( dgram.data(), dgram.size(),
                         &sender, &senderPort );
                         
        //processTheDatagram(datagram);
        //qDebug() << "accept--" << dgram.toHex();
        TransferInfo ti( &sd, sender, senderPort );
        
        switch ( ntohs( wordOfArray( dgram )[0] ) )
        {
            case ACK:
            {
                ClientReadSession* rs = findRSession( ti );
                
                if ( !rs )
                {
                    sendError( ti, IllegalOp, "ACK packet when no file is being uploaded" );
                    return;
                }
                
                if ( rs->parseAck( dgram ) )
                {
                    emit sentFile( rs->currentFile(), rs->currentFilename() );
                    emit updateOk( m_terminalId );
                    reads.removeOne( rs );
                    delete rs;
                }

                 //qDebug()<<rs->progNum;
                 //qDebug()<<rs->lastprogNum;
                if(rs->lastprogNum>100) rs->lastprogNum=0;
                if(rs->progNum>rs->lastprogNum)
                {
                   emit currentProgressNum( m_terminalId, rs->progNum );
                   rs->lastprogNum=rs->progNum;
                }
            }
            break;
            
            case DATA:
            {
                ClientWriteSession* ws = findWSession( ti );
                
                if ( !ws )
                {
                    sendError( ti, IllegalOp, "DATA packet when no file is being downloaded" );
                    return;
                }
                
                if ( ws->parseData( dgram ) )
                {
                    emit receivedFile( ws->currentFile(), ws->currentFilename() );
                    writes.removeOne( ws );
                    delete ws;
                }
            }
            break;
            
            case OERROR:
            {
                Session* s;
                
                if ( ( s = findWSession( ti ) ) )
                {
                    writes.removeOne( reinterpret_cast<ClientWriteSession*>( s ) );
                }
                else if ( ( s = findRSession( ti ) ) )
                {
                    reads.removeOne( reinterpret_cast<ClientReadSession*>( s ) );
                }
                else
                {
                    qWarning( "Error received without a session opened for the peer" );
                }
                
                if ( s )
                    qWarning(
                        "Error packet received, peer session aborted\n%s [%d]",
                        dgram.data() + 4,
                        ntohs( wordOfArray( dgram )[1] )
                    );
            }
            break;
            
            default:
                sendError( ti, IllegalOp, "Illegal TFTP opcode" );
                return;
        }
    }
    
//  QByteArray dgram( sd.bytesAvailable() );
//  sd.readBlock( dgram.data(), dgram.size() );


}

/*!
\brief Initiate an upload transfer to the server
\param localname Name of the file in the local filesystem
\param remotename Name of the file on the remote side

\note For client, the concept of Read and Write are inverted: to a ReadSession
    on one side, the other side is always using a WriteSesssion!
*/
void Client::putFile( const QString& localname, const QString& remotename, Mode trmode )
{
    if(reads.count()>0) return;
    qDebug()<<"@@@@:"<<reads.count();
    reads.push_back( new ClientReadSession( TransferInfo( &sd, m_remoteAddr, 69 ), localname, remotename, trmode ) );
    qDebug()<<"---------------------------";
}

/*!
\brief Initiate a download transfer to the server
\param localname Name of the file in the local filesystem
\param remotename Name of the file on the remote side

\note For client, the concept of Read and Write are inverted: to a ReadSession
    on one side, the other side is always using a WriteSesssion!
*/
void Client::getFile( const QString& localname, const QString& remotename, Mode trmode )
{
    writes.push_back( new ClientWriteSession( TransferInfo( &sd ), localname, remotename, trmode ) );
}


void Client::updateTeminalStatu()
{
    m_onlineCount++;
    
    if ( m_onlineCount > 10 )
    {
        m_online = false;
    }
}

void Client::setOnline()
{
    m_onlineCount = 0;
    m_online = true;
}

QString Client::getTerminalId()
{
    return m_terminalId;
}
QHostAddress Client::getRemoteAddr()
{
    return m_remoteAddr;
}

void Client::sentTerminalId( QString _terminalId )
{
    m_terminalId = _terminalId;
}
bool Client::getOnlineStatus()
{
    return m_online;
}
