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

#include <QtNetwork>

#include "networkserver.h"
#include "networkserverthread.h"

NetworkServer::NetworkServer(QObject* parent)
      : QTcpServer(parent)
{
}

bool NetworkServer::start(QString& addr, quint16 port)
{
   emit(serverState(NetworkServer::START));

   if (!this->listen(QHostAddress(addr), port)) {
        this->close();
        qDebug() << "Server error";
        emit(serverState(NetworkServer::ERR_START, this->errorString()));
        return false;
   }

   qDebug() << "Server listening..";

   emit(serverState(NetworkServer::RUN));
   return true;
}

bool NetworkServer::stop()
{
   this->close();
   emit(serverState(NetworkServer::STOP));
   return true;
}

void NetworkServer::incomingConnection(int sock)
{
   qDebug() << "server::incomingConnection()";
   NetworkServerThread *thread = new NetworkServerThread(this, sock);
   connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
   thread->start();
}

#include "networkserver.moc"
