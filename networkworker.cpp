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

#include <QRunnable>
#include <QTcpSocket>

#include "networkservice.h"
#include "networkworker.h"

NetworkWorker::NetworkWorker(QTcpSocket* _tcpSocket, NetworkService::DataType _dataType, void* _data)
      : tcpSocket(_tcpSocket), dataType(_dataType), dataBlock()
{
   QDataStream out(&dataBlock, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_0);
   out << (quint16)0;
   
   switch(dataType) {
   case NetworkService::STRING:
      out << QString("Ahoj :)");
   }

   out.device()->seek(0);
   out << (quint16)(dataBlock.size() - sizeof(quint16));

}

void NetworkWorker::run()
{
   qDebug() << "NetworkWorker::run()";

   tcpSocket->write(dataBlock);
}

#include "networkworker.moc"
