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

#ifndef CANVASCONTAINMENT_H
#define CANVASCONTAINMENT_H

#include <QGraphicsScene>
#include <QPalette>
class QGraphicsSceneWheelEvent;
class QGraphicsView;
class QPainter;
class Canvas;

class CanvasContainment : public QGraphicsScene
{
   Q_OBJECT

   public:
   CanvasContainment(QWidget *parent = 0);
   ~CanvasContainment();

   QGraphicsView* view();

   signals:

   public slots:
   void gesture(int code);
   void addCanvas(Canvas* c);
   void removeCanvas(Canvas* c = 0);
   void focusToCanvas(Canvas* c);
   void clearCanvas(Canvas* c = 0);
   void renderCanvas(QIODevice& device, Canvas* c = 0);

   protected:
   void drawBackground(QPainter* p, const QRectF& re);
   void wheelEvent(QGraphicsSceneWheelEvent* e);

   private slots:
   void updateLayout(const QRectF& newRect);
   void changeColor(QPalette::ColorRole role, const QColor& color);
   void changeThickness(int num);

   private:
   struct Private;
   Private* d;

};

#endif
