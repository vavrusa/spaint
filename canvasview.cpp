#include <QPainter>
#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   setFrameStyle(QFrame::NoFrame);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   setSceneRect(0, 0, 640, 480);
   show();
}

void CanvasView::drawForeground(QPainter* p, const QRectF& r)
{
   // Draw foreground
   QGraphicsView::drawForeground(p, r);

   // Draw overlay
   int headerHeight = 25;
   int headerWidth = 0.2 * contentsRect().width();
   QPointF headerPt = mapToScene(contentsRect().topLeft());
   headerPt.setX(headerPt.x() + 2 * headerWidth);

   QRect headerRect(headerPt.toPoint(), QSize(headerWidth, headerHeight));

   p->save();

   // Rounded rectangle
   p->setOpacity(0.6);
   p->setPen(Qt::NoPen);
   p->setBrush(Qt::black);
   p->setClipRect(contentsRect());
   p->drawRoundedRect(headerRect.adjusted(0, -headerHeight, 0, 0), 20.0, 20.0);

   // Text overlay
   p->setOpacity(1.0);
   p->setPen(Qt::white);
   p->drawText(headerRect, Qt::AlignCenter, mCanvas->name());
   p->restore();
}

#include "canvasview.moc"
