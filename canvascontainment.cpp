#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMap>
#include "canvascontainment.h"
#include "canvasview.h"
#include "canvas.h"

// Static settings
const static int BorderWidth = 10;

struct CanvasContainment::Private {

   Private() : animation(0), view(0)
   {}

   QPropertyAnimation*       animation;     // Current animation
   QGraphicsWidget*         container;      // Container for layout
   QGraphicsView*            view;          // Graphics scene
   QGraphicsLinearLayout*    layout;        // Layout
   QList<Canvas*>            list;          // Ordered list
   QList<Canvas*>::iterator  current;       // Focused widget
   QMap<Canvas*,QGraphicsProxyWidget*> map; // Map Canvas -> Widget
};

CanvasContainment::CanvasContainment(QWidget *parent)
   : QGraphicsScene(parent), d(new Private)
{
   // Create graphics view
   d->container = new QGraphicsWidget();
   d->view = new QGraphicsView(this, parent);

   // Disable scrollbars and set anchor for resizing
   d->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   d->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   d->view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
   d->view->setCacheMode(QGraphicsView::CacheBackground);
   setBackgroundBrush(Qt::gray);

   // Create layout
   d->layout = new QGraphicsLinearLayout(Qt::Horizontal, d->container);
   d->container->setLayout(d->layout);
   addItem(d->container);

   // Invalidate current canvas ptr
   d->current = d->list.end();
   connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(updateLayout(QRectF)));
}

CanvasContainment::~CanvasContainment()
{
   d->container->deleteLater();
   delete d;
}

QGraphicsView* CanvasContainment::view()
{
   return d->view;
}

void CanvasContainment::addCanvas(Canvas* c)
{
   // Create canvas viewport
   CanvasView* view = new CanvasView(c);

   // Create graphics proxy
   QSizeF defaultSize = c->sceneRect().size();
   QGraphicsProxyWidget* proxy = addWidget(view);
   proxy->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

   // Add canvas to list
   d->list.append(c);
   d->current = d->list.end() - 1;
   d->map.insert(c, proxy);
   d->layout->addItem(proxy);

   // Update rect
   setSceneRect(sceneRect().adjusted(0, 0, defaultSize.width() * 1.5, 0));
   qDebug() << "New canvas: " << (*d->current)->name();
}

void CanvasContainment::removeCanvas(Canvas* c)
{
   // Current canvas
   if(c == 0)
      c = *d->current;

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

      // Create animation
      if(d->animation == 0) {
         d->animation = new QPropertyAnimation(d->view, "sceneRect");
      }

      // Plan animation to shift current viewport to target rect
      QRectF targetRect(proxy->sceneBoundingRect());
      d->animation->stop();
      d->animation->setDuration(600);
      d->animation->setEasingCurve(QEasingCurve::OutBack);
      d->animation->setStartValue(d->view->sceneRect());
      d->animation->setEndValue(targetRect);
      d->animation->start();
   }
}

void CanvasContainment::clearCanvas(Canvas* c)
{
   // Current canvas
   if(c == 0)
      c = *d->current;

   QList<QGraphicsItem*>::iterator it;
   QList<QGraphicsItem*> list(c->items());
   for(it = list.begin(); it < list.end(); ++it)
      c->removeItem(*it);
}

void CanvasContainment::updateLayout(const QRectF& newRect)
{
   // Layout update
   if(!newRect.isValid())
      return;

   // Update layout
   d->container->setGeometry(newRect);
   d->layout->setSpacing(Canvas::defaultSizeHint().width() * 0.25);

   // Current
   focusToCanvas(*d->current);
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
   QList<Canvas*>::iterator it;
   for(it = d->list.begin(); it < d->list.end(); ++it) {

      // Get current rect
      p->save();
      QRectF currentRect(d->map[*it]->sceneBoundingRect());
      grad.setStart(currentRect.topLeft());
      grad.setFinalStop(grad.start() - QPointF(0, borderWidth));

      // Corner and border rects
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

      // Calculate font metrics
      QFont labelFont(font());
      labelFont.setPixelSize(18);
      QFontMetrics fnm(labelFont);

      // Get label rect
      QString label(tr("Canvas: ") + (*it)->name());
      QRectF labelRect(fnm.boundingRect(label));
      QPointF labelPt(currentRect.topRight());
      labelPt.setX(labelPt.x() - labelRect.width());
      labelPt.setY(labelPt.y() - 1.25 * labelRect.height());
      labelRect.moveTo(labelPt);

      // Draw text
      p->setOpacity(0.85);
      p->setFont(labelFont);
      p->setPen(Qt::white);
      p->drawText(labelRect.bottomLeft(), label);
      p->restore();
   }
}

#include "canvascontainment.moc"
