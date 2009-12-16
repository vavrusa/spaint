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

#ifndef NETWORKSERVERTHREAD_H
#define NETWORKSERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include "canvas.h"

class NetworkServerThread : public QThread
{
   Q_OBJECT

public:
   NetworkServerThread(QObject* parent, int sock);

   void run();

public slots:
   void sendData(QString data);
   void sendData(int data);
   void sendData(Canvas* canvas);
   //void sendData(QPainterPath path);
   void receiveData();
   void terminate();

private:
   QByteArray blockData;
   QTcpSocket* tcpSocket;
   int socketDescriptor;
};

#endif // NETWORKSERVERTHREAD_H
