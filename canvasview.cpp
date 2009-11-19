#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   show();
}

#include "canvasview.moc"
