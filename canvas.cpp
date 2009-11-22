#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPainter>
#include <QGradient>
#include <QDebug>
#include "canvas.h"
#include "canvasmgr.h"

Canvas::Canvas(const QString& name, CanvasMgr* parent)
      : QGraphicsScene(parent), mState(Idle), mGlyph(0), mName(name)
{
   setSceneRect(0, 0, 640, 480);
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
   if(e->button() == Qt::LeftButton) {
      mGlyph = addPath(QPainterPath(), QPen(Qt::black));
      mState = Drawing;
   }

   // Right mouse starts gesture
   if(e->button() == Qt::RightButton) {
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
   // End of drawing
   if(mState == Drawing) {
      qDebug() << "Polygon finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();
      emit(pathCreated(mGlyph->path()));
      mGlyph = 0;
      mState = Idle;
   }

   // End of gesture
   if(mState == Gesture) {
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
