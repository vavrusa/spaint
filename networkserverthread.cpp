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

#include <QThread>
#include <QTcpSocket>

#include "networkservice.h"
#include "networkserverthread.h"

NetworkServerThread::NetworkServerThread(QObject* parent, int sock)
      : QThread(parent), socketDescriptor(sock)
{
}

void NetworkServerThread::run()
{
   qDebug() << "serverThread::run()";

   tcpSocket = new QTcpSocket;

   if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
      qDebug() << "tcpSocket error in thread";
      //emit serverState(NetworkServer::ERR_RUN, tcpSocket->errorString());
   }

   tcpSocket->write(blockData);
   blockData.clear();

   connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(getData()));
   connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(terminate()));

   exec();

   tcpSocket->disconnectFromHost();

   qDebug() << "serverThread::run() exit()";
}

void NetworkServerThread::sendData(QString data)
{
   qDebug() << "serverThread::setData() " << data;
   QDataStream out(&blockData, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << (quint16)0;
   out << data;
   out.device()->seek(0);
   out << (quint16)(blockData.size() - sizeof(quint16));
}

void NetworkServerThread::sendData(int data)
{

}

void NetworkServerThread::sendData(QPainterPath path)
{

}

void NetworkServerThread::sendData(Canvas* canvas)
{
   qDebug() << "serverThread::setData() " << data;
   QDataStream out(&blockData, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << (quint16)0;
   out << data;
   out.device()->seek(0);
   out << (quint16)(blockData.size() - sizeof(quint16));
}

void NetworkServerThread::receiveData()
{
   qDebug() << "serverThread::receiveData()";
}

void NetworkServerThread::terminate()
{
   qDebug() << "serverThread::terminate() client_disconnected()";
}

#include "networkserverthread.moc"
