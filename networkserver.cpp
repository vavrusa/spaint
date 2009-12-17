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
#include <QThreadPool>
#include <QList>
#include <QMap>

#include "networkserver.h"
#include "networkworker.h"

struct NetworkServer::Private
{
public:
   Private()
   {}

   QList<QTcpSocket*> clients;
   QMap<QTcpSocket*,QMutex*> clientLock;
   QList<Canvas*> canvases;
   QMultiMap<Canvas*,QTcpSocket*> canvasClients;
};

NetworkServer::NetworkServer(QObject* parent)
      : QTcpServer(parent), d(new Private)
{
   //QThreadPool::globalInstance()->setMaxThreadCount(5);
}

NetworkServer::~NetworkServer()
{
   delete d;
}

bool NetworkServer::observe(CanvasMgr* cm)
{
   connect(cm, SIGNAL(canvasCreated(Canvas*,bool)), this, SLOT(offerCanvas(Canvas*,bool)));
   connect(cm, SIGNAL(canvasRemoved(Canvas*,bool)), this, SLOT(disofferCanvas(Canvas*,bool)));

   return true;
}

bool NetworkServer::start(quint16 port)
{
   emit serverState(NetworkServer::START);

   if (!this->listen(QHostAddress::Any, port)) {
        this->close();
        qDebug() << "Server error";
        emit serverState(NetworkServer::ERR_START, this->errorString());
        return false;
   }

   qDebug() << "Server listening on port " << port << "...";

   emit serverState(NetworkServer::RUN);
   return true;
}

bool NetworkServer::stop()
{
   this->close();

   // foreach threads -> wait||terminate()
   // foreach sockets -> disconnect()

   emit serverState(NetworkServer::STOP);
   return true;
}

void NetworkServer::incomingConnection(int socketDescriptor)
{
   qDebug() << "server::incomingConnection(" << socketDescriptor << ")";

   QTcpSocket* tcpSocket = new QTcpSocket();

   if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
      qDebug() << "NetworkServer::incomingConnection() tcpSocket error";
   }

   d->clients << tcpSocket;
   //QMutex* lock = new QMutex();
   //d->clientLock.insert(tcpSocket, lock);

   connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanConnections()));
   connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
   connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this, SLOT(tst(QAbstractSocket::SocketState)));

   foreach (Canvas* canvas, d->canvases)
      offerCanvasToClient(tcpSocket, canvas);
}

void NetworkServer::tst(QAbstractSocket::SocketState state)
{
   qDebug() << "Client QTcpSocket changed state." << state;
}

void NetworkServer::readData()
{
   qDebug() << "NetworkServer::readData()";

}

void NetworkServer::cleanConnections()
{
   for (QList<QTcpSocket*>::iterator it = d->clients.begin(); it != d->clients.end(); ++it)
      if ((*it)->state() == QAbstractSocket::UnconnectedState)
         d->clients.erase(it);
   //TODO: delete other stuff as mutexes etc.
}

bool NetworkServer::offerCanvasToClient(QTcpSocket* tcpSocket, Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvasToClient()";

   NetworkService::CANVAS_stub* stub = new NetworkService::CANVAS_stub;
   stub->canvas = canvas;

   NetworkWorker* worker = new NetworkWorker(tcpSocket, NetworkService::CANVAS, stub);
   QThreadPool::globalInstance()->start(worker);

   // FIXME: Pernament subscribing - wait for "SUBSCRIBE" instead
   d->canvasClients.insert(canvas, tcpSocket);
}

bool NetworkServer::offerCanvas(Canvas* canvas, bool locally)
{
   // if not locally created canvas, do not offer
   if (!locally)
      return false;

   qDebug() << "NetworkServer::offerCanvas()";

   d->canvases << canvas;

   // TODO: Subscribing to canvas data (offer), don't do it pernamently

   connect(canvas, SIGNAL(pathCreated(Canvas*,QGraphicsPathItem*)),
           this, SLOT(sendCreatedPath(Canvas*,QGraphicsPathItem*)));

   foreach (QTcpSocket* client, d->clients)
      offerCanvasToClient(client, canvas);

   return true;
}

bool NetworkServer::disofferCanvas(Canvas* canvas)
{
   qDebug() << "NetworkServer::disofferCanvas()";

   //delete canvas in list

   //foreach clients dissofferCanvas();

   return true;
}

bool NetworkServer::sendCreatedPath(Canvas* canvas, QGraphicsPathItem* path)
{
   QMultiMap<Canvas*, QTcpSocket*>::iterator it = d->canvasClients.find(canvas);
   while (it != d->canvasClients.end() && it.key() == canvas) {
      qDebug() << "NetworkServer::sendCreatedPath(" << canvas << ", client=" << it.value() << ")";

      NetworkService::CANVASPATH_stub* stub = new NetworkService::CANVASPATH_stub;
      stub->canvas = canvas;
      stub->item = path;
      //stub->path = new QPainterPath(path);

      NetworkWorker* worker = new NetworkWorker(it.value(), NetworkService::CANVASPATH, stub);
      QThreadPool::globalInstance()->start(worker);
      ++it;
   }
}

#include "networkserver.moc"
