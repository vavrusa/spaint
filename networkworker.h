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

#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H

#include <QRunnable>
#include <QTcpSocket>

#include "networkservice.h"

class NetworkWorker : public QObject, public QRunnable
{
   Q_OBJECT

public:
   NetworkWorker(QTcpSocket* tcpSocket, NetworkService::DataType dataType, void* data);
   ~NetworkWorker();

   void run();

signals:
   void error(QAbstractSocket::SocketError);

private:
   class Private;
   Private* d;
};

#endif // NETWORKWORKER_H
