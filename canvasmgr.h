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

#ifndef CANVASMGR_H
#define CANVASMGR_H

#include <QString>
#include <QPainterPath>
#include "canvas.h"

class CanvasMgr : public QObject
{
   Q_OBJECT

   public:
   CanvasMgr(QObject* parent = 0);
   ~CanvasMgr();

   public slots:
   void init();
   Canvas* create(const QString& name = QString(), bool locally = true);
   void importPath(const QString& name, QPainterPath path);
   bool remove(Canvas* canvas, bool locally = true);

   signals:
   void canvasCreated(Canvas* canvas, bool locally = true);
   void canvasRemoved(Canvas* canvas, bool locally = true);

   private:
   struct Private;
   Private* d;

};

#endif // CANVASMGR_H
