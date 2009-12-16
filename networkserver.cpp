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

#include "networkserver.h"
#include "networkworker.h"

struct NetworkServer::Private
{
public:
   Private()
   {}

   QList<QTcpSocket*> clients;
   QList<Canvas*> canvases;
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
   connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(offerCanvas(Canvas*)));
   connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(disofferCanvas(Canvas*)));

   return true;
}

bool NetworkServer::start(quint16 port)
{
   emit(serverState(NetworkServer::START));

   if (!this->listen(QHostAddress::Any, port)) {
        this->close();
        qDebug() << "Server error";
        emit(serverState(NetworkServer::ERR_START, this->errorString()));
        return false;
   }

   qDebug() << "Server listening on port " << port << "...";

   emit(serverState(NetworkServer::RUN));
   return true;
}

bool NetworkServer::stop()
{
   this->close();

   // foreach threads -> wait||terminate()
   // foreach sockets -> disconnect()

   emit(serverState(NetworkServer::STOP));
   return true;
}

void NetworkServer::incomingConnection(int sock)
{
   qDebug() << "server::incomingConnection(" << sock << ")";

   QTcpSocket* tcpSocket = new QTcpSocket(this);

   if (!tcpSocket->setSocketDescriptor(sock)) {
      qDebug() << "tcpSocket error in thread";
      //emit serverState(NetworkServer::ERR_RUN, tcpSocket->errorString());
   }

   d->clients << tcpSocket;

   connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanConnections()));
   connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));

   foreach (Canvas* canvas, d->canvases)
      offerCanvasToClient(tcpSocket, canvas);
}

void NetworkServer::cleanConnections()
{
   for (QList<QTcpSocket*>::iterator it = d->clients.begin(); it != d->clients.end(); ++it)
      if ((*it)->state() == QAbstractSocket::UnconnectedState)
         d->clients.erase(it);
}

bool NetworkServer::offerCanvasToClient(QTcpSocket* client, Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvasToClient()";

    NetworkWorker* worker = new NetworkWorker(client, NetworkService::STRING, canvas);// static_cast<void*>(&canvas));
    QThreadPool::globalInstance()->start(worker);

}

bool NetworkServer::offerCanvas(Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvas()";

   d->canvases << canvas;

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

//void receiveData()

#include "networkserver.moc"
