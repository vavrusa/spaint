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

#include <QDebug>
#include <QList>
#include "canvas.h"
#include "canvasmgr.h"

struct CanvasMgr::Private {
   QList<Canvas*> local;
   QList<Canvas*> imported;
};

CanvasMgr::CanvasMgr(QObject* parent)
      : QObject(parent), d(new Private)
{
}

CanvasMgr::~CanvasMgr()
{
   QList<Canvas*>::iterator it;
   for(it = d->local.begin(); it < d->local.end(); ++it)
      remove(*it);
   for(it = d->imported.begin(); it < d->imported.end(); ++it)
      remove(*it);

   delete d;
}

void CanvasMgr::init()
{
   // Load main canvas
   create(tr("Main Canvas"));
   //create(tr("Canvas #2"));
}

Canvas* CanvasMgr::create(const QString& name, bool locally)
{
   /* Find and eventually return the canvas of the same name */
   if (locally) {
      foreach(Canvas* canvas, d->local)
         if (canvas->name() == name)
            return canvas;
   } else {
      foreach(Canvas* canvas, d->imported)
         if (canvas->name() == name)
            return canvas;
   }

   Canvas* c = new Canvas(name, this);

   if (locally)
      d->local << c;
   else
      d->imported << c;

   emit canvasCreated(c, locally);

   return c;
}

bool CanvasMgr::remove(Canvas* canvas, bool locally)
{
   // Emit signal
   emit canvasRemoved(canvas, locally);

   // TODO: remove from imported and local list
   canvas->deleteLater();
   return true;
}

void CanvasMgr::importPath(const QString& name, QPainterPath path, QPen pen, QBrush brush)
{
   qDebug() << "CanvasMgr::importPath(" << name << ", path)";
   foreach (Canvas* canvas, d->imported) {
      if (canvas->name() == name) {
         canvas->importPath(path, pen, brush);
         qDebug() << "CanvasMgr::importPath() .. found Canvas";
         break;
      }
   }
}

#include "canvasmgr.moc"
