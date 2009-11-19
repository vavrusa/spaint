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
   QPointF pt = e->scenePos();

   // Clip to scene rect
   if(!sceneRect().contains(pt)) {
      if(pt.x() < sceneRect().left())   pt.setX(sceneRect().left());
      if(pt.x() > sceneRect().right())  pt.setX(sceneRect().right());
      if(pt.y() < sceneRect().top())    pt.setY(sceneRect().top());
      if(pt.y() > sceneRect().bottom()) pt.setY(sceneRect().bottom());
   }

   static QPointF lastPos = pt;
   if((pt - lastPos).manhattanLength() > 40) {

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
      mState = Idle;
   }

   QGraphicsScene::mouseReleaseEvent(e);
}

void Canvas::drawBackground(QPainter* p, const QRectF& re)
{
   // Draw background
   QRectF sRect = sceneRect();
   p->fillRect(re, Qt::gray);

   // Draw shadow
   int borderWidth = 10;
   QLinearGradient grad;
   grad.setColorAt(0,   QColor(127,127,127));
   grad.setColorAt(0.3, QColor(148,148,148));
   grad.setColorAt(1,   Qt::gray);
   grad.setStart(sRect.topLeft());
   grad.setFinalStop(grad.start() - QPointF(0, borderWidth));

   QPointF aPt(borderWidth * 0.5, borderWidth * 0.5);
   QRectF aRect(grad.start() - aPt,
                QSizeF(aPt.x(), aPt.y()));
   QRectF bRect(grad.start() - QPointF(0, borderWidth),
                QPointF(sRect.width(), borderWidth));

   // Top
   p->fillRect(bRect, QBrush(grad));

   // Bottom
   grad.setStart(sRect.bottomLeft());
   grad.setFinalStop(sRect.bottomLeft() + QPointF(0, borderWidth));
   bRect.moveTo(sRect.bottomLeft());
   p->fillRect(bRect, QBrush(grad));

   // Left
   grad.setStart(sRect.topLeft());
   grad.setFinalStop(sRect.topLeft() - QPointF(borderWidth, 0));
   bRect = QRectF(grad.finalStop(), QSizeF(borderWidth, sRect.height()));
   p->fillRect(bRect, QBrush(grad));

   // Right
   grad.setStart(sRect.topRight());
   grad.setFinalStop(sRect.topRight() + QPointF(borderWidth, 0));
   bRect.moveTo(grad.start());
   p->fillRect(bRect, QBrush(grad));

   // Shadow corners
   grad.setStart(aRect.bottomRight());
   grad.setFinalStop(aRect.topLeft());
   p->fillRect(aRect, QBrush(grad));

   aRect.moveTo(sRect.topRight() + QPointF(0, -aPt.y()));
   grad.setStart(aRect.bottomLeft());
   grad.setFinalStop(aRect.topRight());
   p->fillRect(aRect, QBrush(grad));

   aRect.moveTo(sRect.bottomLeft() + QPointF(-aPt.x(), 0));
   grad.setStart(aRect.topRight());
   grad.setFinalStop(aRect.bottomLeft());
   p->fillRect(aRect, QBrush(grad));

   aRect.moveTo(sRect.bottomRight());
   grad.setStart(aRect.topLeft());
   grad.setFinalStop(aRect.bottomRight());
   p->fillRect(aRect, QBrush(grad));

   // Draw scene
   p->fillRect(sceneRect(), Qt::white);

   QGraphicsScene::drawBackground(p, re);
}

#include "canvas.moc"
