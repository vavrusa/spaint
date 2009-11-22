#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMap>
#include "canvascontainment.h"
#include "canvasview.h"
#include "canvas.h"

// Static settings
const static int BorderWidth = 10;

struct CanvasContainment::Private {

   Private() : view(0)
   {}

   QGraphicsView*            view;          // Graphics scene
   QList<Canvas*>            list;          // Ordered list
   QList<Canvas*>::iterator  current;       // Focused widget
   QMap<Canvas*,QGraphicsProxyWidget*> map; // Map Canvas -> Widget
};

CanvasContainment::CanvasContainment(QWidget *parent)
   : QGraphicsScene(parent), d(new Private)
{
   QSize defaultSize = Canvas::defaultSizeHint();
   d->view = new QGraphicsView(this, parent);
   d->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   d->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   d->view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
   d->view->setSceneRect(0, - defaultSize.height() * 0.5,
                         defaultSize.width(), defaultSize.height());
   setBackgroundBrush(Qt::gray);
   d->current = d->list.end();
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
   QSize defaultSize = Canvas::defaultSizeHint();
   QGraphicsProxyWidget* proxy = addWidget(view);
   setSceneRect(-defaultSize.width(), -defaultSize.height() * 0.5 - BorderWidth,
                (d->map.size() + 1) * (defaultSize.width() * 1.5) + defaultSize.width() * 0.5,
                defaultSize.height() + BorderWidth);
   proxy->setPos(d->map.size() * defaultSize.width() * 1.5, -defaultSize.height() * 0.5);
   d->view->setSceneRect(proxy->sceneBoundingRect());
   d->list.append(c);
   d->current = d->list.end() - 1;
   d->map.insert(c, proxy);
   invalidate(sceneRect());
   qDebug() << "New canvas: " << (*d->current)->name();
}

void CanvasContainment::removeCanvas(Canvas* c)
{
   // Fetch proxy
   QGraphicsProxyWidget* proxy = d->map.take(c);
   if(proxy == 0)
      return;

   // Remove from scene
   d->current--;
   d->list.erase(d->current + 1);
   focusToCanvas(*d->current);

   removeItem(proxy);

   // Delete widget
   CanvasView* view = qobject_cast<CanvasView*>(proxy->widget());
   view->deleteLater();
}

void CanvasContainment::renderCanvas(QIODevice& device, Canvas* c)
{
   // Available with kinetic
   qDebug() << "CanvasContainment::renderCanvas() called";

   // No canvas given, use current
   if(c == 0) {
      c = *d->current;
   }

   // Render
   QGraphicsProxyWidget* proxy = d->map[c];
   QWidget* widget = proxy->widget();
   QPixmap pixmap(widget->size());
   widget->render(&pixmap);
   pixmap.save(&device, "PNG");
}

void CanvasContainment::wheelEvent(QGraphicsSceneWheelEvent* e)
{
   // Get direction
   if(d->current == d->list.end())
      return;

   // Scroll backward
   QList<Canvas*>::iterator prev(d->current);
   if(e->delta() < 0) {
      if(d->current != d->list.begin()) {
         --d->current;
      }
   }
   // Scroll forward
   else {
      if(d->current != d->list.end() - 1) {
         ++d->current;
      }
   }

   // Focus to current canvas
   if(prev != d->current) {
      qDebug() << "Current canvas: " << (*prev)->name() << " -> " << (*d->current)->name();
      focusToCanvas(*d->current);
   }
}

void CanvasContainment::focusToCanvas(Canvas* c)
{
   QGraphicsProxyWidget* proxy = d->map[c];
   if(proxy != 0) {

      // Shift current viewport to target rect
      QRectF targetRect(proxy->sceneBoundingRect());
      d->view->setSceneRect(targetRect);
   }
}

void CanvasContainment::drawBackground(QPainter* p, const QRectF& re)
{
   // Paint background
   QGraphicsScene::drawBackground(p, re);

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

      QPointF aPt(borderWidth * 0.45, borderWidth * 0.45);
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
