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

#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QTcpServer>
#include <QPainterPath>

#include "canvasmgr.h"
#include "canvas.h"

class NetworkServer : public QTcpServer
{
   Q_OBJECT

public:
   enum State {
      START,
      RUN,
      STOP,
      ERR_START,
      ERR_RUN,
      ERR_STOP
   };

   NetworkServer(QObject* parent);
   ~NetworkServer();

   bool observe(CanvasMgr* cm);
   bool start(quint16 port);
   bool stop();

signals:
   void serverState(NetworkServer::State, const QString& msg = QString());

private slots:
   bool offerCanvas(Canvas* canvas, bool local);
   bool disofferCanvas(Canvas* canvas);
   bool sendCreatedPath(Canvas* canvas, QGraphicsPathItem* path);
   void readData();
   void cleanConnections();
   void tst(QAbstractSocket::SocketState);

protected:
   void incomingConnection(int sock);

private:
   bool offerCanvasToClient(QTcpSocket* tcpSocket, Canvas* canvas);
   bool offerAllCanvasesToClient(QTcpSocket* tcpSocket);

   struct Private;
   Private* d;
};

#endif // NETWORKSERVER_H
