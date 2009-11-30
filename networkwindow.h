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

#ifndef NETWORKWINDOW_H
#define NETWORKWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"

class NetworkWindow : public QMainWindow
{
   Q_OBJECT

public:
   NetworkWindow(QWidget* parent = 0);
   ~NetworkWindow();

   bool observe(NetworkService* net);

protected:
   void saveSettings();
   void loadSettings();
   void closeEvent(QCloseEvent* event);

private slots:
   void showServerState(NetworkServer::state state, const QString& msg = QString());

private:
   struct Private;
   Private* d;
};

class NetworkNewClientConnection : public QWidget
{
   Q_OBJECT

public:
   NetworkNewClientConnection(QWidget* parent = 0);
   ~NetworkNewClientConnection();

signals:
   void submitForm(QString& host, QString& port);
   void cancelForm();

private slots:
   void okClicked();
   void cancelClicked();

private:
   struct Private;
   Private* d;
};

#endif // NETWORKWINDOW_H
