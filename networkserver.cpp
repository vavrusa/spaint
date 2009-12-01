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

#include <QMap>
#include <QtNetwork>

#include "networkserver.h"
#include "networkserverthread.h"

struct NetworkServer::Private
{
public:
   Private()
   {}

   QMap<int, NetworkServerThread*> threads;
   QList<Canvas*> offeredCanvases;
};

NetworkServer::NetworkServer(QObject* parent)
      : QTcpServer(parent), d(new Private)
{
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

   QMap<int, NetworkServerThread*>::iterator it;
   for (it = d->threads.begin(); it != d->threads.end(); ++it) {
      it.value()->terminate();
      delete it.value();
      d->threads.erase(it);
   }

   emit(serverState(NetworkServer::STOP));
   return true;
}

void NetworkServer::incomingConnection(int sock)
{
   qDebug() << "server::incomingConnection(" << sock << ")";

   if (d->threads.contains(sock)) {
      qDebug() << "Server::error: Socket already in use.";
      return;
   }

   NetworkServerThread *thread = new NetworkServerThread(this, sock);
   d->threads[sock] = thread;
   connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

   thread->sendData(QString("Some ASCII data.."));
   thread->start();
}

bool NetworkServer::offerCanvas(Canvas* canvas)
{
   qDebug() << "NetworkServer::offerCanvas()";

   d->offeredCanvases.push_back(canvas);

   //foreach clients offerCanvas
   return true;
}

bool NetworkServer::disofferCanvas(Canvas* canvas)
{
   qDebug() << "NetworkServer::disofferCanvas()";

   //delete canvas in list

   //foreach clients dissofferCanvas();

   return true;
}


#include "networkserver.moc"
