/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Vojtech Vitek <vojtech.vitek AT gmail.com>                 *
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

#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QtNetwork>

#include "canvas.h"
#include "canvasmgr.h"
#include "networkserver.h"
#include "networkclient.h"

class NetworkService : public QObject
{
   Q_OBJECT

public:
   NetworkService(QObject *parent = 0);
   ~NetworkService();

   bool observe(CanvasMgr* cm);

public slots:
   bool startServer(QString addr = QString("127.0.0.1"), quint16 port = 6666);
   bool stopServer();
   bool startClient(QString address, quint16 port);
   bool stopClients();

private slots:
   bool offerCanvas(Canvas*);
   bool disofferCanvas(Canvas*);

public:
   NetworkServer* server;
   QList<NetworkClient*>* clients;

private:
   struct Private;
   Private* d;
};

#endif // NETWORKSERVICE_H
