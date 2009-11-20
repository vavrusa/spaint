#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMap>
#include "canvascontainment.h"
#include "canvasview.h"
#include "canvas.h"

struct CanvasContainment::Private {

   Private()
      : current(0), view(0), state(CanvasContainment::Focused)
   {}

   Canvas*                   current;// Current canvas
   QGraphicsView*            view;   // Graphics scene
   CanvasContainment::State  state;  // State tracking
   QMap<Canvas*,QGraphicsProxyWidget*> map; // Map Canvas -> Widget
};

CanvasContainment::CanvasContainment(QWidget *parent)
   : QGraphicsScene(parent), d(new Private)
{
   d->view = new QGraphicsView(this, parent);
   d->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   d->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CanvasContainment::~CanvasContainment()
{
   delete d;
}

QGraphicsView* CanvasContainment::view()
{
   return d->view;
}

void CanvasContainment::addCanvas(Canvas* c)
{
   CanvasView* view = new CanvasView(c);
   QGraphicsProxyWidget* proxy = addWidget(view);
   setSceneRect(-320, -240, (d->map.size() + 1) * (640 + 320) + 320, 480);
   proxy->setPos(d->map.size() * (640 + 320), -240);
   d->view->centerOn(proxy);
   d->map.insert(c, proxy);
   d->current = c;
}

void CanvasContainment::removeCanvas(Canvas* c)
{
   // Fetch proxy
   QGraphicsProxyWidget* proxy = d->map.take(c);
   if(proxy == 0)
      return;

   // Remove from scene
   removeItem(proxy);

   // Delete widget
   CanvasView* view = qobject_cast<CanvasView*>(proxy->widget());
   view->deleteLater();
}

void CanvasContainment::wheelEvent(QGraphicsSceneWheelEvent* e)
{
   // TODO trigger animation -> shift
   if(d->state == Focused) {
      qDebug() << "Exposed";
      d->state = Exposed;
   }
   else {
      d->state = Focused;
   }
}

void CanvasContainment::drawBackground(QPainter* p, const QRectF& re)
{
   // Draw background
   p->fillRect(re, Qt::gray);

   // Draw shadow
   int borderWidth = 10;
   QLinearGradient grad;
   grad.setColorAt(0,   QColor(127,127,127));
   grad.setColorAt(0.3, QColor(148,148,148));
   grad.setColorAt(1,   Qt::gray);

   // Iterate visible canvases
   QList<QGraphicsItem*> list = items();
   QList<QGraphicsItem*>::iterator it;
   for(it = list.begin(); it < list.end(); ++it) {

      QRectF currentRect((*it)->sceneBoundingRect());
      grad.setStart(currentRect.topLeft());
      grad.setFinalStop(grad.start() - QPointF(0, borderWidth));

      QPointF aPt(borderWidth * 0.5, borderWidth * 0.5);
      QRectF aRect(grad.start() - aPt,
                   QSizeF(aPt.x(), aPt.y()));
      QRectF bRect(grad.start() - QPointF(0, borderWidth),
                   QSizeF(currentRect.width(), borderWidth));

      // Top
      p->fillRect(bRect, QBrush(grad));

      // Bottom
      grad.setStart(currentRect.bottomLeft());
      grad.setFinalStop(currentRect.bottomLeft() + QPointF(0, borderWidth));
      bRect.moveTo(currentRect.bottomLeft());
      p->fillRect(bRect, QBrush(grad));

      // Left
      grad.setStart(currentRect.topLeft());
      grad.setFinalStop(currentRect.topLeft() - QPointF(borderWidth, 0));
      bRect = QRectF(grad.finalStop(), QSizeF(borderWidth, currentRect.height()));
      p->fillRect(bRect, QBrush(grad));

      // Right
      grad.setStart(currentRect.topRight());
      grad.setFinalStop(currentRect.topRight() + QPointF(borderWidth, 0));
      bRect.moveTo(grad.start());
      p->fillRect(bRect, QBrush(grad));

      // Shadow corners
      grad.setStart(aRect.bottomRight());
      grad.setFinalStop(aRect.topLeft());
      p->fillRect(aRect, QBrush(grad));

      aRect.moveTo(currentRect.topRight() + QPointF(0, -aPt.y()));
      grad.setStart(aRect.bottomLeft());
      grad.setFinalStop(aRect.topRight());
      p->fillRect(aRect, QBrush(grad));

      aRect.moveTo(currentRect.bottomLeft() + QPointF(-aPt.x(), 0));
      grad.setStart(aRect.topRight());
      grad.setFinalStop(aRect.bottomLeft());
      p->fillRect(aRect, QBrush(grad));

      aRect.moveTo(currentRect.bottomRight());
      grad.setStart(aRect.topLeft());
      grad.setFinalStop(aRect.bottomRight());
      p->fillRect(aRect, QBrush(grad));
   }
}

#include "canvascontainment.moc"
