/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Marek Vavrusa    <xvavru00 AT stud.fit.vutbr.cz>           *
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

#include <QApplication>
#include <cstdlib>
#include "canvas.h"
#include "networkservice.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
   // Run QApplication   
   QApplication app(argc, argv);
   app.setOrganizationName("VUT FIT");
   app.setOrganizationDomain("fit.vutbr.cz");
   app.setApplicationName("SharedPaint");

   // Create canvas manager
   CanvasMgr cm;

   // Create application window
   MainWindow window;
   window.observe(&cm);
   window.show();

   // Create network service
   NetworkService net;
   window.observe(&net);
   net.observe(&cm);
   net.start();

   // Initialize canvases
   cm.init();

   // Run event-loop
   return app.exec();
}
