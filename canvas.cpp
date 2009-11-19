#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QDebug>
#include "canvas.h"
#include "canvasmgr.h"

Canvas::Canvas(const QString& name, CanvasMgr* parent)
      : QGraphicsScene(parent), mState(Idle), mGlyph(0), mName(name)
{
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
   static QPointF lastPos = e->scenePos();
   if((e->scenePos() - lastPos).manhattanLength() > 40) {

      QPainterPath path = mGlyph->path();
      if(path.isEmpty()) {
         path.moveTo(e->buttonDownScenePos(Qt::LeftButton));
      }

      path.lineTo(e->scenePos());
      mGlyph->setPath(path);
      lastPos = e->scenePos();
   }

   QGraphicsScene::mouseMoveEvent(e);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   if(e->button() == Qt::LeftButton) {
      qDebug() << "Drawing started: " << e->scenePos();
      mGlyph = addPath(QPainterPath(), QPen(Qt::black));
      mState = Drawing;
   }

   QGraphicsScene::mousePressEvent(e);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
   if(e->button() == Qt::LeftButton) {
      qDebug() << "Drawing finished: " << e->scenePos();
      mState = Idle;
   }

   QGraphicsScene::mouseReleaseEvent(e);
}

#include "canvas.moc"
