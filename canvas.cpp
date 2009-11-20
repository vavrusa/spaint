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
   if(!mGlyph)
      return;

   QPointF pt = e->scenePos();

   // Clip to scene rect
   if(!sceneRect().contains(pt)) {
      if(pt.x() < sceneRect().left())   pt.setX(sceneRect().left());
      if(pt.x() > sceneRect().right())  pt.setX(sceneRect().right());
      if(pt.y() < sceneRect().top())    pt.setY(sceneRect().top());
      if(pt.y() > sceneRect().bottom()) pt.setY(sceneRect().bottom());
   }

   static QPointF lastPos = pt;
   if((pt - lastPos).toPoint().manhattanLength() > 10) {

      QPainterPath path = mGlyph->path();
      if(path.isEmpty()) {
         path.moveTo(e->buttonDownScenePos(Qt::LeftButton));
      }

      path.lineTo(pt);
      mGlyph->setPath(path);
      lastPos = pt;
   }

   QGraphicsScene::mouseMoveEvent(e);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   if(e->button() == Qt::LeftButton) {
      mGlyph = addPath(QPainterPath(), QPen(Qt::black));
      mState = Drawing;
   }

   QGraphicsScene::mousePressEvent(e);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
   if(e->button() == Qt::LeftButton) {
      qDebug() << "Polygon finished - pts:" << mGlyph->path().elementCount()
               << "~length: " << mGlyph->path().length();
      mGlyph = 0;
      mState = Idle;
   }

   QGraphicsScene::mouseReleaseEvent(e);
}


#include "canvas.moc"
