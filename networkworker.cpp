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

#include <QRunnable>
#include <QTcpSocket>

#include "networkservice.h"
#include "networkworker.h"

struct NetworkWorker::Private
{
public:
   Private(int _socketDescriptor, NetworkService::DataType _dataType, void* _data)
      : socketDescriptor(_socketDescriptor), tcpSocket(0), dataType(_dataType), data(_data)
   {
   }

   int socketDescriptor;
   QTcpSocket* tcpSocket;
   NetworkService::DataType dataType;
   void* data;
};

NetworkWorker::NetworkWorker(int _socketDescriptor, NetworkService::DataType _dataType, void* _data)
      : QRunnable(), d(new Private(_socketDescriptor, _dataType, _data))
{
}

NetworkWorker::~NetworkWorker()
{
   delete d;
}

void NetworkWorker::run()
{
   qDebug() << "NetworkWorker::run()";

   qDebug() << "d->socketDescriptor=" << d->socketDescriptor;
   d->tcpSocket = new QTcpSocket();

   if (!d->tcpSocket->setSocketDescriptor(d->socketDescriptor)) {
      qDebug() << "NetworkWorker::run() d->tcpSocket error in thread";
      return;
   }

   QByteArray dataBlock;
   QDataStream out(&dataBlock, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_6);
   out << (quint32)0;

   out << static_cast<quint32>(d->dataType);

   switch(d->dataType) {
   case NetworkService::STRING:
      out << QString("String");
      break;
   case NetworkService::CANVAS:
   {
      Canvas* canvas = static_cast<Canvas*>(d->data);
      out << canvas->name();
      break;
   }
   default:
      qDebug() << "NetworkWorker::run() Unknown dataType in thread";
   }

   out.device()->seek(0);
   out << (quint32)(dataBlock.size() - sizeof(quint32));

   d->tcpSocket->write(dataBlock);
   d->tcpSocket->waitForBytesWritten();

   delete d->tcpSocket;
}

#include "networkworker.moc"
