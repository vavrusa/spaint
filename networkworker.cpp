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

#include <QVariant>
#include <QRunnable>
#include <QTcpSocket>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QMutex>

#include "networkservice.h"
#include "networkworker.h"

class NetworkWorker::Private : public QObject
{
public:
   Private(QTcpSocket* _tcpSocket, NetworkService::DataType _dataType, void* _data)
      : tcpSocket(_tcpSocket), dataType(_dataType), data(_data)
   {
   }

   QThread* parentThread;
   QTcpSocket* tcpSocket;
   NetworkService::DataType dataType;
   void* data;
   QMutex mutex;
};

NetworkWorker::NetworkWorker(QTcpSocket* _tcpSocket, NetworkService::DataType _dataType, void* _data)
      : QObject(), QRunnable(), d(new Private(_tcpSocket, _dataType, _data))
{
   d->parentThread = thread();
 //  d->mutex.lock();
}

NetworkWorker::~NetworkWorker()
{
 //  d->mutex.unlock();
   delete d;
}

void NetworkWorker::run()
{
   moveToThread(thread());

   qDebug() << "NetworkWorker::run()";

   QByteArray dataBlock;
   QDataStream out(&dataBlock, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_6);
   out << (quint32)0;

   out << static_cast<quint32>(d->dataType);

   switch(d->dataType) {
   case NetworkService::CANVAS:
   {
      qDebug() << "NetworkWorker::run() sending Canvas";
      NetworkService::CANVAS_stub* stub = static_cast<NetworkService::CANVAS_stub*>(d->data);
      out << stub->canvas->name();
      foreach(QGraphicsPathItem* item, stub->canvas->pathItems()) {
         out << item->path();
         out << item->pen();
         out << item->brush();
      }
      break;
   }
   case NetworkService::CANVASPATH:
   {
      qDebug() << "NetworkWorker::run() sending Canvas Path";
      NetworkService::CANVASPATH_stub* stub = static_cast<NetworkService::CANVASPATH_stub*>(d->data);
      out << stub->canvas->name();
      out << stub->item->path();
      out << stub->item->pen();
      out << stub->item->brush();
      break;
   }
   default:
      qDebug() << "NetworkWorker::run() Unknown dataType in thread";
   }

   out.device()->seek(0);
   out << (quint32)(dataBlock.size() - sizeof(quint32));
   qDebug() << "Bytes to send:" << (quint32)(dataBlock.size() - sizeof(quint32));

   d->tcpSocket->write(dataBlock);
   d->tcpSocket->waitForBytesWritten();

   moveToThread(d->parentThread);
}

#include "networkworker.moc"
