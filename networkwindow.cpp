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

#include <QGraphicsView>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include <QtNetwork>
#include "networkwindow.h"
#include "networkservice.h"
#include "networkserver.h"

// Const
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 300

struct NetworkWindow::Private
{
public:
   Private()
   {}

   NetworknewClient* newClient;
};

NetworkWindow::NetworkWindow(QWidget* parent)
      : QMainWindow(parent), d(new Private)
{
   // Load window defaults
   setWindowTitle(tr("Shared paint - network"));
   setWindowIcon(QIcon(":/icons/spaint.png"));
   resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

   d->newClient = new NetworknewClient(this);
   setCentralWidget(d->newClient);

   // Load settings
   loadSettings();
}

NetworkWindow::~NetworkWindow()
{
   delete d->newClient;
   delete d;
}

bool NetworkWindow::observe(NetworkService* net)
{
   connect(d->newClient, SIGNAL(submitForm(QString,quint16)),
           net, SLOT(startClient(QString,quint16)));

   // TODO: switch to new client view
   connect(d->newClient, SIGNAL(submitForm(QString,quint16)),
           this, SLOT(hide()));

   // TODO: switch to default view
   connect(d->newClient, SIGNAL(cancelForm()),
           this, SLOT(hide()));

   return true;
}

void NetworkWindow::showServerState(NetworkServer::state state, const QString &msg)
{
   switch (state) {
      case NetworkServer::ERR_START:
         QMessageBox::critical(this, tr("Shared Paint Server"),
                               tr("Unable to start the server: %1.")
                               .arg(msg));
         break;

      case NetworkServer::START:
         // starting
         break;

      case NetworkServer::RUN:
         // running
         break;

      case NetworkServer::STOP:
         // stopped
         break;

      default:
         QMessageBox::information(this, tr("Shared Paint Server"),
                                  tr("NOT IMPLEMENTED: Unknown state"));
   }
}

void NetworkWindow::closeEvent(QCloseEvent *event)
{
   // Save settings
   saveSettings();

   // TODO
   event->accept();
}

void NetworkWindow::saveSettings()
{
   QSettings set;

   // Window settings
   set.setValue("NetworkWindow/State",     saveState());
   set.setValue("NetworkWindow/Size",      size());
   set.setValue("NetworkWindow/Position",  pos());
   set.setValue("NetworkWindow/Maximized", windowState().testFlag(Qt::WindowMaximized));
}

void NetworkWindow::loadSettings()
{
   QSettings set;

   // Window settings
   if(set.contains("NetworkWindow/State"))
   {
      resize(set.value("NetworkWindow/Size").toSize());
      move(set.value("NetworkWindow/Position").toPoint());

      if(set.value("NetworkWindow/Maximized").toBool())
         setWindowState(windowState() | Qt::WindowMaximized);

      restoreState(set.value("NetworkWindow/State").toByteArray());
   }
   else
   {
      // Defaults
      resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   }
}

struct NetworknewClient::Private : public QWidget
{
public:
   Private()
   {
   }

   QString host;
   QLabel* hostLabel;
   QLineEdit* hostEdit;

   quint16 port;
   QLabel* portLabel;
   QLineEdit* portEdit;

   QPushButton* ok;
   QPushButton* cancel;

   QGridLayout* layout;
};

NetworknewClient::NetworknewClient(QWidget* parent)
      : QWidget(parent), d(new Private)
{
   d->hostLabel = new QLabel(QString(tr("IP address:")), this);
   d->hostEdit = new QLineEdit("", this);
   d->hostLabel->setBuddy(d->hostEdit);
   d->hostLabel->setFocus();

   d->portLabel = new QLabel(QString(tr("Port:")), this);
   d->portEdit = new QLineEdit("", this);
   d->portLabel->setBuddy(d->portEdit);

   d->ok = new QPushButton(tr("Ok"), this);
   d->cancel = new QPushButton(tr("Cancel"), this);

   d->layout = new QGridLayout(this);
   d->layout->addWidget(d->hostLabel, 0, 0);
   d->layout->addWidget(d->hostEdit, 0, 1);
   d->layout->addWidget(d->portLabel, 1, 0);
   d->layout->addWidget(d->portEdit, 1, 1);
   d->layout->addWidget(d->ok, 2, 0);
   d->layout->addWidget(d->cancel, 2, 1);

   connect(d->ok, SIGNAL(clicked()), this, SLOT(okClicked()));
   connect(d->cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

   this->setLayout(d->layout);
}

NetworknewClient::~NetworknewClient()
{
   delete d;
}

void NetworknewClient::okClicked()
{
   // VALIDATE FORM
   emit submitForm(d->hostEdit->text(), d->portEdit->text().toInt());
}

void NetworknewClient::cancelClicked()
{
   emit cancelForm();
}

#include "networkwindow.moc"
