#include <QPainter>
#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   show();
}

void CanvasView::drawForeground(QPainter* p, const QRectF& r)
{
   // Draw overlay
   int headerHeight = 25;
   int headerWidth = 0.2 * contentsRect().width();
   QPointF headerPt = mapToScene(contentsRect().topLeft());
   headerPt.setX(headerPt.x() + 2 * headerWidth);

   QRect headerRect(headerPt.toPoint(), QSize(headerWidth, headerHeight));


   p->save();
   p->setOpacity(0.6);
   p->setPen(Qt::NoPen);
   p->setBrush(Qt::black);
   p->drawRoundedRect(headerRect.adjusted(0, -headerHeight, 0, 0), 20.0, 20.0);
   p->setOpacity(1.0);
   p->setPen(Qt::white);
   p->drawText(headerRect, Qt::AlignCenter, mCanvas->name());
   p->restore();

   QGraphicsView::drawForeground(p, r);
}

#include "canvasview.moc"
