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
#ifndef TFTPCLIENT_H
#define TFTPCLIENT_H

#include "tftp.h"

#include <QObject>
#include <QUdpSocket>
#include "clientread.h"
#include "clientwrite.h"



/*!
    \brief TFTP Client implementation

    This class is used to creare a minimal TFTP client, to receive
    and send files.
    */
class Client : public QObject
{
        Q_OBJECT
    public:
    
        Client( const QHostAddress& localAddr,  uint16_t localPort, const QHostAddress& remoteAddr, uint16_t remotePort );
        //Client(const QHostAddress &addr, uint16_t port = defaultPort);
        ~Client();
        
        void putFile( const QString& localname, const QString& remotename, Mode trmode = Octet );
        void getFile( const QString& localname, const QString& remotename, Mode trmode = Octet );
        void sentTerminalId( QString _terminalId );
        void setOnline();
        bool getOnlineStatus();
        QString getTerminalId();
        QHostAddress getRemoteAddr();
        
    signals:
        /*!
            \brief Receiving file completed
            \param dev QIODevice instance where the file was wrote
            \param remotefilename Name of the file on the remote side
        
            This signal is emitted when the receiving of a file is completed
            */
        void receivedFile( QIODevice* dev, const QString remotefilename );
        
        /*!
            \brief Sending file completed
            \param dev QIODevice instance from which the file was read
            \param remotefilename Name of the file on the remote side
        
            This signal is emitted when the sending of a file is completed
            */
        void sentFile( QIODevice* dev, QString remotefilename );
        void updateOk( QString terminalId );
        void currentProgressNum( QString terminalId, int progressNum );
    protected slots:
        void dataReceived();
        
    protected:
        QList<ClientWriteSession*> writes;
        //!< List of all opened write instances
        QList<ClientReadSession*> reads;
        //!< List of all opened read instances
        
        ClientWriteSession* findWSession( const TransferInfo& ti );
        ClientReadSession* findRSession( const TransferInfo& ti );
        
        QUdpSocket sd;  //!< Socket device bound to the port
        
    private:
        QHostAddress m_localAddr;
        QHostAddress m_remoteAddr;
        uint16_t m_localPort;
        uint16_t m_remotePort;
        QString m_terminalId;
        bool  m_online;
        unsigned int m_onlineCount;
    private slots:
        void updateTeminalStatu();
        
        
};



#endif
