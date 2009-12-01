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

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QPainterPath>
#include <QDebug>
#include "canvas.h"
#include "canvasmgr.h"

Canvas::Canvas(const QString& name, CanvasMgr* parent)
      : QGraphicsScene(parent), mState(Idle), mGlyph(0), mName(name)
{
   QSize defaultSize(defaultSizeHint());
   setSceneRect(0, 0, defaultSize.width(), defaultSize.height());
}

QGraphicsView* Canvas::createView(QWidget* parent)
{
   QGraphicsView* view = new QGraphicsView(this, parent);

   // Disable frame and scrollbars
   view->setFrameStyle(QFrame::NoFrame);
   view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setBackgroundBrush(Qt::white);

   // TODO: Antialiasing is too CPU intensive, investigate
   //view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   view->setSceneRect(sceneRect());
   view->setCacheMode(QGraphicsView::CacheBackground);
   connect(this, SIGNAL(sceneRectChanged(QRectF)), view, SLOT(updateSceneRect(QRectF)));
   return view;
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
   // Don't process events when not painting
   if(mState == Idle || mGlyph == 0)
      return;

   QPointF pt = e->scenePos();

   // Clip to scene rect
   if(!sceneRect().contains(pt)) {
      if(pt.x() < sceneRect().left())   pt.setX(sceneRect().left());
      if(pt.x() > sceneRect().right())  pt.setX(sceneRect().right());
      if(pt.y() < sceneRect().top())    pt.setY(sceneRect().top());
      if(pt.y() > sceneRect().bottom()) pt.setY(sceneRect().bottom());
   }

   // Measure distance from last point
   // TODO: sample in time intervals
   int difference = (mState == Drawing) ? 10 : 60;
   static QPointF lastPos = pt;
   if((pt - lastPos).toPoint().manhattanLength() > difference) {

      QPainterPath path = mGlyph->path();
      if(path.isEmpty()) {
         path.moveTo(e->lastScenePos());
      }

      path.lineTo(pt);
      mGlyph->setPath(path);
      lastPos = pt;
   }

   // Default action
   QGraphicsScene::mouseMoveEvent(e);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   // Left mouse starts drawing
   if(mState == Idle && e->button() == Qt::LeftButton) {
      mGlyph = addPath(QPainterPath(), QPen(Qt::black));
      mState = Drawing;
   }

   // Right mouse starts gesture
   if(mState == Idle && e->button() == Qt::RightButton) {
      QPen pen;
      pen.setColor(QColor(215, 0, 25, 64)); // Transparent red
      pen.setWidth(8);
      mGlyph = addPath(QPainterPath(), pen);
      mState = Gesture;
   }

   QGraphicsScene::mousePressEvent(e);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
   // Check polygon
   if(mState != Idle && mGlyph->path().length() == 0)
      return;

   // End of drawing
   if(mState == Drawing && e->button() == Qt::LeftButton) {
      qDebug() << "Polygon finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();
      emit(pathCreated(mGlyph->path()));
      mGlyph = 0;
      mState = Idle;
   }

   // End of gesture
   if(mState == Gesture && e->button() == Qt::RightButton) {
      qDebug() << "Gesture finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();
      emit(gestureCreated(mGlyph->path()));
      removeItem(mGlyph);
      delete mGlyph;
      mGlyph = 0;
      mState = Idle;
   }

   QGraphicsScene::mouseReleaseEvent(e);
}


#include "canvas.moc"
