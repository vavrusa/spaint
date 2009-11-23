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

#include <QMessageBox>
#include <QtNetwork>

#include "canvasmgr.h"
#include "networkservice.h"

/*
struct NetworkService::Private{

   Private() : tst(0)
   {}

   int tst;
};
*/

NetworkService::NetworkService(QWidget *parent)
      : QDialog(parent), server(new NetworkServer(this))/*, d(new Private)*/
{
}

bool NetworkService::observe(CanvasMgr* cm)
{
   connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(offerCanvas()));
   connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(dissofferCanvas()));
   return true;
}

bool NetworkService::start()
{
   if (!server->listen()) {
        QMessageBox::critical(this, tr("Shared Paint Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        server->close();
        return false;
   }

   emit(serverStarted());
   return true;
}

bool NetworkService::stop()
{
   /*server->close();*/
   emit(serverStopped());
   return true;
}

bool NetworkService::offerCanvas(Canvas* canvas)
{
   return true;
}

bool NetworkService::disofferCanvas(Canvas* canvas)
{
   return true;
}

NetworkService::~NetworkService()
{
   delete server;
}

#include "networkservice.moc"
