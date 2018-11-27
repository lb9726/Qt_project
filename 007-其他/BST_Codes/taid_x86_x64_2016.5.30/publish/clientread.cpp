/***************************************************************************
 *   Copyright (C) 2004 by Flameeyes                                       *
 *   dgp85@users.sourceforge.net                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "clientread.h"
#include "utils.h"
#include "Winsock2.h"
//#include <netinet/in.h>

#include <qfile.h>



/*!
\brief Default constructor
\param T TranferInfo struct with peer data
\param local Local filename
\param remote Remote filename
\param trmode Transfer mode

\note This library support ONLY octet transfer mode
*/
ClientReadSession::ClientReadSession( TransferInfo T, const QString& local, const QString& remote, Mode trmode )
    : ReadSession( T )
{
    currentMode = trmode;
    currentBlock = 0;
    m_currentFile = new QFile( local );
    
    if ( ! m_currentFile->open( QIODevice::ReadOnly ) )
    {
        qDebug() << "can't open file";
        return;
    }
    
    sendRequest( remote );
}

/*!
\brief Alternative version of constructor
\param T TransferInfo struct with peer data
\param data a QIODevice instance (QFile or QBuffer) which contains the data to send
\param remote Remote filename to use
\param trmode Transfer mode

\note This library support ONLY octet transfer mode
*/
ClientReadSession::ClientReadSession( TransferInfo T, QIODevice* data, const QString& remote, Mode trmode )
    : ReadSession( T )
{
    currentMode = trmode;
    currentBlock = 0;
    m_currentFile = data;
    progNum=0;
    lastprogNum=0;
    sendRequest( remote );
}

/*!
\brief Actual read request function called by the two constructors
\param remote The filename to use on the remote side

\note This function is called by the two constructor, to avoid code redundancy

This function send a WRQ to the server to initiate transfer. The WRQ format
is this:

<pre>
          2 bytes    string   1 byte     string   1 byte
          -----------------------------------------------
   WRQ   |  02   |  Filename  |   0  |    Mode    |   0  |
          -----------------------------------------------
</pre>
*/
void ClientReadSession::sendRequest( const QString& remote )
{
    loadFile();
    
    QString modestring;
    
    switch ( currentMode )
    {
        case Octet:
            modestring = "octet";
            break;
            
        case NetAscii:
            modestring = "netascii";
            break;
            
        default:
            qFatal( "OERROR! Invalid mode in transfer operation!" );
    }
    
    QByteArray dgram( 2 + remote.length() + 1 + modestring.length() + 1 , 0x00 );
    wordOfArray( dgram )[0] = htons( ( uint16_t )WRQ );
    
    memcpy( dgram.data() + 2, remote.toLatin1(), remote.length() + 1 );
    memcpy( dgram.data() + 2 + remote.length() + 1, modestring.toLatin1(), modestring.length() + 1 );
    qDebug() << dgram.toHex();
    qDebug() << ti.dPort;
    qDebug() << ti.dAddr.toString();

    if ( ti.sd->writeDatagram( dgram.data(), dgram.size(), ti.dAddr, ti.dPort ) == -1 )
    {
        qWarning( "Error sending WRQ packet [%d]", ti.sd->error() );
        qDebug()<< "Error sending WRQ packet [%d]"+ti.sd->errorString();
    }
}

ClientReadSession::~ClientReadSession()
{
}


