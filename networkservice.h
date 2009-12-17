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
#include <QPainterPath>

#include "canvas.h"
#include "canvasmgr.h"
#include "networkserver.h"
#include "networkclient.h"

const QString default_addr("127.0.0.1");
const quint16 default_port(6666);

class NetworkService : public QObject
{
   Q_OBJECT

public:

   enum DataType {
      CANVAS,
      CANVASPATH,
      REMOVECANVAS,
      REMOVECANVASPATH,
      UNKNOWN
   };

   struct CANVAS_stub {
      Canvas* canvas;
   };

   struct CANVASPATH_stub {
      Canvas* canvas;
      QGraphicsPathItem* item;
   };

   NetworkService(QObject *parent = 0);
   ~NetworkService();

   bool observe(CanvasMgr* cm);

public slots:
   bool startServer(quint16 port = default_port);
   bool stopServer();
   bool startClient(QString addr = default_addr, quint16 port = default_port);
   bool stopClients();

public:
   NetworkServer* server;
   QList<NetworkClient*>* clients;

private:
   struct Private;
   Private* d;
};

#endif // NETWORKSERVICE_H
