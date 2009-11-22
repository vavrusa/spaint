#include <QPainter>
#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   setFrameStyle(QFrame::NoFrame);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   show();
}

#include "canvasview.moc"
