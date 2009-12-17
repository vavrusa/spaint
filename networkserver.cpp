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

   QList<int> clients;
   QList<Canvas*> canvases;
   QMultiMap<Canvas*, int> canvasClient;
   // TODO: Client doesn't have to be subscribed to canvas actions (offer state only)
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

void NetworkServer::incomingConnection(int sock)
{
   qDebug() << "server::incomingConnection(" << sock << ")";

   foreach (Canvas* canvas, d->canvases)
      offerCanvasToClient(sock, canvas);
}

void NetworkServer::cleanConnections()
{
   /*
   for (QList<int>::iterator it = d->clients.begin(); it != d->clients.end(); ++it)
      if ((*it)->state() == QAbstractSocket::UnconnectedState)
         d->clients.erase(it);
    */
}

bool NetworkServer::offerCanvasToClient(int sock, Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvasToClient()";

   NetworkService::CANVAS_stub* stub = new NetworkService::CANVAS_stub;
   stub->canvas = canvas;

   NetworkWorker* worker = new NetworkWorker(sock, NetworkService::CANVAS, stub);
   QThreadPool::globalInstance()->start(worker);

   // FIXME: Pernament subscribing - wait for "SUBSCRIBE" instead
   d->canvasClient.insert(canvas, sock);
}

bool NetworkServer::offerCanvas(Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvas()";

   d->canvases << canvas;

   // TODO: Subscribing to canvas data (offer), don't do it pernamently

   connect(canvas, SIGNAL(pathCreated(Canvas*,QPainterPath)),
           this, SLOT(sendCreatedPath(Canvas*,QPainterPath)));

   foreach (int client, d->clients)
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

bool NetworkServer::sendCreatedPath(Canvas* canvas, QPainterPath path)
{
   QMultiMap<Canvas*, int>::iterator it = d->canvasClient.find(canvas);
   while (it != d->canvasClient.end() && it.key() == canvas) {
      qDebug() << "NetworkServer::sendCreatedPath(" << canvas << ", client=" << it.value() << ")";

      NetworkService::CANVASPATH_stub* stub = new NetworkService::CANVASPATH_stub;
      stub->canvas = canvas;
      stub->path = &path;

      NetworkWorker* worker = new NetworkWorker(it.value(), NetworkService::CANVASPATH, stub);
      QThreadPool::globalInstance()->start(worker);
      ++it;
   }
}

#include "networkserver.moc"
