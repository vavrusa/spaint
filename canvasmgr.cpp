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

#include <QList>
#include "canvasmgr.h"

struct CanvasMgr::Private {
   QList<Canvas*> list;
};

CanvasMgr::CanvasMgr(QObject* parent)
      : QObject(parent), d(new Private)
{
}

CanvasMgr::~CanvasMgr()
{
   QList<Canvas*>::iterator it;
   for(it = d->list.begin(); it < d->list.end(); ++it)
      remove(*it);

   delete d;
}

void CanvasMgr::init()
{
   // Load main canvas
   create(tr("Main Canvas"));
   create("Test #1");
   create("Test #2");
}

Canvas* CanvasMgr::create(const QString& name)
{
   // TODO
   Canvas* c = new Canvas(name, this);

   // Emit signal
   emit canvasCreated(c);
   return c;
}

bool CanvasMgr::remove(Canvas* canvas)
{
   // Emit signal
   emit canvasRemoved(canvas);

   // TODO
   canvas->deleteLater();
   return true;
}


#include "canvasmgr.moc"
