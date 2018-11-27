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
#include "readsession.h"
#include "utils.h"
#include "Winsock2.h"
//#include <netinet/in.h>
//#include <netinet/in.h>

#include <qfile.h>



ReadSession::~ReadSession()
{
    delete ba;
}

/*!
\brief Loads data from the file opened into the memory
\note This function allocates all the space needed by the file. This can be
large up to 32Mb. Please be sure you have enough memory to support this.
*/
void ReadSession::loadFile()
{
    ba = new QByteArray;
    
    switch ( currentMode )
    {
        case NetAscii:
            qWarning( "NetAscii transfers not supported" );
            break;
            
        case Octet:
//          uint32_t actual = m_currentFile->readBlock(ba->data(), ba->size());
//          if ( actual != ba->size() ) // Shouldn't be used
//              ba->resize(actual);
            ba->append( m_currentFile->readAll() );
            qDebug() << "filesize:" << ba->count();
			m_currentFile->close();
            break;
    }
}

/*!
\brief Sends the data to the peer
\return This function returns true if all the packets are already sent

The packet format is this:
<pre>
          2 bytes    2 bytes       n bytes
          ---------------------------------
   DATA  | 03    |   Block #  |    Data    |
          ---------------------------------
</pre>

*/
bool ReadSession::sendData()
{
    //qDebug() << "file.size" << ba->size();
    uint32_t offset = currentBlock * 8192;
    int32_t length = ba->size() - offset;
    
    if ( length < 0 )
    {
        return true;
    }
    
    length = length > 8192 ? 8192 : length;
    
    
    QByteArray dgram( length + 4 , 0x00 );
    
    wordOfArray( dgram )[0] = htons( ( uint16_t )DATA );
    wordOfArray( dgram )[1] = htons( currentBlock + 1 );
    
    memcpy( dgram.data() + 4, ba->data() + offset, length );
    //qDebug() << "senddata--" << dgram.count();
    
    if ( ti.sd->writeDatagram( dgram.data(), dgram.size(), ti.dAddr, ti.dPort ) == -1 )
    {
        qWarning( "Error sending data packet [%d]", ti.sd->error() );
    }
    
    return false;
}

/*!
\brief Parses an acknowledge packet and send the right data
\param dgram Datagram to parse
\return false if the ack packet is accepted, and no error is thrown, else true
\note The calling function should delete the object if parseAck() returned true
*/
bool ReadSession::parseAck( const QByteArray& dgram )
{
    if ( ntohs( wordOfArray( dgram )[0] ) != ACK )
    {
        qWarning( "Not an ACK packed passed to parseData()" );
        return true;
    }
    
    //qDebug() << QString::number( currentBlock );
    //qDebug() << QString::number( ntohs( wordOfArray( dgram )[1] ) - 1 );
    uint16_t acceptCode = ntohs( wordOfArray( dgram )[1] );
    
    if ( acceptCode >= 1 )
    {
        acceptCode -= 1;
    }
    
    if ( currentBlock != acceptCode )
    {
        qWarning( "Expected %d, got %d", currentBlock + 1, ntohs( wordOfArray( dgram )[1] ) );
        sendError( ti, IllegalOp, "Error in block sequence" );
        return true;
    }
    
    currentBlock = ntohs( wordOfArray( dgram )[1] );
    progNum = ( currentBlock * 8192 + 0.0 ) / ( ba->size() ) * 100;
    
    return sendData();
}


