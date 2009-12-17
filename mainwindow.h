/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Marek Vantuch    <xvantu00 AT stud.fit.vutbr.cz>           *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "canvasmgr.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
   MainWindow(QWidget* parent = 0);
   ~MainWindow();

   bool observe(CanvasMgr* cm);

   protected:
   void saveSettings();
   void loadSettings();
   void closeEvent(QCloseEvent *event);

   private slots:
   void about();
   void gesture(int code);
   void renderCanvas();

   private:
   struct Private;
   Private* d;

   QMenuBar* createMenuBar();
};

#endif // MAINWINDOW_H
