/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Vojtech Vitek (V-Teq) <vojtech.vitek AT gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QMessageBox>
#include <QtNetwork>

#include "networkservice.h"
#include "networkserverthread.h"

NetworkServerThread::NetworkServerThread(QObject* parent, int sock)
      : QThread(parent), sock(sock)
{
}

void NetworkServerThread::run()
{
   qDebug() << "serverThread::run()";
   QTcpSocket tcpSocket;

   if (!tcpSocket.setSocketDescriptor(sock)) {
     emit error(tcpSocket.error());
     return;
   }

   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << (quint16)0;
   out << QString("Some text..");
   out.device()->seek(0);
   out << (quint16)(block.size() - sizeof(quint16));

   tcpSocket.write(block);
   tcpSocket.disconnectFromHost();
   tcpSocket.waitForDisconnected();
}

#include "networkserverthread.moc"
