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
#include <QList>

#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"
#include "networkclient.h"

struct NetworkService::Private
{
public:
   Private() : server(0)
   {}

   NetworkServer* server;
   QList<NetworkClient*>* clients;
};

NetworkService::NetworkService(QObject* parent)
      : d(new Private)
{
   d->server = new NetworkServer(parent);
   d->clients = new QList<NetworkClient*>;
   d->clients->clear();
}

NetworkService::~NetworkService()
{
   delete d->server;
}

bool NetworkService::observe(CanvasMgr* cm)
{
   connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(offerCanvas(Canvas*)));
   connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(disofferCanvas(Canvas*)));
   return true;
}

bool NetworkService::startClient(QString& host, QString& port)
{
   qDebug() << "startClient() #" << d->clients->count();
   NetworkClient* client = new NetworkClient(this);
   d->clients->append(client);

   return true;
}

bool NetworkService::stopClients()
{
   QList<NetworkClient*>::iterator it;
   for (it = d->clients->begin(); it < d->clients->end(); ++it)
      d->clients->removeOne(*it);
}

bool NetworkService::startServer()
{
   emit(serverState(NetworkServer::start));

   if (!d->server->listen(QHostAddress(QString("127.0.0.1")), 6666)) {
        d->server->close();
        emit(serverState(NetworkServer::errStart, d->server->errorString()));
        return false;
   }

   qDebug() << "Server listening..";

   emit(serverState(NetworkServer::run));
   return true;
}

bool NetworkService::stopServer()
{
   d->server->close();
   emit(serverState(NetworkServer::stop));
   return true;
}

bool NetworkService::offerCanvas(Canvas* canvas)
{
   qDebug() << "network::offerCanvas()";
   return true;
}

bool NetworkService::disofferCanvas(Canvas* canvas)
{
   qDebug() << "network::disofferCanvas()";
   return true;
}

#include "networkservice.moc"
