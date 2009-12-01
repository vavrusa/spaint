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

#include <QList>

#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"
#include "networkclient.h"

struct NetworkService::Private
{
public:
   Private()
   {}

   QList<Canvas*> offeredCanvases;
};

NetworkService::NetworkService(QObject* parent)
      : d(new Private)
{
   server = new NetworkServer(parent);
   clients = new QList<NetworkClient*>;
   clients->clear();
}

NetworkService::~NetworkService()
{
   stopServer();
   stopClients();

   while(!clients->empty()) {
      delete clients->last();
      clients->removeLast();
   }
   delete clients;
   delete server;
}

bool NetworkService::observe(CanvasMgr* cm)
{
   server->observe(cm);
   return true;
}

bool NetworkService::startServer(quint16 port)
{
   qDebug() << "NetworkService::startServer()";
   server->start(port);

   return true;
}

bool NetworkService::startClient(QString host, quint16 port)
{
   qDebug() << "NetworkService::startClient(" << host << ", " << port << ") #" << clients->count();
   NetworkClient* client = new NetworkClient(this);
   clients->push_back(client);
   client->start(host, port);

   return true;
}

bool NetworkService::stopServer()
{
   server->stop();
   return true;
}

bool NetworkService::stopClients()
{
   QList<NetworkClient*>::iterator it;
   for (it = clients->begin(); it < clients->end(); ++it)
      (*it)->stop();
   return true;
}

#include "networkservice.moc"
