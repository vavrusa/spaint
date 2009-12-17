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
#include <QGraphicsColorizeEffect>
#include <QPainterPath>
#include <QCursor>
#include <QDebug>
#include "canvas.h"
#include "canvasmgr.h"

Canvas::Canvas(const QString& name, bool imported, CanvasMgr* parent)
      : QGraphicsScene(parent), mState(Idle), mGlyph(0), mName(name),
        mImported(imported), mTool(Pen), mHovered(0)
{
   QSize defaultSize(defaultSizeHint());
   setSceneRect(0, 0, defaultSize.width(), defaultSize.height());

   // Set default colors
   mPen = QPen(Qt::black);
   mBrush = QBrush(QColor(255, 255, 255, 0));
   mPen.setWidth(1);

   // Create effect
   mEffect = new QGraphicsColorizeEffect(this);
   mEffect->setColor(Qt::red);
   mEffect->setStrength(0.75);
}

QGraphicsView* Canvas::createView(QWidget* parent)
{
   QGraphicsView* view = new QGraphicsView(this, parent);

   // Disable frame and scrollbars
   view->setFrameStyle(QFrame::NoFrame);
   view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setBackgroundBrush(Qt::white);
   view->setMouseTracking(true);

   // TODO: Antialiasing is too CPU intensive, investigate
   //view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   view->setSceneRect(sceneRect());
   view->setCacheMode(QGraphicsView::CacheBackground);
   connect(this, SIGNAL(sceneRectChanged(QRectF)), view, SLOT(updateSceneRect(QRectF)));
   return view;
}

QList<QGraphicsPathItem*> Canvas::pathItems() const
{
   QList<QGraphicsPathItem*> list;
   QList<QGraphicsItem*> src(QGraphicsScene::items());
   foreach(QGraphicsItem* i, src) {
      QGraphicsPathItem* n = dynamic_cast<QGraphicsPathItem*>(i);
      if(n != 0)
         list.append(n);
   }

   return list;
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
   // Don't process events when not painting
   if(mState == Idle || mGlyph == 0) {

      // Hover
      mHovered = itemAt(e->scenePos());

      // Eraser
      if(mTool == Eraser) {
         if(mHovered != 0) {
            mEffect->setEnabled(true);
            mHovered->setGraphicsEffect(mEffect);
         }
         else
            mEffect->setEnabled(false);
         return;
      }

      // Transform
      if(mTool == Transform) {
         QGraphicsScene::mouseMoveEvent(e);
      }

      return;
   }

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
   int difference = (mState == Drawing) ? 5 : 50;
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
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   // Eraser
   if(mTool == Eraser && e->button() == Qt::LeftButton) {
      if(mHovered != 0 && mEffect->isEnabled()) {
         removeItem(mHovered);
      }
      return;
   }

   // Left mouse starts drawing
   if(mState == Idle && e->button() == Qt::LeftButton) {

      // Brush style
      if(mBrush.color().alpha() > 0)
         mBrush.setStyle(Qt::SolidPattern);
      else
         mBrush.setStyle(Qt::NoBrush);

      // Pen
      if(mTool == Pen) {
         mGlyph = addPath(QPainterPath(), mPen, mBrush);
         mState = Drawing;
      }
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
   if(mState == Idle)
      return;

   // End of drawing
   if(mState == Drawing && e->button() == Qt::LeftButton) {
      mGlyph->setFlag(QGraphicsItem::ItemIsMovable);
      qDebug() << "Polygon finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();

      if(!mGlyph->path().isEmpty() && !mImported)
         emit(pathCreated(this, mGlyph));

      mGlyph = 0;
      mState = Idle;
   }

   // End of gesture
   if(mState == Gesture && e->button() == Qt::RightButton) {
      qDebug() << "Gesture finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();
      QPainterPath path = mGlyph->path();
      removeItem(mGlyph);
      delete mGlyph;

      if(!path.isEmpty())
         emit(gestureCreated(path));
         mGlyph = 0;

      mGlyph = 0;
      mState = Idle;

      // Disable effects
      mEffect->setEnabled(false);
   }

   QGraphicsScene::mouseReleaseEvent(e);
}

#include "canvas.moc"
